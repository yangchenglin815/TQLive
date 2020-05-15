/***********************************************************************************
*								RoomManageTableView.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 房管列表View层
***********************************************************************************/
#include "RoomManageTableView.h"
#include <QHeaderView>
#include "HttpAgent.h"
#include <QDebug>

RoomManageTableView::RoomManageTableView(QWidget *parent)
	: QTableView(parent)
	, m_pTipsDlg(NULL)
{
	initUI();
}

RoomManageTableView::~RoomManageTableView()
{
	delete m_pDataModel;
}

void RoomManageTableView::initData(const QList<ST_ROOM_ADMINISTRATOR_RECORD> &roomManagerList)
{
	m_pRoomManagerList.clear();
	m_pRoomManagerList = roomManagerList;

	QVector<QStringList> data;
	for (int i = 0; i < roomManagerList.size(); ++i)
	{
		data.append(QStringList() << m_pRoomManagerList[i].qsNickName
			<< m_pRoomManagerList[i].qsAppointedDateTime
			<< m_pRoomManagerList[i].qsLastEntryRoomDateTime);

	}
	m_pDataModel->setData(data);
	emit m_pDataModel->layoutChanged();

	m_buttonDelegate = new RoomManageDelegate(this);
	connect(m_buttonDelegate, SIGNAL(onDeleteClicked(const QModelIndex &)), this, SLOT(onDeleteClicked(const QModelIndex &)));
	this->setItemDelegateForColumn(3, m_buttonDelegate);
}

void RoomManageTableView::onDeleteClicked(const QModelIndex &index)
{
	if (m_pTipsDlg == NULL)
	{
		m_pTipsDlg = new TipsDlg(DIALOG_DELETE_ROOMMANAGER_OPERATOR, this);
	}

	if (QDialog::Accepted == m_pTipsDlg->exec())
	{
		int row = index.row();
		QString curr_user = m_pDataModel->index(row, 0).data().toString();
		for (int i = 0; i < m_pRoomManagerList.size(); ++i)
		{
			if (m_pRoomManagerList[i].qsNickName == curr_user)
			{
				HttpAgent::instance()->RequestOnDeleteRoomManager(m_pRoomManagerList[i].nUserID);
			}
		}
	}
}

void RoomManageTableView::initUI()
{
	this->verticalHeader()->hide();
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setSelectionMode(QAbstractItemView::NoSelection);
	this->setFocusPolicy(Qt::NoFocus);
	this->setFixedWidth(520);

	m_pDataModel = new RoomManageTableModel();
	this->setModel(m_pDataModel);
	QStringList headers;
	headers << QStringLiteral("房间管理员") << QStringLiteral("任命时间") << QStringLiteral("最后进入房间时间") << QStringLiteral("操作");
	m_pDataModel->setHorizontalHeader(headers);
	emit m_pDataModel->layoutChanged();

	this->horizontalHeader()->setFixedHeight(40);
	this->verticalHeader()->setDefaultSectionSize(34);
	this->setColumnWidth(0, 138);
	this->setColumnWidth(1, 141);
	this->setColumnWidth(2, 141);
	this->setColumnWidth(3, 100);

	for (int i = 0; i < this->horizontalHeader()->count(); ++i)
	{
		this->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
	}
}
