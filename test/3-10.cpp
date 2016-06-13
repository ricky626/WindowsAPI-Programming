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
	if (LengthPts(x+25, y+25, mx, my) < BSIZE)
		return TRUE;
	else return FALSE;
}

typedef struct Cir
{
	int x;
	int y;
	//int check; //0 : 이동시작 1 : 3초 후 정지/이동 시작 2 : 바로 정지
}Cir;

typedef struct Bus
{
	int x;
	int y;
	int speed;
	int check; //0 : 이동시작 1 : 3초 후 정지/이동 시작 2 : 바로 정지
}Bus;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//char temp[] = TEXT("Hello world!");
	//wsprintf(buf, %d %d, a, b);
	HBRUSH rBrush, yBrush, gBrush;
	HBRUSH bBrush;

	static Cir cir[3];
	static Bus bus[3];
	float mx, my;
	static bool stop;


	switch (iMessage)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));

		stop = false;

		for (int i = 0; i < 3; i++)
		{
			cir[i].x = cir[i].y = 25;
			bus[i].check = 0; //0 : 이동시작 1 : 3초 후 정지/이동 시작 2 : 바로 정지
		}

		bus[0].x = rand() % 700;
		bus[0].y = 150;
		bus[0].speed = rand() % 15 + 5;

		bus[1].x = rand() % 700;
		bus[1].y = 300;
		bus[1].speed = rand() % 15 + 5;

		bus[2].x = rand() % 700;
		bus[2].y = 450;
		bus[2].speed = rand() % 15 + 5;

		SetTimer(hwnd, 1, 50, NULL);
		break;

	case WM_TIMER:
		if (wParam == 1)
		{
			if (stop == false)
			{
				for (int i = 0; i < 3; i++)
				{
					if (bus[i].x > 750)
						bus[i].x = -80;
					else
						bus[i].x += bus[i].speed;
				}
			}
		}

		if (wParam == 2)
		{
			if (stop == false)
			{
				stop = true;
				KillTimer(hwnd, 2);
			}
			else if (stop == true)
			{
				stop = false;
				KillTimer(hwnd, 2);
			}	
		}
		
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (InCircle(cir[0].x-25, cir[0].y-25, mx, my))
		{
			if (stop == false)
			{
				stop = true;
			}
		}

		else if (InCircle(cir[1].x-25+50, cir[1].y-25, mx, my))
		{
			SetTimer(hwnd, 2, 3000, NULL);
		}

		else if (InCircle(cir[2].x-25+100, cir[2].y-25, mx, my))
		{
			if (stop == true)
			{
				stop = false;
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		rBrush = CreateSolidBrush(RGB(255, 0, 0));

		yBrush = CreateSolidBrush(RGB(255, 255, 0));

		gBrush = CreateSolidBrush(RGB(0, 255, 0));

		bBrush = CreateSolidBrush(RGB(0, 0, 255));

		SelectObject(hdc, rBrush);
		Ellipse(hdc, cir[0].x-25, cir[0].y-25, cir[0].x+25, cir[0].y+25);

		SelectObject(hdc, yBrush);
		Ellipse(hdc, cir[1].x-25+50, cir[1].y-25, cir[1].x+25+50, cir[1].y+25);

		SelectObject(hdc, gBrush);
		Ellipse(hdc, cir[2].x-25+100, cir[2].y-25, cir[2].x+25+100, cir[2].y+25);

		SelectObject(hdc, bBrush);

		for (int i = 0; i < 3; i++)
			Rectangle(hdc, bus[i].x, bus[i].y, bus[i].x + 120, bus[i].y + 60);

		DeleteObject(rBrush);
		DeleteObject(yBrush);
		DeleteObject(gBrush);
		DeleteObject(bBrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:

		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}