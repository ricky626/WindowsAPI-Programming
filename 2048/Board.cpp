#include "stdafx.h"
#include "Board.h"

#include "BitMap.h"

CBoard::CBoard()
{
}

CBoard::~CBoard()
{
	Release();
}

void CBoard::Initialize()
{
	int tileSize = TILE_SIZE * 2 + 10;

	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		for (int j = 0; j < BOARD_HEIGHT; ++j)
		{
			Point2D temp((j * tileSize) + tileSize, (i * tileSize) + tileSize);

			m_vecTile.push_back(new Tile(temp));
		}
	}

	m_pBit = new CBitMap;
	m_pBit->LoadBmp(L"../Texture/board.bmp");
}

void CBoard::Progress()
{

}

void CBoard::Render(HDC hdc)
{
	BitBlt(hdc,
		m_vecTile[0]->pos.x - TILE_SIZE - 10, m_vecTile[0]->pos.y - TILE_SIZE - 10,
		450, 450, m_pBit->GetMemDC(), 0, 0, SRCCOPY);
}

void CBoard::Release()
{
	for (auto p = m_vecTile.begin(); p != m_vecTile.end(); )
	{
		SAFE_DELETE(*p);
		p = m_vecTile.erase(p);
	}
}

void CBoard::Reset()
{
	for (auto p = m_vecTile.begin(); p != m_vecTile.end(); ++p)
		(*p)->option = 0;
}