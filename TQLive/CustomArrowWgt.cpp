/***********************************************************************************
*								CustomArrowWgt.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-11
*
* Description: 自定义带三角的气泡窗口
***********************************************************************************/
#include "CustomArrowWgt.h"
#include <QPaintEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QSound>

#define SHADOW_WIDTH 15                 // 窗口阴影宽度;
#define TRIANGLE_WIDTH 12               // 小三角的宽度;
#define TRIANGLE_HEIGHT 6              // 小三角的高度;
#define BORDER_RADIUS 4                 // 窗口边角的弧度;

CustomArrowWgt::CustomArrowWgt(QWidget *parent)
	: QDialog(parent)
	, m_nStartPosX((124 - TRIANGLE_WIDTH) / 2)
	, m_nTriangleW(TRIANGLE_WIDTH)
	, m_nTriangleH(TRIANGLE_HEIGHT)
{
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	// 设置阴影边框;
	auto shadowEffect = new QGraphicsDropShadowEffect(this);
	shadowEffect->setOffset(0, 0);
	shadowEffect->setColor(Qt::gray);
	shadowEffect->setBlurRadius(SHADOW_WIDTH);
	this->setGraphicsEffect(shadowEffect);
	
	setFixedSize(124, 126);
}

CustomArrowWgt::~CustomArrowWgt()
{

}

void CustomArrowWgt::setStartPos(int startPosX)
{
	m_nStartPosX = startPosX;
}

void CustomArrowWgt::setTriangleInfo(int width, int height)
{
	m_nTriangleW = width;
	m_nTriangleH = height;
}

void CustomArrowWgt::setCenterWidget(QWidget * widget)
{
	QHBoxLayout *hMainLayout = new QHBoxLayout(this);
	hMainLayout->addWidget(widget);
	hMainLayout->setSpacing(0);
	hMainLayout->setContentsMargins(SHADOW_WIDTH, SHADOW_WIDTH + m_nTriangleH, SHADOW_WIDTH, SHADOW_WIDTH + m_nTriangleH);
}

void CustomArrowWgt::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor("#333333"));

	QPolygon trianglePolygon;
	trianglePolygon << QPoint(m_nStartPosX, height() - SHADOW_WIDTH - m_nTriangleH);
	trianglePolygon << QPoint(m_nStartPosX + m_nTriangleW / 2, height() - SHADOW_WIDTH);
	trianglePolygon << QPoint(m_nStartPosX + m_nTriangleW, height() - SHADOW_WIDTH - m_nTriangleH);

	QPainterPath drawPath;
	drawPath.addRoundedRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, \
		width() - SHADOW_WIDTH * 2, height() - SHADOW_WIDTH * 2 - m_nTriangleH), \
		BORDER_RADIUS, BORDER_RADIUS);

	drawPath.addPolygon(trianglePolygon);
	painter.drawPath(drawPath);
}

void CustomArrowWgt::keyPressEvent(QKeyEvent *event)
{
	Q_UNUSED(event)

	if (event->key() == Qt::Key_1 && (event->modifiers() & Qt::ControlModifier)) {
		QSound::play(QStringLiteral("./Resource/欢呼.wav"));
	}
	else if (event->key() == Qt::Key_2 && (event->modifiers() & Qt::ControlModifier)) {
		QSound::play(QStringLiteral("./Resource/加油.wav"));
	}
	else if (event->key() == Qt::Key_3 && (event->modifiers() & Qt::ControlModifier)) {
		QSound::play(QStringLiteral("./Resource/嘘声.wav"));
	}

	return QDialog::keyPressEvent(event);
}
