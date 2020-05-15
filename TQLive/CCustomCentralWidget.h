#pragma once

#include <QWidget>

class CCustomCentralWidget : public QWidget
{
	Q_OBJECT

public:
	CCustomCentralWidget(QWidget *parent);
	~CCustomCentralWidget();

	//bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
	//void mousePressEvent(QMouseEvent* e) override;
	//void mouseMoveEvent(QMouseEvent* e) override;

protected:
	void paintEvent(QPaintEvent *event);

//private:
//	int m_bouderWidth;
//	QPoint m_curPos;
};
