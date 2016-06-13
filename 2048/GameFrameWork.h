#pragma once

#include "MyHeader.h"

class CObj;
class CBlock;
class CBitMap;

class CGameFrameWork
{
	HDC				m_hdc;
	HINSTANCE		m_hInstance;

	int				m_nWndClientWidth;
	int				m_nWndClientHeight;

	int				m_nGoal;
	int				m_nScore;

	bool			m_bMove;

	KEYBOARD_DIR	m_eKeyDir;

	CObj*			m_pBoard;
	vector<CObj*>	m_vecBlock;

	CBitMap*		m_pBackBuffer;


	// Replay를 위해 추가한 부분 //
	chrono::system_clock::time_point startTime;
	chrono::system_clock::time_point currTime;

	chrono::duration<double> elapsedTime;

	bool m_bRecord;
	bool m_bReplay;
	bool m_bStepByStep;
	bool m_bSpaceDown;

	vector<RecordData> m_vecRecord;

	Point2D m_NewBlockPos;

	int m_nReplayCnt;

public:
	void Reset();

	void RecordStart(); 
	void SaveFile();

	void Replay(bool step);
	/////////////////////////////

public:
	void CreateBlock();

	/////////////////////////////
	void CreateBlock(const Point2D&);
	/////////////////////////////

	void MoveBlock();
	
	bool CheckGameOver();

	void SortByIndex();

	CBlock* FindBlock(int);

public:
	CGameFrameWork();
	~CGameFrameWork();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	void BuildObjects();
	void ReleaseObjects();
	
	void ProcessInput();
	void AnimateObjects();
	void Render();
	void FrameAdvance();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID,
		WPARAM wParam, LPARAM lParam);
};