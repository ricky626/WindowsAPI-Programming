#include "stdafx.h"
#include "Block.h"

#include "BitMap.h"

CBlock::CBlock()
{
}

CBlock::CBlock(const Point2D& pos, int val, int idx)
	: m_tInfo(pos, val, idx)
{
}

CBlock::~CBlock()
{
	Release();
}

void CBlock::Initialize()
{
	m_pBit = new CBitMap;
	m_pBit->LoadBmp(L"../Texture/2.bmp");
}

void CBlock::Progress()
{
}

void CBlock::Render(HDC hdc)
{
	BitBlt(hdc,
		m_tInfo.pos.x - TILE_SIZE, m_tInfo.pos.y - TILE_SIZE,
		450, 450, m_pBit->GetMemDC(), 0, 0, SRCCOPY);
}

void CBlock::Release()
{

}

void CBlock::Move()
{
	m_tInfo.pos.x += m_ptMovePower.x;
	m_tInfo.pos.y += m_ptMovePower.y;
}

void CBlock::SetBlockBmp(int val)
{
	switch (val)
	{
	case 4:
		m_pBit->LoadBmp(L"../Texture/4.bmp");
		break;

	case 8:
		m_pBit->LoadBmp(L"../Texture/8.bmp");
		break;

	case 16:
		m_pBit->LoadBmp(L"../Texture/16.bmp");
		break;

	case 32:
		m_pBit->LoadBmp(L"../Texture/32.bmp");
		break;

	case 64:
		m_pBit->LoadBmp(L"../Texture/64.bmp");
		break;

	case 128:
		m_pBit->LoadBmp(L"../Texture/128.bmp");
		break;

	case 256:
		m_pBit->LoadBmp(L"../Texture/256.bmp");
		break;

	case 512:
		m_pBit->LoadBmp(L"../Texture/512.bmp");
		break;

	case 1024:
		m_pBit->LoadBmp(L"../Texture/1024.bmp");
		break;

	case 2048:
		m_pBit->LoadBmp(L"../Texture/2048.bmp");
		break;
	}
}