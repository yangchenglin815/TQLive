#include <stdio.h>
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "SDL2/SDL.h"
};

//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

int thread_exit=0;
int thread_pause=0;
AVFormatContext	*pFormatCtx;
AVFrame	*pFrame, *pFrameYUV;
AVCodecContext	*pCodecCtx;
struct SwsContext *img_convert_ctx;

int sfp_refresh_thread(void *opaque)
{
	thread_exit=0;
	thread_pause=0;

	AVPacket *packet;
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));

	int videoindex = (int)opaque;

	int got_picture;

	static int nReadAVPacketCount = 0;

	while (!thread_exit) 
	{
		if(!thread_pause)
		{
			while (1)
			{
				/*
					返回流中的下一帧
					对于视频数据 AVPacket包含一帧数据
					对于音频数据 固定长度的帧(PCM):AVPacket可能包含若干帧 非固定长度的帧(MPEG):AVPacket包含一帧数据
				*/
				nReadAVPacketCount++;
				nReadAVPacketCount++;

				RTMP_LogPrintf("[*******%d threadid:%08x]read AVPacket from bytestream IO Context start.\n", nReadAVPacketCount, ::GetCurrentThreadId());

				if (av_read_frame(pFormatCtx, packet) < 0)										// 触发接收一帧视频数据操作
				{
					thread_exit = 1;
				}

				RTMP_LogPrintf("[*******%d threadid:%08x]read AVPacket from bytestream IO Context end.\n", nReadAVPacketCount, ::GetCurrentThreadId());
				
				// 数据包中的流索引等于视频流索引
				if (packet->stream_index == videoindex)
					break;
			}

			// 解码AVPacket AVPacket--->AVFrame
			int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);			// 解码一帧视频数据操作
			if (ret < 0)
			{
				printf("Decode Error.\n");
				return -1;
			}

			av_free_packet(packet);

			if (got_picture)
			{
				sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);		// 原始视频数据处理
			}

			SDL_Event event;
			event.type = SFM_REFRESH_EVENT;
			SDL_PushEvent(&event);																// 触发显示一帧视频数据操作
		}

		//SDL_Delay(40);
	}

	thread_exit = 0;
	thread_pause = 0;

	//Break
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}

int InitSockets()
{
#ifdef WIN32
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(1, 1);
	return (WSAStartup(version, &wsaData) == 0);
#endif
}

void CleanupSockets()
{
#ifdef WIN32
	WSACleanup();
#endif
}

struct buffer_data {
	unsigned char *ptr;
	size_t size; ///< size left in the buffer
};

RTMP *rtmp;
int countbufsize = 0;

// ffmpeg重复调用该函数[bufsize可能会出现大于32768的情况] bufsize为期待读取的数据 一次如果没有复制完会调用多次复制
int fill_iobuffer(void * opaque, uint8_t *iobuffer, int bufsize)
{
	//if (!feof(fp_open)) 
	//{
	//	int true_size = fread(buf, 1, buf_size, fp_open);
	//	return true_size;
	//}
	//else 
	//{
	//	return -1;
	//}

	struct buffer_data *bd = (struct buffer_data *)opaque;

	static int nRecvCount = 0;
	nRecvCount++;

	RTMP_LogPrintf("[*******%d threadid:%08x]receive data from server. expected read:%dByte\n", nRecvCount, ::GetCurrentThreadId(), bufsize);

	int nRead = RTMP_Read(rtmp, (char *)bd->ptr, 4096);
	if (nRead > 0)
	{
		memcpy(iobuffer, bd->ptr, nRead);
		countbufsize += nRead;
		RTMP_LogPrintf("[*******%d threadid:%08x]copy data to bytestream IO Context.  actual read:%dByte total:%.2fKB\n", nRecvCount, ::GetCurrentThreadId(), nRead, countbufsize * 1.0 / 1024);
		return nRead;
	}
	else
	{
		return -1;
	}
}

