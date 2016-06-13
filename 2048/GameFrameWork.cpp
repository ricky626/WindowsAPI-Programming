#include "stdafx.h"
#include "GameFrameWork.h"

#include "Board.h"
#include "BitMap.h"
#include "Block.h"

CGameFrameWork::CGameFrameWork()
{
	m_nWndClientWidth = WINDOW_WIDTH;
	m_nWndClientHeight = WINDOW_HEIGHT;
}

CGameFrameWork::~CGameFrameWork()
{
}

// 다음 함수는 응용 프로그램이 실행되면 호출된다는 것에 유의
bool CGameFrameWork::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hdc = GetDC(g_hWnd);
	m_hInstance = hInstance;

	m_nGoal = 10000;
	m_nScore = 0;

	m_bMove = false;

	BuildObjects();

	////////////////////////////////////
	m_bRecord = false;
	m_bReplay = false;

	m_bStepByStep = false;
	m_bSpaceDown = false;

	m_nReplayCnt = 0;
	////////////////////////////////////

	return(true);
}

void CGameFrameWork::OnDestroy()
{
	ReleaseObjects();

	////////////////////////////////////
	////////////////////////////////////
}

void CGameFrameWork::BuildObjects()
{
	m_pBoard = new CBoard();
	m_pBoard->Initialize();

	m_pBackBuffer = new CBitMap();
	m_pBackBuffer->LoadBmp(L"../Texture/backbuffer.bmp");

	CreateBlock();
}

void CGameFrameWork::CreateBlock()
{
	srand(unsigned(time(NULL)));

	vector<Tile*> tile = dynamic_cast<CBoard*>(m_pBoard)->GetTile();

	int idx = rand() % 16;

	while (tile[idx]->option != 0)
		idx = rand() % 16;

	tile[idx]->option = 1;

	CObj* block = new CBlock(tile[idx]->pos, 2, idx);
	block->Initialize();

	m_vecBlock.push_back(block);

	/////////////////////////////////////////
	m_NewBlockPos = tile[idx]->pos;
	/////////////////////////////////////////
}

void CGameFrameWork::CreateBlock(const Point2D& pos)
{
	vector<Tile*> tile = dynamic_cast<CBoard*>(m_pBoard)->GetTile();

	int idx = 0;

	for (auto p = tile.cbegin(); p != tile.cend(); ++p, ++idx)
	{
		if ((*p)->pos.x == pos.x && (*p)->pos.y == pos.y)
			break;
	}

	tile[idx]->option = 1;

	CObj* block = new CBlock(pos, 2, idx);
	block->Initialize();

	m_vecBlock.push_back(block);
}

void CGameFrameWork::ReleaseObjects()
{
	SAFE_DELETE(m_pBoard);

	for (auto p = m_vecBlock.begin(); p != m_vecBlock.end();)
	{
		SAFE_DELETE(*p);
		p = m_vecBlock.erase(p);
	}
}

void CGameFrameWork::ProcessInput()
{
	if (CheckGameOver())
	{
		TextOut(m_pBackBuffer->GetMemDC(), 50, 530, L" Game Over !! ", 14);
		m_bMove = false;
		return;
	}

	//////////////////////////////////////////////////
	if (m_bReplay)
	{
		if (m_vecRecord.size() <= m_nReplayCnt)
		{
			Rectangle(m_pBackBuffer->GetMemDC(), 0, 0, 800, 600);

			m_bReplay = false;

			TextOut(m_pBackBuffer->GetMemDC(), 50, 530, L" 리플레이가 종료되었습니다. ", 16);

			return;
		}

		if (!m_bStepByStep) {
			currTime = chrono::system_clock::now();

			elapsedTime = currTime - startTime;

			if (m_vecRecord[m_nReplayCnt].time <= elapsedTime.count())
			{
				m_bMove = true;

				m_eKeyDir = static_cast<KEYBOARD_DIR>(m_vecRecord[m_nReplayCnt].key);
			}
		}

		else
		{
			if (m_bSpaceDown)
			{
				m_bSpaceDown = false;
				m_bMove = true;
				m_eKeyDir = static_cast<KEYBOARD_DIR>(m_vecRecord[m_nReplayCnt].key);
			}
		}
	}
	//////////////////////////////////////////////////
}

