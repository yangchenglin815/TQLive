#include "stdafx.h"
#include "CCustomButtonGroup.h"
#include "CCustomButton.h"
//#include "Skin.h"

CCustomButtonGroup::CCustomButtonGroup(QWidget *parent)
	: QWidget(parent)
	, m_pBoxLayout(NULL)
	, m_nCurTabBtnIndex(0)
	, m_nTabBtnWidth(135)
	, m_nTabBtnHeight(100)
	, m_pContextMenu(NULL)
	//, m_pLineEdit(NULL)
{
	m_pBoxLayout = new QVBoxLayout(this);
	m_pBoxLayout->setSpacing(0);
	m_pBoxLayout->setContentsMargins(0, 0, 0, 0);
	m_pBoxLayout->setObjectName(QStringLiteral("verticalLayout"));
}

CCustomButtonGroup::CCustomButtonGroup(QWidget *parent, TAB_CONTROL_STYLE eTabControlStyle)
	: QWidget(parent)
	, m_pBoxLayout(NULL)
	, m_nCurTabBtnIndex(0)
	, m_nTabBtnWidth(135)
	, m_nTabBtnHeight(100)
	, m_pContextMenu(NULL)
	//, m_pLineEdit(NULL)
{
	if (eTabControlStyle == HORIZONTAL_STYLE)
	{
		m_pBoxLayout = new QHBoxLayout(this);
		m_pBoxLayout->setSpacing(30);
		m_pBoxLayout->setContentsMargins(40, 22, 0, 10);
		m_pBoxLayout->setObjectName(QStringLiteral("horizontalLayout"));
		setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
		this->setStyleSheet("background-color: rgba(38, 38, 38, 1);"
			"border:0px solid rgba(88, 88, 88, 1);"
		"margin: 22px 0px 10px 40px;");
		this->setLayout(m_pBoxLayout);

	}
	else
	{
		m_pBoxLayout = new QVBoxLayout(this);
		m_pBoxLayout->setSpacing(0);
		m_pBoxLayout->setContentsMargins(0, 0, 0, 0);
		m_pBoxLayout->setObjectName(QStringLiteral("verticalLayout"));
	}
}

CCustomButtonGroup::~CCustomButtonGroup()
{
}

void CCustomButtonGroup::SetCustomButtonGeometry(int nWidth, int nHeight)
{
	m_nTabBtnWidth = nWidth;
	m_nTabBtnHeight = nHeight;
	this->setMinimumHeight(m_nTabBtnHeight);
	this->setMaximumHeight(m_nTabBtnHeight);
}

void CCustomButtonGroup::SetCurrentTabButton(int nNewTabBtnIndex)
{
	m_nCurTabBtnIndex = nNewTabBtnIndex;
}

void CCustomButtonGroup::slotChangeTabBtn(int nNewTabBtnIndex)
{
	// 按钮切换
	if (nNewTabBtnIndex == m_nCurTabBtnIndex)
	{
	    //m_obVTabBtnList[m_nCurTabBtnIndex]->SetBtnState(DOWN_STATE);
		return;
	}

	if (m_nCurTabBtnIndex != -1)				// 已删除分类--->nNewTabBtnIndex分类
	{
		m_obVTabBtnList[m_nCurTabBtnIndex]->SetBtnState(NORMAL_STATE);
	}

	m_obVTabBtnList[nNewTabBtnIndex]->SetBtnState(DOWN_STATE);
	m_nCurTabBtnIndex = nNewTabBtnIndex;

	emit signalTabButtonSwitch(m_nCurTabBtnIndex);
}

