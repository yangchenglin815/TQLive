/***********************************************************************************
*								BarrageAssistantDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 弹幕助手对话框
***********************************************************************************/
#include "BarrageAssistantDlg.h"
#include <QListView>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QKeyEvent>
#include "FramelessHelper.h"
#include "HttpAgent.h"
#include "EnumData.h"
#include "BarrageStatisticsAgent.h"
#include "GlobalSignal.h"
#include "UserConfig.h"

BarrageAssistantDlg::BarrageAssistantDlg(QWidget *parent)
	: QDialog(parent)
	, m_curr_fontSize(12)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setWindowOpacity(0.9);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);

	InitUI();
	InitConnect();

	m_pStatisticsThread = new BarrageStatisticsAgent;
	qRegisterMetaType<BarrageStatisticsInfo>("BarrageStatisticsInfo");
	connect(this, &BarrageAssistantDlg::onSigUpdateThread, m_pStatisticsThread, &BarrageStatisticsAgent::onUpdate);
	connect(this, &BarrageAssistantDlg::onSigClearStatisticsData, m_pStatisticsThread, &BarrageStatisticsAgent::onClear);
	connect(GlobalSignal::instance(), SIGNAL(onSigClearStatisticsData()), this, SLOT(onClearStatisticsThread()));
	connect(GlobalSignal::instance(), SIGNAL(onSigInitStatisticsData()), this, SLOT(onInitStatisticsThread()));
	m_pStatisticsThread->start();
}

BarrageAssistantDlg::~BarrageAssistantDlg()
{

}

void BarrageAssistantDlg::keyPressEvent(QKeyEvent *event)
{
	if (UserConfig::instance()->getIsOpenHotKey())
	{
		int keyInt = event->key();
		Qt::Key key = static_cast<Qt::Key>(keyInt);
		if (key == Qt::Key_unknown)
		{
			qDebug() << "Unknown key from a macro probably";
			return;
		}

		if (key == Qt::Key_Control)
		{
			keyInt = Qt::CTRL;
		}
		else if (key == Qt::Key_Shift)
		{
			keyInt = Qt::SHIFT;
		}
		else if (key == Qt::Key_Alt)
		{
			keyInt = Qt::ALT;
		}
		else if (key == Qt::Key_Meta)
		{
			keyInt = Qt::META;
		}
		else
		{
			Qt::KeyboardModifiers modifiers = event->modifiers();
			if (modifiers & Qt::ShiftModifier)
				keyInt += Qt::SHIFT;
			if (modifiers & Qt::ControlModifier)
				keyInt += Qt::CTRL;
			if (modifiers & Qt::AltModifier)
				keyInt += Qt::ALT;
			if (modifiers & Qt::MetaModifier)
				keyInt += Qt::META;

			RespondHotKey(QKeySequence(keyInt).toString(QKeySequence::NativeText));
			return;
		}

		RespondHotKey(QKeySequence(keyInt).toString(QKeySequence::NativeText).remove("+"));
	}
}

void BarrageAssistantDlg::onRecvChatRoomMsg(const BarrageDetialInfo &barrageDetialInfo)
{
	if (!m_pIsLiving)
	{
		return;
	}

	m_pBarrageDetialInfo = barrageDetialInfo;
	m_pBarragePoolRecord.append(m_pBarrageDetialInfo);

	switch (m_pBarrageDetialInfo.nMsgType)
	{
	case USER_LOGOUT_LIVE:
		break;
	case USER_LOGIN_LIVE:
		InitBarragePool(USER_LOGIN_LIVE);
		break;
	case USER_SEND_GIFT:
		InitGiftPool();
		return;
	case USER_SEND_MSG:
		InitBarragePool(USER_SEND_MSG);
		break;
	default:
		return;
	}

	BarrageStatisticsInfo barrageStatisticsInfo;
	barrageStatisticsInfo.nMsgType      = m_pBarrageDetialInfo.nMsgType;
	barrageStatisticsInfo.nUserId       = m_pBarrageDetialInfo.nUserId;
	barrageStatisticsInfo.nUserVipLevel = m_pBarrageDetialInfo.nUserVipLevel;
	barrageStatisticsInfo.nEnterTime    = m_pBarrageDetialInfo.nEnterTime;
	barrageStatisticsInfo.nLeaveTime    = m_pBarrageDetialInfo.nLeaveTime;
	barrageStatisticsInfo.nUserRole     = m_pBarrageDetialInfo.nUserRole;
	
	emit onSigUpdateThread(barrageStatisticsInfo);
}

void BarrageAssistantDlg::onClearStatisticsThread()
{
	m_pIsLiving = false;

	emit onSigClearStatisticsData();
}

void BarrageAssistantDlg::onInitStatisticsThread()
{
	m_pBarragePoolRecord.clear();
	UpdateList();

	m_pIsLiving = true;
}

void BarrageAssistantDlg::InitUI()
{
	QStringList strList;
	strList << "12" << "14" << "16" << "18" << "20" << "22" << "24";
	ui.combox_fontNo->setView(new QListView);
	ui.combox_fontNo->addItems(strList);
}

