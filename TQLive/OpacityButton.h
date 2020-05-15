#pragma once

#include <QWidget>

class OpacityButton : public QWidget
{
	Q_OBJECT

public:
	OpacityButton(QString text, QWidget *parent = Q_NULLPTR);
	~OpacityButton();

signals:
	void onSigClicked();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);

private:
	QString m_sText;
};
