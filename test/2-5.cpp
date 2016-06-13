#include <windows.h> 
#include <stdio.h> 
BOOL InvalidateRect(HWND hwnd, const RECT lpRECT, BOOL bERASE);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	HINSTANCE g_hInst;
	g_hInst = hInstance;
	LPCTSTR lpszClass = "windowclass";
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
	hWnd = CreateWindow(
		lpszClass,
		"my first window program",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		800,
		600,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

bool b_color = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[10][100];
	static SIZE size;
	static int x_count, yPos;

	switch (iMsg)
	{
	case WM_CREATE:
		CreateCaret(hwnd, NULL, 5, 15);
		ShowCaret(hwnd);
		yPos = 0;
		x_count = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//GetTextExtentPoint(hdc, str[yPos], strlen(str[yPos]), &size);
		
	
		//if (b_color == true)
		//	SetTextColor(hdc, RGB(255, 0, 0));
		//else
		//	SetTextColor(hdc, RGB(0, 0, 0));

		/*for (int i = 0; i <= yPos; i++)
		{
			if (b_color && *str[i] >= '1' && *str[i] <= '9')
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				TextOut(hdc, 0, i * 20, str[i], strlen(str[i]));
			}
			else
			{
				SetTextColor(hdc, RGB(0, 0, 0));
				TextOut(hdc, 0, i * 20, str[i], strlen(str[i]));
			}
		}*/
		for (int i = 0; i < yPos + 1; i++)
		{
			for (int j = 0; j < 10; j++){

				GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);
				SetCaretPos(size.cx, i * 20);

				if (str[0][0] >= 48 && str[0][0] <= 57){
					SetTextColor(hdc, RGB(255, 0, 0));
					TextOut(hdc, 0, 0, &str[0][0], strlen(&str[0][0]));
				}
				if (str[i][j] >= 48 && str[i][j] <= 57) {
					if (i == 0 && j == 0)
						continue;
					SetTextColor(hdc, RGB(255, 0, 0));
					size.cx = 8;
					TextOut(hdc, size.cx - 8, i * 20, &str[i][j], strlen(&str[i][j]));
					break;
				}
				else if (str[i][j] < 48 || str[i][j] > 57){
					SetTextColor(hdc, RGB(0, 0, 0));
					TextOut(hdc, 0, i * 20, str[i], strlen(str[i]));
				}
			}
		}
	
		SetCaretPos(size.cx, yPos * 20);
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		if (wParam == VK_BACK)
		{
			if (x_count == 0 && yPos == 0) break;
			x_count--;
			if (x_count < 0)
			{
				yPos--;
				x_count = strlen(str[yPos]);
			}
		}
		else if (wParam == VK_RETURN)
		{
			if (yPos < 9)
			{
				x_count = 0;
				yPos++;
			}
		}
		else if (wParam == VK_ESCAPE)
		{
			x_count = 0;
			yPos = 0;
		}
		else if (wParam == VK_TAB)
		{
			for (int i = 0; i < 4; i++)
				str[yPos][x_count++] = ' ';
		}
		else if (wParam == 'q' || wParam == 'Q')
		{
			HideCaret(hwnd);
			DestroyCaret();
			PostQuitMessage(0);
		}
		else if (wParam == '1' || wParam == '2' || wParam == '3' || wParam == '4'
			|| wParam == '5' || wParam == '6' || wParam == '7' || wParam == '8' || wParam == '9' && yPos < 10)
		{
			yPos++;
			x_count = 0;
	
			str[yPos][x_count++] = wParam;	
			b_color = true;
		}
		else
		{
			b_color = false;
				
			if (x_count > 9)
			{
				yPos++;
				x_count = 0;
			}

			str[yPos][x_count++] = wParam;
			
			
		}
		if (yPos > 9) break;
		str[yPos][x_count] = '\0';

		InvalidateRgn(hwnd, NULL, TRUE);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_DESTROY:
		HideCaret(hwnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}