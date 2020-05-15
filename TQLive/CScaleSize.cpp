#include "CScaleSize.h"
#include <QPoint>
#include <QWidget>

#define ZONE_MEASURE 5

CScaleSize::CScaleSize(QObject *parent)
	: QObject(parent)
{
}

CScaleSize::~CScaleSize()
{
}

EN_ZONE_TYPE CScaleSize::CalcCursorLocation(QWidget *pSrcWidget, const QPoint &ptCursorPos)
{
	QRect rcArea = pSrcWidget->rect();
	QPoint ptLeftTop = rcArea.topLeft();
	QPoint ptRightBottom = rcArea.bottomRight();

	int x = ptCursorPos.x();
	int y = ptCursorPos.y();

	int nWidth = rcArea.width(), nHeight = rcArea.height();

	// æ≈π¨∏Òº∆À„
	QRect szDropArea[9];
	szDropArea[0].setRect(ptLeftTop.x() + ZONE_MEASURE, ptLeftTop.y() + ZONE_MEASURE, nWidth - ZONE_MEASURE - ZONE_MEASURE, nHeight - ZONE_MEASURE - ZONE_MEASURE);
	szDropArea[1].setRect(ptLeftTop.x() + ZONE_MEASURE, ptLeftTop.y(), nWidth - ZONE_MEASURE - ZONE_MEASURE, ZONE_MEASURE);
	szDropArea[2].setRect(ptLeftTop.x() + ZONE_MEASURE, ptRightBottom.y() - ZONE_MEASURE, nWidth - ZONE_MEASURE - ZONE_MEASURE, ZONE_MEASURE);

	szDropArea[3].setRect(ptLeftTop.x(), ptLeftTop.y(), ZONE_MEASURE, ZONE_MEASURE);
	szDropArea[4].setRect(ptLeftTop.x(), ptLeftTop.y() + ZONE_MEASURE, ZONE_MEASURE, nHeight - ZONE_MEASURE - ZONE_MEASURE);
	szDropArea[5].setRect(ptLeftTop.x(), ptRightBottom.y() - ZONE_MEASURE, ZONE_MEASURE, ZONE_MEASURE);

	szDropArea[6].setRect(ptRightBottom.x() - ZONE_MEASURE, ptLeftTop.y(), ZONE_MEASURE, ZONE_MEASURE);
	szDropArea[7].setRect(ptRightBottom.x() - ZONE_MEASURE, ptLeftTop.y() + ZONE_MEASURE, ZONE_MEASURE, nHeight - ZONE_MEASURE - ZONE_MEASURE);
	szDropArea[8].setRect(ptRightBottom.x() - ZONE_MEASURE, ptRightBottom.y() - ZONE_MEASURE, ZONE_MEASURE, ZONE_MEASURE);

	for (int i = 0; i < 9; i++)
	{
		if (true == szDropArea[i].contains(ptCursorPos))
		{
			return (EN_ZONE_TYPE)i;
		}
	}

	return NOTCONTAIN;
}

void CScaleSize::ScaleByCursorZoneType(QWidget *pDstWidget, EN_ZONE_TYPE eZoneType, const QPoint &ptCursorPos, int nMinWidth, int nMinHeight)
{
	QRect rcWindowArea = pDstWidget->rect();
	QPoint ptLeftTop = pDstWidget->mapToGlobal(rcWindowArea.topLeft());
	QPoint ptRightBottom = pDstWidget->mapToGlobal(rcWindowArea.bottomRight());

	QRect rcDestArea(ptLeftTop, ptRightBottom);

	switch (eZoneType) {
	case LEFT:
	{
		if (ptRightBottom.x() - ptCursorPos.x() <= nMinWidth) {
			rcDestArea.setX(ptLeftTop.x());
		}
		else {
			rcDestArea.setX(ptCursorPos.x());
		}

		break;
	}
	case RIGHT:
	{
		rcDestArea.setWidth(ptCursorPos.x() - ptLeftTop.x());
		break;
	}
	case TOP:
	{
		//if (ptRightBottom.y() - ptCursorPos.y() <= nMinHeight) {

		//	//rcDestArea.setY(ptLeftTop.y());
		//	rcDestArea.setY(0);
		//}
		//else {

		//	rcDestArea.setY(ptCursorPos.y());
		//}

		rcDestArea.setHeight(ptRightBottom.y() - ptCursorPos.y());
		qDebug("cursor y:%d rightBottom y:%d height:%d", ptCursorPos.y(), ptRightBottom.y(), ptRightBottom.y() - ptCursorPos.y());

		break;
	}
	case BOTTOM:
	{
		rcDestArea.setHeight(ptCursorPos.y() - ptLeftTop.y());
		break;
	}
	case LEFTTOP:
		if (ptRightBottom.x() - ptCursorPos.x() <= nMinWidth)
			rcDestArea.setX(ptLeftTop.x());
		else
			rcDestArea.setX(ptCursorPos.x());
		if (ptRightBottom.y() - ptCursorPos.y() <= nMinHeight)
			rcDestArea.setY(ptLeftTop.y());
		else
			rcDestArea.setY(ptCursorPos.y());
		break;
	case RIGHTTOP:
		rcDestArea.setWidth(ptCursorPos.x() - ptLeftTop.x());
		rcDestArea.setY(ptCursorPos.y());
		break;
	case LEFTBOTTOM:
		rcDestArea.setX(ptCursorPos.x());
		rcDestArea.setHeight(ptCursorPos.y() - ptLeftTop.y());
		break;
	case RIGHTBOTTOM:
		rcDestArea.setWidth(ptCursorPos.x() - ptLeftTop.x());
		rcDestArea.setHeight(ptCursorPos.y() - ptLeftTop.y());
		break;
	default:
		break;
	}

	pDstWidget->setGeometry(rcDestArea);

	qDebug("width:%d height:%d", rcDestArea.width(), rcDestArea.height());
}
