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

typedef struct
{
	int x;
	int y;
	int color[3];
	bool check;
	int minus;
}Cir;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//char temp[] = TEXT("Hello world!");
	//wsprintf(buf, %d %d, a, b);
	static Cir cir[10];
	HBRUSH hBrush;

	float mx, my;
	static bool Selection;


	switch (iMessage)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cir[i].color[j] = rand() % 256;
			}
		}
		for (int i = 0; i < 10; i++)
		{
			cir[i].x = ((rand() % 14) + 1) * 50;
			cir[i].y = ((rand() % 10) + 1) * 50;
		}
		for (int i = 0; i < 10; i++)
		{
			cir[i].check = true;
			cir[i].minus = 0;
		}

		Selection = FALSE;
		SetTimer(hwnd, 1, 5000, NULL);

		break;

	case WM_TIMER:
		if (wParam == 1)
		{
			for (int i = 0; i < 10; i++)
			{
				if (cir[i].check != false)
				{
					cir[i].x = ((rand() % 14) + 1) * 50;
					cir[i].y = ((rand() % 10) + 1) * 50;
				}
			}
		}
		else if (wParam == 2)
		{
			for (int i = 0; i < 10; i++)
			{
				if (cir[i].check == false)
				{
					if (cir[i].minus == 25)
					{
						cir[i].minus = 0;
						cir[i].x = ((rand() % 14) + 1) * 50;
						cir[i].y = ((rand() % 10) + 1) * 50;
						cir[i].color[0] = rand() % 256;
						cir[i].color[1] = rand() % 256;
						cir[i].color[2] = rand() % 256;
						cir[i].check = true;
						KillTimer(hwnd, 2);
					}
					else
					{
						cir[i].minus += 5;
					}

				}
			}
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		for (int i = 0; i < 10; i++)
		{
			if (InCircle(cir[i].x, cir[i].y, mx, my))
			{
				Selection = TRUE;
				cir[i].check = false;
				SetTimer(hwnd, 2, 30, NULL);
				break;
			}
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONUP:
		if (Selection == TRUE)
			Selection = FALSE;

		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < 10; i++)
		{
			hBrush = CreateSolidBrush(RGB(cir[i].color[0], cir[i].color[1], cir[i].color[2]));
			SelectObject(hdc, hBrush);

			Ellipse(hdc, cir[i].x - 25 + cir[i].minus, cir[i].y - 25 + cir[i].minus, cir[i].x + 25 - cir[i].minus, cir[i].y + 25 - cir[i].minus);
		}
		//DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}