void CGameFrameWork::AnimateObjects()
{
	if (m_bMove && m_vecBlock.size() < 16)
	{
		MoveBlock();

		int cnt = 0;

		for (auto p = m_vecBlock.cbegin(); p != m_vecBlock.cend(); ++p)
			if (dynamic_cast<CBlock*>(*p)->GetBlockState() == BLOCK_MOVE)
				++cnt;

		if (0 == cnt)
		{
			m_bMove = false;

			///////////////////////////////////////////////////////
			if (!m_bReplay)
			{
				CreateBlock();

				if (m_bRecord)
					m_vecRecord.push_back(RecordData(elapsedTime.count(), m_eKeyDir, m_NewBlockPos));
			}

			else
				CreateBlock(m_vecRecord[m_nReplayCnt++].newBlockPos);
			///////////////////////////////////////////////////////


			for (auto p = m_vecBlock.begin(); p != m_vecBlock.end();)
			{
				if (dynamic_cast<CBlock*>(*p)->GetBlockInfo().addCheck == 1)
				{
					SAFE_DELETE(*p);
					p = m_vecBlock.erase(p);

					continue;
				}

				else if (dynamic_cast<CBlock*>(*p)->GetBlockInfo().addCheck == 2)
				{
					int val = dynamic_cast<CBlock*>(*p)->GetBlockInfo().value;

					dynamic_cast<CBlock*>(*p)->SetBlockValue(val * 2);
					dynamic_cast<CBlock*>(*p)->SetBlockBmp(val * 2);

					dynamic_cast<CBlock*>(*p)->SetBlockAddCheck(0);

					m_nScore += val * 2;
				}

				++p;
			}
		}
	}

	for (auto p = m_vecBlock.begin(); p != m_vecBlock.end(); ++p)
	{
		int idx = dynamic_cast<CBlock*>(*p)->GetBlockInfo().index;

		Point2D pt = dynamic_cast<CBoard*>(m_pBoard)->GetTile()[idx]->pos;

		if (pt.x != dynamic_cast<CBlock*>(*p)->GetBlockInfo().pos.x ||
			pt.y != dynamic_cast<CBlock*>(*p)->GetBlockInfo().pos.y)
		{
			dynamic_cast<CBlock*>(*p)->Move();
		}

		else
			dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_STOP);
	}
}

void CGameFrameWork::Render()
{
	m_pBoard->Render(m_pBackBuffer->GetMemDC());

	for (auto p = m_vecBlock.cbegin(); p != m_vecBlock.cend(); ++p)
		(*p)->Render(m_pBackBuffer->GetMemDC());

	TCHAR szBuff[128] = L"";
	wsprintf(szBuff, L" 점수 : %d ", m_nScore);
	TextOut(m_pBackBuffer->GetMemDC(), 50, 20, szBuff, lstrlen(szBuff));

	wsprintf(szBuff, L" 목표 점수 : %d ", m_nGoal);
	TextOut(m_pBackBuffer->GetMemDC(), 150, 20, szBuff, lstrlen(szBuff));

	BitBlt(m_hdc, 0, 0, 800, 600, m_pBackBuffer->GetMemDC(), 0, 0, SRCCOPY);
}

void CGameFrameWork::FrameAdvance()
{
	ProcessInput();
	AnimateObjects();

	Render();
}

LRESULT CALLBACK CGameFrameWork::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID,
	WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;

	//case WM_KEYDOWN:
	case WM_KEYUP:
		if (m_bMove) break;
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return 0;
}

void CGameFrameWork::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_UP:
			m_eKeyDir = KEY_UP;
			m_bMove = true;

			////////////////////////////////////////
			currTime = chrono::system_clock::now();
			elapsedTime = currTime - startTime;
			////////////////////////////////////////
			break;

		case VK_DOWN:
			m_eKeyDir = KEY_DOWN;
			m_bMove = true;

			////////////////////////////////////////
			currTime = chrono::system_clock::now();
			elapsedTime = currTime - startTime;
			////////////////////////////////////////
			break;

		case VK_LEFT:
			m_eKeyDir = KEY_LEFT;
			m_bMove = true;

			////////////////////////////////////////
			currTime = chrono::system_clock::now();
			elapsedTime = currTime - startTime;
			////////////////////////////////////////
			break;

		case VK_RIGHT:
			m_eKeyDir = KEY_RIGHT;
			m_bMove = true;

			////////////////////////////////////////
			currTime = chrono::system_clock::now();
			elapsedTime = currTime - startTime;
			////////////////////////////////////////
			break;

		case VK_SPACE:
			if (m_bStepByStep)
				m_bSpaceDown = true;
			break;

		case VK_ESCAPE:
			///////////
			SaveFile();
			///////////
			::PostQuitMessage(0);
			break;
		}
		break;
	}
}

void CGameFrameWork::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

CBlock* CGameFrameWork::FindBlock(int idx)
{
	for (auto p = m_vecBlock.cbegin(); p != m_vecBlock.cend(); ++p)
		if (dynamic_cast<CBlock*>(*p)->GetBlockInfo().index == idx)
			return dynamic_cast<CBlock*>(*p);
	return nullptr;
}

