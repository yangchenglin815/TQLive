/***********************************************************************************
*								RoomManageAssistantDlg.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 房管助手对话框
***********************************************************************************/
#include "RoomManageAssistantDlg.h"
#include "FramelessHelper.h"
#include "HttpAgent.h"
#include "GlobalSignal.h"

RoomManageAssistantDlg::RoomManageAssistantDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	FramelessHelper *helper = new FramelessHelper;
	helper->activateOn(this);
	helper->setWidgetResizable(false);
	helper->setDbClickOn(false);

	InitUI();
	InitConnect();
}

RoomManageAssistantDlg::~RoomManageAssistantDlg()
{

}

void RoomManageAssistantDlg::InitData()
{
	HttpAgent::instance()->RequestOnGetForbiddenUserList();
	HttpAgent::instance()->RequestOnGetRoomManagerList();
}

void RoomManageAssistantDlg::onGetForbiddenUserList(int nStatuCode, const QString &qsInfo, QList<ST_PROHIBIT_TO_SPEAK_RECORD> userList)
{
	if (nStatuCode == 200)
	{
		ui.forbiddenRecordTableView->initData(userList);
		this->exec();
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatuCode, qsInfo);
	}
}

void RoomManageAssistantDlg::onGetRoomManagerList(int nStatuCode, const QString & qsInfo, QList<ST_ROOM_ADMINISTRATOR_RECORD> userList)
{
	if (nStatuCode == 200)
	{
		ui.roomManageTableView->initData(userList);
		this->exec();
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatuCode, qsInfo);
	}
}

void RoomManageAssistantDlg::onCancelForbiddenUser(int nStatuCode, const QString & qsInfo)
{
	if (nStatuCode == 200)
	{
		HttpAgent::instance()->RequestOnGetForbiddenUserList();
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatuCode, qsInfo);
	}
}

void RoomManageAssistantDlg::onDeleteRoomManager(int nStatuCode, const QString &qsInfo)
{
	if (nStatuCode == 200)
	{
		HttpAgent::instance()->RequestOnGetRoomManagerList();
	}
	else
	{
		GlobalSignal::instance()->onSigCatchException(nStatuCode, qsInfo);
	}
}

void RoomManageAssistantDlg::InitUI()
{
	
}

void RoomManageAssistantDlg::InitConnect()
{
	connect(ui.btn_close, &QPushButton::clicked, this, &QDialog::close);

	connect(HttpAgent::instance(), SIGNAL(onSigGetForbiddenUserList(int, const QString &, QList<ST_PROHIBIT_TO_SPEAK_RECORD>)), 
		this, SLOT(onGetForbiddenUserList(int, const QString &, QList<ST_PROHIBIT_TO_SPEAK_RECORD>)));
	connect(HttpAgent::instance(), SIGNAL(onSigGetRoomManagerList(int, const QString &, QList<ST_ROOM_ADMINISTRATOR_RECORD>)),
		this, SLOT(onGetRoomManagerList(int, const QString &, QList<ST_ROOM_ADMINISTRATOR_RECORD>)));
	connect(HttpAgent::instance(), SIGNAL(onSigCancelForbiddenUser(int, const QString &)), this, SLOT(onCancelForbiddenUser(int, const QString &)));
	connect(HttpAgent::instance(), SIGNAL(onSigDeleteRoomManager(int, const QString &)), this, SLOT(onDeleteRoomManager(int, const QString &)));

	connect(ui.btn_forbiddenRecord, &QPushButton::clicked, this, [&](bool checked) {
		if (checked) {
			HttpAgent::instance()->RequestOnGetForbiddenUserList();
			ui.stackedWidget->setCurrentWidget(ui.page_forbiddenRecord);
		}
	});

	connect(ui.btn_roomManage, &QPushButton::clicked, this, [&](bool checked) {
		if (checked) {
			HttpAgent::instance()->RequestOnGetRoomManagerList();
			ui.stackedWidget->setCurrentWidget(ui.page_roomManage);
		}
	});
}
