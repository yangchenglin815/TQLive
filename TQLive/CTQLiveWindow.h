#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CTQLiveWindow.h"
#include "CCustomMainWindow.h"

#define PADDING 2



class CTQLiveWindow : public QMainWindow
{
	Q_OBJECT

	enum Direction { UP = 0, DOWN = 1, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };

public:
	CTQLiveWindow(QWidget *parent = Q_NULLPTR);
	~CTQLiveWindow();
	void region(const QPoint &cursorGlobalPoint);

protected:
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);

protected:
	//virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public:
	Ui::CTQLiveWindowClass ui;
	bool isLeftPressDown;  // 判断左键是否按下
	QPoint dragPosition;   // 窗口移动拖动时需要记住的点 
	Direction dir;        // 窗口大小改变时，记录改变方向
};
