#include "CLiveContentSelectPromptDlg.h"
#include <QKeyEvent>

CLiveContentSelectPromptDlg *CLiveContentSelectPromptDlg::m_pPromptDlg = NULL;

CLiveContentSelectPromptDlg::CLiveContentSelectPromptDlg(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	ui.setupUi(this);

	this->setStyleSheet("QDialog {background-color: rgba(38, 38, 38, 1);"
		"border:1px solid rgba(88, 88, 88, 1);}");
	connect(ui.pb_ok, SIGNAL(clicked()), this, SLOT(slotAcceptSelection()));
	connect(ui.pb_close, SIGNAL(clicked()), this, SLOT(slotRejectSelection()));
	connect(ui.pb_default, SIGNAL(clicked()), this, SLOT(slotDefaultSelection()));
}

CLiveContentSelectPromptDlg::~CLiveContentSelectPromptDlg()
{

}

void CLiveContentSelectPromptDlg::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
	{
		return;
	}
}

CLiveContentSelectPromptDlg *CLiveContentSelectPromptDlg::DisplayPromptDlg(QWidget *parent, const QString &qsPromptText, int nUserData, const QString &qsTitle)
{
	if (m_pPromptDlg == NULL)
	{
		m_pPromptDlg = new CLiveContentSelectPromptDlg(parent);
		m_pPromptDlg->setModal(true);
	}

	m_pPromptDlg->m_nUserData = nUserData;
	m_pPromptDlg->ui.lb_prompt_msg->setText(qsPromptText);
	m_pPromptDlg->ui.lb_title->setText(qsTitle);
	m_pPromptDlg->show();

	return m_pPromptDlg;
}

void CLiveContentSelectPromptDlg::slotTurnoffDlg()
{
	this->close();
	emit signalTurnoffDlg(m_nUserData);
}

void CLiveContentSelectPromptDlg::SetPromptDlgDisplayContent(const QString &qsPromptText, int nUserData, const QString &qsTitle)
{
	this->m_nUserData = nUserData;
	this->ui.lb_prompt_msg->setText(qsPromptText);
	this->ui.lb_title->setText(qsTitle);
}

void CLiveContentSelectPromptDlg::slotAcceptSelection()
{
	this->accept();
}

void CLiveContentSelectPromptDlg::slotRejectSelection()
{
	this->reject();
}
void CLiveContentSelectPromptDlg::slotDefaultSelection()
{
	this->done(2);
}