// 设计目的需要定制的风格太多const QRect &rcTextPos = QRect(0, 0, 0, 0)
void CCustomButtonGroup::AddFunctionTabButton(const QString &qsBtnText, const QString &qsNormal, const QString &qsOver, const QString &qsPressing, const QString &qsDown, const QString &qsDisable)
{
	CCustomButton *pCustomBtn = new CCustomButton(this);
	pCustomBtn->SetCustomButtonBkFileName(qsNormal, qsOver, qsPressing, qsDown, qsDisable);
	pCustomBtn->SetCustomButtonGeometry(m_nTabBtnWidth, m_nTabBtnHeight);

    //按钮文字颜色切换
   // pCustomBtn->SetCustomButtonTextColor(QColor(255, 255, 255),QColor(255, 255, 255),"",QColor(255, 255, 255),QColor(208, 208, 208));
	//QPalette palette;
	//palette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
	//palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(208, 208, 208));
	//pCustomBtn->setPalette(palette);

	//QFont font = gSkin.GetFontStyle(CSkin::FONT_TEN_PIXEL);
	//pCustomBtn->setFont(font);
	//pCustomBtn->SetCustomButtonTextColor(QColor(255, 255, 255));
	//pCustomBtn->setText(qsBtnText);
	//int nPixSize = font.pixelSize();

	// 按钮的非paintEvent()中使用返回的结构未定义
	//QPainter painter(pCustomBtn);
	//bool bIsActive = painter.isActive();
	//QFontMetrics fm = painter.fontMetrics();
	//int nWidth = fm.width(qsBtnText);

	// QML方式
	//painter.setTextSize(size);
	//float fWidth = painter.measureText(text);

	// 按钮文本区域正确方式
	//QFontMetrics fontMetrics(font);
	//int nWidth = fontMetrics.width(qsBtnText);

	// 对齐方式矩形区域居中(默认对齐方式)
	QRect rcTextPos;
	rcTextPos.setTop(30);
	//rcTextPos.setHeight(nPixSize + 4);
	rcTextPos.setBottom(m_nTabBtnHeight);
	rcTextPos.setLeft(0);
	rcTextPos.setRight(m_nTabBtnWidth);
	pCustomBtn->SetCustomButtonTextArea(rcTextPos);
	
	m_pBoxLayout->addWidget(pCustomBtn);

	//int nTabBtnListSize = m_obVTabBtnList.size();
	//pCustomBtn->SetUserData(nTabBtnListSize);
	m_obVTabBtnList.push_back(pCustomBtn);
	connect(pCustomBtn, SIGNAL(signalClickBtn(int)), this, SLOT(slotChangeTabBtn(int)));
}

void CCustomButtonGroup::AddTabButton(const QString &qsBtnText, const QString szBkImagePath[], int nWidth, int nHeight)
{
	QString qsFamily = QString::fromLocal8Bit("微软雅黑");
	QFont font(qsFamily, 9, 400);

	QFontMetrics fm(font);
	int nActualWidth = fm.width(qsBtnText);
	nWidth = nActualWidth;

	CCustomButton *pCustomBtn = new CCustomButton(this);
	pCustomBtn->SetCustomButtonBkFileName(szBkImagePath[0], szBkImagePath[1], szBkImagePath[2], szBkImagePath[3], szBkImagePath[4]);
	pCustomBtn->SetCustomButtonGeometry(nWidth, nHeight);
	pCustomBtn->setText(qsBtnText);

	QRect rcTextArea(0, 0, nWidth, nHeight);
	pCustomBtn->SetCustomButtonTextArea(rcTextArea);


	pCustomBtn->setFont(font);

	pCustomBtn->SetCustomButtonTextColor(QColor(220, 220, 220), QColor(220, 220, 220), QColor(220, 220, 220), QColor(74, 177, 52), QColor(220, 220, 220));

	m_pBoxLayout->addWidget(pCustomBtn);

	int nTabBtnListSize = m_obVTabBtnList.size();
	pCustomBtn->SetUserData(nTabBtnListSize);
	m_obVTabBtnList.push_back(pCustomBtn);
	connect(pCustomBtn, SIGNAL(signalCustomClicked(int)), this, SLOT(slotChangeTabBtn(int)));
}

