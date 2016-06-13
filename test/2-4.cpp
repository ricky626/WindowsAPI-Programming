#include <Windows.h>
#include <Windowsx.h>

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 1-3";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsh, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hlnst = hInstance;

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_HSCROLL | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 100, 100,
		800, 600, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hdc2; //출력할 영역을 지정할 수 있는 타입  hdc 변수는 출력할 영역을 얻어오면 얻어온 영역을 지정할 수 있음
	PAINTSTRUCT ps; //출력될 영역에 대한 정보를 저장할 구조체 공간에 대한 주소
	static char str[10][11];
	static int y_count, count, last;
	static bool bRGB = false;
	static SIZE size;

	switch (uMsg) {

	case WM_PAINT:    //화면이 깨질 때 마다 그려준다
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < y_count + 1; i++)
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


		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd); // 빈 화면에 캐럿 표시
		count = 0;
		y_count = 0;
		size.cx = 0;
		last = 0;
		return 0;

	case WM_KEYDOWN:
		if (wParam >= '0'
			&& wParam <= '9' && y_count < 10)
		{
			last = count;
			y_count++;
			count = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		hdc = GetDC(hWnd);

		if (VK_RETURN == wParam && y_count < 10) //엔터
		{
			last = count;
			count = 0;
			count--;
			y_count++;
		}

		if (str[y_count][count - 1] >= '0'
			&& str[y_count][count - 1] <= '9' && y_count < 10)
		{
			last = count;
			y_count++;
			count = 0;
		}

		if (count == 10 && y_count < 10){ // 10이넘어갈떄
			last = count;
			count = 0;
			y_count++;
		}
		else if (y_count<10)
			str[y_count][count++] = wParam;


		if (str[y_count][count - 1] == 'q'
			|| str[y_count][count - 1] == 'Q')
			PostQuitMessage(0);
		else
			str[y_count][count] = '￦0'; //널문자넣기

		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}