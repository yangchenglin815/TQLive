/**
 * 函数调用步骤如下：
 *
 * [初始化]
 * Direct3DCreate9()：获得IDirect3D9
 * IDirect3D9->CreateDevice()：通过IDirect3D9创建Device（设备）
 * IDirect3DDevice9->CreateTexture()：通过Device创建一个Texture（纹理）。
 * IDirect3DDevice9->CreateVertexBuffer()：通过Device创建一个VertexBuffer（顶点缓存）。
 * IDirect3DVertexBuffer9->Lock()：锁定顶点缓存。
 * memcpy()：填充顶点缓存。
 * IDirect3DVertexBuffer9->Unlock()：解锁顶点缓存。
 *
 * [循环渲染数据]
 * IDirect3DTexture9->LockRect()：锁定纹理。
 * memcpy()：填充纹理数据
 * IDirect3DTexture9->UnLockRect()：解锁纹理。
 * IDirect3DDevice9->BeginScene()：开始绘制。
 * IDirect3DDevice9->SetTexture()：设置当前要渲染的纹理。
 * IDirect3DDevice9->SetStreamSource()：绑定VertexBuffer。
 * IDirect3DDevice9->SetFVF()：设置Vertex格式。
 * IDirect3DDevice9->DrawPrimitive()：渲染。
 * IDirect3DDevice9->EndScene()：结束绘制。
 * IDirect3DDevice9->Present()：显示出来。
 *
 * This software plays RGB/YUV raw video data using Direct3D.
 * It uses Texture in D3D to render the pixel data.
 * Compared to another method (use Surface), it's more flexible
 * but a little difficult.
 *
 * The process is shown as follows:
 *
 * [Init]
 * Direct3DCreate9()：Get IDirect3D9.
 * IDirect3D9->CreateDevice()：Create a Device.
 * IDirect3DDevice9->CreateTexture()：Create a Texture.
 * IDirect3DDevice9->CreateVertexBuffer()：Create a VertexBuffer.
 * IDirect3DVertexBuffer9->Lock()：Lock VertexBuffer.
 * memcpy()：Fill VertexBuffer.
 * IDirect3DVertexBuffer9->Unlock()：UnLock VertexBuffer.
 *
 * [Loop to Render data]
 * IDirect3DTexture9->LockRect()：Lock Texture.
 * memcpy()：Fill pixel data...
 * IDirect3DTexture9->UnLockRect()：UnLock Texture.
 * IDirect3DDevice9->BeginScene()：Begin to draw.
 * IDirect3DDevice9->SetTexture()：Set current Texture.
 * IDirect3DDevice9->SetStreamSource()：Bind VertexBuffer.
 * IDirect3DDevice9->SetFVF()：Set Vertex Format.
 * IDirect3DDevice9->DrawPrimitive()：Render.
 * IDirect3DDevice9->EndScene()：End drawing.
 * IDirect3DDevice9->Present()：Show on the screen.
 */

#include <stdio.h>
#include <tchar.h>
 //#include <d3d9.h>

#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>
#ifdef __cplusplus
};
#endif
#endif

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

