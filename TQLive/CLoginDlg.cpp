#include "stdafx.h"
#include "CController.h"
#include "CTaskNotification.h"
#include "CLoginDlg.h"
#include "CDataCentre.h"
#include "ConstData.h"
#include "EnumData.h"
#include "TipsDlg.h"

#include <QDesktopServices>
#include <QKeyEvent>

CLoginDlg::CLoginDlg(QWidget *parent, bool bLoadUserInfo)
	: QDialog(parent)
	//, m_bIsPeekState(false)
	, m_bIsInitializedStateA(false)
	, m_bIsInitializedStateB(false)
	, m_bLoadUserInfo(bLoadUserInfo)
{
	ui.setupUi(this);

	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowTitle(QStringLiteral("登录"));

	g_dataCentre.InitConfiguration();

	CController::GetInstance().InitController();

	VerifyUserLoginInfo(LOGIN_INITIALIZE);

	//bool result = ui.horizontalLayout_D->setAlignment(ui.pb_retrieve_pwd, Qt::AlignBottom);
	//ui.horizontalLayout_D->setAlignment(ui.pb_free_register, Qt::AlignBottom);
	//ui.horizontalLayout_B->setAlignment(ui.pb_login, Qt::AlignTop);
	//ui.horizontalLayout_C->setAlignment(ui.pb_weibo_login, Qt::AlignTop);
	//ui.horizontalLayout_C->setAlignment(ui.pb_qq_login, Qt::AlignTop);
	//ui.horizontalLayout_C->setAlignment(ui.pb_wechat_login, Qt::AlignTop);

	ui.lb_phone_login->adjustSize();

	//ui.cb_close_window->SetCustomButtonBkFileName("cb_close_window_normal.png", "cb_close_window_hover.png", "cb_close_window_pressing.png");
	//ui.cb_minimize_window->SetCustomButtonBkFileName("cb_minimize_window_normal.png", "cb_minimize_window_hover.png", "cb_minimize_window_pressing.png");

	connect(ui.pb_login, SIGNAL(clicked()), this, SLOT(slotLoginBtn()));
	connect(this, SIGNAL(signalAutoLogin()), this, SLOT(slotLoginBtn()));
	//connect(ui.cb_minimize_window, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(ui.cb_close_window, SIGNAL(clicked()), this, SLOT(close()));

	//connect(ui.lb_peek_password, &CCustomLabel::clicked, this, &CLoginDlg::slotPeekPassword); 

	connect(ui.lb_phone_login, &CCustomLabel::clicked, this, &CLoginDlg::slotPhoneLogin);
	connect(ui.lb_account_login, &CCustomLabel::clicked, this, &CLoginDlg::slotAccountLogin);

	InitConnect();

	//connect(ui.cb_remember_pwd, &QCheckBox::stateChanged, this, &CLoginDlg::slotRemeberPwdStateChanged);
	//connect(ui.cb_auto_login, &QCheckBox::stateChanged, this, &CLoginDlg::slotAutoLoginStateChanged);
	

	//ui.le_password->setEchoMode(QLineEdit::Password);

	//ui.le_account_number->setStyleSheet("QLineEdit { font-family: \"Segoe UI\";"
	//	"font-size: 16px;"
	//	"}");

	// 

	// Segoe UI字体在不同尺寸下的字体渲染效果（未达到14pt无抗锯齿效果）
	// ui.cb_remember_pwd->setStyleSheet("font: 14pt \"Segoe UI\";");
	// ui.cb_auto_login->setStyleSheet("font: 20px \"Segoe UI\";");

	//ui.cb_remember_pwd->setStyleSheet("QCheckBox { font-family: \"Segoe UI\";"
	//	"font-size: 16px;"
	//	"}");

	QFont fontA("Arial", 20, QFont::Normal);
	//fontA.setPixelSize(20);
	
	//ui.le_account_number->setFont(fontA);
	//CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
	//pPhoneLoginWidget->ui.le_phone_number->setFont(fontA);

	//ui.cb_remember_pwd->setFont(fontA);

	//QFont font = ui.le_account_number->font();
	//int nFontSize = font.pixelSize();
	//int nPointSize = font.pointSize();

	//QFontMetrics fontMetrics(font);
	//int nHeight = fontMetrics.height();		//12
	//int nAscent = fontMetrics.ascent();		//10
	//int nDescent = fontMetrics.descent();	//2

	bool result = connect((QObject *)CTaskNotification::GetInstance(), SIGNAL(signalLoginTaskNotification(int, QString)), this, SLOT(slotLoginTaskNotification(int, QString)));

	//ui.sw_login_method->setCurrentIndex(0);
	//ui.lb_phone_login->setPixmap(QPixmap(":/TQLive/images/lb_accout_login_normal.png"));

	if (m_bLoadUserInfo == true)
	{
		QString qsAccountNumber, qsPassword;
		bool isRememberPwd = false;
		//QList<STAccountInformation> obLAccountInformation;
		//CController::GetInstance().AcquireUserInformation(obLAccountInformation);

		CController::GetInstance().AcquireUserInformation(m_obLAccountInformation);

		STAccountInformation stAccountInfo;

		// 手机登录页面初始化
		stAccountInfo.eLoginMethod = kPhoneMethod;
		int posA = m_obLAccountInformation.indexOf(stAccountInfo);

		if (posA != -1)
		{
			//qsAccountNumber = m_obLAccountInformation[posA].qsAccountNumber;
			//qsPassword = m_obLAccountInformation[posA].qsPassword;
			//isRememberPwd = m_obLAccountInformation[posA].isRememberPwd;

			//pPhoneLoginWidget->ui.le_phone_number->setText(qsAccountNumber);

			//ui.cb_remember_pwd->setChecked(isRememberPwd);

			////if (isRememberPwd == true)
			////{
			////	ui.le_password->setText(qsPassword);
			////}

			//if (isRememberPwd == true)
			//{
			//	pPhoneLoginWidget->ui.le_password->setText(qsPassword);
			//}

			m_szAccountInfo[0] = m_obLAccountInformation[posA];

			CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
			pPhoneLoginWidget->ui.le_phone_number->setText(m_szAccountInfo[0].qsAccountNumber);

			if (m_szAccountInfo[0].isRememberPwd == true)
			{
				pPhoneLoginWidget->ui.le_password->setText(m_szAccountInfo[0].qsPassword);
				pPhoneLoginWidget->ui.cb_remember_pwd->setChecked(m_szAccountInfo[0].isRememberPwd);
			}
		}

		// 探球号登录页面初始化
		stAccountInfo.eLoginMethod = kAccountMethod;
		int posB = m_obLAccountInformation.indexOf(stAccountInfo);
		if (posB != -1)
		{
			//qsAccountNumber = obLAccountInformation[posB].qsAccountNumber;
			m_szAccountInfo[1] = m_obLAccountInformation[posB];

			CAccountLoginWidget *pAccountLoginWidget = (CAccountLoginWidget *)ui.sw_login_method->widget(1);
			pAccountLoginWidget->ui.le_account_number->setText(m_szAccountInfo[1].qsAccountNumber);

			if (m_szAccountInfo[1].isRememberPwd)
			{
				pAccountLoginWidget->ui.le_password->setText(m_szAccountInfo[1].qsPassword);
				pAccountLoginWidget->ui.cb_remember_pwd->setChecked(m_szAccountInfo[1].isRememberPwd);
			}

			// 不适配手机号不记住，探球号记住的情景
			//ui.cb_remember_pwd->setChecked(m_szAccountInfo[1].isRememberPwd);
		}


		if (!m_obLAccountInformation.isEmpty())
		{
			// 共用页面初始化
			if (m_obLAccountInformation[0].autoLoginMark == true)
			{
				//ui.cb_auto_login->setChecked(true);

				if (m_obLAccountInformation[0].eLoginMethod == kAccountMethod)
				{
					//ui.sw_login_method->setCurrentIndex(1);
					this->slotAccountLogin();
					CAccountLoginWidget *pAccountLoginWidget = (CAccountLoginWidget *)ui.sw_login_method->widget(1);
					pAccountLoginWidget->ui.cb_auto_login->setChecked(true);

					//ui.cb_remember_pwd->setChecked(m_szAccountInfo[1].isRememberPwd);
					//m_bIsInitializedStateB = true;
				}
				else
				{
					//ui.cb_remember_pwd->setChecked(m_szAccountInfo[0].isRememberPwd);
					//m_bIsInitializedStateA = true;

					CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->widget(0);
					pPhoneLoginWidget->ui.cb_auto_login->setChecked(true);

				}

				QTimer::singleShot(1000, this, SLOT(slotAutoLoginTimer()));
			}
		}
	}

	

	//if (!obLAccountInformation.isEmpty())
	//{
	//	qsAccountNumber = obLAccountInformation[0].qsAccountNumber;
	//	qsPassword = obLAccountInformation[0].qsPassword;
	//	isRememberPwd = obLAccountInformation[0].isRememberPwd;
	//}

	//ui.le_account_number->setText(qsAccountNumber);
	//CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
	
	
	//pPhoneLoginWidget->ui.le_phone_number->setText(qsAccountNumber);

	//ui.cb_remember_pwd->setChecked(isRememberPwd);

	//if (isRememberPwd == true)
	//{
	//	ui.le_password->setText(qsPassword);
	//}

	/*隐藏部分功能，二期开发*/
	ui.label_5->hide();
	ui.line->hide();
	ui.line_3->hide();

	ui.label_6->hide();
	ui.label_7->hide();
	ui.label_8->hide();

	ui.pb_login->setProperty("userId", 777);
}

