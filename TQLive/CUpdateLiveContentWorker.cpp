#include "CUpdateLiveContentWorker.h"
#include "CDataCentre.h"

CUpdateLiveContentWorker::CUpdateLiveContentWorker(int nCategoryID, int nMatchID, int nLiveRecordID, const void *userData)
	: m_nCategoryID(nCategoryID)
	, m_nMatchID(nMatchID)
	, m_nLiveRecordID(nLiveRecordID)
	, m_userData(userData)
{
	m_pfnCallback = CTaskCallback::OnUpdateLiveContentTask;
}

CUpdateLiveContentWorker::~CUpdateLiveContentWorker()
{
}

void CUpdateLiveContentWorker::slotExpireTimer()
{
	m_pNetworkReply->abort();
}

void CUpdateLiveContentWorker::SendRequestToServer(const QString &qsURL)
{
	QUrl url = qsURL;

	// application/x-www-form-urlencoded方式提交数据 对应Content-Type
	QByteArray byteArray;
	byteArray.append(QString("sportType=%1&matchId=%2&recordId=%3").arg(m_nCategoryID).arg(m_nMatchID).arg(m_nLiveRecordID));
	
	QNetworkRequest request(url);
	request.setRawHeader(QByteArray("token"), g_dataCentre.stUserInformation.qsToken.toUtf8());
	request.setRawHeader(QByteArray("Accept"), QByteArray("application/json,text/plain,*/*"));
	request.setRawHeader(QByteArray("clientType"), QByteArray("pc"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	m_pExpireTimer = new QTimer();
	m_pExpireTimer->setInterval(10000);
	m_pExpireTimer->start();

	connect(m_pExpireTimer, &QTimer::timeout, this, &CUpdateLiveContentWorker::slotExpireTimer);

	// 初始化QNetworkAccessManager底层库有一个异常 0x76CD33D2 (KernelBase.dll)处(位于 TQLive.exe 中)引发的异常: 0x000006A6: 绑定句柄无效。。
	m_pNetworkReply = CTaskManager::GetInstance().GetNetworkAccessInstance().post(request, byteArray);

	connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotErrorProcess(QNetworkReply::NetworkError)));
	connect(m_pNetworkReply, &QNetworkReply::finished, this, &CUpdateLiveContentWorker::slotFinished);

	//QEventLoop loop;
	//connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//loop.exec();
	//CLogRecord::WriteToOutputWindow("http/https request end.");

	QString qsMsg;
	int nStatusCode;

	QNetworkReply::NetworkError eNetworkError = m_pNetworkReply->error();

	if (eNetworkError != QNetworkReply::NoError)
	{
		QString qsErrorInfo = m_pNetworkReply->errorString();
		qDebug() << qsErrorInfo;
		nStatusCode = eNetworkError;
		qsMsg = qsErrorInfo;
	}
}

void CUpdateLiveContentWorker::slotDoWork(CWorker *receiver)
{
	if (receiver != this)
	{
		return;
	}

	CLogRecord::WriteToOutputWindow("update live content request start.");

	QString qsSuffix = QString("/api/live/stream/sportType/update");
	QString qsURL = g_dataCentre.m_qsServerAddress + qsSuffix;
	this->SendRequestToServer(qsURL);
}

void CUpdateLiveContentWorker::slotFinished()
{
	QNetworkReply *reply = (QNetworkReply *)this->sender();

	m_pExpireTimer->stop();
	m_pExpireTimer->deleteLater();
	reply->deleteLater();

	
	QByteArray byteArray = reply->readAll();

	QString qsURL;
	this->AcquireRedirectionURL(reply, qsURL);

	if (!qsURL.isEmpty())
	{
		this->SendRequestToServer(qsURL);
		return;
	}

	
	//QString qsData(byteArray);
	//QVariant variant = reply->header(QNetworkRequest::ContentLengthHeader);
	//long long llTotalFileSize = variant.toLongLong();
	//map<QString, QString> map;
	QString qsMsg;
	int nStatusCode = 0;

	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
	if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
	{
		QJsonObject jsonObject = document.object();

		nStatusCode = jsonObject["status"].toInt();

		if (nStatusCode == 200)
		{
			QJsonObject jsonData = jsonObject["data"].toObject();
		}
		else
		{
			qsMsg = jsonObject["msg"].toString();
		}
	}

	CLogRecord::WriteToOutputWindow("update live content request request end. state code:%d desc:%s", nStatusCode, qsMsg.toLocal8Bit().constData());
	this->m_pfnCallback(nStatusCode, qsMsg, m_userData);

	this->deleteLater();
}

void CUpdateLiveContentWorker::slotErrorProcess(QNetworkReply::NetworkError eNetworkError)
{
	m_pExpireTimer->stop();
	m_pExpireTimer->deleteLater();

	QNetworkReply *pNetworkReply = (QNetworkReply *)this->sender();

	// 无需触发finish信号
	disconnect(pNetworkReply, &QNetworkReply::finished, this, &CUpdateLiveContentWorker::slotFinished);

	QString qsErrorInfo = pNetworkReply->errorString();
	CLogRecord::WriteToOutputWindow("update live content request end. error code:%d desc:%s", eNetworkError, qsErrorInfo.toLocal8Bit().constData());
	m_pfnCallback(eNetworkError, qsErrorInfo, m_userData);

	pNetworkReply->deleteLater();
	this->deleteLater();
}