int main(int argc, char* argv[])
{
	// librtmp下载网络流媒体数据
	InitSockets();

	double duration = -1;
	int nRead;
	//is live stream ?
	bool bLiveStream = true;


	//int bufsize = 1024 * 1024 * 10;
	int bufsize = 32 * 1024;
	unsigned char *buf = (unsigned char*)malloc(bufsize);
	memset(buf, 0, bufsize);
	long countbufsize = 0;

	//RTMP *rtmp = RTMP_Alloc();
	rtmp = RTMP_Alloc();

	RTMP_Init(rtmp);
	//set connection timeout,default 30s
	rtmp->Link.timeout = 10;

	// HKS's live URL
	if (!RTMP_SetupURL(rtmp, "rtmp://58.200.131.2:1935/livetv/hunantv"/*"rtmp://playpba.tanqiu.com/pba/pba-to-tanqiu?auth_key=1602844110-0-0-7aad892b22af4b4cad2c4900b5c94514"*//*"rtmp://live.hkstv.hk.lxdns.com/live/hks/"*/))
	{
		RTMP_Log(RTMP_LOGERROR, "SetupURL Err\n");
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}
	if (bLiveStream) {
		rtmp->Link.lFlags |= RTMP_LF_LIVE;
	}

	//1hour
	RTMP_SetBufferMS(rtmp, 3600 * 1000);

	if (!RTMP_Connect(rtmp, NULL)) {
		RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}

	if (!RTMP_ConnectStream(rtmp, 0)) {
		RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}

	//AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	//AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	//AVFrame	*pFrame, *pFrameYUV;
	unsigned char *out_buffer;
	//AVPacket *packet;
	//int ret, got_picture;

	//------------SDL----------------
	int screen_w, screen_h;
	SDL_Window *screen;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect;
	SDL_Thread *video_tid;
	SDL_Event event;

	//struct SwsContext *img_convert_ctx;

	//char filepath[]="bigbuckbunny_480x272.h265";
	//char filepath[] = "Titanic.ts";

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	struct buffer_data bd = { 0 };

	//// 处理读取到的数据
	//while (nRead = RTMP_Read(rtmp, (char *)buf, bufsize)) 
	//{
	//	// fwrite(buf, 1, nRead, fp);
	//	bd.ptr = buf;
	//	bd.size = nRead;

	//	countbufsize += nRead;
	//	RTMP_LogPrintf("Receive: %5dByte, Total: %5.2fkB\n", nRead, countbufsize*1.0 / 1024);
	//}

	// 将buf中的网络流媒体数据传送到iobuffer中
	bd.ptr = (unsigned char *)av_malloc(4096);
	unsigned char * iobuffer = (unsigned char *)av_malloc(4096);

	// 字节流IO上下文
	AVIOContext *avio = avio_alloc_context(iobuffer, 4096, 0, &bd, fill_iobuffer, NULL, NULL);
	pFormatCtx->pb = avio;

	// 格式化IO上下文 触发fill_iobuffer回调 打开输入流&&读取输入流头部信息 If you want to use custom IO, preallocate the format context and set its pb field.
	/*
		支持本地磁盘路径(file协议)、流媒体协议URL(RTMP、http、https)
	*/
	int err = avformat_open_input(&pFormatCtx, "nothing", NULL, NULL);

	RTMP_LogPrintf("[*******main thread. threadid:%08x]avformat_open_input end.\n", ::GetCurrentThreadId());

	if (err != 0) {
		printf("Couldn't open input stream.\n");
		return -1;
	}

	// 读取多媒体文件的数据包以获取流信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}

	RTMP_LogPrintf("[*******main thread. threadid:%08x]avformat_find_stream_info end.\n", ::GetCurrentThreadId());

	/*
		查找多媒体文件中的视频流
		多媒体文件中的所有流列表:  AVFormatContext.streams
		流列表中的元素数目： AVFormatContext.	nb_streams
		Codec context associated with this stream：AVStream.codec
		New streams are created with avformat_new_stream()
		demuxing: streams are created by libavformat in avformat_open_input().
		muxing: streams are created by the user before avformat_write_header().
	*/
	videoindex = -1;

	for (i = 0; i < pFormatCtx->nb_streams; i++)
	{
		// 流中编解码器类型
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}

	if (videoindex == -1)
	{
		printf("Didn't find a video stream.\n");
		return -1;
	}

	pCodecCtx = pFormatCtx->streams[videoindex]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

	if(pCodec == NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("Could not open codec.\n");
		return -1;
	}

	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height, 1);

	//Output Info-----------------------------
	//printf("---------------- File Information ---------------\n");
	//av_dump_format(pFormatCtx,0,filepath,0);
	//printf("-------------------------------------------------\n");
	
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) 
	{  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 
	//SDL 2.0 Support for multiple windows
	screen_w = pCodecCtx->width;
	screen_h = pCodecCtx->height;

	screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_OPENGL);

	if(!screen) 
	{  
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
		return -1;
	}

	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);  

	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = screen_w;
	sdlRect.h = screen_h;

	video_tid = SDL_CreateThread(sfp_refresh_thread, NULL, (void *)videoindex);
	//------------SDL End------------
	//Event Loop

	static int nCount = 0;
	
	for (;;) 
	{
		//Wait
		SDL_WaitEvent(&event);
		if(event.type == SFM_REFRESH_EVENT)
		{
			//SDL---------------------------
			SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );  
			SDL_RenderClear( sdlRenderer );  
			//SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
			SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);  
			SDL_RenderPresent( sdlRenderer );  
			//SDL End-----------------------
		}
		else if(event.type == SDL_KEYDOWN)
		{
			//Pause
			if(event.key.keysym.sym==SDLK_SPACE)
				thread_pause=!thread_pause;
		}
		else if(event.type == SDL_QUIT)
		{
			thread_exit=1;
		}
		else if(event.type == SFM_BREAK_EVENT)
		{
			break;
		}

	}

	sws_freeContext(img_convert_ctx);

	SDL_Quit();

	//--------------
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);


	if (buf) 
	{
		free(buf);
	}

	if (rtmp) 
	{
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		CleanupSockets();
		rtmp = NULL;
	}

	return 0;
}