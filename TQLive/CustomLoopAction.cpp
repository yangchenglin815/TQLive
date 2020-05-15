#include "CustomLoopAction.h"
#include <QHBoxLayout>

CustomLoopAction::CustomLoopAction(LoopTypeData loopTypeData, QWidget *parent)
	: QPushButton(parent)
	, m_pLoopTypeData(loopTypeData)
{
	InitUI();
	InitConnect();
}

CustomLoopAction::~CustomLoopAction()
{

}

void CustomLoopAction::enterEvent(QEvent * event)
{
	Q_UNUSED(event)

	this->setCursor(Qt::PointingHandCursor);

	m_pIconLabel->setStyleSheet(QString("background:transparent; border-image:url(%1);").arg(m_pLoopTypeData.sHoverIcon));
	m_pNameLabel->setStyleSheet(QString("font-family:Microsoft YaHei; font-size:12px; \
              color:#4AB134; background:transparent;"));
}

void CustomLoopAction::leaveEvent(QEvent * event)
{
	Q_UNUSED(event)

	this->setCursor(Qt::ArrowCursor);

	m_pIconLabel->setStyleSheet(QString("background:transparent; border-image:url(%1);").arg(m_pLoopTypeData.sIcon));
	m_pNameLabel->setStyleSheet(QString("font-family:Microsoft YaHei; font-size:12px; \
              color:#999999; background:transparent;"));
}

void CustomLoopAction::InitUI()
{
	setStyleSheet(QString("QPushButton{ background:transparent; }"));

	m_pIconLabel = new QLabel;
	m_pIconLabel->setFixedSize(QSize(13, 14));
	m_pIconLabel->setStyleSheet(QString("background:transparent; border-image:url(%1);").arg(m_pLoopTypeData.sIcon));

	m_pNameLabel = new QLabel;
	m_pNameLabel->setStyleSheet(QString("font-family:Microsoft YaHei; font-size:12px; \
              color:#999999; background:transparent;"));
	m_pNameLabel->setText(m_pLoopTypeData.sName);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->setSpacing(12);
	layout->setContentsMargins(10, 0, 0, 0);
	layout->addWidget(m_pIconLabel);
	layout->addWidget(m_pNameLabel);
	this->setLayout(layout);
}

void CustomLoopAction::InitConnect()
{
	connect(this, &QPushButton::clicked, [&]() {
		emit onLoopClicked(m_pLoopTypeData.nType);
	});
}