CLoginDlg::~CLoginDlg()
{
	int userId = ui.pb_login->property("userId").toInt();
}

void CLoginDlg::slotAutoLoginTimer()
{
	emit signalAutoLogin();
}

void CLoginDlg::slotRemeberPwdStateChanged(int state)
{
	//if (Qt::Unchecked == state)
	//{
	//	ui.cb_auto_login->setChecked(false);
	//}
}

void CLoginDlg::slotAutoLoginStateChanged(int state)
{
	//if (Qt::Checked == state)
	//{
	//	ui.cb_remember_pwd->setChecked(true);
	//}
}

void CLoginDlg::onRegisterAccountClicked()
{
	QString url = "";

	if (g_dataCentre.m_eEnvironmentType == kTestEnvironment)
	{
		url = REGISTER_ACCOUNT_TEST_API;
	}
	else if (g_dataCentre.m_eEnvironmentType == kFormalEnvironment)
	{
		url = REGISTER_ACCOUNT_API;
	}

	if (!url.isEmpty())
	{
		QDesktopServices::openUrl(QUrl(url));
	}
}

void CLoginDlg::onForgetPasswdClicked()
{
	QString url = "";

	if (g_dataCentre.m_eEnvironmentType == kTestEnvironment)
	{
		url = FORGET_PASSWD_TEST_API;
	}
	else if (g_dataCentre.m_eEnvironmentType == kFormalEnvironment)
	{
		url = FORGET_PASSWD_API;
	}

	if (!url.isEmpty())
	{
		QDesktopServices::openUrl(QUrl(url));
	}
}

