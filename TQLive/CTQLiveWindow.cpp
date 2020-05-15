#include "stdafx.h"
#include "CTQLiveWindow.h"
#include <windowsx.h>

CTQLiveWindow::CTQLiveWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//ui.chb_head_bar->setParent(this);
	//ui.chb_head_bar->SetBkColor(QColor(255, 0, 0, 255));
	//ui.chb_head_bar->update();
	//this->setMouseTracking(true);


	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	//this->setAttribute(Qt::WA_TranslucentBackground, true);

	//this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
	//this->setAttribute(Qt::WA_TranslucentBackground, true);

	isLeftPressDown = false;

	this->dir = NONE;
	this->setMinimumHeight(100);
	this->setMinimumWidth(150);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint); // 设置成无边框对话框
	this->setMouseTracking(true);
	this->ui.centralWidget->setMouseTracking(true);

	
	
}

CTQLiveWindow::~CTQLiveWindow()
{

}

//bool CTQLiveWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
//{
//	int m_nBorder = 50;
//	//Q_UNUSED(eventType)
//		MSG *param = static_cast<MSG *>(message);
//
//	switch (param->message)
//	{
//	case WM_NCHITTEST:
//	{
//		int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
//		int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();
//
//		//// if mouse in the child Qwidget of mainfrme, we should not handle it
//		//if (childAt(nX, nY) != NULL)
//		//  return QWidget::nativeEvent(eventType, message, result);
//
//		*result = HTCAPTION;
//
//		// if mouse in the border of mainframe,we zoom mainframe
//		if ((nX > 0) && (nX < m_nBorder))
//			*result = HTLEFT;
//
//		if ((nX > this->width() - m_nBorder) && (nX < this->width()))
//			*result = HTRIGHT;
//
//		if ((nY > 0) && (nY < m_nBorder))
//			*result = HTTOP;
//
//		if ((nY > this->height() - m_nBorder) && (nY < this->height()))
//			*result = HTBOTTOM;
//
//		if ((nX > 0) && (nX < m_nBorder) && (nY > 0)
//			&& (nY < m_nBorder))
//			*result = HTTOPLEFT;
//
//		if ((nX > this->width() - m_nBorder) && (nX < this->width())
//			&& (nY > 0) && (nY < m_nBorder))
//			*result = HTTOPRIGHT;
//
//		if ((nX > 0) && (nX < m_nBorder)
//			&& (nY > this->height() - m_nBorder) && (nY < this->height()))
//			*result = HTBOTTOMLEFT;
//
//		if ((nX > this->width() - m_nBorder) && (nX < this->width())
//			&& (nY > this->height() - m_nBorder) && (nY < this->height()))
//			*result = HTBOTTOMRIGHT;
//
//		if (*result == HTCAPTION)
//		{
//			return false;
//		}
//
//		return true;
//	}
//	}
//	return QMainWindow::nativeEvent(eventType, message, result);
//}


void CTQLiveWindow::region(const QPoint &cursorGlobalPoint)
{
	// 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());

	int x = cursorGlobalPoint.x();
	int y = cursorGlobalPoint.y();

	if (tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
		// 左上角
		dir = LEFTTOP;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
	}
	else if (x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
		// 右下角
		dir = RIGHTBOTTOM;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
		//左下角
		dir = LEFTBOTTOM;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
		// 右上角
		dir = RIGHTTOP;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x <= tl.x() + PADDING && x >= tl.x()) {
		// 左边
		dir = LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (x <= rb.x() && x >= rb.x() - PADDING) {
		// 右边
		dir = RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (y >= tl.y() && y <= tl.y() + PADDING) {
		// 上边
		dir = UP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (y <= rb.y() && y >= rb.y() - PADDING) {
		// 下边
		dir = DOWN;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else {
		// 默认
		dir = NONE;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}

void CTQLiveWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		isLeftPressDown = false;
		if (dir != NONE) {
			this->releaseMouse();
			this->setCursor(QCursor(Qt::ArrowCursor));
		}
	}
}

void CTQLiveWindow::mousePressEvent(QMouseEvent *event)
{
	switch (event->button()) {
	case Qt::LeftButton:
		isLeftPressDown = true;
		if (dir != NONE) {
			this->mouseGrabber();
		}
		else {
			dragPosition = event->globalPos() - this->frameGeometry().topLeft();
		}
		break;
	case Qt::RightButton:
		this->close();
		break;
	default:
		QMainWindow::mousePressEvent(event);
	}

}

void CTQLiveWindow::mouseMoveEvent(QMouseEvent *event)
{
	QPoint gloPoint = event->globalPos();
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());

	if (!isLeftPressDown) {
		this->region(gloPoint);
	}
	else {

		if (dir != NONE) {
			QRect rMove(tl, rb);

			switch (dir) {
			case LEFT:
				if (rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				break;
			case RIGHT:
				rMove.setWidth(gloPoint.x() - tl.x());
				break;
			case UP:
				if (rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case DOWN:
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			case LEFTTOP:
				if (rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				if (rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case RIGHTTOP:
				rMove.setWidth(gloPoint.x() - tl.x());
				rMove.setY(gloPoint.y());
				break;
			case LEFTBOTTOM:
				rMove.setX(gloPoint.x());
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			case RIGHTBOTTOM:
				rMove.setWidth(gloPoint.x() - tl.x());
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			default:
				break;
			}
			this->setGeometry(rMove);
		}
		else {
			move(event->globalPos() - dragPosition);
			event->accept();
		}
	}

	QMainWindow::mouseMoveEvent(event);
}

