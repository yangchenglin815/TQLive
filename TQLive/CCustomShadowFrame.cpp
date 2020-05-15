#include "stdafx.h"
#include "CCustomShadowFrame.h"
#include <QPainter>
#include <QtCore/qmath.h>

CCustomShadowFrame::CCustomShadowFrame()
	: m_nShadowFrameWidth(5)
	, m_nRoundedRadius(5)
{

}

CCustomShadowFrame::~CCustomShadowFrame()
{

}

void CCustomShadowFrame::Draw(QWidget *pDestWidget)
{
	QPainter painter(pDestWidget);
	painter.setRenderHint(QPainter::Antialiasing, true);
	
	QPainterPath painterPath;
	painterPath.setFillRule(Qt::WindingFill);

	int cx = m_nShadowFrameWidth, cy = m_nShadowFrameWidth, radius = m_nRoundedRadius;
	int w = pDestWidget->width() - cx * 2;
	int h = pDestWidget->height() - cy * 2;

	// 绘制窗口客户区背景
	painterPath.addRoundedRect(cx, cy, w, h, 0, 0);
	// painter.fillPath(painterPath, QBrush(Qt::white));
	// painter.fillPath(painterPath, QBrush(Qt::red));

	painter.fillPath(painterPath, QBrush(QColor(58, 57, 58)));

	// 绘制窗口边缘（外边距区域）渐进透明效果
	QColor color(0, 0, 0, 40);
	for(int i = 0; i < cx; i++)  
	{  
		QPainterPath path;  
		path.setFillRule(Qt::WindingFill);  
		path.addRoundedRect(cx - i, cy - i, pDestWidget->width() - ( cx - i) * 2, pDestWidget->height() - (cy - i) * 2, radius, radius);

		//int x = qSqrt(cx);
		//int nAlpha = qSqrt(cx) - qSqrt(i) * 50;
		int nAlpha = 150 - qSqrt(i) * 50;
		color.setAlpha(nAlpha);
		painter.setPen(color);  
		painter.drawPath(path);  
	}  
}