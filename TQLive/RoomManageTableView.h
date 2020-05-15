#pragma once

#include <QTableView>
#include <QMouseEvent>
#include "RoomManageDelegate.h"
#include "RoomManageTableModel.h"
#include "StructData.h"
#include "CDataCentre.h"
#include "TipsDlg.h"

class RoomManageTableView : public QTableView
{
	Q_OBJECT

public:
	explicit RoomManageTableView(QWidget *parent = 0);
	~RoomManageTableView();

	RoomManageTableModel* tableModel() { return m_pDataModel; }
	void initData(const QList<ST_ROOM_ADMINISTRATOR_RECORD> &roomManagerList);

private slots:
	void onDeleteClicked(const QModelIndex &index);

private:
	void initUI();

private:
	QList<ST_ROOM_ADMINISTRATOR_RECORD> m_pRoomManagerList;
	RoomManageTableModel *m_pDataModel;
	RoomManageDelegate *m_buttonDelegate;
	TipsDlg *m_pTipsDlg;
};