void CGameFrameWork::SortByIndex()
{
	if (m_eKeyDir == KEY_UP)
	{
		sort(m_vecBlock.begin(), m_vecBlock.end(), [](CObj* a, CObj* b) {

			int x = dynamic_cast<CBlock*>(a)->GetBlockInfo().index;
			int y = dynamic_cast<CBlock*>(b)->GetBlockInfo().index;

			return x < y;
		});
	}

	else if (m_eKeyDir == KEY_DOWN)
	{
		sort(m_vecBlock.begin(), m_vecBlock.end(), [](CObj* a, CObj* b) {

			int x = dynamic_cast<CBlock*>(a)->GetBlockInfo().index;
			int y = dynamic_cast<CBlock*>(b)->GetBlockInfo().index;

			return x > y;
		});
	}

	else if (m_eKeyDir == KEY_LEFT)
	{
		sort(m_vecBlock.begin(), m_vecBlock.end(), [](CObj* a, CObj* b) {

			int x = dynamic_cast<CBlock*>(a)->GetBlockInfo().index % 4;
			int y = dynamic_cast<CBlock*>(b)->GetBlockInfo().index % 4;

			return x < y;
		});
	}

	else if (m_eKeyDir == KEY_RIGHT)
	{
		sort(m_vecBlock.begin(), m_vecBlock.end(), [](CObj* a, CObj* b) {

			int x = dynamic_cast<CBlock*>(a)->GetBlockInfo().index % 4;
			int y = dynamic_cast<CBlock*>(b)->GetBlockInfo().index % 4;

			return x > y;
		});
	}
}

void CGameFrameWork::MoveBlock()
{
	SortByIndex();

	vector<Tile*> tile = dynamic_cast<CBoard*>(m_pBoard)->GetTile();

	for (auto p = m_vecBlock.begin(); p != m_vecBlock.end(); ++p)
	{
		int idx = dynamic_cast<CBlock*>(*p)->GetBlockInfo().index;

		if (m_eKeyDir == KEY_LEFT)
		{
			if (idx % 4 != 0)
			{
				if (tile[idx - 1]->option == 0)
				{
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx - 1, 1);

					dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
					dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx - 1);
					dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(-10, 0));
				}

				else if (tile[idx - 1]->option == 1)
				{
					CObj* currBlock = FindBlock(idx);
					CObj* nextBlock = FindBlock(idx - 1);

					if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().value
						== dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().value)
					{
						if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().addCheck == 0
							&& dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().addCheck == 0)
						{
							dynamic_cast<CBlock*>(currBlock)->SetBlockAddCheck(1);
							dynamic_cast<CBlock*>(nextBlock)->SetBlockAddCheck(2);

							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx - 1, 1);

							dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
							dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx - 1);
							dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(-10, 0));
						}
					}
				}
			}
		}

		else if (m_eKeyDir == KEY_RIGHT)
		{
			if (idx % 4 != 3)
			{
				if (tile[idx + 1]->option == 0)
				{
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx + 1, 1);

					dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
					dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx + 1);
					dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(10, 0));
				}

				else if (tile[idx + 1]->option == 1)
				{
					CObj* currBlock = FindBlock(idx);
					CObj* nextBlock = FindBlock(idx + 1);

					if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().value
						== dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().value)
					{
						if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().addCheck == 0
							&& dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().addCheck == 0)
						{
							dynamic_cast<CBlock*>(currBlock)->SetBlockAddCheck(1);
							dynamic_cast<CBlock*>(nextBlock)->SetBlockAddCheck(2);

							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx + 1, 1);

							dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
							dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx + 1);
							dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(10, 0));
						}
					}
				}
			}
		}

		else if (m_eKeyDir == KEY_UP)
		{
			if (3 < idx)
			{
				if (tile[idx - 4]->option == 0)
				{
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx - 4, 1);

					dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
					dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx - 4);
					dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(0, -10));
				}

				else if (tile[idx - 4]->option == 1)
				{
					CObj* currBlock = FindBlock(idx);
					CObj* nextBlock = FindBlock(idx - 4);

					if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().value
						== dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().value)
					{
						if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().addCheck == 0
							&& dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().addCheck == 0)
						{
							dynamic_cast<CBlock*>(currBlock)->SetBlockAddCheck(1);
							dynamic_cast<CBlock*>(nextBlock)->SetBlockAddCheck(2);

							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx - 4, 1);

							dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
							dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx - 4);
							dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(0, -10));
						}
					}
				}
			}
		}

		else if (m_eKeyDir == KEY_DOWN)
		{
			if (idx < 12)
			{
				if (tile[idx + 4]->option == 0)
				{
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
					dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx + 4, 1);

					dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
					dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx + 4);
					dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(0, 10));
				}

				else if (tile[idx + 4]->option == 1)
				{
					CObj* currBlock = FindBlock(idx);
					CObj* nextBlock = FindBlock(idx + 4);

					if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().value
						== dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().value)
					{
						if (dynamic_cast<CBlock*>(currBlock)->GetBlockInfo().addCheck == 0
							&& dynamic_cast<CBlock*>(nextBlock)->GetBlockInfo().addCheck == 0)
						{
							dynamic_cast<CBlock*>(currBlock)->SetBlockAddCheck(1);
							dynamic_cast<CBlock*>(nextBlock)->SetBlockAddCheck(2);

							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx, 0);
							dynamic_cast<CBoard*>(m_pBoard)->SetTileOption(idx + 4, 1);

							dynamic_cast<CBlock*>(*p)->SetBlockState(BLOCK_MOVE);
							dynamic_cast<CBlock*>(*p)->SetBlockIndex(idx + 4);
							dynamic_cast<CBlock*>(*p)->SetMovePower(Point2D(0, 10));
						}
					}
				}
			}
		}
	}
}

