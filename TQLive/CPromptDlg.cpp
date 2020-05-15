#include "CPromptDlg.h"
#include <QKeyEvent>

CPromptDlg *CPromptDlg::m_pPromptDlg = NULL;

CPromptDlg::CPromptDlg(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	ui.setupUi(this);

	this->setStyleSheet("QDialog {background-color: rgba(38, 38, 38, 1);"
		"border:1px solid rgba(88, 88, 88, 1);}");
	connect(ui.pb_ok, SIGNAL(clicked()), this, SLOT(slotTurnoffDlg()));
	connect(ui.pb_close, SIGNAL(clicked()), this, SLOT(slotTurnoffDlg()));
}

CPromptDlg::~CPromptDlg()
{

}

void CPromptDlg::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Escape)
	{
		return;
	}
}

CPromptDlg *CPromptDlg::DisplayPromptDlg(QWidget *parent, const QString &qsPromptText, int nUserData, const QString &qsTitle)
{
	if (m_pPromptDlg == NULL)
	{
		m_pPromptDlg = new CPromptDlg(parent);
		m_pPromptDlg->setModal(true);
	}

	m_pPromptDlg->m_nUserData = nUserData;
	m_pPromptDlg->ui.lb_prompt_msg->setText(qsPromptText);
	m_pPromptDlg->ui.lb_title->setText(qsTitle);
	m_pPromptDlg->show();

	return m_pPromptDlg;
}

void CPromptDlg::slotTurnoffDlg()
{
	this->close();
	emit signalTurnoffDlg(m_nUserData);
}


