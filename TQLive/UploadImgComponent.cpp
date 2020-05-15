/***********************************************************************************
*								UploadImgComponent.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2020-03
*
* Description: 上传图片功能的加载组件（包含显示本地图片及网络图片）
***********************************************************************************/
#include "UploadImgComponent.h"
#include "CommonFunc.h"
#include <QPainter>
#include <QImage>
#include <QPixmap>

UploadImgComponent::UploadImgComponent(QWidget *parent)
	: QWidget(parent)
{

}

UploadImgComponent::~UploadImgComponent()
{

}

void UploadImgComponent::setCoverImg(QString coverUrl)
{
	m_sCoverLocalUrl = coverUrl;
	update();
}

void UploadImgComponent::setWebCoverImg(QString coverUrl)
{
	if (coverUrl.isEmpty())
		return;

	m_sCoverImgName = CommonFunc::getUploadImgFormat(coverUrl);
	m_currentPic = new QPixmap;
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	QSslConfiguration config;
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	QNetworkRequest request;
	request.setRawHeader("cache-control", "no-cache");
	request.setRawHeader("pragma", "no-cache");
	request.setSslConfiguration(config);
	request.setUrl(QUrl(coverUrl));
	manager->get(request);
}

void UploadImgComponent::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)


	QPainter painter;
	if (m_sCoverLocalUrl.isEmpty())
		return;

	QPixmap img(m_sCoverLocalUrl);
	QPixmap newImg = img.scaled(162, 90, Qt::KeepAspectRatio);
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawPixmap(0, 0, 162, 90, newImg);
	painter.end();
}

void UploadImgComponent::replyFinished(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		m_currentPic->loadFromData(reply->readAll());
		
		QString filename = QString("./image/%1").arg(m_sCoverImgName);
        m_currentPic->save(filename);
		
		setCoverImg(filename);
	}
}
