#pragma once

#include "CWorker.h"
#include <QTimer>

class CUpdateLiveContentWorker : public CWorker
{
	Q_OBJECT

public:
	CUpdateLiveContentWorker(int nCategoryID, int nMatchID, int nLiveRecordID, const void *userData);
	~CUpdateLiveContentWorker();
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
	int m_nCategoryID;
	int m_nMatchID;
	int m_nLiveRecordID;
	const void *m_userData;
};
