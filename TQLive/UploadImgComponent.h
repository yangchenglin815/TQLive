#pragma once

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkReply>

class UploadImgComponent : public QWidget
{
	Q_OBJECT

public:
	UploadImgComponent(QWidget *parent = 0);
	~UploadImgComponent();

public:
	void setCoverImg(QString coverUrl);
	void setWebCoverImg(QString coverUrl);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QString m_sCoverLocalUrl;
	QString m_sCoverImgName;
	QPixmap *m_currentPic;

private slots:
    void replyFinished(QNetworkReply* reply);
};
