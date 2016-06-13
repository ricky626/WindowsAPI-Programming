#include <windows.h> // 윈도우 헤더 파일
#include <stdio.h>
#include <math.h>
#include "resource.h"

HINSTANCE g_hInst;

LPCTSTR lpszClass = L"CT Map Tool";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	HACCEL hAcc;
	hAcc = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 100, 50, 970, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAcc, &Message))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

	}

	return Message.wParam;
}

typedef struct _Object
{
	int					x;
	int					y;

}_Object;

float LengthPts(int x1, int y1, int x2, int y2)
{
	return (sqrt((float)(x1 - x2)*(x1 - x2) + (float)(y1 - y2)*(y1 - y2)));
}

BOOL InCircle(int x, int y, int mx, int my, int BSIZE)
{
	if (LengthPts(x + BSIZE, y + BSIZE, mx, my) < BSIZE)
		return				TRUE;
	else
		return				FALSE;
}

#define Width 16
#define Height 12

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2;

	HBITMAP hBackBit, hOldBitmap;
	static HBITMAP bmp_background, bmp_bum;

	static HBITMAP bmp_start, bmp_end;

	static HBITMAP bmp_black, bmp_white;
	
	static HBITMAP bmp_red, bmp_blue, bmp_green, bmp_yellow, bmp_purple;
	static HBITMAP bmp_Dred, bmp_Dblue, bmp_Dgreen, bmp_Dyellow, bmp_Dpurple;
	static HBITMAP bmp_Sred, bmp_Sblue, bmp_Sgreen, bmp_Syellow, bmp_Spurple;
	

	static _Object info_red;//0
	static _Object info_blue;//1
	static _Object info_green;//2
	static _Object info_orange;//3
	static _Object info_pink;//4
	static _Object info_skyblue;//5
	static _Object info_black;//6
	static _Object info[19];
	static int dotsize;

	static _Object  Paint[Height][Width];
	static int		mapcheck[Height][Width];
	static int		memory;
	static _Object	nemo;

	int mx, my;
	FILE* f;
	FILE* fp;

	switch (iMessage)
	{
	case WM_CREATE:

		bmp_background		= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
		bmp_black			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLACK));
		bmp_white			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WHITE));

		bmp_red				= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_RED));
		bmp_yellow			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_YELLOW));
		bmp_green			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GREEN));
		bmp_blue			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLUE));
		bmp_purple			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PURPLE));

		bmp_Dred			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DRED));
		bmp_Dyellow			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DYELLOW));
		bmp_Dgreen			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DGREEN));
		bmp_Dblue			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DBLUE));
		bmp_Dpurple			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DPURPLE));

		bmp_Sred			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SRED));
		bmp_Syellow			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SYELLOW));
		bmp_Sgreen			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SGREEN));
		bmp_Sblue			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SBLUE));
		bmp_Spurple			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SPURPLE));

		bmp_start			= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_START));
		bmp_end				= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_END));

		bmp_bum				= (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BUM));

		
		
		info[0] = { 600, 200 }; // white
		info[1] = { 650, 200 }; // red
		info[2] = { 700, 200 }; // yellow
		info[3] = { 750, 200 }; // green
		info[4] = { 800, 200 }; // blue
		info[5] = { 850, 200 }; // purple
		info[6] = { 900, 200 }; // black
		info[7] = { 600, 100 }; // start
		info[8] = { 650, 100 }; // end

		info[9] = { 650, 300 }; // Dred
		info[10] = { 700, 300 }; // Dyellow
		info[11] = { 750, 300 }; // Dgreen
		info[12] = { 800, 300 }; // Dblue
		info[13] = { 850, 300 }; // Dpurple

		info[14] = { 650, 400 }; // Sred
		info[15] = { 700, 400 }; // Syellow
		info[16] = { 750, 400 }; // Sgreen
		info[17] = { 800, 400 }; // Sblue
		info[18] = { 850, 400 }; // Spurple


		nemo.x = -100;
		nemo.y = -100;
		dotsize = 0;

		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				Paint[i][j].x = j * 32 + 32;
				Paint[i][j].y = i * 32 + 32;
				mapcheck[i][j] = 0;
			}
		}
		memory = 0;


		break;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case ID_LOAD:

			fp = fopen("Stage1.txt", "r");

			for (int i = 0; i < Height; i++)
			{
				for (int j = 0; j < Width; j++)
				{
					fscanf(fp, "%02d ", &mapcheck[i][j]);
				}
			}

			fclose(fp);

			break;

		case ID_SAVE:

			f = fopen("Stage1.txt", "w+");

			for (int i = 0; i < Height; i++)
			{
				for (int j = 0; j < Width; j++)
				{
					fprintf(f, "%02d ", mapcheck[i][j]);
				}
			}
			fclose(f);
			break;
		case ID_INIT:
			
			for (int i = 0; i < Height; i++)
			{
				for (int j = 0; j < Width; j++)
				{
					mapcheck[i][j] = 0;
				}
			}

			break;
		case ID_QUIT:
			PostQuitMessage(0);
			break;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_RBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (InCircle(Paint[i][j].x, Paint[i][j].y, mx, my, 16))
				{
					mapcheck[i][j] = 0;
					break;
				}
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);

		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);


		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (InCircle(Paint[i][j].x, Paint[i][j].y, mx, my, 16))
				{
					mapcheck[i][j] = memory;
					break;
				}
			}
		}
		for (int i = 0; i < 19; i++)
		{
			if (InCircle(info[i].x, info[i].y, mx, my, 16))
			{
				memory = i;
				nemo = info[i];
				break;
			}
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		memdc = CreateCompatibleDC(hdc);
		hBackBit = CreateCompatibleBitmap(hdc, 1024, 768);
		hOldBitmap = (HBITMAP)SelectObject(memdc, hBackBit);

		memdc2 = CreateCompatibleDC(hdc);

		SelectObject(memdc2, bmp_background);
		BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);

		SelectObject(memdc2, bmp_white);

		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (mapcheck[i][j] == 0) SelectObject(memdc2, bmp_white);
				else if (mapcheck[i][j] == 1) SelectObject(memdc2, bmp_red);
				else if (mapcheck[i][j] == 2) SelectObject(memdc2, bmp_yellow);
				else if (mapcheck[i][j] == 3) SelectObject(memdc2, bmp_green);
				else if (mapcheck[i][j] == 4) SelectObject(memdc2, bmp_blue);
				else if (mapcheck[i][j] == 5) SelectObject(memdc2, bmp_purple);
				else if (mapcheck[i][j] == 6) SelectObject(memdc2, bmp_black);

				else if (mapcheck[i][j] == 7) SelectObject(memdc2, bmp_start);
				else if (mapcheck[i][j] == 8) SelectObject(memdc2, bmp_end);

				else if (mapcheck[i][j] == 9) SelectObject(memdc2, bmp_Dred);
				else if (mapcheck[i][j] == 10) SelectObject(memdc2, bmp_Dyellow);
				else if (mapcheck[i][j] == 11) SelectObject(memdc2, bmp_Dgreen);
				else if (mapcheck[i][j] == 12) SelectObject(memdc2, bmp_Dblue);
				else if (mapcheck[i][j] == 13) SelectObject(memdc2, bmp_Dpurple);

				else if (mapcheck[i][j] == 14) SelectObject(memdc2, bmp_Sred);
				else if (mapcheck[i][j] == 15) SelectObject(memdc2, bmp_Syellow);
				else if (mapcheck[i][j] == 16) SelectObject(memdc2, bmp_Sgreen);
				else if (mapcheck[i][j] == 17) SelectObject(memdc2, bmp_Sblue);
				else if (mapcheck[i][j] == 18) SelectObject(memdc2, bmp_Spurple);

				if (mapcheck[i][j] == 9 || mapcheck[i][j] == 10 || mapcheck[i][j] == 11 || mapcheck[i][j] == 12 || mapcheck[i][j] == 13)
				{
					dotsize = 1;
					dotsize = 1;
				}
				else
					dotsize = 0;

				BitBlt(memdc, Paint[i][j].x + dotsize, Paint[i][j].y + dotsize, Paint[i][j].x + 32 + dotsize, Paint[i][j].y + 32 + dotsize, memdc2, 0, 0, SRCCOPY);

				
			}
		}
		
		SelectObject(memdc, memdc2);
		SelectObject(memdc, GetStockObject(NULL_BRUSH));

		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				Rectangle(memdc, j * 32 + 32, i * 32 + 32, j * 32 + 32 + 32, i * 32 + 32 + 32);
			}
		}

		SelectObject(memdc2, bmp_white);		BitBlt(memdc, info[0].x, info[0].y, info[0].x + 32, info[0].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_red);			BitBlt(memdc, info[1].x, info[1].y, info[1].x + 32, info[1].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_yellow);		BitBlt(memdc, info[2].x, info[2].y, info[2].x + 32, info[2].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_green);		BitBlt(memdc, info[3].x, info[3].y, info[3].x + 32, info[3].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_blue);			BitBlt(memdc, info[4].x, info[4].y, info[4].x + 32, info[4].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_purple);		BitBlt(memdc, info[5].x, info[5].y, info[5].x + 32, info[5].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_black);		BitBlt(memdc, info[6].x, info[6].y, info[6].x + 32, info[6].y + 32, memdc2, 0, 0, SRCCOPY);

		SelectObject(memdc2, bmp_start);		BitBlt(memdc, info[7].x, info[7].y, info[7].x + 32, info[7].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_end);			BitBlt(memdc, info[8].x, info[8].y, info[8].x + 32, info[8].y + 32, memdc2, 0, 0, SRCCOPY);

		SelectObject(memdc2, bmp_Dred);			BitBlt(memdc, info[9].x, info[9].y, info[9].x + 30, info[9].y + 30, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Dyellow);		BitBlt(memdc, info[10].x, info[10].y, info[10].x + 30, info[10].y + 30, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Dgreen);		BitBlt(memdc, info[11].x, info[11].y, info[11].x + 30, info[11].y + 30, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Dblue);		BitBlt(memdc, info[12].x, info[12].y, info[12].x + 30, info[12].y + 30, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Dpurple);		BitBlt(memdc, info[13].x, info[13].y, info[13].x + 30, info[13].y + 30, memdc2, 0, 0, SRCCOPY);

		SelectObject(memdc2, bmp_Sred);			BitBlt(memdc, info[14].x, info[14].y, info[14].x + 32, info[14].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Syellow);		BitBlt(memdc, info[15].x, info[15].y, info[15].x + 32, info[15].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Sgreen);		BitBlt(memdc, info[16].x, info[16].y, info[16].x + 32, info[16].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Sblue);		BitBlt(memdc, info[17].x, info[17].y, info[17].x + 32, info[17].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_Spurple);		BitBlt(memdc, info[18].x, info[18].y, info[18].x + 32, info[18].y + 32, memdc2, 0, 0, SRCCOPY);

		SelectObject(memdc2, bmp_bum);			BitBlt(memdc, 25, 450, 625, 530, memdc2, 0, 0, SRCCOPY);

		SelectObject(memdc, memdc2);
		SelectObject(memdc, GetStockObject(NULL_BRUSH));


		Rectangle(memdc, nemo.x - 3, nemo.y - 3, nemo.x + 32 + 3, nemo.y + 32 + 3);

		

		DeleteDC(memdc2);

		BitBlt(hdc, 0, 0, 1024, 768, memdc, 0, 0, SRCCOPY);

		SelectObject(memdc, hOldBitmap);
		DeleteObject(hBackBit);

		DeleteDC(memdc);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}