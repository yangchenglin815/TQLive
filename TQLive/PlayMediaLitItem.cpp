/***********************************************************************************
*								PlayMediaLitItem.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-11
*
* Description: 媒体播放列表项
***********************************************************************************/

#include "PlayMediaLitItem.h"
#include "AddMediaSourceDlg.h"
#include "CommonFunc.h"

PlayMediaLitItem::PlayMediaLitItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	InitUI();
	InitConnect();
	InitStateMachine();
}

PlayMediaLitItem::~PlayMediaLitItem()
{
	
}

void PlayMediaLitItem::setState(int state)
{
	switch (state)
	{
	case STATE_COMMON:
		TranslateToFreeState();
		break;
	case STATE_ACTIVE:
		TranslateToActiveState();
		break;
	case STATE_HOVER:
		TranslateToHoverState();
		break;
	case STATE_SELECTED:
		TranslateToSelectedState();
		break;
	default:
		break;
	}
}

void PlayMediaLitItem::setData(MediaSourceData mediaData)
{
	m_pMediaSourceData = mediaData;
	setToolTip(mediaData.sMediaName);

	ui.label_mediaName->setText(m_pMediaSourceData.sMediaName);
	ui.label_playTime->setText(CommonFunc::getTimeDurationByInt(m_pMediaSourceData.nTotalTimeDuration));
}

void PlayMediaLitItem::enterEvent(QEvent * event)
{
	Q_UNUSED(event)

	if (m_pMediaSourceData.bActive) {
		setState(STATE_SELECTED);
	}
	else {
		setState(STATE_HOVER);
	}
}

void PlayMediaLitItem::leaveEvent(QEvent *event)
{
	Q_UNUSED(event)

	if (m_pMediaSourceData.bActive) {
		setState(STATE_ACTIVE);
	}
	else {
		setState(STATE_COMMON);
	}
}

void PlayMediaLitItem::onSwitchToFreeState()
{
	if (m_pMediaSourceData.bActive) {
		setState(STATE_ACTIVE);
		return;
	}

	ui.wgt_control->setHidden(true);
	ui.label_playTime->setHidden(true);

	this->setStyleSheet(QString("#label_selected { background: transparent; } \
           #label_mediaName { color: #999999; background: transparent; } \
           #wgt_control { background: transparent; }"));
}

void PlayMediaLitItem::onSwitchToActiveState()
{
	ui.wgt_control->setHidden(true);
	ui.label_playTime->setHidden(false);

	this->setStyleSheet(QString("#label_selected { background: #4AB134; } \
           #label_mediaName { color: #4AB134; background: transparent; } \
           #wgt_control { background: transparent; }"));
}

void PlayMediaLitItem::onSwitchToHoverState()
{
	ui.wgt_control->setHidden(false);
	ui.label_playTime->setHidden(true);

	this->setStyleSheet(QString("#label_selected { background: transparent; } \
           #label_mediaName { color: #999999; background: transparent; } \
           #wgt_control { background: transparent; }"));
}

void PlayMediaLitItem::onSwitchToSelectedState()
{
	ui.wgt_control->setHidden(false);
	ui.label_playTime->setHidden(true);

	this->setStyleSheet(QString("#label_selected { background: #4AB134; } \
           #label_mediaName { color: #4AB134; background: transparent; } \
           #wgt_control { background: transparent; }"));
}

void PlayMediaLitItem::onDelItemClicked()
{
	emit onSigDeleteItem(m_pMediaSourceData.nId);
}

void PlayMediaLitItem::onUpItemClicked()
{
	emit onSigUpItem(m_pMediaSourceData.nId);
}

void PlayMediaLitItem::onDownItemClicked()
{
	emit onSigDownItem(m_pMediaSourceData.nId);
}

void PlayMediaLitItem::InitUI()
{
	ui.button_up->setCursor(Qt::PointingHandCursor);
	ui.button_down->setCursor(Qt::PointingHandCursor);
	ui.button_del->setCursor(Qt::PointingHandCursor);
}

void PlayMediaLitItem::InitConnect()
{
	connect(ui.button_del, SIGNAL(clicked()), this, SLOT(onDelItemClicked()));
	connect(ui.button_up, SIGNAL(clicked()), this, SLOT(onUpItemClicked()));
	connect(ui.button_down, SIGNAL(clicked()), this, SLOT(onDownItemClicked()));
}

void PlayMediaLitItem::InitStateMachine()
{
	m_pStateMachine  = new QStateMachine;
	m_pFreeState     = new QState(m_pStateMachine);
	m_pActiveState   = new QState(m_pStateMachine);
	m_pHoverState    = new QState(m_pStateMachine);
	m_pSelectedState = new QState(m_pStateMachine);
	
	connect(m_pFreeState, SIGNAL(entered()), this, SLOT(onSwitchToFreeState()));
	connect(m_pActiveState, SIGNAL(entered()), this, SLOT(onSwitchToActiveState()));
	connect(m_pHoverState, SIGNAL(entered()), this, SLOT(onSwitchToHoverState()));
	connect(m_pSelectedState, SIGNAL(entered()), this, SLOT(onSwitchToSelectedState()));

	m_pFreeState->addTransition(this, SIGNAL(TranslateToActiveState()), m_pActiveState);
	m_pFreeState->addTransition(this, SIGNAL(TranslateToHoverState()), m_pHoverState);
	m_pActiveState->addTransition(this, SIGNAL(TranslateToFreeState()), m_pFreeState);
	m_pActiveState->addTransition(this, SIGNAL(TranslateToHoverState()), m_pHoverState);
	m_pActiveState->addTransition(this, SIGNAL(TranslateToSelectedState()), m_pSelectedState);
	m_pHoverState->addTransition(this, SIGNAL(TranslateToFreeState()), m_pFreeState);
	m_pHoverState->addTransition(this, SIGNAL(TranslateToActiveState()), m_pActiveState);
	m_pSelectedState->addTransition(this, SIGNAL(TranslateToActiveState()), m_pActiveState);
	

	m_pStateMachine->setInitialState(m_pFreeState);
	m_pStateMachine->start();
}
