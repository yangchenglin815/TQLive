#pragma once

#include <QDialog>
#include <QList>
#include "ui_FeedbackDlg.h"
#include "SmtpMime"

namespace SimpleMail {
	class Server;
}

using namespace SimpleMail;

class FeedbackDlg : public QDialog
{
	Q_OBJECT

public:
	FeedbackDlg(QWidget *parent = Q_NULLPTR);
	~FeedbackDlg();

private Q_SLOTS:
    void onCommitClicked();
	void onTextChanged();

private:
	void SendMailSync(const MimeMessage &msg);
	void ErrorMessage(const QString & message);
	void AddAttachment();
	void InitConnect();
	void InitUI();

private:
	Ui::FeedbackDlg ui;
	QList<QString> m_attachmentList;
};