//////Flexible Vertex Format, FVF
////typedef struct
////{
////	FLOAT       x,y,z;      // vertex untransformed position
////	FLOAT       rhw;        // eye distance
////	D3DCOLOR    diffuse;    // diffuse color
////	FLOAT       tu, tv;     // texture relative coordinates
////} CUSTOMVERTEX;
////
////CRITICAL_SECTION  m_critial;
////HWND     m_hVideoWnd;  // 视频窗口
////
////IDirect3D9 *m_pDirect3D9= NULL;
////IDirect3DDevice9 *m_pDirect3DDevice= NULL;
////IDirect3DTexture9 *m_pDirect3DTexture= NULL;
////IDirect3DVertexBuffer9 *m_pDirect3DVertexBuffer= NULL;
////
////// Custom flexible vertex format (FVF), which describes custom vertex structure
////#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
////
////
//////Select one of the Texture mode (Set '1'):
////#define TEXTURE_DEFAULT 1
//////Rotate the texture
////#define TEXTURE_ROTATE  0
//////Show half of the Texture
////#define TEXTURE_HALF    0
////
//////Width, Height
////const int screen_w=500,screen_h=500;
////const int pixel_w=320,pixel_h=180;
////FILE *fp=NULL;
//////Bit per Pixel
////const int bpp=32;
////
////unsigned char buffer[pixel_w*pixel_h*bpp/8];
////
////
////void Cleanup()
////{
////	EnterCriticalSection(&m_critial);
////	if(m_pDirect3DVertexBuffer)
////		m_pDirect3DVertexBuffer->Release();
////	if(m_pDirect3DTexture)
////		m_pDirect3DTexture->Release();
////	if(m_pDirect3DDevice)
////		m_pDirect3DDevice->Release();
////	if(m_pDirect3D9)
////		m_pDirect3D9->Release();
////	LeaveCriticalSection(&m_critial);
////}
////
////
////int InitD3D( HWND hwnd, unsigned long lWidth, unsigned long lHeight )
////{
////	HRESULT lRet;
////	InitializeCriticalSection(&m_critial);
////
////	Cleanup();
////	EnterCriticalSection(&m_critial);
////	// Create IDirect3D
////	m_pDirect3D9 = Direct3DCreate9( D3D_SDK_VERSION );
////	if ( m_pDirect3D9 == NULL ){
////		LeaveCriticalSection(&m_critial);
////		return -1;
////	}
////
////	if ( lWidth == 0 || lHeight == 0 ){
////		RECT rt;
////		GetClientRect( hwnd, &rt );
////		lWidth = rt.right-rt.left;
////		lHeight = rt.bottom-rt.top;
////	}
////	
////	/*
////	//Get Some Info
////	//Retrieves device-specific information about a device.
////	D3DCAPS9 d3dcaps;
////	lRet=m_pDirect3D9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3dcaps);
////	int hal_vp = 0;
////	if( d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ){
////		//save in hal_vp the fact that hardware vertex processing is supported.
////		hal_vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
////	}
////	// get D3DDISPLAYMODE
////	D3DDISPLAYMODE d3dDisplayMode;
////	lRet = m_pDirect3D9->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3dDisplayMode );
////	if ( FAILED(lRet) ){
////		LeaveCriticalSection(&m_critial);
////		return -1;
////	}
////	*/
////
////	//D3DPRESENT_PARAMETERS Describes the presentation parameters.
////	D3DPRESENT_PARAMETERS d3dpp;
////	ZeroMemory( &d3dpp, sizeof(d3dpp) );
////	d3dpp.BackBufferWidth = lWidth;   
////	d3dpp.BackBufferHeight = lHeight;
////	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	
////	//d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
////	d3dpp.BackBufferCount        = 1;
////	d3dpp.MultiSampleType   = D3DMULTISAMPLE_NONE;	
////	d3dpp.SwapEffect  = D3DSWAPEFFECT_COPY;	
////	d3dpp.hDeviceWindow  = hwnd;
////	d3dpp.Windowed   = TRUE;
////	d3dpp.EnableAutoDepthStencil = FALSE;
////	d3dpp.Flags = D3DPRESENTFLAG_VIDEO;
////	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT;
////
////	m_hVideoWnd = hwnd;
////
////	//Creates a device to represent the display adapter.
////	//Adapter:		Ordinal number that denotes the display adapter. D3DADAPTER_DEFAULT is always the primary display 
////	//D3DDEVTYPE:	D3DDEVTYPE_HAL((Hardware Accelerator), or D3DDEVTYPE_SW(SoftWare)
////	//BehaviorFlags：D3DCREATE_SOFTWARE_VERTEXPROCESSING, or D3DCREATE_HARDWARE_VERTEXPROCESSING
////	lRet = m_pDirect3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL,
////		D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &d3dpp, &m_pDirect3DDevice );
////	
////	/*
////	//Set some property
////	//SetSamplerState()
////	// Texture coordinates outside the range [0.0, 1.0] are set
////	// to the texture color at 0.0 or 1.0, respectively.
////	IDirect3DDevice9_SetSamplerState(m_pDirect3DDevice, 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
////	IDirect3DDevice9_SetSamplerState(m_pDirect3DDevice, 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
////	// Set linear filtering quality
////	IDirect3DDevice9_SetSamplerState(m_pDirect3DDevice, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
////	IDirect3DDevice9_SetSamplerState(m_pDirect3DDevice, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
////	//SetRenderState()
////	//set maximum ambient light
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_AMBIENT, D3DCOLOR_XRGB(255,255,0));
////	// Turn off culling
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_CULLMODE, D3DCULL_NONE);
////	// Turn off the zbuffer
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_ZENABLE, D3DZB_FALSE);
////	// Turn off lights
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_LIGHTING, FALSE);
////	// Enable dithering
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_DITHERENABLE, TRUE);
////	// disable stencil
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_STENCILENABLE, FALSE);
////	// manage blending
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_ALPHABLENDENABLE, TRUE);
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_ALPHATESTENABLE,TRUE);
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_ALPHAREF, 0x10);
////	IDirect3DDevice9_SetRenderState(m_pDirect3DDevice, D3DRS_ALPHAFUNC,D3DCMP_GREATER);
////	// Set texture states
////	IDirect3DDevice9_SetTextureStageState(m_pDirect3DDevice, 0, D3DTSS_COLOROP,D3DTOP_MODULATE);
////	IDirect3DDevice9_SetTextureStageState(m_pDirect3DDevice, 0, D3DTSS_COLORARG1,D3DTA_TEXTURE);
////	IDirect3DDevice9_SetTextureStageState(m_pDirect3DDevice, 0, D3DTSS_COLORARG2,D3DTA_DIFFUSE);
////	// turn off alpha operation
////	IDirect3DDevice9_SetTextureStageState(m_pDirect3DDevice, 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
////	*/
////
////
////	//Creates a texture resource.
////	//Usage: 
////	//D3DUSAGE_SOFTWAREPROCESSING: If this flag is used, vertex processing is done in software. 
////	//							If this flag is not used, vertex processing is done in hardware.
////	//D3DPool: 
////	//D3D3POOL_DEFAULT:	Resources are placed in the hardware memory (Such as video memory)
////	//D3D3POOL_MANAGED:	Resources are placed automatically to device-accessible memory as needed.
////	//D3DPOOL_SYSTEMMEM:  Resources are placed in system memory.
////
////	lRet = m_pDirect3DDevice->CreateTexture(lWidth, lHeight, 1, D3DUSAGE_SOFTWAREPROCESSING,
////		D3DFMT_X8R8G8B8,
////		D3DPOOL_MANAGED,
////		&m_pDirect3DTexture, NULL );
////
////
////	if ( FAILED(lRet) ){
////		LeaveCriticalSection(&m_critial);
////		return -1;
////	}
////	// Create Vertex Buffer
////	lRet = m_pDirect3DDevice->CreateVertexBuffer( 4 * sizeof(CUSTOMVERTEX),
////		0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pDirect3DVertexBuffer, NULL );
////	if ( FAILED(lRet) ){
////		LeaveCriticalSection(&m_critial);
////		return -1;
////	}
////
////	/* -0.5f is a "feature" of DirectX and it seems to apply to Direct3d also */
////#if TEXTURE_HALF
////	CUSTOMVERTEX vertices[] ={
////		{-0.5f,			-0.5f,			0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.0f,0.0f},
////		{lWidth-0.5f,	-0.5f,			0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.5f,0.0f},
////		{lWidth - 0.5f,	lHeight-0.5f,	0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.5f,1.0f},
////		{-0.5f,			lHeight-0.5f,	0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.0f,1.0f}
////	};
////#elif TEXTURE_ROTATE
////	//Rotate Texture?
////	CUSTOMVERTEX vertices[] ={
////		{lWidth/4-0.5f,		-0.5f,			0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.0f,0.0f},
////		{lWidth-0.5f,		lHeight/4-0.5f,	0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),1.0f,0.0f},
////		{lWidth*3/4-0.5f,	lHeight-0.5f,	0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),1.0f,1.0f},
////		{-0.5f,				lHeight*3/4-0.5f,0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.0f,1.0f}
////	};
////#else
////	CUSTOMVERTEX vertices[] ={
////		{-0.5f,			-0.5f,			0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.0f,0.0f},
////		{lWidth-0.5f,	-0.5f,			0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),1.0f,0.0f},
////		{lWidth - 0.5f,	lHeight-0.5f,	0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),1.0f,1.0f},
////		{-0.5f,			lHeight-0.5f,	0.0f,	1.0f,D3DCOLOR_ARGB(255, 255, 255, 255),0.0f,1.0f}
////	};
////#endif
////
////
////	// Fill Vertex Buffer
////	CUSTOMVERTEX *pVertex;
////	lRet = m_pDirect3DVertexBuffer->Lock( 0, 4 * sizeof(CUSTOMVERTEX), (void**)&pVertex, 0 );
////	if ( FAILED(lRet) ){
////		LeaveCriticalSection(&m_critial);
////		return -1;
////	}
////	memcpy(pVertex, vertices, sizeof(vertices));
////
////	m_pDirect3DVertexBuffer->Unlock();
////	LeaveCriticalSection(&m_critial);
////	return 0;
////}
////
////
////bool Render()
////{
////	LRESULT lRet;
////	//Read Data
////	//RGB
////	if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
////		// Loop
////		fseek(fp, 0, SEEK_SET);
////		fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp);
////	}
////
////	if(buffer == NULL || m_pDirect3DDevice == NULL) 
////		return false;
////	//Clears one or more surfaces
////	lRet = m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
////		D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);
////	
////	D3DLOCKED_RECT d3d_rect;
////	//Locks a rectangle on a texture resource.
////	//And then we can manipulate pixel data in it.
////	lRet = m_pDirect3DTexture->LockRect( 0, &d3d_rect, 0, 0 );
////	if ( FAILED(lRet) ){
////		return false;
////	}
////	// Copy pixel data to texture
////	byte *pSrc = buffer;
////	byte *pDest = (byte *)d3d_rect.pBits;
////	int stride = d3d_rect.Pitch;
////	unsigned long i = 0;
////
////
////	int pixel_w_size=pixel_w*bpp/8;
////	for(unsigned long i=0; i< pixel_h; i++){
////		memcpy( pDest, pSrc, pixel_w_size );
////		pDest += stride;
////		pSrc += pixel_w_size;
////	}
////
////	m_pDirect3DTexture->UnlockRect( 0 );
////
////	//Begin the scene
////	if ( FAILED(m_pDirect3DDevice->BeginScene()) ){
////		return false;
////	}
////
////	lRet = m_pDirect3DDevice->SetTexture( 0, m_pDirect3DTexture );
////
////	
////	//Binds a vertex buffer to a device data stream.
////	m_pDirect3DDevice->SetStreamSource( 0, m_pDirect3DVertexBuffer,
////		0, sizeof(CUSTOMVERTEX) );
////	//Sets the current vertex stream declaration.
////	lRet = m_pDirect3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
////	//Renders a sequence of nonindexed, geometric primitives of the 
////	//specified type from the current set of data input streams.
////	m_pDirect3DDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
////	m_pDirect3DDevice->EndScene();
////	//Presents the contents of the next buffer in the sequence of back 
////	//buffers owned by the device.
////	m_pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
////	return true;
////}

