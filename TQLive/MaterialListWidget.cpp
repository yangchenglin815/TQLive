/***********************************************************************************
*								MaterialListWidget.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-10
*
* Description: 素材列表
***********************************************************************************/
#include "MaterialListWidget.h"
#include "CustomMaterialButton.h"
#include "UserConfig.h"

MaterialListWidget::MaterialListWidget(QWidget *parent)
	: QListWidget(parent)
{
	InitUI();
	InitConnect();
}

MaterialListWidget::~MaterialListWidget()
{

}

void MaterialListWidget::onCustomMaterialButtonActive(int materialType)
{
	switch (materialType)
	{
	case MATERIAL_CAMERA:
		emit onSigAddMaterialOfCamera();
		break;
	case MATERIAL_VIDEO:
		emit onSigAddMaterialOfVideo();
		break;
	case MATERIAL_IMAGE:
		emit onSigAddMaterialOfImage();
		break;
	case MATERIAL_TEXT:
		emit onSigAddMaterialOfText();
		break;
	case MATERIAL_SOURCE:
		emit onSigAddMaterialOfSource();
		break;
	case MATERIAL_CAPTURE:
		emit onSigAddMaterialOfCapture();
		break;
	case MATERIAL_FULLSCREEN:
		emit onSigAddMaterialOfFullScreen();
		break;
	case MATERIAL_WINDOW:
		emit onSigAddMaterialOfWindow();
		break;
	default:
		break;
	}
}

void MaterialListWidget::InitUI()
{
	this->setViewMode(QListView::IconMode);
	this->setSpacing(9);
	this->setResizeMode(QListWidget::Adjust);
	this->setMovement(QListWidget::Static);

	auto materialTypeData = UserConfig::instance()->getMaterialIni();
	for (auto& it : materialTypeData)
	{
		CustomMaterialButton *button = new CustomMaterialButton(it);
		button->setCursor(Qt::PointingHandCursor);
		connect(button, SIGNAL(onCustomMaterialButtonActive(int)), this, SLOT(onCustomMaterialButtonActive(int)));
		QListWidgetItem *itemWidget = new QListWidgetItem(this);
		itemWidget->setSizeHint(QSize(46, 50));
		this->addItem(itemWidget);
		this->setItemWidget(itemWidget, button);
	}
}

void MaterialListWidget::InitConnect()
{

}
