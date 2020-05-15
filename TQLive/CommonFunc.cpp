/***********************************************************************************
*								CommonFunc.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 通用算法类
***********************************************************************************/
#include "CommonFunc.h"
#include "EnumData.h"
#include <sstream>
#include <QDebug>
#include "CPtr.h"

int CommonFunc::getCurrentBitrateIndex(int vbitrate)
{
	int bitrateIndex = BITRATE_3000;

	switch (vbitrate)
	{
	case 800:
		bitrateIndex = BITRATE_800;
		break;
	case 1000:
		bitrateIndex = BITRATE_1000;
		break;
	case 1200:
		bitrateIndex = BITRATE_1200;
		break;
	case 1500:
		bitrateIndex = BITRATE_1500;
		break;
	case 2000:
		bitrateIndex = BITRATE_2000;
		break;
	case 3000:
		bitrateIndex = BITRATE_3000;
		break;
	case 4000:
		bitrateIndex = BITRATE_4000;
		break;
	default:
		break;
	}

	return bitrateIndex;
}

int CommonFunc::getCurrentBitrateByIndex(int index)
{
	int bitrate = 3000;

	switch (index)
	{
	case BITRATE_800:
		bitrate = 800;
		break;
	case BITRATE_1000:
		bitrate = 1000;
		break;
	case BITRATE_1200:
		bitrate = 1200;
		break;
	case BITRATE_1500:
		bitrate = 1500;
		break;
	case BITRATE_2000:
		bitrate = 2000;
		break;
	case BITRATE_3000:
		bitrate = 3000;
		break;
	case BITRATE_4000:
		bitrate = 4000;
		break;
	default:
		break;
	}

	return bitrate;
}

int CommonFunc::getFrameIntervalIndex(QString frameInterval)
{
	int index = FRAME_INTERVAL_30;

	if (frameInterval == "20")
	{
		index = FRAME_INTERVAL_20;
	}
	else if (frameInterval == "25 PAL")
	{
		index = FRAME_INTERVAL_25;
	}
	else if (frameInterval == "30")
	{
		index = FRAME_INTERVAL_30;
	}
	else if (frameInterval == "40")
	{
		index = FRAME_INTERVAL_40;
	}
	else if (frameInterval == "50 PAL")
	{
		index = FRAME_INTERVAL_50;
	}
	else if (frameInterval == "60")
	{
		index = FRAME_INTERVAL_60;
	}

	return index;
}

QString CommonFunc::getFrameIntervalByIndex(int index)
{
	QString frameInterval = "";

	switch (index)
	{
	case FRAME_INTERVAL_20:
		frameInterval = "20";
		break;
	case FRAME_INTERVAL_25:
		frameInterval = "25 PAL";
		break;
	case FRAME_INTERVAL_30:
		frameInterval = "30";
		break;
	case FRAME_INTERVAL_40:
		frameInterval = "40";
		break;
	case FRAME_INTERVAL_50:
		frameInterval = "50 PAL";
		break;
	case FRAME_INTERVAL_60:
		frameInterval = "60";
		break;
	default:
		break;
	}

	return frameInterval;
}

string CommonFunc::ResString(uint32_t cx, uint32_t cy)
{
	stringstream res;
	res << cx << "x" << cy;
	return res.str();
}

bool CommonFunc::ConvertResText(const char * res, uint32_t & cx, uint32_t & cy)
{
	BaseLexer lex;
	base_token token;
	
	lexer_start(lex, res);

	/* parse width */
	if (!lexer_getbasetoken(lex, &token, IGNORE_WHITESPACE))
		return false;
	if (token.type != BASETOKEN_DIGIT)
		return false;

	cx = std::stoul(token.text.array);

	/* parse 'x' */
	if (!lexer_getbasetoken(lex, &token, IGNORE_WHITESPACE))
		return false;
	if (strref_cmpi(&token.text, "x") != 0)
		return false;

	/* parse height */
	if (!lexer_getbasetoken(lex, &token, IGNORE_WHITESPACE))
		return false;
	if (token.type != BASETOKEN_DIGIT)
		return false;

	cy = std::stoul(token.text.array);

	/* shouldn't be any more tokens after this */
	if (lexer_getbasetoken(lex, &token, IGNORE_WHITESPACE))
		return false;

	return true;
}