void *CCustomButtonGroup::AddGenreTabButton(const QString &qsBtnText, const QString &qsNormal, const QString &qsOver, const QString &qsPressing, const QString &qsDown, const QString &qsDisable)
{
	CCustomButton *pCustomBtn = new CCustomButton(this);
	pCustomBtn->SetCustomButtonBkFileName(qsNormal, qsOver, qsPressing, qsDown, qsDisable);
	pCustomBtn->SetCustomButtonGeometry(m_nTabBtnWidth, m_nTabBtnHeight);
	pCustomBtn->SetCustomButtonTextAlignmentStyle(Qt::AlignLeft | Qt::AlignVCenter);
	//pCustomBtn->SetRightClickedMark(true);
	//QFont font = gSkin.GetFontStyle(CSkin::Smaller);
	//pCustomBtn->setFont(font);

	QRect rcTextPos(40, 0, m_nTabBtnWidth - 40 - 20, m_nTabBtnHeight);
	pCustomBtn->SetCustomButtonTextArea(rcTextPos);
	pCustomBtn->setText(qsBtnText);
	pCustomBtn->setObjectName(qsBtnText);
	m_pBoxLayout->addWidget(pCustomBtn);

	int nTabBtnListSize = m_obVTabBtnList.size();
	//pCustomBtn->SetUserData(nTabBtnListSize);
	m_obVTabBtnList.push_back(pCustomBtn);
	connect(pCustomBtn, SIGNAL(signalClickBtn(int)), this, SLOT(slotChangeTabBtn(int)));
	//connect(pCustomBtn, SIGNAL(signalTextChange(QString)), this, SIGNAL(signalTextChange(QString))); 
	connect(pCustomBtn, SIGNAL(signalTextChange(QString)), this, SLOT(slotLineEditFocusout(QString)));

	return pCustomBtn;

	// 新添加的按钮进入可编辑状态
	//pCustomBtn->EnterEditableStatus();

}

void CCustomButtonGroup::ClearAllSubControl()
{
	QLayoutItem *pLayoutItem = NULL;
	while ((pLayoutItem = m_pBoxLayout->takeAt(0)) != 0) 
	{
		delete pLayoutItem;
	}

	for (int i = 0; i < m_obVTabBtnList.size(); i++)
	{
		delete m_obVTabBtnList[i];
	}

	m_obVTabBtnList.clear();
}

void CCustomButtonGroup::AddGenreStaticTabButton(int nGenreId, const QString &qsBtnText, QMenu* pMenu,
	const QString &qsNormal, const QString &qsOver, const QString &qsPressing, const QString &qsDown, const QString &qsDisable)
{
	CCustomButton *pCustomBtn = new CCustomButton(this);
	pCustomBtn->SetCustomButtonBkFileName(qsNormal, qsOver, qsPressing, qsDown, qsDisable);
	pCustomBtn->SetCustomButtonGeometry(m_nTabBtnWidth, m_nTabBtnHeight);
	pCustomBtn->SetCustomButtonTextAlignmentStyle(Qt::AlignLeft | Qt::AlignVCenter);
	//pCustomBtn->SetRightClickedMark(true);

	QRect rcTextPos(40, 0, m_nTabBtnWidth - 40 - 20, m_nTabBtnHeight);
	pCustomBtn->SetCustomButtonTextArea(rcTextPos);
	pCustomBtn->setText(qsBtnText);
	pCustomBtn->setObjectName(qsBtnText);
	m_pBoxLayout->addWidget(pCustomBtn);

	//pCustomBtn->SetUserData(nGenreId);
	//pCustomBtn->SetEditMark(false);
	//if(pMenu)
	//	pCustomBtn->SetContextMenu(pMenu);
	m_obVTabBtnList.push_back(pCustomBtn);
	connect(pCustomBtn, SIGNAL(signalClickBtn(int)), this, SLOT(slotChangeTabBtn(int)));
}

void CCustomButtonGroup::slotLineEditFocusout(QString qsText)
{
	emit signalTextChange(this->sender(), qsText);
}

void CCustomButtonGroup::DeleteGenreTabButtonByPointer(void *pTabButton)
{
	int nIndex = m_obVTabBtnList.indexOf((CCustomButton *)pTabButton, 0);
	if (nIndex != -1)
	{
		QLayoutItem *pLayoutItem = m_pBoxLayout->takeAt(nIndex);
		delete pLayoutItem;
	}
}

CCustomButton *CCustomButtonGroup::GetTabBtnPointer(int nIndex)
{
	return m_obVTabBtnList[nIndex];
}

