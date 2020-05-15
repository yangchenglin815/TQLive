#include "stdafx.h"
#include "CCustomButton.h"

CCustomButton::CCustomButton(QWidget *parent)
    : QPushButton(parent)
    , m_nEnterBtnState(m_nCurrentBtnState)
    , m_nCurrentBtnState(NORMAL_STATE)
    , m_nBtnType(BT_NORMAL | BT_OVER | BT_PRESSING | BT_DISABLE)
    , m_rcButtonText(0, 0, 100, 100)
    , m_eAlignmentStyle(Qt::AlignCenter)
  
{
	// 子控件不要设置透明通道
	
    SetCustomButtonTextColor(QColor(219, 237, 216, 255), QColor(219, 237, 216, 255), QColor(219, 237, 216, 255), QColor(219, 237, 216, 255), QColor(219, 237, 216, 255));
	connect(this, SIGNAL(clicked()), this, SLOT(slotReponseClicked()));
	this->setText("");
}

CCustomButton::~CCustomButton()
{

}

void CCustomButton::SetBtnType(int nBtnType)
{
	m_nBtnType = nBtnType;
}

void CCustomButton::SetCustomButtonTextArea(const QRect &rcButtonText)
{
    m_rcButtonText = rcButtonText;
}

void CCustomButton::SetBtnTextColor(const QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::ButtonText, color);
    this->setPalette(palette);
}

void CCustomButton::SetBtnTextFont(const QFont &font)
{
    this->setFont(font);
}

void CCustomButton::SetCustomButtonTextColor(const QColor &qcNormal, const QColor &qcOver, const QColor &qcPressing, const QColor &qcDown, const QColor &qcDisable)
{
    m_szTextColor[NORMAL_STATE] = qcNormal;
    m_szTextColor[OVER_STATE] = qcOver;
    m_szTextColor[PRESSING_STATE] = qcPressing;
    m_szTextColor[DOWN_STATE] = qcDown;
    m_szTextColor[DISABLE_STATE] = qcDisable;
}

void CCustomButton::SetCustomButtonBkFileName(const QString &qsNormal, const QString &qsOver, const QString &qsPressing, const QString &qsDown, const QString &qsDisable)
{	
    m_szFileName[NORMAL_STATE] = qsNormal;
    m_szFileName[OVER_STATE] = qsOver;
    m_szFileName[PRESSING_STATE] = qsPressing;
    m_szFileName[DOWN_STATE] = qsDown;
    m_szFileName[DISABLE_STATE] = qsDisable;
}

void CCustomButton::SetCustomButtonGeometry(int nWidth, int nHeight)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    this->setMinimumWidth(m_nWidth);
    this->setMaximumWidth(m_nWidth);
    this->setMinimumHeight(m_nHeight);
    this->setMaximumHeight(m_nHeight);
}

void CCustomButton::SetCustomButtonTextAlignmentStyle(Qt::Alignment eAlignmentStyle)
{
    m_eAlignmentStyle = eAlignmentStyle;
}

