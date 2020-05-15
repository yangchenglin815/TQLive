#pragma once
#include "StructData.h"

class CameraCacheJson
{
public:
	static CameraCacheJson* instance();
	void release();
	void Read();
	void Write();

private:
	CameraCacheJson();

public:
	QList<CameraCacheData> m_pCameraCacheList;
	bool m_bIsExist = false;

private:
	static CameraCacheJson* m_pInstance;
	QString m_pJsonPath;
};

