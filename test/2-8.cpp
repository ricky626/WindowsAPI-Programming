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
	static int xPos, yPos;
	static SIZE size;
	static char str[255];
	static int sel;
	static int data[5];
	static int i;
	static int index;

	switch (iMessage)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5, 15);
		ShowCaret(hwnd); // 빈 화면에 캐럿 표시

		size.cx = 0;
		sel = 0;
		for (int k = 0; k < 5; k++)
		{
			data[k] = 0;
		}
		i = 0;
		index = 0;
		xPos = 0;
		yPos = 530;
		
		break;
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
		}
		else if (wParam == VK_SPACE)
		{
			if (i < 5)
			{
				data[i] = atoi(str);
				i++;
			}
		}
		else if (wParam == VK_BACK)
		{		
			if (xPos == 0)
				break;
			xPos--;
		}
		else
		{
			str[xPos++] = wParam;
			
		}
		str[xPos] = '\0';
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:

		
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_KEYUP:

		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetTextExtentPoint(hdc, str, strlen(str), &size);

		if (data[0] == 3)
			Rectangle(hdc, data[1], data[2], data[3], data[4]);


		Rectangle(hdc, 0, 510, 300, 560);//글상자

		

		TextOut(hdc, 0, yPos, str, strlen(str));
		
		SetCaretPos(size.cx, yPos);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam,
		lParam));
}