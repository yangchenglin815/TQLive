#include "CheckBox.h"

CheckBox::CheckBox(QWidget *parent)
	: QCheckBox(parent)
{
	InitUI();
	InitConnect();
}

CheckBox::~CheckBox()
{
	
}

void CheckBox::setData(const ST_GROUP_INFO & groupChatInfo)
{
	QString tempText = "";
	m_pGroupChatInfo = groupChatInfo;

	if (m_pGroupChatInfo.qsGroupNumber.isEmpty())
	{
		tempText = m_pGroupChatInfo.qsGroupName;
	}
	else
	{
		tempText = m_pGroupChatInfo.qsGroupName + QString("(%1)").arg(m_pGroupChatInfo.qsGroupNumber);
	}

	int length = tempText.length();
	if (length > CHECKBOX_MAX_LENGTH)
	{
		setToolTip(tempText);
		tempText.replace(CHECKBOX_MAX_LENGTH, length - CHECKBOX_MAX_LENGTH, "...");
	}
	setText(tempText);
}

void CheckBox::InitUI()
{
	setStyleSheet(QString("QCheckBox{font-size:12px;color:#999999;background:transparent;spacing:6px;} \
                           QCheckBox::indicator{width:12px;height:12px;} \
                           QCheckBox::indicator:unchecked{image:url(:/TQLive/images/settings_radio_default.png);} \
                           QCheckBox::indicator:unchecked:hover{image:url(:/TQLive/images/settings_radio_default_hover.png);} \
                           QCheckBox::indicator:checked{image:url(:/TQLive/images/settings_radio_selected.png);} \
                           QCheckBox::indicator:checked:hover{image:url(:/TQLive/images/settings_radio_selected_hover.png);} \
                           QToolTip{font-family:Microsoft YaHei;font-size:12px;color:#999999;min-height:20px;border-radius:2px;}"));

	setAutoExclusive(true);
}

void CheckBox::InitConnect()
{
	connect(this, &CheckBox::stateChanged, this, [&](int state) {
		if (state == Qt::Checked)
		{
			emit sigOnStateChanged(m_pGroupChatInfo);
		}
	});
}
