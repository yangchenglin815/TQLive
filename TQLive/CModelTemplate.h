#pragma once
#include "CCustomTableModel.h"

template<typename T>
class CModelTemplate : public CCustomTableModel
{
public:
	CModelTemplate();
	virtual ~CModelTemplate();
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	void ResetTableModelData(QList<T> &obLMusicInformation);

private:
	QStringList m_slSectionName;
	QList<T> m_obLDataItemList;
};

template <typename T>
CModelTemplate<T>::CModelTemplate()
{
	m_slSectionName << QString::fromLocal8Bit("时间") << QString::fromLocal8Bit("赛事") << QString::fromLocal8Bit("主队 vs 客队") << QString::fromLocal8Bit("操作");
}

template <typename T>
CModelTemplate<T>::~CModelTemplate()
{
}

template <typename T>
int CModelTemplate<T>::rowCount(const QModelIndex & parent) const
{
	return m_obLDataItemList.size();
}

template <typename T>
int CModelTemplate<T>::columnCount(const QModelIndex & parent) const
{
	return m_slSectionName.size();
}

template <typename T>
QVariant CModelTemplate<T>::data(const QModelIndex &index, int role) const
{
	int nColumn = index.column();
	int nRow = index.row();
	const T &info = m_obLDataItemList[nRow];
	switch (role)
	{
	case Qt::DisplayRole:
	{
		return info[nColumn];
	}
	case Qt::TextAlignmentRole:
	{
		return Qt::AlignCenter;
	}
	default:
		break;
	}
	return QVariant();
}

template <typename T>
QVariant CModelTemplate<T>::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		switch (role)
		{
		case Qt::DisplayRole:
		{
			return m_slSectionName[section];
		}
		}
	}

	return QVariant();
}

template <typename T>
void CModelTemplate<T>::ResetTableModelData(QList<T> &obLInfo)
{

	this->beginResetModel();
	m_obLDataItemList = obLInfo;
	this->endResetModel();
}

