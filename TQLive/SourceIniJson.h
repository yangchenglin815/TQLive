#pragma once
#include "StructData.h"

class SourceIniJson
{
public:
	static SourceIniJson* instance();
	void release();
	void Read();
	void Write();

private:
	SourceIniJson();

public:
	QList<TextSourceIniData> m_pSourceIniList;
	bool m_bIsExist = false;

private:
	static SourceIniJson* m_pInstance;
	QString m_pJsonPath;
};