void CLoginDlg::onApplyLivePower()
{
	QString url = "";

	if (g_dataCentre.m_eEnvironmentType == kTestEnvironment)
	{
		url = LIVE_PERSONAL_TEST_API;
	}
	else if (g_dataCentre.m_eEnvironmentType == kFormalEnvironment)
	{
		url = LIVE_PERSONAL_API;
	}

	if (!url.isEmpty())
	{
		QDesktopServices::openUrl(QUrl(url));
	}
}

void CLoginDlg::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
		slotLoginBtn();
	}
}

void CLoginDlg::slotLoginBtn()
{
	//QSize sizeA = ui.pb_retrieve_pwd->sizeHint();
	//QSize sizeB = ui.pb_retrieve_pwd->size();
	//QSize sizeC = ui.pb_retrieve_pwd->minimumSizeHint();

	//int left, top, right, bottom;
	//ui.horizontalLayout_D->getContentsMargins(&left, &top, &right, &bottom);
	//QRect rc = ui.horizontalLayout_D->geometry();

	VerifyUserLoginInfo(LOGIN_INITIALIZE);

	//QString qsAccountNumber = ui.le_account_number->text();

	QString qsAccountNumber;
	QString qsPassword;
	EN_LOGIN_METHOD eLoginMethod;

	int nIndex = ui.sw_login_method->currentIndex();

	if (nIndex == 0)
	{
		CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
		qsAccountNumber = pPhoneLoginWidget->ui.le_phone_number->text();
		qsPassword = pPhoneLoginWidget->ui.le_password->text();
		eLoginMethod = kPhoneMethod;
		m_szAccountInfo[2].isRememberPwd = pPhoneLoginWidget->ui.cb_remember_pwd->isChecked();
		m_szAccountInfo[2].autoLoginMark = pPhoneLoginWidget->ui.cb_auto_login->isChecked();

		if (qsAccountNumber.isEmpty())
		{
			pPhoneLoginWidget->ui.le_phone_number->setFocus();
			VerifyUserLoginInfo(LOGIN_ACCOUNT_NULL);
			return;
		}
		else if (qsAccountNumber.length() != 11)
		{
			pPhoneLoginWidget->ui.le_phone_number->setFocus();
			pPhoneLoginWidget->ui.le_phone_number->clear();
			VerifyUserLoginInfo(LOGIN_PHONENUMBER_FORMAT_ERR);
			return;
		}
		else if (qsPassword.isEmpty())
		{
			pPhoneLoginWidget->ui.le_password->setFocus();
			VerifyUserLoginInfo(LOGIN_PASSWD_NULL);
			return;
		}
		else if (qsPassword.length() < 6)
		{
			pPhoneLoginWidget->ui.le_password->setFocus();
			pPhoneLoginWidget->ui.le_password->clear();
			VerifyUserLoginInfo(LOGIN_PASSWD_FORMAT_ERR);
			return;
		}
	}
	else
	{
		CAccountLoginWidget *pAccountLoginWidget = (CAccountLoginWidget *)ui.sw_login_method->currentWidget();
		qsAccountNumber = pAccountLoginWidget->ui.le_account_number->text();
		qsPassword = pAccountLoginWidget->ui.le_password->text();
		eLoginMethod = kAccountMethod;
		m_szAccountInfo[2].isRememberPwd = pAccountLoginWidget->ui.cb_remember_pwd->isChecked();
		m_szAccountInfo[2].autoLoginMark = pAccountLoginWidget->ui.cb_auto_login->isChecked();

		if (qsAccountNumber.isEmpty())
		{
			pAccountLoginWidget->ui.le_account_number->setFocus();
			VerifyUserLoginInfo(LOGIN_ACCOUNT_NULL);
			return;
		}
		else if (qsPassword.isEmpty())
		{
			pAccountLoginWidget->ui.le_password->setFocus();
			VerifyUserLoginInfo(LOGIN_PASSWD_NULL);
			return;
		}
		else if (qsPassword.length() < 6)
		{
			pAccountLoginWidget->ui.le_password->setFocus();
			pAccountLoginWidget->ui.le_password->clear();
			VerifyUserLoginInfo(LOGIN_PASSWD_FORMAT_ERR);
			return;
		}
	}

	m_szAccountInfo[2].eLoginMethod = eLoginMethod;
	m_szAccountInfo[2].qsAccountNumber = qsAccountNumber;
	m_szAccountInfo[2].qsPassword = qsPassword;


	//QString qsPassword = ui.le_password->text();

	ui.pb_login->setText(QString::fromLocal8Bit("正在登录..."));
	ui.pb_login->setEnabled(false);

	CController::GetInstance().WriteToLogFile("login client start. user:%s", qsAccountNumber.toLocal8Bit().constData());
	CController::GetInstance().PostLoginTask(qsAccountNumber, qsPassword, nIndex);
	//CController::GetInstance().PostInitTask();
}

