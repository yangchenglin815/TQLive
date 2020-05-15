#pragma once

#include <QWidget>
#include "ui_CTQMainWidget.h"
#include <QMouseEvent>
#include <QMainWindow>

#define PADDING 2
enum Direction { UP = 0, DOWN = 1, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };

class CTQMainWidget : public QMainWindow
{
	Q_OBJECT

public:
	CTQMainWidget(QWidget *parent = Q_NULLPTR);
	~CTQMainWidget();
	void region(const QPoint &cursorGlobalPoint);

protected:
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);

public:
	Ui::CTQMainWidget ui;
	bool isLeftPressDown;  // 判断左键是否按下
	QPoint dragPosition;   // 窗口移动拖动时需要记住的点 
	Direction dir;        // 窗口大小改变时，记录改变方向
};
