/***********************************************************************************
*								RoomManageTableModel.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 房管列表Model层
***********************************************************************************/
#include "RoomManageTableModel.h"

RoomManageTableModel::RoomManageTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

RoomManageTableModel::~RoomManageTableModel()
{
	
}

int RoomManageTableModel::rowCount(const QModelIndex & parent) const
{
	return m_pData.size();
}

int RoomManageTableModel::columnCount(const QModelIndex & parent) const
{
	return m_HorizontalHeader.count();
}

QVariant RoomManageTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role == Qt::TextAlignmentRole)
	{
		QVariant value = Qt::AlignCenter;
		return value;
	}

	if (role == Qt::DisplayRole) {
		int ncol = index.column();
		int nrow = index.row();

		QStringList values = m_pData.at(nrow);
		if (values.size() > ncol)
			return values.at(ncol);
		else
			return QVariant();
	}
	return QVariant();
}

Qt::ItemFlags RoomManageTableModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	return flag;
}

void RoomManageTableModel::setHorizontalHeader(const QStringList & headers)
{
	m_HorizontalHeader = headers;
}

QVariant RoomManageTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		return m_HorizontalHeader.at(section);
	}
	return QAbstractTableModel::headerData(section, orientation, role);
}

void RoomManageTableModel::setData(const QVector<QStringList>& data)
{
	m_pData = data;
}

void RoomManageTableModel::operationToRemoveItemAtRow(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	m_pData.removeAt(row);
	endRemoveRows();
}