void BarrageAssistantDlg::InitConnect()
{
	qRegisterMetaType<BarrageDetialInfo>("BarrageDetialInfo");
	connect(HttpAgent::instance(), SIGNAL(onSigRecvChatRoomMsg(const BarrageDetialInfo &)), this, SLOT(onRecvChatRoomMsg(const BarrageDetialInfo &)));
	connect(ui.btn_close, &QPushButton::clicked, this, &BarrageAssistantDlg::close);
	connect(ui.combox_fontNo, &QComboBox::currentTextChanged, [&](const QString &text) {
		m_curr_fontSize = text.toInt();
		UpdateList();
	});
}

void BarrageAssistantDlg::UpdateList()
{
	ui.textEdit_content->clear();
	ui.textEdit_gift->clear();

	for (int i = 0; i < m_pBarragePoolRecord.size(); ++i)
	{
		m_pBarrageDetialInfo = m_pBarragePoolRecord[i];
		switch (m_pBarrageDetialInfo.nMsgType)
		{
		case USER_LOGIN_LIVE:
			InitBarragePool(USER_LOGIN_LIVE);
			break;
		case USER_SEND_GIFT:
			InitGiftPool();
			break;
		case USER_SEND_MSG:
			InitBarragePool(USER_SEND_MSG);
			break;
		default:
			break;
		}
	}
}

