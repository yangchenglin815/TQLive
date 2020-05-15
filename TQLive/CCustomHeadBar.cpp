#include "stdafx.h"
#include "CCustomHeadBar.h"
#include "CScaleSize.h"

CCustomHeadBar::CCustomHeadBar(QWidget *parent)
	: QWidget(parent)
	, m_bMoveSwitch(true)
	, m_qsBkImageFilePath("")
	, m_bMousePressMark(false)
{
	this->setMouseTracking(m_bMoveSwitch);
	m_bkColor = QColor(47, 47, 47, 255);						// 透明通道导致颜色不一致
	m_pHostWidget = parent;
	//this->setAttribute(Qt::WA_StaticContents);
}

void CCustomHeadBar::SetBkImageFilePath(const QString &qsFilePath)
{
	m_qsBkImageFilePath = qsFilePath;
}

void CCustomHeadBar::mousePressEvent(QMouseEvent *event)
{  
	if (m_bMoveSwitch)										// 是否开启鼠标左键拖动功能
	{
		if (event->button() == Qt::LeftButton)
		{
			m_ptCursorStartPos = event->globalPos();		// 鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）// 光标位置
			m_bMousePressMark = true;
		}
	}
	
	// 否则QWidget部分代码不会执行，可能出现莫名其妙的错误
	QWidget::mousePressEvent(event);
}

// QWidget样式设置导致QPalette设置失效
void CCustomHeadBar::SetBkColor(const QColor &bkColor)
{
	m_bkColor = bkColor;
	
	//QPalette palette(this->palette());
	//palette.setColor(QPalette::Background, m_bkColor);
	//this->setAutoFillBackground(true);
	//this->setPalette(palette);
}

void CCustomHeadBar::SetHostWindow(QWidget *pWidget)
{
	m_pHostWidget = pWidget;

}

