#include "CUploadWorker.h"
#include "CDataCentre.h"
#include <QFile>

CUploadWorker::CUploadWorker(const QString &qsSrcPath, const void *userData)
	: m_qsSrcPath(qsSrcPath)
	, m_userData(userData)
{
	m_pfnCallback = CTaskCallback::OnUpdateLiveContentTask;
}

CUploadWorker::~CUploadWorker()
{
}

void CUploadWorker::slotExpireTimer()
{
	m_pNetworkReply->abort();
}

void CUploadWorker::SendRequestToServer(const QString &qsURL)
{
	QUrl url = qsURL;

	QNetworkRequest request(url);
	//request.setRawHeader(QByteArray("token"), g_dataCentre.stUserInformation.qsToken.toUtf8());
	//request.setRawHeader(QByteArray("Accept"), QByteArray("application/json,text/plain,*/*"));
	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	request.setRawHeader(QByteArray("clientType"), QByteArray("pc"));

	m_pExpireTimer = new QTimer();
	m_pExpireTimer->setInterval(10000);
	m_pExpireTimer->start();

	connect(m_pExpireTimer, &QTimer::timeout, this, &CUploadWorker::slotExpireTimer);

	m_pHttpMultiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QHttpPart imagePart;
	imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
	imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"wallhaven - 5wr3m7.jpg\""));

	QFile *file = new QFile(m_qsSrcPath);
	file->open(QIODevice::ReadOnly);
	imagePart.setBodyDevice(file);
	file->setParent(m_pHttpMultiPart); // we cannot delete the file now, so delete it with the multiPart

	m_pHttpMultiPart->append(imagePart);

	m_pNetworkReply = CTaskManager::GetInstance().GetNetworkAccessInstance().post(request, m_pHttpMultiPart);
	m_pHttpMultiPart->setParent(m_pNetworkReply);

	connect(m_pNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotErrorProcess(QNetworkReply::NetworkError)));
	connect(m_pNetworkReply, &QNetworkReply::finished, this, &CUploadWorker::slotFinished);

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

void CUploadWorker::slotDoWork(CWorker *receiver)
{
	if (receiver != this)
	{
		return;
	}

	CLogRecord::WriteToOutputWindow("upload file to server request start.");

	QString qsSuffix = QString("/api/live/multipart/upload");
	QString qsURL = g_dataCentre.m_qsServerAddress + qsSuffix;
	this->SendRequestToServer(qsURL);
}

void CUploadWorker::slotFinished()
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

void CUploadWorker::slotErrorProcess(QNetworkReply::NetworkError eNetworkError)
{
	m_pExpireTimer->stop();
	m_pExpireTimer->deleteLater();

	QNetworkReply *pNetworkReply = (QNetworkReply *)this->sender();

	// 无需触发finish信号
	disconnect(pNetworkReply, &QNetworkReply::finished, this, &CUploadWorker::slotFinished);

	QString qsErrorInfo = pNetworkReply->errorString();
	CLogRecord::WriteToOutputWindow("update live content request end. error code:%d desc:%s", eNetworkError, qsErrorInfo.toLocal8Bit().constData());
	m_pfnCallback(eNetworkError, qsErrorInfo, m_userData);

	pNetworkReply->deleteLater();
	this->deleteLater();
}