bool CGameFrameWork::CheckGameOver()
{
	int tmp = 0;

	vector<Tile*> tile = dynamic_cast<CBoard*>(m_pBoard)->GetTile();

	for (auto p = tile.cbegin(); p != tile.cend(); ++p)
		if ((*p)->option == 1) ++tmp;

	if (tmp == tile.size())
		return true;
	return false;
}

////////////////////////////////////////////////////////////////////////////////
ostream& operator<<(ostream& os, const Point2D& pt)
{
	os << pt.x << " " << pt.y;
	return os;
}

ofstream& operator<<(ofstream& os, const vector<RecordData>& v)
{
	for (auto data : v)
		os << data.key << " " << data.newBlockPos << " " << data.time << endl;

	return os;
}

ifstream& operator>>(ifstream& is, vector<RecordData>& v)
{
	int key;
	Point2D pos;
	double time;

	while (is >> key >> pos.x >> pos.y >> time)
		v.push_back(RecordData(time, key, pos));

	return is;
}

void CGameFrameWork::Reset()
{
	dynamic_cast<CBoard*>(m_pBoard)->Reset();

	for (auto p = m_vecBlock.begin(); p != m_vecBlock.end();)
	{
		SAFE_DELETE(*p);
		p = m_vecBlock.erase(p);
	}

	m_nScore = 0;
}

void CGameFrameWork::RecordStart()
{
	Rectangle(m_pBackBuffer->GetMemDC(), 0, 0, 800, 600);

	m_bReplay = false;

	if (m_bRecord)
	{
		TextOut(m_pBackBuffer->GetMemDC(), 50, 560, L" 시도할 수 없습니다. ", 13);
		return;
	}
	TextOut(m_pBackBuffer->GetMemDC(), 50, 530, L" 녹화 진행중입니다. ", 12);

	Reset();

	for (auto p = m_vecRecord.begin(); p != m_vecRecord.end();)
		p = m_vecRecord.erase(p);
	m_vecRecord.clear();

	CreateBlock();

	m_vecRecord.push_back(RecordData(0.f, -1, m_NewBlockPos));

	m_bRecord = true;

	startTime = chrono::system_clock::now();
}

void CGameFrameWork::SaveFile()
{
	ofstream out("test.txt", ios::trunc);
	out << m_vecRecord;
	out.close();

	m_bRecord = false;

	TextOut(m_pBackBuffer->GetMemDC(), 50, 530, L" 녹화가 종료되었습니다. ", 14);
}

void CGameFrameWork::Replay(bool step)
{
	TextOut(m_pBackBuffer->GetMemDC(), 50, 530, L" 리플레이가 진행중입니다. ", 15);

	m_nReplayCnt = 0;
	m_bReplay = true;
	m_bRecord = false;
	m_bStepByStep = step;

	if (step)
		TextOut(m_pBackBuffer->GetMemDC(), 50, 550, L" SPACE BAR를 누르세요. ", 18);

	Reset();

	for (auto p = m_vecRecord.begin(); p != m_vecRecord.end();)
		p = m_vecRecord.erase(p);
	m_vecRecord.clear();

	ifstream in("test.txt");
	in >> m_vecRecord;
	in.close();

	CreateBlock(m_vecRecord[m_nReplayCnt++].newBlockPos);

	startTime = chrono::system_clock::now();
}
////////////////////////////////////////////////////////////////////////////////