void CCustomButtonGroup::DeleteGenreTabButtonByIndex(int nIndex)
{
	int nLayoutIndex = m_pBoxLayout->indexOf(m_obVTabBtnList[nIndex]);
	QLayoutItem *pLayoutItem = m_pBoxLayout->takeAt(nLayoutIndex);
	m_pBoxLayout->removeItem(pLayoutItem);
	delete pLayoutItem;

	m_obVTabBtnList[nIndex]->hide();
	m_obVTabBtnList[nIndex]->deleteLater();
	m_obVTabBtnList.removeAt(nIndex);

	// 修改绑定的下标
	//for (int i = nIndex; i < m_obVTabBtnList.size(); i++)
	//{
	//	m_obVTabBtnList[i]->SetUserData(i);
	//}

	this->update();
}

void CCustomButtonGroup::AddHorizontalSpacer()
{
	m_pBoxLayout->addStretch(); 
}

void CCustomButtonGroup::AddVerticalSpacer()
{
	//m_pBoxLayout->addStretch(); 

	m_pSpaceItem = new QSpacerItem(5, 5, QSizePolicy::Ignored, QSizePolicy::Expanding);
	m_pBoxLayout->addSpacerItem(m_pSpaceItem);
}

void CCustomButtonGroup::RemoveVerticalSpacer()
{
	m_pBoxLayout->removeItem(m_pSpaceItem);
}

void CCustomButtonGroup::AddQLabelControl(const QString &qsLabelText)
{
	QLabel *pLabel = new QLabel(qsLabelText, this);
	pLabel->setStyleSheet("QLabel{padding-left: 8px;padding-top: 0px}");
	pLabel->setMinimumSize(this->m_nTabBtnWidth, this->m_nTabBtnHeight);
	pLabel->setMaximumSize(this->m_nTabBtnWidth, this->m_nTabBtnHeight);
	//QFont font = gSkin.GetFontStyle(CSkin::Smaller);
	//pLabel->setFont(font);
	m_pBoxLayout->addWidget(pLabel);
}

void CCustomButtonGroup::SetDefaultDownBtn(int nIndex)
{
	if (nIndex < m_obVTabBtnList.size())
	{
		m_obVTabBtnList[nIndex]->SetBtnState(DOWN_STATE);
		m_nCurTabBtnIndex = nIndex;

		emit signalTabButtonSwitch(nIndex);
	}
}

void CCustomButtonGroup::SetEnableStatus(int nIndex, bool bEnable)
{
	//if (nIndex < m_obVTabBtnList.size())
	//{
	//	m_obVTabBtnList[nIndex]->SetCustomButtonEnable(bEnable);
	//}
}

void CCustomButtonGroup::CalcScrollAreaSize()
{
	int nHeight = (m_obVTabBtnList.size() + 1) * 30;
	int nWidth = 176;

	if (nHeight > 524)
	{
		nWidth = 176 - 10;
		this->setGeometry(0, 0, nWidth, nHeight);
	}
	else
	{
		nHeight = 524;
		this->setGeometry(0, 0, nWidth, nHeight);
	}
}

void CCustomButtonGroup::SetContextMenu(QMenu *pContextMenu)
{
	m_pContextMenu = pContextMenu;
}

void CCustomButtonGroup::contextMenuEvent(QContextMenuEvent *event)
{
	QPoint point = event->pos();				//得到窗口坐标

	if (m_pContextMenu == NULL)
	{
		return;
	}

	m_pContextMenu->exec(QCursor::pos());
}

CCustomButton *CCustomButtonGroup::GetCustomButtonByIndex(unsigned int nIndex)
{
	return m_obVTabBtnList[nIndex];
}

int CCustomButtonGroup::GetTabControlBtnNum()
{
	return m_obVTabBtnList.size();
}

//void CCustomButtonGroup::mouseDoubleClickEvent(QMouseEvent *event)
//{
//	//QLineEdit *pLineEdit = new QLineEdit("New Playlist", this);
//	//QRect rcArea = this->rect();
//	//pLineEdit->setGeometry(0, 0, rcArea.width(), rcArea.height());
//	//pLineEdit->show();
//	//emit signalDoubleClicked();
//	
//	if (NULL == m_pLineEdit)
//	{
//		//QString qsText = this->text();
//		m_pLineEdit = new QLineEdit("New"/*qsText*/, this);
//		QRect rcArea = this->rect();
//		m_pLineEdit->setGeometry(0, 0, rcArea.width(), rcArea.height());
//		m_pLineEdit->show();
//		//emit signalDoubleClicked();
//	}
//}

void CCustomButtonGroup::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}