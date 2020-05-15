#pragma once

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

class ScrollTextComponent : public QWidget
{
	Q_OBJECT

public:
	explicit ScrollTextComponent(QWidget *parent = Q_NULLPTR);
	~ScrollTextComponent();

public:
	/*设置文字内容*/
	void setCurrentText(QString text, QString url);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);

private:
	/*初始化UI*/
	void InitUI();

private:
	QPropertyAnimation *m_TopPropertyAnimation;
	QPropertyAnimation *m_CurrentPropertyAnimation;
	QString m_pCurrentUrl;

	QLabel *m_TopLabel;
	QLabel *m_CurrentLabel;
};