int CommonFunc::getResolutionIndex(string resolution)
{
	int index = RESOLUTION_720P;

	if (resolution == "1280x720")
	{
		index = RESOLUTION_720P;
	}
	else if (resolution == "800x450")
	{
		index = RESOLUTION_450P;
	}
	else if (resolution == "640x360")
	{
		index = RESOLUTION_360P;
	}
	else if (resolution == "1920x1080")
	{
		index = RESOLUTION_1080P;
	}

	return index;
}

QString CommonFunc::getResolutionByIndex(int index)
{
	QString outputResolution = "1280x720";

	switch (index)
	{
	case RESOLUTION_720P:
		outputResolution = "1280x720";
		break;
	case RESOLUTION_450P:
		outputResolution = "800x450";
		break;
	case RESOLUTION_360P:
		outputResolution = "640x360";
		break;
	case RESOLUTION_1080P:
		outputResolution = "1920x1080";
		break;
	default:
		break;
	}

	return outputResolution;
}

int CommonFunc::getOutputEncoderIndex(QString outputEncoder)
{
	int index = ENCODER_CPU;

	if (outputEncoder == "x264")
	{
		index = ENCODER_CPU;
	}
	else if (outputEncoder == "nvenc")
	{
		index = ENCODER_GPU;
	}

	return index;
}

QString CommonFunc::getOutputEncoderByIndex(int index)
{
	QString outputEncoder = "x264";

	switch (index)
	{
	case ENCODER_CPU:
		outputEncoder = "x264";
		break;
	case ENCODER_GPU:
		outputEncoder = "nvenc";
		break;
	default:
		break;
	}

	return outputEncoder;
}

int CommonFunc::getAdOutputEncoderIndex(QString adOutputEncoder)
{
	int index = ENCODER_CPU;

	if (adOutputEncoder == "obs_x264")
	{
		index = ENCODER_CPU;
	}
	else if (adOutputEncoder == "ffmpeg_nvenc")
	{
		index = ENCODER_GPU;
	}

	return index;
}

QString CommonFunc::getAdOutputEocoderByIndex(int index)
{
	QString adOutputEncoder = "obs_x264";

	switch (index)
	{
	case ENCODER_CPU:
		adOutputEncoder = "obs_x264";
		break;
	case ENCODER_GPU:
		adOutputEncoder = "ffmpeg_nvenc";
		break;
	default:
		break;
	}

	return adOutputEncoder;
}

OBSData CommonFunc::GetDataFromJsonFile(const char * jsonFile)
{
	char fullPath[512];
	obs_data_t *data = nullptr;

	int ret = GetProfilePath(fullPath, sizeof(fullPath), jsonFile);
	if (ret > 0) {
		BPtr<char> jsonData = os_quick_read_utf8_file(fullPath);
		if (!!jsonData) {
			data = obs_data_create_from_json(jsonData);
		}
	}

	if (!data)
		data = obs_data_create();
	OBSData dataRet(data);
	obs_data_release(data);
	return dataRet;
}

int CommonFunc::getOutputBitrateIndex(QString outputBitrate)
{
	int index = OUTPUT_BITRATE_VBR;

	if (outputBitrate == "VBR")
	{
		index = OUTPUT_BITRATE_VBR;
	}
	else if (outputBitrate == "CBR")
	{
		index = OUTPUT_BITRATE_CBR;
	}

	return index;
}

