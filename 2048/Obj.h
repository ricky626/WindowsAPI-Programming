#pragma once

#include "MyHeader.h"

class CBitMap;

class CObj
{
protected:
	CBitMap* m_pBit;

public:
	virtual void Initialize() = 0;
	virtual void Progress() = 0;
	virtual void Render(HDC) = 0;
	virtual void Release() = 0;

public:
	CObj();
	virtual ~CObj();
};

