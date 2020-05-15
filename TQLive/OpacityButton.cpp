/***********************************************************************************
*								OpacityButton.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-12
*
* Description: 带有非透明度的按钮
***********************************************************************************/
#include "OpacityButton.h"
#include <QPainter>
#include <QMouseEvent>

OpacityButton::OpacityButton(QString text, QWidget *parent)
	: QWidget(parent)
	, m_sText(text)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	setMouseTracking(true);
}

OpacityButton::~OpacityButton()
{

}

void OpacityButton::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	QPainter painter(this);
	QLinearGradient grad(0, 0, width(), height());
	QGradientStops gs;

	gs << QGradientStop(0.0, QColor(0, 0, 0, 100))
		<< QGradientStop(0.5, QColor(0, 0, 0, 100))
		<< QGradientStop(1.0, QColor(0, 0, 0, 100));
	grad.setStops(gs);
	painter.fillRect(0, 0, width(), height(), grad);

	QFont font;
	font.setFamily(QStringLiteral("微软雅黑"));
	font.setPixelSize(12);
	painter.setFont(font);
	painter.setPen(QPen(QColor("#EEEEEE")));
	painter.drawText(QRect(0, 0, this->width(), this->height()), Qt::AlignCenter,
		QStringLiteral("%1").arg(m_sText));
}

void OpacityButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		emit onSigClicked();
	}
}
