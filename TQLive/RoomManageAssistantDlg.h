#pragma once

#include <QDialog>
#include "ui_RoomManageAssistantDlg.h"

class RoomManageAssistantDlg : public QDialog
{
	Q_OBJECT

public:
	RoomManageAssistantDlg(QWidget *parent);
	~RoomManageAssistantDlg();

	void InitData();

private slots:
    void onGetForbiddenUserList(int nStatuCode, const QString &qsInfo, QList<ST_PROHIBIT_TO_SPEAK_RECORD> userList);
	void onGetRoomManagerList(int nStatuCode, const QString &qsInfo, QList<ST_ROOM_ADMINISTRATOR_RECORD> userList);
	void onCancelForbiddenUser(int nStatuCode, const QString &qsInfo);
	void onDeleteRoomManager(int nStatuCode, const QString &qsInfo);
  
private:
	void InitUI();
	void InitConnect();

private:
	Ui::RoomManageAssistantDlg ui;
};
