#include "CCompetitionButton.h"
#include <QPainter>
#include <QStyleOption>

CCompetitionButton::CCompetitionButton(QWidget *parent)
	: QPushButton(parent)
	, m_nUserData(0)
{
}

CCompetitionButton::~CCompetitionButton()
{
}

void CCompetitionButton::SetUserData(int nUserData)
{
	m_nUserData = nUserData;
}

int CCompetitionButton::GetUserID()
{
	return m_nUserData;
}

void CCompetitionButton::paintEvent(QPaintEvent *event)
{
	//QPushButton::paintEvent(event);
	QPainter painter(this);

	QStyleOption option;
	option.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

	QString qsButtonText = this->text();
	if (qsButtonText != "")
	{
		QTextOption textOption(Qt::AlignCenter);
		QRect rect = this->rect();
		QFontMetrics fm = painter.fontMetrics();
		QString qsElidedText = fm.elidedText(text(), Qt::ElideRight, rect.width(), Qt::TextShowMnemonic);
		painter.drawText(rect, qsElidedText, textOption);
	}
	
}