void CCustomHeadBar::mouseMoveEvent(QMouseEvent *event)
{
	// 移动开关打开并且鼠标左键按下的位置是非子控件区域 移动父窗口
	// 处理左键按下位置为子控件区域 拖动位置为父窗口区域的情况
	//if (m_bMoveSwitch && m_bMousePressMark)
	//{
	//	//QWidget *parent = (QWidget *)this->parent();

	//	if (m_pHostWidget)
	//	{
	//		QPoint ptCursorMovePos = event->globalPos();							
	//		QPoint ptDstPos = m_pHostWidget->pos() + (ptCursorMovePos - m_ptCursorStartPos);

	//		QDesktopWidget *pDesk = QApplication::desktop();
	//		int nWorkAreaWidth = pDesk->availableGeometry().width();
	//		int nWorkAreaHeight = pDesk->availableGeometry().height();

	//		//if (ptDstPos.x() > -(m_pMoveWidget->rect().width() - 20) && ptDstPos.x() < nWorkAreaWidth - 20 && ptDstPos.y() > 20 && ptDstPos.y() < nWorkAreaHeight - 20)
	//		//{
	//		//	m_pMoveWidget->move(ptDstPos);
	//		//	m_ptCursorStartPos = ptCursorMovePos;
	//		//}

	//		// 是否需要左、右、下各预留主窗口一般区域弹出模态对话框时
	//		//QRect rcOriginScope(QPoint(-(parent->rect().width() - 20), 20), QPoint(nWorkAreaWidth - 20,  nWorkAreaHeight - 20));
	//		/*if (rcOriginScope.contains(ptDstPos))
	//		{
	//			parent->move(ptDstPos);
	//			m_ptCursorStartPos = ptCursorMovePos;
	//		}*/
	//		//else
	//		//{
	//		//	m_pMoveWidget->move((nWorkAreaWidth - m_pMoveWidget->width()) / 2, (nWorkAreaHeight - m_pMoveWidget->height()) / 2);
	//		//	m_ptCursorStartPos.setX((nWorkAreaWidth - m_pMoveWidget->width()) / 2);
	//		//	m_ptCursorStartPos.setY((nWorkAreaHeight - m_pMoveWidget->height()) / 2);
	//		//}

	//		//if (ptDstPos.x() < 0)
	//		//{
	//		//	qDebug("test. parent->pos():%d Delta:%d", parent->pos().x(), ptCursorMovePos.x() - m_ptCursorStartPos.x());
	//		//}

	//		m_pHostWidget->move(ptDstPos);
	//		m_ptCursorStartPos = ptCursorMovePos;
	//	}
	//}

	//// 注释:阻止事件传递到父组件
	//QWidget::mouseMoveEvent(event);						// 父组件OBSBasic中处理mouseMoveEvent事件

	//if (m_bMousePressMark)
	//{
	//	//QRect rcWndArea = this->rect();
	//	//rcWndArea.adjust(5, 5, -5, 0);
	//	//if (rcWndArea.contains(event->pos(), false))
	//	//{
	//	//	// 缩放处理
	//	//	QWidget::mouseMoveEvent(event);
	//	//}
	//	//else
	//	//{

	//	//}


	//}

	QPoint ptCursorPos = event->globalPos();

	//qDebug("x:%d y:%d", ptCursorPos.x(), ptCursorPos.y());

	EN_ZONE_TYPE eZoneType = CScaleSize::CalcCursorLocation(this, event->pos());

	int nMinimumWidth = m_pHostWidget->minimumWidth();
	int nMinimumHeight = m_pHostWidget->minimumHeight();

	switch (eZoneType)
	{
	case CENTRE:
	case LEFTBOTTOM:
	case BOTTOM:
	case RIGHTBOTTOM:
	{
		this->setCursor(QCursor(Qt::ArrowCursor));

		if (m_bMoveSwitch && m_bMousePressMark)
		{
			QPoint ptDstPos = m_pHostWidget->pos() + (ptCursorPos - m_ptCursorStartPos);
			m_pHostWidget->move(ptDstPos);
			m_ptCursorStartPos = ptCursorPos;
		}
		
		break;
	}
	// 处理缩放
	case LEFTTOP:
	{
		this->setCursor(QCursor(Qt::SizeFDiagCursor));

		if (m_bMousePressMark)
		{
			CScaleSize::ScaleByCursorZoneType(m_pHostWidget, eZoneType, ptCursorPos, nMinimumWidth, nMinimumHeight);
		}
		
		break;
	}
	case RIGHTTOP:
	{
		this->setCursor(QCursor(Qt::SizeBDiagCursor));

		if (m_bMousePressMark)
		{
			CScaleSize::ScaleByCursorZoneType(m_pHostWidget, eZoneType, ptCursorPos, nMinimumWidth, nMinimumHeight);
		}

		break;
	}
	case LEFT:
	case RIGHT:
	{
		this->setCursor(QCursor(Qt::SizeHorCursor));

		if (m_bMousePressMark)
		{
			CScaleSize::ScaleByCursorZoneType(m_pHostWidget, eZoneType, ptCursorPos, nMinimumWidth, nMinimumHeight);
		}

		break;
	}
	case TOP:
	{
		this->setCursor(QCursor(Qt::SizeVerCursor));

		if (m_bMousePressMark)
		{
			CScaleSize::ScaleByCursorZoneType(m_pHostWidget, eZoneType, ptCursorPos, nMinimumWidth, nMinimumHeight);

			qDebug("x:%d y:%d", ptCursorPos.x(), ptCursorPos.y());
		}

		break;
	}
	default:
		break;
	}
}

void CCustomHeadBar::mouseReleaseEvent(QMouseEvent *event)
{  
	m_bMousePressMark = false;
	QWidget::mouseReleaseEvent(event);
	
}

void CCustomHeadBar::paintEvent(QPaintEvent *event)
{
	QRect rc = event->rect();

	QPainter painter(this);
	
	//painter.setClipRect(rc);

	if (m_qsBkImageFilePath == "")
	{
		//QStyleOption option;
		//option.init(this);
		//style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

		//QWidget::paintEvent(event);

		QBrush brush(this->m_bkColor);
		
		painter.setPen(Qt::NoPen);
		painter.setBrush(brush);
		painter.drawRect(this->rect());
	}
	else
	{
		QPixmap pixmap(m_qsBkImageFilePath);
		painter.drawPixmap(this->rect(), pixmap);
	}
}

CCustomHeadBar::~CCustomHeadBar()
{
}
