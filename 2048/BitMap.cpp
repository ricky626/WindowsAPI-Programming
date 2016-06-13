#include "StdAfx.h"
#include "BitMap.h"

CBitMap::CBitMap(void)
{
}

CBitMap::~CBitMap(void)
{
	Release();
}

CBitMap* CBitMap::LoadBmp(PTCHAR pFileName)
{
	m_hdc = GetDC(g_hWnd);
	m_MemDC = CreateCompatibleDC(m_hdc);

	m_bitmap = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (!m_bitmap)
	{
		MessageBox(g_hWnd, pFileName, L"LOAD_IMAGE_FALSE", FALSE);
	}

	m_oldbitmap = (HBITMAP)SelectObject(m_MemDC, m_bitmap);

	return this;
}

void CBitMap::Release(void)
{
	SelectObject(m_MemDC, m_bitmap);
	DeleteDC(m_MemDC);
	DeleteObject(m_bitmap);
	DeleteObject(m_oldbitmap);
	ReleaseDC(g_hWnd, m_hdc);
}