RTMP *rtmp;
AVFormatContext	*pFormatCtx;
AVFrame	*pFrame, *pFrameYUV;
AVCodecContext	*pCodecCtx;
struct SwsContext *img_convert_ctx;

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
HWND hwnd = NULL;

//ID3D11Device *m_pDirect3D11 = NULL;

// 2D纹理对象数组
ID3D11Texture2D *texturePlanes_[3];

// 2D纹理对应的着色器资源视图对象数组（着色器资源，在着色器中对纹理进行采样）
ID3D11ShaderResourceView* resourceViewPlanes_[3];

// 2D纹理对应的渲染目标视图（纹理作为渲染目标，Direct3D 渲染到纹理）
/*
   资源不能被直接绑定到一个管线阶段；我们只能把与资源关联的资源视图绑定到不同的管线阶段。
   纹理可以被绑定到渲染管线的不同阶段；例如，比较常见的情况是将纹理作为渲染目标（即，Direct3D 渲染到纹理）和着色器资源（即，在着色器中对纹理进行采样）。当创建用于这两种目的的纹理资源时，应使用绑定标志值
*/
//ID3D11RenderTargetView

void WriteToOutputWindow(const char *sFormat, ...)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	DWORD dwThreadId = ::GetCurrentThreadId();
	int nLenA = _scprintf("*******[%02d:%02d:%02d:%03d threadid:0x%08x]\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, dwThreadId);

	va_list ap;
	va_start(ap, sFormat);
	int nLenB = _vscprintf(sFormat, ap);

	LPSTR sDst = new CHAR[nLenA + nLenB + 1];

	sprintf(sDst, "*******[%02d:%02d:%02d:%03d threadid:0x%08x]", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, dwThreadId);
	vsprintf(sDst + nLenA - 1, sFormat, ap);
	sprintf(sDst + nLenA + nLenB - 1, "\n");

	va_end(ap);

	OutputDebugStringA(sDst);

	delete[]sDst;
	sDst = NULL;
}

