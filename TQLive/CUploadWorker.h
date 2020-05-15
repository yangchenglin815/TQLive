#pragma once

#include "CWorker.h"
#include <QTimer>
#include <QHttpMultiPart>

class CUploadWorker : public CWorker
{
	Q_OBJECT

public:
	CUploadWorker(const QString &qsSrcPath, const void *userData);
	~CUploadWorker();
	void SendRequestToServer(const QString &qsURL);

public slots:
	void slotDoWork(CWorker *receiver);
	void slotFinished();
	void slotErrorProcess(QNetworkReply::NetworkError eNetworkError);
	void slotExpireTimer();
	

private:
	TASK_CALLBACK m_pfnCallback;
	QTimer *m_pExpireTimer;
	QNetworkReply *m_pNetworkReply;
	const void *m_userData;
	QString m_qsSrcPath;
	QHttpMultiPart *m_pHttpMultiPart;
};