QString CommonFunc::getAppAdapterName()
{
	QString adapterName;
	CPtr<IDXGIFactory1> factory;
	CPtr<IDXGIAdapter1> adapter;
	HRESULT hr;
	UINT i = 0;

	blog(LOG_INFO, "CommonFunc::getAppAdapterName:   Available Video Adapters: ");
	IID factoryIID = dxgiFactory2;

	hr = CreateDXGIFactory1(factoryIID, (void**)factory.Assign());
	if (FAILED(hr))
		return adapterName;

	hr = factory->EnumAdapters1(0, &adapter);
	if (FAILED(hr))
		return adapterName;

	DXGI_ADAPTER_DESC desc;
	char name[512] = "";

	hr = adapter->GetDesc(&desc);
	if (FAILED(hr))
		return adapterName;

	os_wcs_to_utf8(desc.Description, 0, name, sizeof(name));
	blog(LOG_INFO, "CommonFunc::getAppAdapterName:   %s", name);
	return QString::fromUtf8(name);
}

QString CommonFunc::getCurrAdapterName()
{
	IDXGIFactory1 * pFactory;
	IDXGIAdapter1 * pAdapter;

	QString adapterName;
	int iAdapterNum = 0;

	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory));
	if (FAILED(hr))
		return adapterName;

	blog(LOG_INFO, "CommonFunc::getCurrAdapterName:   Current Video Adapters: ");
	while (pFactory->EnumAdapters1(iAdapterNum++, &pAdapter) == S_OK) {
		DXGI_ADAPTER_DESC desc;
		char name[512] = "";

		hr = pAdapter->GetDesc(&desc);
		if (FAILED(hr))
			continue;

		/* ignore Microsoft's 'basic' renderer' */
		if (desc.VendorId == 0x1414 && desc.DeviceId == 0x8c)
			continue;

		os_wcs_to_utf8(desc.Description, 0, name, sizeof(name));
		blog(LOG_INFO, "CommonFunc::getCurrAdapterName:   Adapter %u: %s", iAdapterNum, name);

		bool isOk = false;
		IDXGIOutput *pOutput;
		UINT i = 0;
		while (pAdapter->EnumOutputs(i++, &pOutput) == S_OK) {
			DXGI_OUTPUT_DESC outputDesc;
			if (FAILED(pOutput->GetDesc(&outputDesc)))
				continue;

			RECT rect = outputDesc.DesktopCoordinates;
			blog(LOG_INFO, "CommonFunc::getCurrAdapterName:   output %u: "
				"pos={%d, %d}, "
				"size={%d, %d}, "
				"attached=%s",
				i,
				rect.left, rect.top,
				rect.right - rect.left, rect.bottom - rect.top,
				outputDesc.AttachedToDesktop ? "true" : "false");

			isOk = true;
		}

		if (isOk) {
			adapterName = QString::fromUtf8(name);
		}
	}

	return adapterName;
}

QString CommonFunc::getMediaNameByPath(QString path)
{
	QString name;
	QStringList list = path.split("/");
	if (list.size() > 0) {
		name = list[list.size() - 1];
	}
	
	return name;
}

QString CommonFunc::getTimeDurationByInt(int time)
{
	QString timeDuration = "00:00:00";

	int iHour = time / 1000 / 60 / 60;
	int iMinute = (time / 1000 / 60) % 60;
	int iSecond = (time / 1000) % 60 % 60;

	QString hour = (iHour > 9) ? QString::number(iHour) : QString("0%1").arg(iHour);
	QString minute = (iMinute > 9) ? QString::number(iMinute) : QString("0%1").arg(iMinute);
	QString second = (iSecond > 9) ? QString::number(iSecond) : QString("0%1").arg(iSecond);

	timeDuration = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);

	return timeDuration;
}

QString CommonFunc::getUploadImgFormat(QString webUrl)
{
	QString imgFormat;

	QStringList list = webUrl.split("/");
	int length = list.size();
	if (length > 0)
		imgFormat = list.at(length - 1);

	return imgFormat;
}
