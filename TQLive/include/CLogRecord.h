#pragma once

#include <QObject>

struct ST_CONFIGURATION
{
	QString qsServerAddress;
	int nPort;
	unsigned long long ulAppID;
	int nServerType;
};

class CLogRecord : public QObject
{
	Q_OBJECT

private:
	CLogRecord(QObject *parent);
	~CLogRecord();

public:
	static void WriteToOutputWindow(const char *sFormat, ...);
	static void WriteToLogFile(const char *sFormat, va_list argp);
	static void ReadNetworkConfiguration(ST_CONFIGURATION &stConfiguration);
};
