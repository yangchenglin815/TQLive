/***********************************************************************************
*								FeedbackDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2020-05
*
* Description: 反馈提交对话框
***********************************************************************************/
#include "FeedbackDlg.h"
#include "server.h"
#include "serverreply.h"
#include "FramelessHelper.h"
#include "ConstData.h"

#include <QErrorMessage>
#include <QMessageBox>
#include <QFileInfo>

using namespace SimpleMail;

#define MAX_LENGTH 200

FeedbackDlg::FeedbackDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setDbClickOn(false);
	helper->setWidgetResizable(false);

	InitUI();
	InitConnect();
}

FeedbackDlg::~FeedbackDlg()
{

}

void FeedbackDlg::onCommitClicked()
{
	QString textContent = ui.textEdit_feedback->toPlainText();
	if (textContent.isEmpty())
	{
		ErrorMessage(QStringLiteral("请输入内容！"));
		return;
	}

	MimeMessage message;
	message.setSender(QString("test@163.com"));
	message.setSubject(QStringLiteral("意见反馈"));
	message.addTo(QString("test@163.com"));

	auto content = new MimeHtml;
	content->setHtml(ui.textEdit_feedback->toHtml());
	message.addPart(content);

	AddAttachment();
	for (auto &file : m_attachmentList)
	{
		QFileInfo fileInfo(file);
		if (fileInfo.isFile())
		   message.addPart(new MimeAttachment(new QFile(file)));
	}

	SendMailSync(message);
}

void FeedbackDlg::onTextChanged()
{
	QString textContent = ui.textEdit_feedback->toPlainText();
	int length = textContent.count();
	if (length > MAX_LENGTH)
	{
		int position = ui.textEdit_feedback->textCursor().position();
		QTextCursor textCursor = ui.textEdit_feedback->textCursor();
		textContent.remove(position - (length - MAX_LENGTH), length - MAX_LENGTH);
		ui.textEdit_feedback->setText(textContent);
		textCursor.setPosition(position - (length - MAX_LENGTH));
		ui.textEdit_feedback->setTextCursor(textCursor);
	}
}

void FeedbackDlg::SendMailSync(const MimeMessage &msg)
{
	Sender::ConnectionType ct = Sender::TcpConnection;
	QString host = "smtp.163.com";
	quint16 port = 25;
	QString user = "test@163.com";
	QString passwd = "ABCDEFG";

	Sender smtp(host, port, ct);
	smtp.setUser(user);
	smtp.setPassword(passwd);

	if (!smtp.sendMail(msg))
	{
		ErrorMessage(QStringLiteral("邮件发送失败: %1").arg(smtp.lastError()));
		return;
	}
	else
	{
		QMessageBox okMessage(this);
		okMessage.setText(QStringLiteral("提交成功"));
		okMessage.exec();
	}

	smtp.quit();
	accept();
}

void FeedbackDlg::ErrorMessage(const QString & message)
{
	QErrorMessage err(this);

	err.showMessage(message);

	err.exec();
}

void FeedbackDlg::AddAttachment()
{
	m_attachmentList.append(QApplication::applicationDirPath() + "/TQLive.log");
	m_attachmentList.append(QApplication::applicationDirPath() + "/TQLive.dmp");
}

void FeedbackDlg::InitConnect()
{
	connect(ui.button_close, &QPushButton::clicked, this, &FeedbackDlg::close);
	connect(ui.button_commit, &QPushButton::clicked, this, &FeedbackDlg::onCommitClicked);
	connect(ui.textEdit_feedback, &QTextEdit::textChanged, this, &FeedbackDlg::onTextChanged);
}

void FeedbackDlg::InitUI()
{
	ui.button_close->setCursor(Qt::PointingHandCursor);
	ui.button_commit->setCursor(Qt::PointingHandCursor);
}
