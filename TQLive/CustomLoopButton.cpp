/***********************************************************************************
*								CustomLoopButton.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-11
*
* Description: 自定义列表循环按钮
***********************************************************************************/
#include "CustomLoopButton.h"

CustomLoopButton::CustomLoopButton(QWidget *parent)
	: QPushButton(parent)
	, m_nLoopType(LOOP_CYCLE)
{
	ui.setupUi(this);
}

CustomLoopButton::~CustomLoopButton()
{

}

void CustomLoopButton::setLoopType(int type)
{
	m_nLoopType = type;

	QString image;
	QString text;
	switch (m_nLoopType)
	{
	case LOOP_RANDOM:
		image = "video_icon_random_normal.png";
		text = QStringLiteral("随机");
		break;
	case LOOP_SINGLE:
		image = "video_icon_a single_normal.png";
		text = QStringLiteral("单个");
		break;
	case LOOP_CYCLE:
		image = "video_icon_cycle_normal.png";
		text = QStringLiteral("列表");
		break;
	default:
		break;
	}

	ui.label->setText(text);
	ui.widget->setStyleSheet(QString("#label{ color:#EEEEEE; }  \
        #label_icon{ border-image:url(:/TQLive/images/%1); }").arg(image));
}

void CustomLoopButton::enterEvent(QEvent *event)
{
	Q_UNUSED(event)

	this->setCursor(Qt::PointingHandCursor);

	QString image;
	switch (m_nLoopType)
	{
	case LOOP_RANDOM:
		image = "video_icon_random_hover.png";
		break;
	case LOOP_SINGLE:
		image = "video_icon_a single_hover.png";
		break;
	case LOOP_CYCLE:
		image = "video_icon_cycle_hover.png";
		break;
	default:
		break;
	}
	ui.widget->setStyleSheet(QString("QLabel{ color:#4AB134; }  \
		#label_icon{ border-image: url(:/TQLive/images/%1); }").arg(image));
}

void CustomLoopButton::leaveEvent(QEvent * event)
{
	Q_UNUSED(event)

	this->setCursor(Qt::ArrowCursor);

	QString image;
	switch (m_nLoopType)
	{
	case LOOP_RANDOM:
		image = "video_icon_random_normal.png";
		break;
	case LOOP_SINGLE:
		image = "video_icon_a single_normal.png";
		break;
	case LOOP_CYCLE:
		image = "video_icon_cycle_normal.png";
		break;
	default:
		break;
	}
	ui.widget->setStyleSheet(QString("#label{ color:#EEEEEE; }  \
        #label_icon{ border-image:url(:/TQLive/images/%1); }").arg(image));
}
