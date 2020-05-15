#pragma once

#include "StructData.h"

class SourceIniXML
{
public:
	static SourceIniXML* instance();
	void release();
	void ReadXml();
	void WriteXml();

private:
	SourceIniXML();

public:
	QList<SourceIniData> m_pSourceIniList;
	bool m_bIsExist = false;

private:
	static SourceIniXML* m_pInstance;
	QString m_pXmlPath;
};
