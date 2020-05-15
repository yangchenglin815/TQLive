#pragma once

enum BUTTON_STATE {
	NORMAL_STATE,
	OVER_STATE,
	PRESSING_STATE,
	DOWN_STATE,
	DISABLE_STATE
};

#define BT_NORMAL			0x00000001
#define BT_OVER				0x00000002
#define BT_PRESSING			0x00000010
#define BT_DOWN				0x00000004
#define BT_DISABLE			0x00000020
#define BT_CUSTOM_CURSOR	0x00000040

class CCustomButton : public QPushButton
{
    Q_OBJECT

public:
    CCustomButton(QWidget *parent);
    ~CCustomButton();

    void SetBtnType(int nBtnType);
	void SetBtnState(BUTTON_STATE eBtnState);
	void SetBtnTextColor(const QColor &color = QColor(94, 162, 14));
    void SetBtnTextFont(const QFont &font);

	void SetUserData(int userData)
	{
		m_nUserData = userData;
	}

	int GetUserData()
	{
		return m_nUserData;
	}

    void SetCustomButtonGeometry(int nWidth, int nHeight);
    void SetCustomButtonTextArea(const QRect &rcButtonText);
    void SetCustomButtonTextAlignmentStyle(Qt::Alignment eAlignmentStyle);

    void SetCustomButtonTextColor(const QColor &qcNormal = QColor(255, 255, 255),
                                  const QColor &qcOver = QColor(255, 255, 255),
                                  const QColor &qcPressing = QColor(255, 255, 255),
                                  const QColor &qcDown = QColor(255, 255, 255),
                                  const QColor &qcDisable = QColor(255, 255, 255));

    void SetCustomButtonBkFileName(const QString &qsNormal = "", const QString &qsOver = "", const QString &qsPressing = "", const QString &qsDown = "", const QString &qsDisable = "");
  
protected:
    virtual void enterEvent(QEvent *);  
    virtual void leaveEvent(QEvent *);  
    virtual void mousePressEvent(QMouseEvent *event);  
    virtual void mouseReleaseEvent(QMouseEvent *event);  
    virtual void paintEvent(QPaintEvent *);
	virtual bool event(QEvent *event);

signals:
	void signalCustomClicked(int);

private slots:
	void slotReponseClicked();

private:
    QString m_szFileName[5];
    QColor m_szTextColor[5];
    QString m_qsFileNamePrefix;
    int m_nBtnType;
    int m_nCurrentBtnState;					// 按钮当前状态
    int m_nEnterBtnState;					// 进入时按钮状态 normal、down（稳定状态)
    int m_nWidth;
    int m_nHeight;
    QRect m_rcButtonText;
	Qt::Alignment m_eAlignmentStyle;
	int m_nUserData;
 };
