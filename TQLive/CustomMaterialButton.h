#pragma once

#include <QToolButton>
#include "StructData.h"

class CustomMaterialButton : public QToolButton
{
	Q_OBJECT

public:
	explicit CustomMaterialButton(MaterialTypeData materialTypeData, QWidget *parent = 0);
	~CustomMaterialButton();

signals:
	void onCustomMaterialButtonActive(int materialType);

protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private:
	void InitUI();
	void InitConnect();
	void InitStyle();

private:
	MaterialTypeData m_pMaterialTypeData;
};
