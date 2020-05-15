#pragma once


class CCustomShadowFrame
{
public:
	CCustomShadowFrame();
	~CCustomShadowFrame();
	void Draw(QWidget *pDestWidget);
	void SetFrameInfo(int nWidth, int nRoundedRadius);

private:
	int m_nShadowFrameWidth;
	int m_nRoundedRadius;
};


