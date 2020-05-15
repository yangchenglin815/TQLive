#pragma once

#include <QItemDelegate>

class RoomManageDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	explicit RoomManageDelegate(QObject *parent = 0);
	~RoomManageDelegate();

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);


signals:
	void onDeleteClicked(const QModelIndex &index);

private:
	QMap<QModelIndex, QStyleOptionButton*> m_pButtons;
};
