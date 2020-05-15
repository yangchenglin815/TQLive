#pragma once

#include <QDialog>
#include "ui_CLoginDlg.h"
#include "CCustomShadowFrame.h"
#include "CController.h"

class CLoginDlg : public QDialog
{
	Q_OBJECT

public:
	CLoginDlg(QWidget *parent = Q_NULLPTR, bool bLoadUserInfo = true);
	~CLoginDlg();
	void DisableUserInteraction();

public slots:
	void slotLoginBtn();
	void slotLoginTaskNotification(int nStatusCode, QString qsMsg);
	//void slotPeekPassword();
	void slotPhoneLogin();
	void slotAccountLogin();
	void slotAutoLogin();
	void slotAutoLoginTimer();
	void slotRemeberPwdStateChanged(int state);
	void slotAutoLoginStateChanged(int state);

	void onRegisterAccountClicked();
	void onForgetPasswdClicked();
	void onApplyLivePower();

protected:
	void keyPressEvent(QKeyEvent *event);


signals:
	void signalAutoLogin();

protected:
	void paintEvent(QPaintEvent *event);

private:
	void InitConnect();
	void VerifyUserLoginInfo(int statusCode);

private:
	Ui::CLoginDlg ui;
	CCustomShadowFrame m_shadowFrame;
	//bool m_bIsPeekState;
	QList<STAccountInformation> m_obLAccountInformation;
	STAccountInformation m_szAccountInfo[3];
	bool m_bIsInitializedStateA, m_bIsInitializedStateB;
	bool m_bLoadUserInfo;
};
