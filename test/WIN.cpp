#include <windows.h> // 윈도우 헤더 파일
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//char temp[] = TEXT("Hello world!");
	//wsprintf(buf, %d %d, a, b);
	static int xPos, yPos;
	HBRUSH rBrush, bBrush, redBrush, blueBrush;
	static int Timer1Count = 0;
	static int move;
	static int headX, headY, TailX, TailY;

	switch (iMessage)
	{
	case WM_CREATE:
		xPos = 0;
		yPos = 0;
		move = 0;
		headX = 0;
		headY = 0;
		TailX = 0;
		TailY = 0;

		SetTimer(hwnd, 1, 30, NULL);
		break;

	case WM_TIMER:
		if (move == 0) //Right
		{
			if (xPos >= 675)
			{
				headX -= 50;
				TailX += 50;
				move = 1;
			}
			else
				xPos += 10;
		}
		else if (move == 1) // Left
		{
			if (xPos <= 0)
			{
				headX += 50;
				TailX -= 50;
				move = 0;
			}

			else
				xPos -= 10;
		}
		else if (move == 2) // Up
		{
			if (yPos < 0)
			{
				headY += 50;
				headX -= 100;
				TailY -= 50;
				move = 3;
			}
			else
				yPos -= 10;
		}
		else if (move == 3) // Down
		{
			if (yPos >= 500)
			{
				headY -= 50;
				headX += 100;
				TailY += 50;
				move = 2;
			}
			else
				yPos += 10;
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_CHAR:
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:

		if (wParam == VK_RIGHT && xPos <= 675)
		{
			if (move != 0)
			{
				headX += 50;
				TailX -= 50;
			}
			move = 0;
		}
		else if (wParam == VK_LEFT && xPos > 0)
		{
			if (move != 1)
			{
				headX -= 50;
				TailX += 50;
			}
			move = 1;
		}
		else if (wParam == VK_UP && yPos > 0)
		{
			if (move != 2)
			{
				headY += 50;
				headX -= 100;
				TailY -= 50;
			}
			move = 2;
		}
		else if (wParam == VK_DOWN && yPos <= 500)
		{
			if (move != 3)
			{
				headY -= 50;
				headX += 100;
				TailY += 50;
			}
			move = 3;
		}

		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		bBrush = CreateSolidBrush(RGB(0, 0, 255));
		rBrush = CreateSolidBrush(RGB(255, 0, 0));
		redBrush = (HBRUSH)SelectObject(hdc, rBrush);
		blueBrush = (HBRUSH)SelectObject(hdc, bBrush);


		SelectObject(hdc, rBrush);
		Ellipse(hdc, xPos + 50 + headX, yPos + headY, xPos + 100 + headX, yPos + 50 + headY);//head

		SelectObject(hdc, bBrush);
		Ellipse(hdc, xPos + TailX, yPos + TailY, xPos + 50 + TailX, yPos + 50 + TailY);//tail



		DeleteObject(redBrush);
		DeleteObject(blueBrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam,
		lParam));
}