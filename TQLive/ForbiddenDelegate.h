#pragma once

#include <QItemDelegate>

class ForbiddenDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	explicit ForbiddenDelegate(QObject *parent = 0);
	~ForbiddenDelegate();

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);


signals:
	void onForbiddenClicked(const QModelIndex &index);

private:
	QMap<QModelIndex, QStyleOptionButton*> m_pButtons;
};
