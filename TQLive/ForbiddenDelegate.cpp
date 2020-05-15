/***********************************************************************************
*								ForbiddenDelegate.cpp
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 禁言记录按键代理
***********************************************************************************/
#include "ForbiddenDelegate.h"
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>

ForbiddenDelegate::ForbiddenDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}

ForbiddenDelegate::~ForbiddenDelegate()
{

}

void ForbiddenDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QStyleOptionButton* button = m_pButtons.value(index);
	if (!button) {
		button = new QStyleOptionButton();
		button->state |= QStyle::State_Enabled;
		button->iconSize = QSize(42, 18);
		button->icon = QIcon(QString(":/TQLive/images/forbidden.png"));

		(const_cast<ForbiddenDelegate *>(this))->m_pButtons.insert(index, button);
	}
	button->rect = QRect(option.rect.left() + (100 - 42)/2, option.rect.top() + (34 - 18)/2, 42, 18); //
	painter->save();

	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());

	}
	painter->restore();
	QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);
}

bool ForbiddenDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		if (m_pButtons.contains(index))
		{
			emit onForbiddenClicked(index);
		}
	}
	return false;
}
