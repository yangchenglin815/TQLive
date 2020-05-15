#pragma once

#include <QAbstractTableModel>

class RoomManageTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit RoomManageTableModel(QObject *parent = 0);
	~RoomManageTableModel();

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	void setHorizontalHeader(const QStringList& headers);
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	void setData(const QVector<QStringList>& data);
	QVector<QStringList>& DataVector() { return m_pData; }
	void operationToRemoveItemAtRow(int row);

private:
	QStringList m_HorizontalHeader;
	QVector<QStringList> m_pData;
};
