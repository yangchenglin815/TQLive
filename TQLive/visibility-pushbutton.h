#pragma once

#include <QPushButton>
#include <QPixmap>

class QPaintEvernt;

class VisibilityPushButton : public QPushButton
{
	Q_OBJECT

	QPixmap buttonImage;

public:
	VisibilityPushButton(int buttonType);
	~VisibilityPushButton();

protected:
	void paintEvent(QPaintEvent *event) override;
};