void BarrageAssistantDlg::ImageAddText(QString imagePath, int userLevel)
{
	if (imagePath.isEmpty())
	{
		return;
	}

	//QFile::remove("./newLevel.png");
	
	QImage image = QPixmap(imagePath).toImage();

	QImage newImage = image.scaled(image.width() * m_curr_fontSize / 12, m_curr_fontSize + 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPainter painter(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

	QPen pen = painter.pen();
	pen.setColor(Qt::white);
	QFont font = painter.font();
	font.setBold(true);
	font.setPixelSize(m_curr_fontSize);
	font.setFamily("Microsoft YaHei");
	painter.setPen(pen);
	painter.setFont(font);
	painter.drawText(newImage.rect(), Qt::AlignCenter, QString("LV%1").arg(userLevel));

	newImage.save(QString("./image/newLevel%1.png").arg(userLevel), "PNG");
}

void BarrageAssistantDlg::roleImageAddText(QString imagePath, int userRole)
{
	if (imagePath.isEmpty())
	{
		return;
	}

	//QFile::remove("./newRole.png");

	QImage image = QPixmap(imagePath).toImage();

	QImage newImage = image.scaled(image.width() * m_curr_fontSize / 12, m_curr_fontSize + 4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPainter painter(&newImage);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

	QPen pen = painter.pen();
	pen.setColor(Qt::white);
	QFont font = painter.font();
	font.setBold(true);
	font.setPixelSize(m_curr_fontSize);
	font.setFamily("Microsoft YaHei");
	painter.setPen(pen);
	painter.setFont(font);
	painter.drawText(newImage.rect(), Qt::AlignCenter, getTextByUserRole(userRole));

	newImage.save(QString("./image/newRole%1.png").arg(userRole), "PNG");
}

void BarrageAssistantDlg::InitBarragePool(int barrageMsgType)
{
	QString strHtml;
	QDir dir;
	QString iamgePath = QApplication::applicationDirPath() + "/image";
	if (!dir.exists(iamgePath ))
	{
		dir.mkdir(iamgePath);
	}

	ImageAddText(getImagePathByUserLevel(m_pBarrageDetialInfo.nUserLevel), m_pBarrageDetialInfo.nUserLevel);
	roleImageAddText(getImagePathByUserRole(m_pBarrageDetialInfo.nUserRole), m_pBarrageDetialInfo.nUserRole);

	switch (barrageMsgType)
	{
	case USER_LOGIN_LIVE:
		if (m_pBarrageDetialInfo.nUserRole == USERROLE_HOMIE || m_pBarrageDetialInfo.nUserRole == USERROLE_ROOMADMIN
			|| m_pBarrageDetialInfo.nUserRole == USERROLE_SUPERADMIN)
		{
			strHtml = QString("<html> \
                                  <head> \
                                  <style> \
                                  font{font-size:%1px;color:white;line-height:14px;} #f{color:green;} \
                                  </style> \
                                  </head> \
                                  <body> \
                                  <font>%2 </font><img src=\"./image/newRole%3.png\"> <img src=\"./image/newLevel%4.png\"><font id=\"f\"> %5 </font><font> %6</font> \
                                  </body> \
                                  </html>").arg(m_curr_fontSize).arg(QStringLiteral("欢迎")).arg(m_pBarrageDetialInfo.nUserRole).arg(m_pBarrageDetialInfo.nUserLevel).arg(m_pBarrageDetialInfo.sUserName).arg(QStringLiteral("进入直播间"));
		}
		else
		{
			strHtml = QString("<html> \
                                  <head> \
                                  <style> \
                                  font{font-size:%1px;color:white;line-height:14px;} #f{color:green;} \
                                  </style> \
                                  </head> \
                                  <body> \
                                  <font>%2 </font><img src=\"./image/newLevel%3.png\"><font id=\"f\"> %4 </font><font> %5</font> \
                                  </body> \
                                  </html>").arg(m_curr_fontSize).arg(QStringLiteral("欢迎")).arg(m_pBarrageDetialInfo.nUserLevel).arg(m_pBarrageDetialInfo.sUserName).arg(QStringLiteral("进入直播间"));
		}
		break;
	case USER_SEND_MSG:
		if (m_pBarrageDetialInfo.nUserRole == USERROLE_HOMIE || m_pBarrageDetialInfo.nUserRole == USERROLE_ROOMADMIN
			|| m_pBarrageDetialInfo.nUserRole == USERROLE_SUPERADMIN)
		{
			strHtml = QString("<html> \
                                  <head> \
                                  <style> \
                                  font{font-size:%1px;color:white;line-height:14px;} #f{color:green;} \
                                  </style> \
                                  </head> \
                                  <body> \
                                  <img src=\"./image/newRole%2.png\"> <img src=\"./image/newLevel%3.png\"><font id=\"f\"> %4</font><font>%5</font> \
                                  </body> \
                                  </html>").arg(m_curr_fontSize).arg(m_pBarrageDetialInfo.nUserRole).arg(m_pBarrageDetialInfo.nUserLevel).arg(m_pBarrageDetialInfo.sUserName).arg(QStringLiteral("：") + m_pBarrageDetialInfo.sMsgInfo);
		}
		else
		{
			strHtml = QString("<html> \
                                  <head> \
                                  <style> \
                                  font{font-size:%1px;color:white;line-height:14px;} #f{color:green;} \
                                  </style> \
                                  </head> \
                                  <body> \
                                  <img src=\"./image/newLevel%2.png\"><font id=\"f\"> %3</font><font>%4</font> \
                                  </body> \
                                  </html>").arg(m_curr_fontSize).arg(m_pBarrageDetialInfo.nUserLevel).arg(m_pBarrageDetialInfo.sUserName).arg(QStringLiteral("：") + m_pBarrageDetialInfo.sMsgInfo);
		}
		break;
	default:
		break;
	}

	if (!strHtml.isEmpty())
	{
		ui.textEdit_content->append(strHtml);
	}
}

void BarrageAssistantDlg::InitGiftPool()
{
	QString msg = QStringLiteral("：给你送了X%1%2").arg(m_pBarrageDetialInfo.nGiftNum).arg(m_pBarrageDetialInfo.sGiftName);
	QString strHtml = QString("<html> \
                                  <head> \
                                  <style> \
                                  font{font-size:%1px;color:green;line-height:14px;} #f{color:white;} \
                                  </style> \
                                  </head> \
                                  <body> \
                                  <font>%2</font><font id=\"f\">%3</font> \
                                  </body> \
                                  </html>").arg(m_curr_fontSize).arg(m_pBarrageDetialInfo.sUserName).arg(msg);

	ui.textEdit_gift->append(strHtml);
}

QString BarrageAssistantDlg::getImagePathByUserLevel(int userLevel)
{
	QString imagePath;

	if (userLevel >= 1 && userLevel <= 20)
	{
		imagePath = QString(":/TQLive/images/level1.png");
	}
	else if (userLevel >= 21 && userLevel <= 40)
	{
		imagePath = QString(":/TQLive/images/level2.png");
	}
	else if (userLevel >= 41 && userLevel <= 60)
	{
		imagePath = QString(":/TQLive/images/level3.png");
	}
	else if (userLevel >= 61 && userLevel <= 80)
	{
		imagePath = QString(":/TQLive/images/level4.png");
	}
	else if (userLevel >= 81 && userLevel <= 100)
	{
		imagePath = QString(":/TQLive/images/level5.png");
	}

	return imagePath;
}

QString BarrageAssistantDlg::getImagePathByUserRole(int userRole)
{
	QString imagePath;

	if (userRole == USERROLE_HOMIE)
	{
		imagePath = QString(":/TQLive/images/role1.png");
	}
	else if (userRole == USERROLE_ROOMADMIN)
	{
		imagePath = QString(":/TQLive/images/role2.png");
	}
	else if (userRole == USERROLE_SUPERADMIN)
	{
		imagePath = QString(":/TQLive/images/role3.png");
	}

	return imagePath;
}

QString BarrageAssistantDlg::getTextByUserRole(int userRole)
{
	QString text;

	if (userRole == USERROLE_HOMIE)
	{
		text = QStringLiteral("主播");
	}
	else if (userRole == USERROLE_ROOMADMIN)
	{
		text = QStringLiteral("房管");
	}
	else if (userRole == USERROLE_SUPERADMIN)
	{
		text = QStringLiteral("超管");
	}

	return text;
}

void BarrageAssistantDlg::RespondHotKey(QString hotKey)
{
	if (hotKey == UserConfig::instance()->getBarrageHotKey())
	{
		this->close();
	}
	else if (hotKey == UserConfig::instance()->getMicHotKey())
	{
		emit onSigMicHotKeyTrigger();
	}
	else if (hotKey == UserConfig::instance()->getLiveHotKey())
	{
		emit onSigLiveHotKeyTrigger();
	}
}