void CLoginDlg::slotAutoLogin()
{
	//ui.lb_prompt_msg->setText("");

	//QString qsAccountNumber;
	//QString qsPassword;
	//EN_LOGIN_METHOD eLoginMethod;

	//if (ui.sw_login_method->currentIndex() == 0)
	//{
	//	CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
	//	qsAccountNumber = pPhoneLoginWidget->ui.le_phone_number->text();
	//	qsPassword = m_szAccountInfo[0].qsPassword;
	//	eLoginMethod = kPhoneMethod;

	//}
	//else
	//{
	//	CAccountLoginWidget *pAccountLoginWidget = (CAccountLoginWidget *)ui.sw_login_method->currentWidget();
	//	qsAccountNumber = pAccountLoginWidget->ui.le_account_number->text();
	//	qsPassword = m_szAccountInfo[1].qsPassword;
	//	eLoginMethod = kAccountMethod;
	//}

	//m_szAccountInfo[2].eLoginMethod = eLoginMethod;
	//m_szAccountInfo[2].qsAccountNumber = qsAccountNumber;
	//m_szAccountInfo[2].qsPassword = qsPassword;
	//m_szAccountInfo[2].isRememberPwd = ui.cb_remember_pwd->isChecked();
	//m_szAccountInfo[2].autoLoginMark = ui.cb_auto_login->isChecked();


	////QString qsPassword = ui.le_password->text();

	//ui.pb_login->setText(QString::fromLocal8Bit("正在登录..."));
	//ui.pb_login->setEnabled(false);
	//CController::GetInstance().PostLoginTask(qsAccountNumber, qsPassword);
}

