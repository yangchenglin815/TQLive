/***********************************************************************************
*								TextTemplateCom.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2020-03
*
* Description: 文本素材模板类
***********************************************************************************/

#include "TextTemplateCom.h"
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QDebug>

TextTemplateCom::TextTemplateCom(QWidget *parent)
	: QWidget(parent)
{

}

TextTemplateCom::~TextTemplateCom()
{

}

void TextTemplateCom::RenderText(QString text, QString path)
{
	m_sText = text;
	m_sPath = path;

	update();
}

void TextTemplateCom::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	if (m_sPath.isEmpty())
		return;

	QPainter painter;
	QPixmap pixmap(":/TQLive/images/textHead.png");

	int pWidth  = pixmap.width();
	int pHeight = pixmap.height();
	int textLength = getTextLength();
	int xPos = 0;
	int yPos = 0;
	int leftSpace = 33;
	int topSpace = 24;
	int rWidth = textLength + 58 > 213 ? textLength + 48 + 48 : 213;
	
	int rHight = pHeight - topSpace;
	int triWidth = 35;
	int triHeight = 17;
	int triSpace = 7;
	int space = xPos + leftSpace + rWidth + 12 - triWidth;

	QPixmap map(leftSpace + rWidth + 12, pHeight);
	map.fill(Qt::transparent);
	painter.begin(&map);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QRect rect(xPos + leftSpace, yPos + topSpace, rWidth, rHight);
	QLinearGradient linear(rect.topLeft(), rect.bottomRight());
	linear.setColorAt(0, QColor("#0967B2"));
	linear.setColorAt(1, QColor("#2CDCDF"));
	linear.setSpread(QGradient::PadSpread);
	painter.setBrush(linear);
	painter.setPen(Qt::NoPen);

	QPointF points[4] = {
		QPointF(xPos + leftSpace, yPos + topSpace),
		QPointF(xPos + leftSpace + rWidth + 12, yPos + topSpace),
		QPointF(xPos + leftSpace + rWidth, yPos + pHeight),
		QPointF(xPos + leftSpace, yPos + pHeight)
	};
	painter.drawPolygon(points, 4);
	painter.drawPixmap(xPos, yPos, pWidth, pHeight, pixmap);

	QPointF points_[3] = {
		QPointF(space, yPos + triSpace),
		QPointF(space + triWidth, yPos + triSpace + triHeight),
		QPointF(space, yPos + triSpace + triHeight)
	};
	painter.setBrush(QColor("#FFFFFF"));
	painter.drawPolygon(points_, 3);

	QFont font;
	font.setFamily(QStringLiteral("微软雅黑"));
	font.setPixelSize(18);
	painter.setFont(font);
	painter.setPen(QPen(QColor("#FFFFFF"), 1, Qt::SolidLine));
	painter.drawText(QRect(xPos + pWidth + 10, yPos + topSpace, rWidth - 48, rHight),
		Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("%1").arg(m_sText));
	painter.end();

	map.save(m_sPath, "PNG");
	QPainter p(this);
	p.drawPixmap(39, 0, map);
}

int TextTemplateCom::getTextLength() const
{
	int count = 0;
	for (int i = 0; i < m_sText.count(); i++) {
		QChar cha = m_sText.at(i);
		ushort uni = cha.unicode();
		if (uni >= 0x4E00 && uni <= 0x9FA5) {
			count += 18;
		}
		else {
			count += 9;
		}
	}
	return count;
}
