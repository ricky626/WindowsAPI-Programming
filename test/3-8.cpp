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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
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

typedef struct Block
{
	float x;
	float y;
	int check;// 0 : 노말	1 : 한번깨진거	2 : 깨진거
}Block;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//char temp[] = TEXT("Hello world!");
	//wsprintf(buf, %d %d, a, b);
	HBRUSH hBrush, bBrush;
	static Block block[2][10];
	
	static float HeroX, HeroY;
	static float ballX, ballY;
	static float ball_moveX, ball_moveY;
	static int count;
	static float mx, my;
	static bool MouseCheck;

	switch (iMessage)
	{
	case WM_CREATE:
		HeroX = 310;
		HeroY = 525;
		count = 40;

		ballX = HeroX + 50;
		ballY = 495;
		
		ball_moveX = -5;
		ball_moveY = -5;

		MouseCheck = false;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				block[i][j].x = 78.35 * j;
				block[i][j].y = 25.0 * i;
				block[i][j].check = 0;
			}
		}

		SetTimer(hwnd, 1, 50, NULL);
		break;

	case WM_TIMER:
		if (wParam == 1)
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (InCircle(ballX, ballY, block[i][j].x, block[i][j].y) && block[i][j].check != 2)
					{
						if (block[i][j].check == 0)
						{
							block[i][j].check = 1;
							ball_moveY *= -1;
							count--;
							break;
						}
						else if (block[i][j].check == 1)
						{
							block[i][j].check = 2;
							ball_moveY *= -1;
							count--;
							break;
						}
					}
				}
			}
			if (count == 0)
				PostQuitMessage(0);

			if (ballX < 0 || ballX > 740)
				ball_moveX *= -1;
			if (ballY < 0 || ballY > 575)
				ball_moveY *= -1;
			if (ballY > HeroY - 25.0)
				ball_moveY *= -1;

			ballX += ball_moveX;
			ballY += ball_moveY;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_CHAR:
		if (wParam == 's')
		{
			if (ball_moveX <= 0)
				ball_moveX -= 1;
			else if (ball_moveX > 0)
				ball_moveX += 1;
			if (ball_moveY <= 0)
				ball_moveY -= 1;
			else if (ball_moveY > 0)
				ball_moveY += 1;			
		}
		else if (wParam == 'd')
		{
			if (ball_moveX <= 0)
				ball_moveX += 1;

			else if (ball_moveX > 0)
				ball_moveX -= 1;

			if (ball_moveY <= 0)
				ball_moveY += 1;

			else if (ball_moveY > 0)
				ball_moveY -= 1;
		}
		break;
	case WM_LBUTTONDOWN:
		MouseCheck = true;
		break;
	case WM_LBUTTONUP:
		MouseCheck = false;
		break;
	case WM_MOUSEMOVE:
		if (MouseCheck == true)
		{
			if (HeroX < 100 || HeroX > 0)
			{
				mx = LOWORD(lParam) - 100;
				HeroX = mx;
			}
		}			
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		bBrush = CreateSolidBrush(RGB(0, 255, 0));

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (block[i][j].check == 0)
				{
					SelectObject(hdc, hBrush);
					Rectangle(hdc, block[i][j].x, block[i][j].y, block[i][j].x + 78.35, block[i][j].y + 25.0);
					//Rectangle(hdc, j * blockX, i * blockY, j * blockX + blockX, i * blockY + blockY);
				}
				else if (block[i][j].check == 1)
				{
					SelectObject(hdc, bBrush);
					Rectangle(hdc, block[i][j].x, block[i][j].y, block[i][j].x + 78.35, block[i][j].y + 25.0);
				}
			}
		}
		
		DeleteObject(hBrush);
		DeleteObject(bBrush);


		hBrush = CreateSolidBrush(RGB(255, 255, 255));

		SelectObject(hdc, hBrush);

		Ellipse(hdc, ballX, ballY, ballX + 30, ballY + 30);

		Rectangle(hdc, HeroX, HeroY, HeroX + 200, 25.0 + HeroY);

		DeleteObject(hBrush);


		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}