int InitD3D(HWND hwnd, unsigned long lWidth, unsigned long lHeight)
{
	//m_pDirect3D11 = D3D11CreateDevice(D3D_SDK_VERSION);

	g_hWnd = hwnd;
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
//
//	UINT createDeviceFlags = 0;
//#ifdef _DEBUG
//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//	D3D_DRIVER_TYPE driverTypes[] =
//	{
//		D3D_DRIVER_TYPE_HARDWARE,
//		D3D_DRIVER_TYPE_WARP,
//		D3D_DRIVER_TYPE_REFERENCE,
//	};
//	UINT numDriverTypes = ARRAYSIZE(driverTypes);
//
//	D3D_FEATURE_LEVEL featureLevels[] =
//	{
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_10_1,
//		D3D_FEATURE_LEVEL_10_0,
//	};
//	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
//
//	DXGI_SWAP_CHAIN_DESC sd;
//	ZeroMemory(&sd, sizeof(sd));
//	sd.BufferCount = 1;
//	sd.BufferDesc.Width = width;
//	sd.BufferDesc.Height = height;
//	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	sd.BufferDesc.RefreshRate.Numerator = 60;
//	sd.BufferDesc.RefreshRate.Denominator = 1;
//	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	sd.OutputWindow = g_hWnd;
//	sd.SampleDesc.Count = 1;
//	sd.SampleDesc.Quality = 0;
//	sd.Windowed = TRUE;
//
//	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
//	{
//		g_driverType = driverTypes[driverTypeIndex];
//		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
//			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel,
//			&g_pImmediateContext);
//		if (SUCCEEDED(hr))
//			break;
//	}
//	if (FAILED(hr))
//		return hr;
//
//	// 创建渲染目标视图
//	ID3D11Texture2D* pBackBuffer = NULL;
//	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
//	if (FAILED(hr))
//		return hr;
//
//	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
//	pBackBuffer->Release();
//	if (FAILED(hr))
//		return hr;
//
//	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
//
//	// 设置视口
//	D3D11_VIEWPORT vp;
//	vp.Width = (FLOAT)width;
//	vp.Height = (FLOAT)height;
//	vp.MinDepth = 0.0f;
//	vp.MaxDepth = 1.0f;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	g_pImmediateContext->RSSetViewports(1, &vp);


	//ID3D11ShaderResourceView* resourceViewPlanes_[3];
	//ID3D11Texture2D *texturePlanes_[3];
	//CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8_UNORM, width, height);
	//textureDesc.MipLevels = 1;
	//textureDesc.Width = width / 2; textureDesc.Height = height / 2;

	//hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &texturePlanes_[0]);
	//hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &texturePlanes_[1]);
	//hr = g_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &texturePlanes_[2]);

	//CD3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc(D3D11_SRV_DIMENSION_TEXTURE2D);
	//for (int i = 0; i < 3; ++i)
	//{
	//	// 创建着色器资源视图（纹理资源不能直接绑定到渲染管线）
	//	g_pd3dDevice->CreateShaderResourceView(texturePlanes_[i], &resourceViewDesc, &resourceViewPlanes_[i]);
	//}

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = S_OK;
	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&FeatureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&g_pSwapChain,
		&g_pd3dDevice,
		&FeatureLevelsSupported,
		&g_pImmediateContext)))
	{
		return hr;
	}

	ID3D11Texture2D* pBackBuffer;
	// Get a pointer to the back buffer
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	// Create a render-target view
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL,
		&g_pRenderTargetView);

	// Bind the view
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	return S_OK;
}