void CLoginDlg::slotLoginTaskNotification(int nStatusCode, QString qsMsg)
{
	CController::GetInstance().WriteToLogFile("login client end. errorCode:%d errorDesc:%s", nStatusCode, qsMsg.toLocal8Bit().constData());

	if (nStatusCode == 200)
	{
		this->accept();

		//QString qsAccountNumber;

		//if (ui.sw_login_method->currentIndex() == 0)
		//{
		//	CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
		//	qsAccountNumber = pPhoneLoginWidget->ui.le_phone_number->text();
		//}
		//else
		//{
		//	CAccountLoginWidget *pAccountLoginWidget = (CAccountLoginWidget *)ui.sw_login_method->currentWidget();
		//	qsAccountNumber = pAccountLoginWidget->ui.le_account_number->text();
		//}

		//QString qsAccountNumber = ui.le_account_number->text();
		//QString qsPassword = ui.le_password->text();

		//bool isRememberPwd = false;
		//if (ui.cb_remember_pwd->isChecked())  
		//{
		//	isRememberPwd = true;
		//}

		//CController::GetInstance().RecordUserInformation(qsAccountNumber, qsPassword, isRememberPwd, kPhoneMethod);

		g_dataCentre.m_stCurrentAccountInfo = m_szAccountInfo[2];
		CController::GetInstance().RecordUserInformation(m_szAccountInfo[2].qsAccountNumber, m_szAccountInfo[2].qsPassword, m_szAccountInfo[2].isRememberPwd, m_szAccountInfo[2].eLoginMethod, m_szAccountInfo[2].autoLoginMark);
		
	}
	else
	{
		ui.pb_login->setText(QString::fromLocal8Bit("登录"));
		qDebug("login failure. nStatusCode:%d", nStatusCode);

		VerifyUserLoginInfo(nStatusCode);
	}

	ui.pb_login->setEnabled(true);

	if (nStatusCode == LOGIN_REFUSED_BY_POWER)
	{
		TipsDlg tipsDlg(DIALOG_IDENTITY_TIPS_OPERATOR, this);
		if (tipsDlg.exec() == QDialog::Accepted)
		{
			onApplyLivePower();
		}
	}
}

