/***********************************************************************************
*								CustomUnderLineButton.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-10
*
* Description: 自定义带下划线按钮
***********************************************************************************/
#include "CustomUnderLineButton.h"

CustomUnderLineButton::CustomUnderLineButton(QWidget *parent)
	: QPushButton(parent)
{
	ui.setupUi(this);
	
	InitUI();
	InitConnect();
}

CustomUnderLineButton::~CustomUnderLineButton()
{

}

void CustomUnderLineButton::enterEvent(QEvent * event)
{
	Q_UNUSED(event)

	this->setCursor(Qt::PointingHandCursor);
}

void CustomUnderLineButton::leaveEvent(QEvent * event)
{
	Q_UNUSED(event)

	this->setCursor(Qt::ArrowCursor);
}

void CustomUnderLineButton::InitUI()
{
	this->setStyleSheet("background:transparent;padding:0px;border:none;");
	this->layout()->setSpacing(0);
	this->layout()->setMargin(0);
}

void CustomUnderLineButton::InitConnect()
{

}
