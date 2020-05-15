#include <QPaintEvent>
#include <QPixmap>
#include <QPainter>
#include "visibility-checkbox.hpp"

#include <util/c99defs.h>

VisibilityCheckBox::VisibilityCheckBox() : QCheckBox()
{
	QString checkedFile;
	QString uncheckedFile;
	if (devicePixelRatio() >= 2) {
		checkedFile = ":/settings/images/live tool_icon_According_to_2x.png";
		uncheckedFile = ":/settings/images/live tool_icon_hidden_2x.png";
	} else {
		checkedFile = ":/settings/images/live tool_icon_According_to.png";
		uncheckedFile = ":/settings/images/live tool_icon_hidden.png";
	}
	checkedImage = QPixmap::fromImage(QImage(checkedFile));
	uncheckedImage = QPixmap::fromImage(QImage(uncheckedFile));
	setMinimumSize(13, 9);

	setStyleSheet("outline: none;");
}

void VisibilityCheckBox::paintEvent(QPaintEvent *event)
{
	UNUSED_PARAMETER(event);

	bool result = isChecked();

	QPixmap &pixmap = isChecked() ? checkedImage : uncheckedImage;
	QImage image(pixmap.size(), QImage::Format_ARGB32);

	QPainter draw(&image);
	draw.setCompositionMode(QPainter::CompositionMode_Source);
	draw.drawPixmap(0, 0, pixmap.width(), pixmap.height(), pixmap);
	draw.setCompositionMode(QPainter::CompositionMode_SourceIn);
	draw.fillRect(QRectF(QPointF(0.0f, 0.0f), pixmap.size()),
			palette().color(foregroundRole()));

	QPainter p(this);
	p.drawPixmap(0, 0, 13, 9, QPixmap::fromImage(image));
}
