#include "CUniversalPromptDlg.h"
#include <QKeyEvent>

CUniversalPromptDlg *CUniversalPromptDlg::m_pPromptDlg = NULL;

CUniversalPromptDlg::CUniversalPromptDlg(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	ui.setupUi(this);

	this->setStyleSheet("QDialog {background-color: rgba(38, 38, 38, 1);"
		"border:1px solid rgba(88, 88, 88, 1);}");
	connect(ui.pb_ok, SIGNAL(clicked()), this, SLOT(slotAcceptSelection()));
	connect(ui.pb_close, SIGNAL(clicked()), this, SLOT(slotRejectSelection()));
	//connect(ui.pb_default, SIGNAL(clicked()), this, SLOT(slotDefaultSelection()));
}

CUniversalPromptDlg::~CUniversalPromptDlg()
{

}

void CUniversalPromptDlg::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
	{
		return;
	}
}

CUniversalPromptDlg *CUniversalPromptDlg::DisplayPromptDlg(QWidget *parent, const QString &qsPromptText, int nUserData, const QString &qsTitle)
{
	if (m_pPromptDlg == NULL)
	{
		m_pPromptDlg = new CUniversalPromptDlg(parent);
		m_pPromptDlg->setModal(true);
	}

	m_pPromptDlg->m_nUserData = nUserData;
	m_pPromptDlg->ui.lb_prompt_msg->setText(qsPromptText);
	m_pPromptDlg->ui.lb_title->setText(qsTitle);
	m_pPromptDlg->show();

	return m_pPromptDlg;
}

void CUniversalPromptDlg::slotTurnoffDlg()
{
	this->close();
	emit signalTurnoffDlg(m_nUserData);
}

void CUniversalPromptDlg::SetPromptDlgDisplayContent(const QString &qsPromptText, int nUserData, const QString &qsTitle)
{
	this->m_nUserData = nUserData;
	this->ui.lb_prompt_msg->setText(qsPromptText);
	this->ui.lb_title->setText(qsTitle);
}

void CUniversalPromptDlg::slotAcceptSelection()
{
	this->accept();
}

void CUniversalPromptDlg::slotRejectSelection()
{
	this->reject();
}
void CUniversalPromptDlg::slotDefaultSelection()
{
	this->done(2);
}


