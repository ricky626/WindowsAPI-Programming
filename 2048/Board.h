#pragma once
#include "Obj.h"

class CBoard :
	public CObj
{
	vector<Tile*> m_vecTile;

public:
	inline const vector<Tile*>& GetTile() const { return m_vecTile; }

public:
	inline void SetTileOption(int idx, int op) { m_vecTile[idx]->option = op; }

public:
	void Reset();

public:
	virtual void Initialize();
	virtual void Progress();
	virtual void Render(HDC);
	virtual void Release();

public:
	CBoard();
	virtual ~CBoard();
};

