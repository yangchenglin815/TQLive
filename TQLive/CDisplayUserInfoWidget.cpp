#include "CDisplayUserInfoWidget.h"

CDisplayUserInfoWidget::CDisplayUserInfoWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//connect(ui.pb_modify_live_room_name, SIGNAL(clicked()), this, SLOT(slotForwardToModificationPage()));
}

CDisplayUserInfoWidget::~CDisplayUserInfoWidget()
{
}

void CDisplayUserInfoWidget::slotForwardToModificationPage()
{

}