void Render(int nParam)
{
	// 只是简单地清除后台缓冲
	float ClearColor[4] = { 1.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha

	float redColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float greenColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float blueColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

	float *pszColor[3] = { redColor, greenColor, blueColor };

	int i = nParam % 3;

	//g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, pszColor[i]);


	//for (int i = 0; i < 3; ++i)
	//{
	//	// 拷贝内存数据到子资源中
	//	g_pImmediateContext->UpdateSubresource(texturePlanes_[i], 0, NULL, pFrameYUV->data[i], pFrameYUV->linesize[i], 0);
	//}

	//// 绑定着色器资源数组到像素着色器阶段
	//g_pImmediateContext->PSSetShaderResources(0, 3, resourceViewPlanes_);

	//g_pSwapChain->Present(0, 0);


	ID3D11Texture2D* pBackBuffer;
	// Get a pointer to the back buffer
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	g_pImmediateContext->UpdateSubresource(pBackBuffer, 0, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0], 0);

	g_pSwapChain->Present(0, 0);
}

void CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();

	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
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



int InitPullStreamModule()
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

	// HKS's live URLrtmp://202.69.69.180:443/webcast/bshdlive-pc
	if (!RTMP_SetupURL(rtmp, "rtmp://58.200.131.2:1935/livetv/gxtv"/*"rtmp://playpba.tanqiu.com/pba/pba-to-tanqiu?auth_key=1602844110-0-0-7aad892b22af4b4cad2c4900b5c94514"*//*"rtmp://live.hkstv.hk.lxdns.com/live/hks/"*/))
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
}