void CCustomButton::paintEvent(QPaintEvent *event)
{	
    QPainter painter(this);

    if (m_szFileName[m_nCurrentBtnState] == "")
    {
        //QPushButton::paintEvent(event);

		QString qsButtonText = this->text();

		//QString qsFamily = QString::fromLocal8Bit("微软雅黑");
		//QFont font(qsFamily, 10, 400);
		//painter.setFont(font);

		QColor qcButtonText = m_szTextColor[m_nCurrentBtnState];
		painter.setPen(qcButtonText);
		if (qsButtonText != "")
		{
			QTextOption textOption(m_eAlignmentStyle/*Qt::AlignLeft | Qt::AlignHCenter*/);

			QFontMetrics fm = painter.fontMetrics();
			QString qsElidedText = fm.elidedText(text(), Qt::ElideRight, m_rcButtonText.width(), Qt::TextShowMnemonic);
			painter.drawText(m_rcButtonText, qsElidedText, textOption);
		}
    }
    else
    {
        //QString qsSkinFilePath = "./images/";
       // QString qsFilePath = qsSkinFilePath + m_szFileName[m_nCurrentBtnState];

        QPixmap pixmap(m_szFileName[m_nCurrentBtnState]);
        QRect rcBtnArea = this->rect();
        painter.drawPixmap(rcBtnArea, pixmap);
        
        QString qsButtonText = this->text();

		QColor qcButtonText  = m_szTextColor[m_nCurrentBtnState];
        painter.setPen(qcButtonText);
        if (qsButtonText != "")
        {
            QTextOption textOption(m_eAlignmentStyle/*Qt::AlignLeft | Qt::AlignHCenter*/);

            QFontMetrics fm = painter.fontMetrics();
            QString qsElidedText = fm.elidedText(text(), Qt::ElideRight, m_rcButtonText.width(), Qt::TextShowMnemonic);
            painter.drawText(m_rcButtonText, qsElidedText, textOption);
        }
    }
}

void CCustomButton::SetBtnState(BUTTON_STATE eBtnState)
{
    if (m_nCurrentBtnState == eBtnState)
    {
        return;
    }

    m_nCurrentBtnState = eBtnState;
    m_nEnterBtnState = m_nCurrentBtnState;
    this->update();
}


/*
最终状态:选中状态、非选中状态、禁用状态
中间状态:悬停状态、按下状态
禁用状态仍然收到事件enterEvent、leaveEvent
*/
void CCustomButton::enterEvent(QEvent *event)
{
    if (m_nBtnType & BT_OVER)
    {
        if (NORMAL_STATE == m_nEnterBtnState/* || DISABLE_STATE == m_nEnterBtnState*/)
        {	
            m_nCurrentBtnState = OVER_STATE;
            this->update();
        }
    }
    
    QPushButton::enterEvent(event);
}

// 弹出右键菜单与对话框时会产生leaveEvent
void CCustomButton::mousePressEvent(QMouseEvent *event)
{
    if (m_nBtnType & BT_PRESSING)
    {
        if (event->button() == Qt::LeftButton)
        {	
            m_nCurrentBtnState = PRESSING_STATE;
            this->update();
		}
        else
        {
            emit clicked();
        }
    }
    
    QPushButton::mousePressEvent(event);
}

void CCustomButton::leaveEvent(QEvent *event)
{
    if (m_nCurrentBtnState == DISABLE_STATE)
    {
        QPushButton::leaveEvent(event);
        return;
    }

    if (NORMAL_STATE == m_nEnterBtnState)
    {
        // 1.normal--->over--->normal 2.normal--->over--->pressing--->normal 
        m_nCurrentBtnState = NORMAL_STATE;
    }
    else
    {
        // 1.down--->down 2.down--->pressing--->down
        m_nCurrentBtnState = DOWN_STATE;
    }

    // 离开时与进入时的初始状态不会发生改变（下一次的进入状态未发生改变）
    this->update();

    QPushButton::leaveEvent(event);
}

// 切换到另外一个状态
void CCustomButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()))
    {
        if (e->button() == Qt::LeftButton)
        {
            emit clicked();
        }
        else
        {
           QPushButton::mouseReleaseEvent(e);
           
        }
    }
    else
    {
        QPushButton::mouseReleaseEvent(e);
    }
}

bool CCustomButton::event(QEvent *event)
{
    if ((event->type() == QEvent::KeyPress))
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int nKeyValue = keyEvent->key();

        switch (nKeyValue)
        {
        case Qt::Key_Space:
        case Qt::Key_Return:
        case Qt::Key_Escape:
            {
                return true;
            }
        default:
            {
                break;
            }
        }
    }

    return QPushButton::event(event);
}

void CCustomButton::slotReponseClicked()
{
	emit signalCustomClicked(m_nUserData);
}