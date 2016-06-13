#pragma once
#include "Obj.h"

class CBitMap;

class CBlock :
	public CObj
{
	BlockInfo m_tInfo;
	BLOCK_STATE m_eState;

	Point2D m_ptMovePower;

public:
	inline const BlockInfo& GetBlockInfo() const { return m_tInfo; }
	inline const BLOCK_STATE GetBlockState() const { return m_eState; }

public:
	//inline void SetBlockPos(const Point2D& pt) { m_tInfo.pos = pt; }
	inline void SetBlockIndex(int idx) { m_tInfo.index = idx; }
	inline void SetBlockValue(int val) { m_tInfo.value = val; }
	inline void SetBlockState(BLOCK_STATE state) { m_eState = state; }
	inline void SetBlockAddCheck(int chk) { m_tInfo.addCheck = chk; }

	inline void SetMovePower(const Point2D& power) { m_ptMovePower = power; }

	void SetBlockBmp(int val);

public:
	void Move();

public:
	virtual void Initialize();
	virtual void Progress();
	virtual void Render(HDC);
	virtual void Release();

public:
	CBlock();
	CBlock(const Point2D&, int, int);
	virtual ~CBlock();
};

