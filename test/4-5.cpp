#include <windows.h> // 윈도우 헤더 파일
#include <time.h>
#include <math.h>
#include "resource.h"

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);;
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

//#define BSIZE 25 // 반지름

float LengthPts(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}

BOOL InCircle(float x, float y, float mx, float my, float BSIZE)
{
	if (LengthPts(x+BSIZE, y+BSIZE, mx, my) < BSIZE)
		return TRUE;
	else return FALSE;
}

typedef struct Shape
{
	float xPos;
	float yPos;
	int type; // 1: 직선// 2: 원// 3: 사각형
	bool check;//false: X// true: 그리기//
	int color[3];
	int size;
}Shape;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen[10], selPen;
	HBRUSH hBrush[10], selBrush;
	static int color_check;
	static HMENU hMenu, hSubMenu;

	static float boxX, boxY;

	static Shape shape[10];
	static BOOL COPY;
	static BOOL SELECT;
	static BOOL UNDO;
	static int curR, curG, curB;
	static int shape_num;
	static int copy_num;
	static int Select_num;

	static float oldX, oldY;
	float mx, my;
	static int count;

	switch (iMessage)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));
		for (int i = 0; i < 10; i++)
		{
			shape[i].xPos = -1000;
			shape[i].yPos = -1000;
			shape[i].type = 0;
			shape[i].size = rand()% 110 + 30;
			shape[i].check = false;

			for (int j = 0; j < 3; j++)
				shape[i].color[j] = 0;
		}

		COPY = FALSE;

		SELECT = FALSE;
		UNDO = FALSE;

		Select_num = -1;
		copy_num = -1;
		shape_num = -1;

		count = 0;
		oldX = -1000;
		oldY = -1000;
		hMenu = GetMenu(hwnd);
		hSubMenu = GetSubMenu(hMenu, 2);
		EnableMenuItem(hSubMenu, ID_EDIT_PASTE, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDIT_UNDO, MF_GRAYED);
		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_DRAW_LINE:
			for (int i = 0; i < 10; i++)
			{
				if (shape[i].check == false)
				{
					shape[i].xPos = rand() % 500;
					shape[i].yPos = rand() % 300;
					shape[i].type = 1; //직선
					shape[i].check = true;
					count++;
					break;
				}
			}
			break;

		case ID_DRAW_ELLIPSE:
			for (int i = 0; i < 10; i++)
			{
				if (shape[i].check == false)
				{
					shape[i].xPos = rand() % 500;
					shape[i].yPos = rand() % 300;
					shape[i].type = 2; //원
					shape[i].check = true;
					count++;
					break;
				}
			}
			break;

		case ID_DRAW_RECTANGLE:
			for (int i = 0; i < 10; i++)
			{
				if (shape[i].check == false)
				{
					shape[i].xPos = rand() % 500;
					shape[i].yPos = rand() % 300;
					shape[i].type = 3; //사각형
					shape[i].check = true;
					count++;
					break;
				}
			}
			break;

		case ID_COLOR_RED:
			shape[Select_num].color[0] = 255;
			shape[Select_num].color[1] = 0;
			shape[Select_num].color[2] = 0;
			
			for (int i = count; i < 10; i++)
			{
				shape[i].color[0] = 255;
				shape[i].color[1] = 0;
				shape[i].color[2] = 0;
			}
			break;

		case ID_COLOR_GREEN:
			shape[Select_num].color[0] = 2;
			shape[Select_num].color[1] = 119;
			shape[Select_num].color[2] = 54;
			
			for (int i = count; i < 10; i++)
			{
				shape[i].color[0] = 2;
				shape[i].color[1] = 119;
				shape[i].color[2] = 54;
			}
			break;
		case ID_COLOR_YELLOW:
			shape[Select_num].color[0] = 255;
			shape[Select_num].color[1] = 255;
			shape[Select_num].color[2] = 0;

			for (int i = count; i < 10; i++)
			{
				shape[i].color[0] = 255;
				shape[i].color[1] = 255;
				shape[i].color[2] = 0;
			}
			break;

		case ID_COLOR_BLUE:
			shape[Select_num].color[0] = 0;
			shape[Select_num].color[1] = 0;
			shape[Select_num].color[2] = 255;

			for (int i = count; i < 10; i++)
			{
				shape[i].color[0] = 0;
				shape[i].color[1] = 0;
				shape[i].color[2] = 255;
			}
			break;

		case ID_COLOR_PURPLE:
			shape[Select_num].color[0] = 121;
			shape[Select_num].color[1] = 21;
			shape[Select_num].color[2] = 110;

			for (int i = count; i < 10; i++)
			{
				shape[i].color[0] = 121;
				shape[i].color[1] = 21;
				shape[i].color[2] = 110;
			}
			break;

		case ID_COLOR_LIGHTGREEN:
			shape[Select_num].color[0] = 0;
			shape[Select_num].color[1] = 255;
			shape[Select_num].color[2] = 0;

			for (int i = count; i < 10; i++)
			{
				shape[i].color[0] = 0;
				shape[i].color[1] = 255;
				shape[i].color[2] = 0;
			}
			break;

		case ID_COLOR_BLACK:
			shape[Select_num].color[0] = 0;
			shape[Select_num].color[1] = 0;
			shape[Select_num].color[2] = 0;

			for (int i = count; i < 10; i++)
			{
				shape[i].color[0] = 0;
				shape[i].color[1] = 0;
				shape[i].color[2] = 0;
			}
			break;

		case ID_COPY_0:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 0;
			break;

		case ID_COPY_1:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 1;
			break;

		case ID_COPY_2:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 2;
			break;

		case ID_COPY_3:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 3;
			break;

		case ID_COPY_4:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 4;
			break;

		case ID_COPY_5:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 5;
			break;

		case ID_COPY_6:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 6;
			break;

		case ID_COPY_7:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 7;
			break;

		case ID_COPY_8:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 8;
			break;

		case ID_COPY_9:
			COPY = TRUE;
			UNDO = TRUE;
			copy_num = 9;
			break;

		case ID_EDIT_PASTE:
			oldX = shape[copy_num].xPos;
			oldY = shape[copy_num].yPos;

			shape[copy_num].xPos = rand() % 500;
			shape[copy_num].yPos = rand() % 300;

			break;

		case ID_EDIT_UNDO:
			if (COPY == TRUE)
			{
				shape[copy_num].xPos = oldX;
				shape[copy_num].yPos = oldY;

				COPY = FALSE;
				UNDO = FALSE;
			}
			
			break;
		}

		Select_num = -1;
		SELECT = FALSE;
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		for (int i = 0; i < 10; i++)
		{
			if (shape[i].type == 1)
			{
				if (InCircle(shape[i].xPos, shape[i].yPos, mx, my, (shape[i].size/2)))
				{
					SELECT = TRUE;
					Select_num = i;
					break;
				}
			}
			
			if (InCircle(shape[i].xPos, shape[i].yPos, mx, my, (shape[i].size/2)))
			{
				SELECT = TRUE;
				Select_num = i;
				break;
			}
			else
			{
				SELECT = FALSE;
			}
				
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		selPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));

		selBrush = CreateSolidBrush(RGB(255, 255, 255));

		EnableMenuItem(hSubMenu, ID_EDIT_PASTE, COPY ? MF_ENABLED : MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDIT_UNDO, UNDO ? MF_ENABLED : MF_GRAYED);

		if (SELECT == TRUE)
		{
			SelectObject(hdc, hBrush);
			SelectObject(hdc, selPen);

			Rectangle(hdc, shape[Select_num].xPos - 10, shape[Select_num].yPos - 10, shape[Select_num].xPos + shape[Select_num].size + 10, shape[Select_num].yPos + shape[Select_num].size + 10);
		}		

		DeleteObject(selBrush);
		DeleteObject(selPen);

		for (int i = 0; i < 10; i++)
		{
			hPen[i] = CreatePen(PS_SOLID, 1, RGB(shape[i].color[0], shape[i].color[1], shape[i].color[2]));

			hBrush[i] = CreateSolidBrush(RGB(shape[i].color[0], shape[i].color[1], shape[i].color[2]));
		}
		
		
		for (int i = 0; i < 10; i++)
		{
			if (shape[i].check == true)
			{
				SelectObject(hdc, hBrush[i]);
				SelectObject(hdc, hPen[i]);

				if (shape[i].type == 1) //직선
				{
					MoveToEx(hdc, shape[i].xPos, shape[i].yPos, NULL);
					LineTo(hdc, shape[i].xPos + shape[i].size, shape[i].yPos + shape[i].size);
				}

				if (shape[i].type == 2) //원
				{
					Ellipse(hdc, shape[i].xPos, shape[i].yPos, shape[i].xPos + shape[i].size, shape[i].yPos + shape[i].size);
				}

				if (shape[i].type == 3) //사각형
				{
					Rectangle(hdc, shape[i].xPos, shape[i].yPos, shape[i].xPos + shape[i].size, shape[i].yPos + shape[i].size);
				}
			}
		}

		for (int i = 0; i < 10; i++)
		{
			DeleteObject(hBrush[i]);
			DeleteObject(hPen[i]);
		}		
		
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}