#pragma once
#include <QString>
#include <QStringList>
#include <string>

#include "obs-app.hpp"
#include "DXGI.h"

using namespace std;

static const IID dxgiFactory2 =
{ 0x50c83a1c, 0xe072, 0x4c48,{ 0x87, 0xb0, 0x36, 0x30, 0xfa, 0x36, 0xa6, 0xd0 } };

class CommonFunc
{
public:
	static int getCurrentBitrateIndex(int vbitrate);
	static int getCurrentBitrateByIndex(int index);

	static int getFrameIntervalIndex(QString frameInterval);
	static QString getFrameIntervalByIndex(int index);

	static string ResString(uint32_t cx, uint32_t cy);
	static bool ConvertResText(const char *res, uint32_t &cx, uint32_t &cy);

	static int getResolutionIndex(string resolution);
	static QString getResolutionByIndex(int index);

	static int getOutputEncoderIndex(QString outputEncoder);
	static QString getOutputEncoderByIndex(int index);

	static int getAdOutputEncoderIndex(QString adOutputEncoder);
	static QString getAdOutputEocoderByIndex(int index);

	static OBSData GetDataFromJsonFile(const char *jsonFile);
	static int getOutputBitrateIndex(QString outputBitrate);

	static QString getAppAdapterName();
	static QString getCurrAdapterName();

	static QString getMediaNameByPath(QString path);
	static QString getTimeDurationByInt(int time);

	static QString getUploadImgFormat(QString webUrl);
};

