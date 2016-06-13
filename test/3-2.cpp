#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PIE 3.141592

typedef struct
{
	int center_X;
	int center_Y;
	int R;
	int R_outline;
	int color[3];
}Figure;

typedef struct
{
	int center_X;
	int center_Y;
	int R;
	int R_outline;
	int color[3];
	int Dir;
}Revolution;
enum { clockwise, Anti_clockwise };

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(
		lpszClass,
		"Window Program 1-3",
		WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_POPUPWINDOW,
		100,
		100,
		800,
		600,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static RECT rectview;
	static char str[50];
	static Revolution rev[4];
	static Figure Poly[4];
	static int line[2];
	static int angle[4];
	static double Theta[4];
	srand((unsigned)time(NULL));
	int i;
	static int Speed;

	switch (iMessage){
	case WM_CREATE:
		GetClientRect(hWnd, &rectview);
		Poly[0].center_X = rectview.right / 4;
		Poly[0].center_Y = rectview.bottom / 4;
		Poly[1].center_X = rectview.right * 3 / 4;
		Poly[1].center_Y = rectview.bottom / 4;
		Poly[2].center_X = rectview.right / 4;
		Poly[2].center_Y = rectview.bottom * 3 / 4;
		Poly[3].center_X = rectview.right * 3 / 4;
		Poly[3].center_Y = rectview.right * 3 / 4;
		for (i = 0; i < 4; ++i)
		{
			Poly[i].R = 50; //반지름
			Poly[i].R_outline = 150;
			rev[i].R = rand() % 50 + 5;
			rev[i].center_X = Poly[i].center_X - 1;
			rev[i].center_Y = Poly[i].center_Y - 1;
			rev[i].Dir = rand() % 2;
			if (rev[i].Dir == clockwise)
				angle[i] = 0;
			else if (rev[i].Dir == Anti_clockwise)
				angle[i] = 360;
			for (int j = 0; j < 3; j++)
			{
				rev[i].color[j] = rand() % 256;//R
			}
		}
		Speed = 30;
		line[0] = rectview.right / 2;
		line[1] = rectview.bottom / 2;
		SetTimer(hWnd, 1, Speed, NULL);
		break;
	case WM_CHAR:
		if (wParam == 'q' || wParam == 'Q')
			PostQuitMessage(0);
		if (wParam == '1') //타이머 수동 시작
			SetTimer(hWnd, 1, Speed, NULL);
		if (wParam == '2') //타이머 수동 종료
			KillTimer(hWnd, 1);
		if (wParam == '+')
			if (Speed > 0)
				Speed -= 2;
		if (wParam == '-')
			Speed += 2;

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_TIMER:
		for (i = 0; i < 4; ++i)
		{
			if (rev[i].Dir == clockwise)
			{
				angle[i]++;
				if (angle[i] == 360)
					angle[i] = 0;
			}
			if (rev[i].Dir == Anti_clockwise)
			{
				angle[i]--;
				if (angle[i] == 0)
					angle[i] = 360;
			}
			(double)Theta[i] = PIE * angle[i] / 180.0;
		}
		rev[0].R++; //크기 증가
		if (rev[0].R == 100)
			rev[0].R = 10;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		for (i = 0; i < 4; ++i)
		{
			SelectObject(hdc, oldBrush);
			Ellipse(hdc, Poly[i].center_X - Poly[i].R_outline, Poly[i].center_Y - Poly[i].R_outline,
				Poly[i].center_X + Poly[i].R_outline, Poly[i].center_Y + Poly[i].R_outline);
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, hBrush);
			Ellipse(hdc, Poly[i].center_X - Poly[i].R, Poly[i].center_Y - Poly[i].R,
				Poly[i].center_X + Poly[i].R, Poly[i].center_Y + Poly[i].R);
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(rev[i].color[0], rev[i].color[1], rev[i].color[2]));
			SelectObject(hdc, hBrush);

			Ellipse(hdc, rev[i].center_X - rev[i].R + Poly[i].R_outline * cos(Theta[i]), rev[i].center_Y - rev[i].R + Poly[i].R_outline * sin(Theta[i]),
				rev[i].center_X + rev[i].R + Poly[i].R_outline * cos(Theta[i]) + rev[i].R, rev[i].center_Y + rev[i].R + Poly[i].R_outline * sin(Theta[i]) + rev[i].R);
			DeleteObject(hBrush);
		}

		hPen = CreatePen(PS_DASH, 5, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		MoveToEx(hdc, 0, rectview.bottom / 2, NULL);
		LineTo(hdc, rectview.right, rectview.bottom / 2);
		MoveToEx(hdc, rectview.right / 2, 0, NULL);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		//wsprintf(str, "(angle : %d Speed : %d)", angle[0], Speed);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}