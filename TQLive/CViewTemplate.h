#pragma once
#include "CCustomTableView.h"
#include "CModelTemplate.h"

template<typename T>
class CViewTemplate : public CCustomTableView
{
public:
	CViewTemplate();
	virtual ~CViewTemplate();

	void ResetData(QList<T> &obLInfo);

public:
	CModelTemplate<T> *m_pCustomModel;
};

template<typename T>
CViewTemplate<T>::CViewTemplate()
{
	m_pCustomModel = new CModelTemplate<T>();
	this->setModel(m_pCustomModel);
}


template<typename T>
CViewTemplate<T>::~CViewTemplate()
{
}

template<typename T>
void CViewTemplate<T>::ResetData(QList<T> &obLInfo)
{
	m_pCustomModel->ResetTableModelData(obLInfo);
}