void CLoginDlg::paintEvent(QPaintEvent *event)
{
	QDialog::paintEvent(event);
	//QStyleOption option;
	//option.init(this);

	//QPainter painter(this);
	//style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

	//m_shadowFrame.Draw(this);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;

	QBrush brush(QColor(47, 47, 47));
	painter.setBrush(brush);
	painter.setPen(Qt::transparent);

	QRect rect = this->rect();
	rect.setWidth(rect.width());
	rect.setHeight(rect.height());
	painter.drawRoundedRect(rect, 4, 4);

	//rect = this->rect();
	//QPen pen(QColor(255, 0, 0));
	//pen.setWidth(5);
	//painter.setPen(pen);

	//painter.drawLine(70, 0, rect.width(), 0);
	//painter.drawLine(0, 0, 0, 50);
	//painter.drawLine(rect.width() - 5, 0, rect.width() - 5, 50);
}

void CLoginDlg::InitConnect()
{
	connect(ui.btn_register, SIGNAL(clicked()), this, SLOT(onRegisterAccountClicked()));
	connect(ui.btn_forget_passwd, SIGNAL(clicked()), this, SLOT(onForgetPasswdClicked()));
}

void CLoginDlg::VerifyUserLoginInfo(int statusCode)
{
	QString errMsg = "";

	switch (statusCode)
	{
	case LOGIN_ACCOUNT_NULL:
		if (ui.sw_login_method->currentIndex() == 0)
		{
			errMsg = QStringLiteral("请输入手机号码！");
		}
		else
		{
			errMsg = QStringLiteral("请输入探球号/昵称！");
		}
		break;
	case LOGIN_PASSWD_NULL:
		errMsg = QStringLiteral("请输入登录密码！");
		break;
	case LOGIN_PASSWD_FORMAT_ERR:
		errMsg = QStringLiteral("请输入6~25位的登录密码！");
		break;
	case LOGIN_PHONENUMBER_FORMAT_ERR:
		errMsg = QStringLiteral("请输入有效的手机号码！");
		break;
	case LOGIN_CONNECTION_TIMEOUT1:
	case LOGIN_CONNECTION_TIMEOUT2:
		errMsg = QStringLiteral("连接超时请稍后再试！");
		break;
	case LOGIN_ACCOUNT_INFO_ERR:
		if (ui.sw_login_method->currentIndex() == 0)
		{
			errMsg = QStringLiteral("手机号或密码不正确！");
		}
		else
		{
			errMsg = QStringLiteral("探球号/昵称或密码不正确！");
		}
		break;
	case LOGIN_FORBIDDEN:
		errMsg = QStringLiteral("您的账号已被封停！");
		break;
	case LOGIN_REFUSED_BY_POWER:
		errMsg = QStringLiteral("您还不是主播，快去申请成为主播吧！");
		break;
	case LOGIN_FORBIDDEN_LIMIT:
		errMsg = QStringLiteral("您的账号已被限时停播！");
		break;
	case LOGIN_FORBIDDEN_FOREVER:
		errMsg = QStringLiteral("抱歉！您的账号已被永久停播。");
		break;
	case 0:
		errMsg = "";
		break;
	case LOGIN_UNABLE_TO_CONNECT_SERVER:
		errMsg = QStringLiteral("无法连接服务器，请稍后重试。");
		break;
	case LOGIN_NETWORK_CONNECTION_ERROR:
		errMsg = QStringLiteral("网络连接异常，请检查网络设置");
		break;
	case LOGIN_UNKNOWN_ERROR:
		errMsg = QStringLiteral("网络连接异常，请稍后重试");
		break;
	default:
		errMsg = QStringLiteral("未知错误！错误码：%1").arg(statusCode);
		break;
	}

	ui.lb_prompt_msg->setText(errMsg);
}

//void CLoginDlg::slotPeekPassword()
//{
//	m_bIsPeekState = !m_bIsPeekState;
//
//	QString qsFilePath;
//
//	if (m_bIsPeekState)
//	{
//		qsFilePath = ":/TQLive/images/lb_peek_password.png";
//		ui.le_password->setEchoMode(QLineEdit::Normal);
//		
//	}
//	else
//	{
//		qsFilePath = ":/TQLive/images/lb_unpeek_password.png";
//		ui.le_password->setEchoMode(QLineEdit::Password);
//	}
//
//	QPixmap pixmap(qsFilePath);
//	ui.lb_peek_password->setPixmap(pixmap);
//}

