#pragma once

#include "MyHeader.h"

class CBitMap
{
private:
	HDC		m_hdc, m_MemDC;
	HBITMAP	m_bitmap, m_oldbitmap;

public:
	HDC	GetMemDC(void) { return m_MemDC; }

public:
	CBitMap* LoadBmp(PTCHAR pFileName);
	void Release(void);

public:
	CBitMap(void);
	~CBitMap(void);
};
