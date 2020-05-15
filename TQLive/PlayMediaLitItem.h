#pragma once

#include <QWidget>
#include <QStateMachine>
#include <QState>
#include "StructData.h"
#include "ui_PlayMediaLitItem.h"

class PlayMediaLitItem : public QWidget
{
	Q_OBJECT

public:
	PlayMediaLitItem(QWidget *parent = Q_NULLPTR);
	~PlayMediaLitItem();

public:
	void setState(int state);
	void setData(MediaSourceData mediaData);
	MediaSourceData getData() {
		return m_pMediaSourceData;
	};

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

signals:
	void TranslateToFreeState();
	void TranslateToActiveState();
	void TranslateToHoverState();
	void TranslateToSelectedState();

	void onSigDeleteItem(int id);
	void onSigUpItem(int id);
	void onSigDownItem(int id);

private slots:
    void onSwitchToFreeState();
    void onSwitchToActiveState();
	void onSwitchToHoverState();
	void onSwitchToSelectedState();

	void onDelItemClicked();
	void onUpItemClicked();
	void onDownItemClicked();

private:
	void InitUI();
	void InitConnect();
	//初始化状态机
	void InitStateMachine();

private:
	Ui::PlayMediaLitItem ui;
	MediaSourceData m_pMediaSourceData;

	//状态机
	QStateMachine *m_pStateMachine;
	QState *m_pFreeState;
	QState *m_pActiveState;
	QState *m_pHoverState;
	QState *m_pSelectedState;
};
