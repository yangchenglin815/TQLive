#pragma once

#include <QObject>

enum EN_ZONE_TYPE
{
	CENTRE,
	TOP,
	BOTTOM,
	LEFTTOP,
	LEFT,
	LEFTBOTTOM,
	RIGHTTOP,
	RIGHT,
	RIGHTBOTTOM,
	NOTCONTAIN
};

class CScaleSize : public QObject
{
	Q_OBJECT

public:
	CScaleSize(QObject *parent);
	~CScaleSize();

	static EN_ZONE_TYPE CalcCursorLocation(QWidget *pSrcWidget, const QPoint &ptCursorPos);
	static void ScaleByCursorZoneType(QWidget *pDstWidget, EN_ZONE_TYPE eZoneType, const QPoint &ptCursorPos, int nMinWidth, int nMinHeight);
};
