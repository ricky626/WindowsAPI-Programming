#include <windows.h> // 윈도우 헤더 파일
#include <time.h>
#include <math.h>
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window program 1-1";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

typedef struct Eat
{
	int x;
	int y;
	int sel;
	bool check;
}Eat;

#define BSIZE 25 // 반지름

float LengthPts(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}

BOOL InCircle(float x, float y, float mx, float my)
{
	if (LengthPts(x, y, mx, my) < BSIZE)
		return TRUE;
	else return FALSE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static Eat eat[10];

	//char temp[] = TEXT("Hello world!");
	//wsprintf(buf, %d %d, a, b);

	HBRUSH rBrush, bBrush, redBrush, blueBrush, gBrush, greenBrush;
	HBRUSH blkBrush, blackBrush, eBrush, eatBrush;

	static int Timer1Count = 0;
	static int move;
	static int HeadX, HeadY, TailX, TailY;
	static int addX, addY;
	static bool addgo;
	static bool Jump_check;
	static int speed;
	bool check;
	int count = 0;
	static int tail[10];
	float mx, my;

	switch (iMessage)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));
		move = 0;
		check = false;
		HeadX = 0;
		HeadY = 0;
		TailX = 0;
		TailY = 0;
		addX = 0;
		addY = 0;
		

		speed = 10;
		Jump_check = false;
		addgo = false;

		for (int i = 0; i < 10; i++)
		{
			eat[i].x = ((rand() % 14) + 1) * 50;
			eat[i].y = ((rand() % 10) + 1) * 50;
			eat[i].sel = rand() % 2;
			eat[i].check = true;
			tail[i] = 0;
		}

		while (check == false)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (eat[i].x == eat[j].x && eat[i].y == eat[j].y)
					{
						eat[i].x = ((rand() % 14) + 1) * 50;
						eat[i].y = ((rand() % 10) + 1) * 50;	
					}
				}
				count++;
			}
			if (count == 10)
				check = true;
		}

		SetTimer(hwnd, 1, 35, NULL);
		SetTimer(hwnd, 2, 5000, NULL); //추가된 애벌레 생성시간
		
		break;

	case WM_TIMER:
		if (wParam == 1)
		{
			if (move == 0) //Right
			{
				if (HeadX >= 675)
				{
					HeadX -= 50;
					TailX += 50;
					move = 1;
				}
				else
				{
					HeadX += speed;
					TailX += speed;
				}
			}
			else if (move == 1) // Left
			{
				if (HeadX < -40)
				{
					HeadX += 50;
					TailX -= 50;
					move = 0;
				}
				else
				{
					HeadX -= speed;
					TailX -= speed;
				}
			}
			else if (move == 2) // Up
			{
				if (HeadY < 2)
				{
					HeadY += 50;
					TailY -= 50;
					move = 3;
				}
				else
				{
					HeadY -= speed;
					TailY -= speed;
				}

			}
			else if (move == 3) // Down
			{
				if (HeadY >= 510)
				{
					HeadY -= 50;
					TailY += 50;
					move = 2;
				}
				else
				{
					HeadY += speed;
					TailY += speed;
				}
			}

			for (int i = 0; i < 10; i++)
			{
				if (InCircle(HeadX, HeadY, eat[i].x, eat[i].y) && eat[i].check == true)
				{
					if (eat[i].sel == 0)
					{
						tail[i]++;
					}

					else if (eat[i].sel == 1)
					{
						if (tail[i] > 1)
							tail[i]--;
					}
					eat[i].check = false;
				}
			}

			if (addgo == true)
			{
				if (HeadX > addX)
					addX += 2;
				if (HeadX < addX)
					addX -= 2;
				if (HeadY > addY)
					addY += 2;
				if (HeadY < addY)
					addY -= 2;
			}
		}
		else if (wParam == 2)
		{
			addgo = true;
			KillTimer(hwnd, 2);
		}
		else if (wParam == 3)
		{
			if (Jump_check == true)
			{
				if (move == 0 || move == 1)
				{
					HeadY += 50;
					TailY += 50;
					addY += 50;
				}
				else if (move == 2 || move == 3)
				{
					HeadX -= 50;
					TailX -= 50;
					addX -= 50;
				}

				Jump_check = false;
				KillTimer(hwnd, 3);
			}
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_CHAR:
		if (wParam == '+')
			speed += 1;
		else if (wParam == '-')
			speed -= 1;
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (mx < HeadX && move != 1) // Left
		{
			if (move == 0)
			{
				HeadX -= 50;
				TailX += 50;
			}
			else if (move == 2)
			{
				TailX += 50;
				TailY -= 50;
			}
			else if (move == 3)
			{
				TailX += 50;
				TailY += 50;
			}
			move = 1;
		}

		if (mx >= HeadX && move != 0) // Right
		{
			if (move == 1)
			{
				HeadX += 50;
				TailX -= 50;
			}
			else if (move == 2)
			{
				TailX -= 50;
				TailY -= 50;
			}
			else if (move == 3)
			{
				TailX -= 50;
				TailY += 50;
			}
			move = 0;
		}
			
		if (my < HeadY && move != 2) // Up
		{
			if (move == 3)
			{
				HeadY -= 50;
				TailY += 50;
			}
			else if (move == 0)
			{
				TailX += 50;
				TailY += 50;
			}
			else if (move == 1)
			{
				TailX -= 50;
				TailY += 50;
			}
			move = 2;
		}
			
		if (my >= HeadY && move != 3) // Down
		{
			if (move == 2)
			{
				HeadY += 50;
				TailY -= 50;
			}
			else if (move == 0)
			{
				TailX += 50;
				TailY -= 50;
			}
			else if (move == 1)
			{
				TailX -= 50;
				TailY -= 50;
			}
			move = 3;
		}
			
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT) // 0 Right
		{
			if (move == 1)
			{
				HeadX += 50;
				TailX -= 50;
			}
			else if (move == 2)
			{
				TailX -= 50;
				TailY -= 50;
			}
			else if (move == 3)
			{
				TailX -= 50;
				TailY += 50;
			}
			move = 0;
		}
		else if (wParam == VK_LEFT) // 1 Left
		{
			if (move == 0)
			{
				HeadX -= 50;
				TailX += 50;
			}
			else if (move == 2)
			{
				TailX += 50;
				TailY -= 50;
			}
			else if (move == 3)
			{
				TailX += 50;
				TailY += 50;
			}
			move = 1;
		}
		else if (wParam == VK_UP) // 2 Up
		{
			if (move == 3)
			{
				HeadY -= 50;
				TailY += 50;
			}
			else if (move == 0)
			{
				TailX += 50;
				TailY += 50;
			}
			else if (move == 1)
			{
				TailX -= 50;
				TailY += 50;
			}
			move = 2;
		}
		else if (wParam == VK_DOWN) // 3 Down
		{
			if (move == 2)
			{
				HeadY += 50;
				TailY -= 50;
			}
			else if (move == 0)
			{
				TailX += 50;
				TailY -= 50;
			}
			else if (move == 1)
			{
				TailX -= 50;
				TailY -= 50;
			}
			move = 3;
		}

		else if (wParam == VK_SPACE && HeadY > 50)
		{
			if (move == 0 || move == 1) //Right Left
			{
				HeadY -= 50;
				TailY -= 50;
				addY -= 50;
			}
			else if (move == 2 || move == 3) // Up Down
			{
				HeadX += 50;
				TailX += 50;
				addX += 50;
			}

			SetTimer(hwnd, 3, 500, NULL); //점프타임
			Jump_check = true;
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		bBrush = CreateSolidBrush(RGB(0, 0, 255));
		rBrush = CreateSolidBrush(RGB(255, 0, 0));
		gBrush = CreateSolidBrush(RGB(0, 255, 0));
		blkBrush = CreateSolidBrush(RGB(0, 0, 0));
		eBrush = CreateSolidBrush(RGB(255, 255, 0));

		redBrush = (HBRUSH)SelectObject(hdc, rBrush);
		blueBrush = (HBRUSH)SelectObject(hdc, bBrush);
		greenBrush = (HBRUSH)SelectObject(hdc, gBrush);
		blackBrush = (HBRUSH)SelectObject(hdc, blkBrush);
		eatBrush = (HBRUSH)SelectObject(hdc, eBrush);

		for (int i = 0; i < 10; i++)
		{
			if (eat[i].check == true)
			{
				if (eat[i].sel == 0)
				{
					SelectObject(hdc, blkBrush);
					Ellipse(hdc, eat[i].x-25, eat[i].y-25, eat[i].x + 25, eat[i].y + 25);
				}
				else if (eat[i].sel == 1)
				{
					SelectObject(hdc, eBrush);
					Ellipse(hdc, eat[i].x-25, eat[i].y-25, eat[i].x + 25, eat[i].y + 25);
				}
			}
		}

		SelectObject(hdc, rBrush);
		Ellipse(hdc, HeadX, HeadY, HeadX+50, HeadY+50);//head

		SelectObject(hdc, bBrush);
		Ellipse(hdc, TailX-50, TailY, TailX , (TailY+50));//tail

		for (int i = 0; i < 10; i++)
		{
			if (eat[i].check == false && eat[i].sel == 1)
			{
				Ellipse(hdc, TailX-50 - (50*tail[i]), TailY, TailX, (TailY+ 50) - (50*tail[i]));//tail
			}
		}
		
		if (addgo == true) //추격 애벌레
		{
			SelectObject(hdc, gBrush);
			Ellipse(hdc, addX-25, addY-25, (addX-25 + 50), (addY-25 + 50));
		}
		
		DeleteObject(redBrush);
		DeleteObject(blueBrush);
		DeleteObject(greenBrush);
		DeleteObject(blackBrush);
		DeleteObject(eatBrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}