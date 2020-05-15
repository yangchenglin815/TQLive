#ifndef MFTABCONTROLWIDGETWIDGET_H
#define MFTABCONTROLWIDGETWIDGET_H

class CCustomButton;

enum TAB_CONTROL_STYLE
{
	HORIZONTAL_STYLE,
	VERTICAL_STYLE
};

class CCustomButtonGroup : public QWidget
{
	Q_OBJECT

public:
	CCustomButtonGroup(QWidget *parent = 0);
	CCustomButtonGroup(QWidget *parent, TAB_CONTROL_STYLE eTabControlStyle);
	~CCustomButtonGroup();

	void AddTabButton(const QString &qsBtnText, const QString szBkImagePath[], int nWidth, int nHeight);
	
	// 按钮文本显示位置变化过多
	void AddFunctionTabButton(const QString &qsBtnText, /*const QRect &rcTextPos = QRect(0, 0, 0, 0), */const QString &qsNormal = "", const QString &qsOver = "", const QString &qsPressing = "", const QString &qsDown = "", const QString &qsDisable = "");
	void *AddGenreTabButton(const QString &qsBtnText, /*const QRect &rcTextPos = QRect(0, 0, 0, 0), */const QString &qsNormal = "", const QString &qsOver = "", const QString &qsPressing = "", const QString &qsDown = "", const QString &qsDisable = "");
	void AddGenreStaticTabButton(int nGenreId, const QString &qsBtnText, QMenu* pMenu = nullptr, const QString &qsNormal = "", const QString &qsOver = "", const QString &qsPressing = "", const QString &qsDown = "", const QString &qsDisable = "");
	void DeleteGenreTabButtonByPointer(void *);
	void DeleteGenreTabButtonByIndex(int nIndex);
	void AddHorizontalSpacer();
	void AddVerticalSpacer();
	void RemoveVerticalSpacer();
	CCustomButton *GetTabBtnPointer(int nIndex);
	void AddQLabelControl(const QString &qsLabelText);
	void SetTabControlStyle(TAB_CONTROL_STYLE eTabControlStyle);
	void SetContextMenu(QMenu *pContextMenu);
	void SetCustomButtonGeometry(int nWidth, int nHeight);
	void SetDefaultDownBtn(int nIndex);
	void SetEnableStatus(int nIndex, bool bEnable);
	CCustomButton *GetCustomButtonByIndex(unsigned int nIndex);
	int GetTabControlBtnNum();
	void SetCurrentTabButton(int nTabButtonIndex);
	void CalcScrollAreaSize();
	void ClearAllSubControl();
	
	// 右键菜单
	void contextMenuEvent(QContextMenuEvent *event);
	//void mouseDoubleClickEvent(QMouseEvent *event);
protected:
	void paintEvent(QPaintEvent *event);
	
signals:
	//void signalChangeTabPage(int);
	void signalTabButtonSwitch(int);				
	void signalTextChange(void *, QString);

public slots:
	void slotChangeTabBtn(int nNewTabBtnIndex);
	// 可编辑按钮失去输入焦点 QString Line Edit中的文本
	void slotLineEditFocusout(QString);
	
private:
	int m_nTabBtnHeight;
	int m_nTabBtnWidth;
	int m_nCurTabBtnIndex;
	//TAB_CONTROL_STYLE m_eTabControlStyle;
	QBoxLayout *m_pBoxLayout;
	QSpacerItem *m_pSpaceItem;
	QList<CCustomButton *> m_obVTabBtnList;
	QMenu *m_pContextMenu;
	//QLineEdit *m_pLineEdit;
};

#endif
