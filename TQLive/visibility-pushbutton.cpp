#include <QPaintEvent>
#include <QPixmap>
#include <QPainter>
#include "visibility-pushbutton.h"
#include "EnumData.h"

#include <util/c99defs.h>

VisibilityPushButton::VisibilityPushButton(int buttonType)
	: QPushButton()
{
	QString buttonFile;
	if (devicePixelRatio() >= 2) {
		switch (buttonType)
		{
		case SOURCEITEM_SETTINGS:
			buttonFile = ":/TQLive/images/live tool_menu_icon_settings_2x.png";
			break;
		case SOURCEITEM_UP:
			buttonFile = ":/TQLive/images/live tool_icon_up_2x.png";
			break;
		case SOURCEITEM_DOWN:
			buttonFile = ":/TQLive/images/live tool_icon_down_2x.png";
			break;
		case SOURCEITEM_DEL:
			buttonFile = ":/TQLive/images/live tool_icon_delete_2x.png";
			break;
		default:
			return;
		}
	}
	else {
		switch (buttonType)
		{
		case SOURCEITEM_SETTINGS:
			buttonFile = ":/TQLive/images/live tool_menu_icon_settings.png";
			break;
		case SOURCEITEM_UP:
			buttonFile = ":/TQLive/images/live tool_icon_up.png";
			break;
		case SOURCEITEM_DOWN:
			buttonFile = ":/TQLive/images/live tool_icon_down.png";
			break;
		case SOURCEITEM_DEL:
			buttonFile = ":/TQLive/images/live tool_icon_delete.png";
			break;
		default:
			return;
		}
	}

	buttonImage = QPixmap::fromImage(QImage(buttonFile));
	setMinimumSize(buttonImage.size());

	setStyleSheet("outline: none;");
}

VisibilityPushButton::~VisibilityPushButton()
{

}

void VisibilityPushButton::paintEvent(QPaintEvent * event)
{
	UNUSED_PARAMETER(event);

	QPixmap &pixmap = buttonImage;
	QImage image(pixmap.size(), QImage::Format_ARGB32);

	QPainter draw(&image);
	draw.setCompositionMode(QPainter::CompositionMode_Source);
	draw.drawPixmap(0, 0, pixmap.width(), pixmap.height(), pixmap);
	draw.setCompositionMode(QPainter::CompositionMode_SourceIn);
	draw.fillRect(QRectF(QPointF(0.0f, 0.0f), pixmap.size()),
		palette().color(foregroundRole()));

	QPainter p(this);
	p.drawPixmap(0, 0, buttonImage.size().width(), buttonImage.size().height(), QPixmap::fromImage(image));
}
