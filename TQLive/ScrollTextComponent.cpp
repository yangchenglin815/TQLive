/***********************************************************************************
*								ScrollTextComponent.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 上下滚动字幕组件
***********************************************************************************/
#include "ScrollTextComponent.h"
#include <QResizeEvent>
#include <QDesktopServices>

ScrollTextComponent::ScrollTextComponent(QWidget *parent)
	: QWidget(parent)
	, m_TopPropertyAnimation(nullptr)
	, m_CurrentPropertyAnimation(nullptr)
	, m_TopLabel(nullptr)
    , m_CurrentLabel(nullptr)
{
	InitUI();
}

ScrollTextComponent::~ScrollTextComponent()
{

}

void ScrollTextComponent::setCurrentText(QString text, QString url)
{
	m_pCurrentUrl = url;

	if (m_CurrentLabel->text().isEmpty())
	{
		m_CurrentLabel->setGeometry(0, 0, this->width(), this->height());
		m_TopLabel->setText(text);
		m_CurrentLabel->setText(text);

		if (m_CurrentLabel->isHidden()) {
			m_CurrentLabel->show();
		}
	}
	else
	{
		QString str = m_CurrentLabel->text();
		m_TopLabel->setText(str);
		m_CurrentLabel->setText(text);

		if (m_CurrentLabel->isHidden()) {
			m_CurrentLabel->show();
		}
		if (m_TopLabel->isHidden()) {
			m_TopLabel->show();
		}

		m_TopPropertyAnimation->stop();
		m_CurrentPropertyAnimation->stop();
		m_TopPropertyAnimation->start();
		m_CurrentPropertyAnimation->start();
	}
}

void ScrollTextComponent::resizeEvent(QResizeEvent * event)
{
	int width = event->size().width();
	int height = event->size().height();

	m_TopPropertyAnimation->setStartValue(QRect(0, 0, width, height));
	m_TopPropertyAnimation->setEndValue(QRect(0, -height, width, height));

	m_CurrentPropertyAnimation->setStartValue(QRect(0, height * 2, width, height));
	m_CurrentPropertyAnimation->setEndValue(QRect(0, 0, width, height));

	return QWidget::resizeEvent(event);
}

void ScrollTextComponent::enterEvent(QEvent * event)
{
	Q_UNUSED(event);

	setCursor(QCursor(Qt::PointingHandCursor));
}

void ScrollTextComponent::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);

	setCursor(QCursor(Qt::ArrowCursor));
}

void ScrollTextComponent::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QDesktopServices::openUrl(m_pCurrentUrl);
	}
}

void ScrollTextComponent::InitUI()
{
	m_TopLabel = new QLabel(this);
	m_CurrentLabel = new QLabel(this);

	QString style = QString("QLabel{background:transparent;color:#DCDCDC;}");
	m_TopLabel->setStyleSheet(style);
	m_CurrentLabel->setStyleSheet(style);

	m_TopLabel->hide();
	m_CurrentLabel->hide();

	m_TopPropertyAnimation = new QPropertyAnimation(this);
	m_TopPropertyAnimation->setTargetObject(m_TopLabel);
	m_TopPropertyAnimation->setPropertyName("geometry");
	m_TopPropertyAnimation->setDuration(700);

	m_CurrentPropertyAnimation = new QPropertyAnimation(this);
	m_CurrentPropertyAnimation->setTargetObject(m_CurrentLabel);
	m_CurrentPropertyAnimation->setPropertyName("geometry");
	m_CurrentPropertyAnimation->setDuration(700);

	setMouseTracking(true);
}
