#pragma once

#include <QDialog>

class CustomArrowWgt : public QDialog
{
	Q_OBJECT

public:
	CustomArrowWgt(QWidget *parent = Q_NULLPTR);
	~CustomArrowWgt();

	//设置小三角的起始位置
	void setStartPos(int startPosX);    

	//设置小三角的宽和高
	void setTriangleInfo(int width, int height);

	//设置中间区域的widget
	void setCenterWidget(QWidget *widget);

protected:
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	int m_nStartPosX;
	int m_nTriangleW;
	int m_nTriangleH;
};