struct buffer_data {
	unsigned char *ptr;
	size_t size; ///< size left in the buffer
};

int countbufsize = 0;


// ffmpeg重复调用该函数[bufsize可能会出现大于32768的情况] bufsize为期待读取的数据 一次如果没有复制完会调用多次复制
int fill_iobuffer(void * opaque, uint8_t *iobuffer, int bufsize)
{
	struct buffer_data *bd = (struct buffer_data *)opaque;

	static int nRecvCount = 0;
	nRecvCount++;

	WriteToOutputWindow("[invocation count:%d]receive data from server. expected read:%dByte", nRecvCount, bufsize);

	int nRead = RTMP_Read(rtmp, (char *)bd->ptr, 4096);
	if (nRead > 0)
	{
		memcpy(iobuffer, bd->ptr, nRead);
		countbufsize += nRead;
		WriteToOutputWindow("[invocation count:%d]write data to bytestream IO Context. actual read:%dByte total:%.2fKB", nRecvCount, nRead, countbufsize * 1.0 / 1024);
		return nRead;
	}
	else
	{
		return -1;
	}
}

struct buffer_data bd = { 0 };

int InitDecodeDataModule()
{
	int				i, videoindex;
	AVCodec			*pCodec;
	unsigned char *out_buffer;

	int screen_w, screen_h;

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

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

	WriteToOutputWindow("avformat_open_input end.\n", ::GetCurrentThreadId());

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

	WriteToOutputWindow("avformat_find_stream_info end.\n", ::GetCurrentThreadId());

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

	if (pCodec == NULL)
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

	out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

	//Output Info-----------------------------
	//printf("---------------- File Information ---------------\n");
	//av_dump_format(pFormatCtx,0,filepath,0);
	//printf("-------------------------------------------------\n");

	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	return videoindex;
}


