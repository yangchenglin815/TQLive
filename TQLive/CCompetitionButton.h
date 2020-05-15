#pragma once

#include <QPushButton>

class CCompetitionButton : public QPushButton
{
	Q_OBJECT

public:
	CCompetitionButton(QWidget *parent);
	~CCompetitionButton();

	void SetUserData(int nUserData);
	int GetUserID();

protected:
	virtual void paintEvent(QPaintEvent *);

private:
	int m_nUserData;
};
