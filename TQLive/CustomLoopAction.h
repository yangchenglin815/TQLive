#pragma once

#include <QPushButton>
#include <QLabel>
#include "StructData.h"

class CustomLoopAction : public QPushButton
{
	Q_OBJECT

public:
	CustomLoopAction(LoopTypeData loopTypeData, QWidget *parent = Q_NULLPTR);
	~CustomLoopAction();

signals:
	void onLoopClicked(int type);

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private:
	void InitUI();
	void InitConnect();

private:
	LoopTypeData m_pLoopTypeData;
	QLabel *m_pIconLabel;
	QLabel *m_pNameLabel;
};