int thread_exit = 0, thread_pause = 0;

// LPTHREAD_START_ROUTINE
DWORD WINAPI WorkerThread(LPVOID p)
{
	thread_exit = 0;
	thread_pause = 0;

	AVPacket *packet;
	packet = (AVPacket *)av_malloc(sizeof(AVPacket));

	int videoindex = (int)p;

	int got_picture;

	DWORD dwStart, dwEnd;
	int ret;

	static int nReadAVPacketCount = 0;

	while (!thread_exit)
	{
		dwStart = GetTickCount();

		if (!thread_pause)
		{
			while (1)
			{
				/*
					返回流中的下一帧
					对于视频数据 AVPacket包含一帧数据
					对于音频数据 固定长度的帧(PCM):AVPacket可能包含若干帧 非固定长度的帧(MPEG):AVPacket包含一帧数据
				*/
				nReadAVPacketCount++;

				WriteToOutputWindow("[invocation count:%d]try read AVPacket from bytestream IO Context start.", nReadAVPacketCount);

				if ((ret = av_read_frame(pFormatCtx, packet)) < 0)										// 触发接收一帧视频数据操作
				{
					thread_exit = 1;
				}

				WriteToOutputWindow("[invocation count:%d]try read AVPacket from bytestream IO Context end. ret:%d", nReadAVPacketCount, ret);

				// 数据包中的流索引等于视频流索引
				if (packet->stream_index == videoindex)
					break;
			}

			// 解码AVPacket AVPacket--->AVFrame
			int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);			// 解码一帧视频数据操作
			if (ret < 0)
			{
				WriteToOutputWindow("worker thread quit. error:decode error");
				//printf("Decode Error.\n");
				//return -1;
			}

			av_free_packet(packet);

			if (got_picture)
			{
				sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);		// 原始视频数据处理
			}

			// 发送发送用户自定义的窗口消息
			static int nCount = 0;
			::PostMessageA(hwnd, WM_USER + 1, nCount, 0);														// 触发显示一帧视频数据操作
			nCount++;
		}

		dwEnd = GetTickCount();

		DWORD dwDelta = dwEnd - dwStart;
		//if ()

		//SDL_Delay(40);
		//::sleep(40);
		::Sleep(10);
		//::WriteToOutputWindow("*******www.\n");
	}

	thread_exit = 0;
	thread_pause = 0;

	// 发送退出消息
	WriteToOutputWindow("worker thread quit.");

	return 0;
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT msg, WPARAM wparma, LPARAM lparam)
{
	switch (msg) {
	case WM_DESTROY:
		//Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_USER + 1:
		Render(wparma);
		break;
	}
	return DefWindowProc(hwnd, msg, wparma, lparam);
}

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
//int main()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = (WNDPROC)MyWndProc;
	wc.lpszClassName = L"D3D";
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	//HWND hwnd = NULL;
	hwnd = CreateWindow(L"D3D", L"Simplest Video Play Direct3D (Texture)", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInstance/*NULL*/, NULL);
	if (hwnd == NULL) {
		return -1;
	}


	if (InitD3D(hwnd, 320, 180) == E_FAIL) {
		return -1;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	InitPullStreamModule();
	int videoindex = InitDecodeDataModule();
	::CreateThread(NULL, 0, WorkerThread, (LPVOID)videoindex, 0, 0);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT) {
		//PeekMessage, not GetMessage
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//else {
		//	//Sleep(40);
		//	//Render();
		//}
	}

	UnregisterClass(L"D3D", hInstance);
	return 0;
}

