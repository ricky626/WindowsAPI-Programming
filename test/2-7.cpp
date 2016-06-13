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
bool check = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH rBrush, bBrush, redBrush, blueBrush;
	static int xPos, yPos;

	switch (iMessage)
	{
	case WM_CREATE:
		xPos = 0;
		yPos = 0;
		check = false;
		break;
	case WM_KEYDOWN:

		if (wParam == VK_LEFT)
		{
			if (xPos >= 80)
				xPos -= 80;
			check = true;
		}
		else if (wParam == VK_RIGHT)
		{
			if (xPos <= 160)
				xPos += 80;
			check = true;
		}
		else if (wParam == VK_UP)
		{
			if (yPos >= 80)
				yPos -= 80;
			check = true;
		}
		else if (wParam == VK_DOWN)
		{
			if (yPos <= 160)
				yPos += 80;
			check = true;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_KEYUP:
		check = false;
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Rectangle(hdc, i * 80, j * 80, i * 80 + 80, j * 80 + 80);
			}
		}

		bBrush = CreateSolidBrush(RGB(0, 0, 255));
		rBrush = CreateSolidBrush(RGB(255, 0, 0));
		redBrush = (HBRUSH)SelectObject(hdc, rBrush);
		blueBrush = (HBRUSH)SelectObject(hdc, bBrush);

		if (check == true)
		{

			SelectObject(hdc, rBrush);
		}

		else if (check == false)
		{

			SelectObject(hdc, bBrush);
		}


		Rectangle(hdc, xPos, yPos, xPos + 80, yPos + 80);


		DeleteObject(redBrush);
		DeleteObject(blueBrush);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam,
		lParam));
}