void CLoginDlg::slotPhoneLogin()
{
	if (ui.sw_login_method->currentIndex() == 0)
	{
		return;
	}

	//if (m_bIsInitializedStateA == false)
	//{
	//	m_bIsInitializedStateA = true;
	//	ui.cb_remember_pwd->setChecked(m_szAccountInfo[0].isRememberPwd);
	//}

	ui.lb_account_login->setPixmap(QPixmap(":/TQLive/images/lb_account_login_normal.png"));
	ui.lb_phone_login->setPixmap(QPixmap(":/TQLive/images/lb_phone_login_pressed.png"));
	ui.sw_login_method->setCurrentIndex(0);

	//QString qsAccountNumber, qsPassword;
	//bool isRememberPwd = false;

	//STAccountInformation stAccountInfo;
	//stAccountInfo.eLoginMethod = kPhoneMethod;
	//int posA = m_obLAccountInformation.indexOf(stAccountInfo);

	//if (posA != -1)
	//{
	//	qsAccountNumber = m_obLAccountInformation[posA].qsAccountNumber;
	//	qsPassword = m_obLAccountInformation[posA].qsPassword;
	//	isRememberPwd = m_obLAccountInformation[posA].isRememberPwd;
	//}
	//
	//CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
	//pPhoneLoginWidget->ui.le_phone_number->setText(qsAccountNumber);

	//ui.cb_remember_pwd->setChecked(isRememberPwd);
	//pPhoneLoginWidget->ui.le_password->setText(qsPassword);

	//CPhoneLoginWidget *pPhoneLoginWidget = (CPhoneLoginWidget *)ui.sw_login_method->currentWidget();
	//pPhoneLoginWidget->ui.le_phone_number->setText(m_szAccountInfo[0].qsAccountNumber);
	//pPhoneLoginWidget->ui.le_password->setText(m_szAccountInfo[0].qsPassword);

	//ui.cb_remember_pwd->setChecked(m_szAccountInfo[0].isRememberPwd);
}



void CLoginDlg::slotAccountLogin()
{
	if (ui.sw_login_method->currentIndex() == 1)
	{
		return;
	}

	//if (m_bIsInitializedStateB == false)
	//{
	//	m_bIsInitializedStateB = true;
	//	ui.cb_remember_pwd->setChecked(m_szAccountInfo[1].isRememberPwd);
	//}

	ui.lb_phone_login->setPixmap(QPixmap(":/TQLive/images/lb_phone_login_normal.png"));
	ui.lb_account_login->setPixmap(QPixmap(":/TQLive/images/lb_account_login_pressed.png"));
	ui.sw_login_method->setCurrentIndex(1);

	//QString qsAccountNumber, qsPassword;
	//bool isRememberPwd = false;

	//STAccountInformation stAccountInfo;
	//stAccountInfo.eLoginMethod = kAccountMethod;
	//int posA = m_obLAccountInformation.indexOf(stAccountInfo);

	//if (posA != -1)
	//{
	//	qsAccountNumber = m_obLAccountInformation[posA].qsAccountNumber;
	//	qsPassword = m_obLAccountInformation[posA].qsPassword;
	//	isRememberPwd = m_obLAccountInformation[posA].isRememberPwd;
	//}

	//CAccountLoginWidget *pAccountLoginWidget = (CAccountLoginWidget *)ui.sw_login_method->currentWidget();
	//pAccountLoginWidget->ui.le_account_number->setText(qsAccountNumber);


	//ui.cb_remember_pwd->setChecked(isRememberPwd);
	//pAccountLoginWidget->ui.le_password->setText(qsPassword);

	//CAccountLoginWidget *pAccountLoginWidget = (CAccountLoginWidget *)ui.sw_login_method->currentWidget();
	//pAccountLoginWidget->ui.le_account_number->setText(m_szAccountInfo[1].qsAccountNumber);
	//pAccountLoginWidget->ui.le_password->setText(m_szAccountInfo[1].qsPassword);
	//ui.cb_remember_pwd->setChecked(m_szAccountInfo[1].isRememberPwd);
}
	
