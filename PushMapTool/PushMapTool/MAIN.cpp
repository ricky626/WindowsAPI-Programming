#include <windows.h> // À©µµ¿ì Çì´õ ÆÄÀÏ
#include <stdio.h>
#include <math.h>
#include "resource.h"

HINSTANCE g_hInst;

LPCTSTR lpszClass = L"Push Push Map Tool";

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
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_THICKFRAME, 100, 50, 970, 600, NULL, (HMENU)NULL, hInstance, NULL);
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

#define Width 15
#define Height 15

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, memdc2;

	HBITMAP hBackBit, hOldBitmap;
	static HBITMAP bmp_background;

	static HBITMAP bmp_start, bmp_end;
	static HBITMAP bmp_white, bmp_blue;
	static HBITMAP bmp_floor1, bmp_floor2, bmp_floor3;
	static HBITMAP bmp_muk, bmp_gee, bmp_bar;
	static HBITMAP bmp_endmuk, bmp_endgee, bmp_endbar;


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

		bmp_background = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
		bmp_white = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WHITE));
		bmp_blue = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLUE));

		bmp_start = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_START));
		bmp_end = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_END));

		bmp_floor1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR1));
		bmp_floor2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR2));
		bmp_floor3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FLOOR3));

		bmp_muk = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MUK));
		bmp_gee = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_GEE));
		bmp_bar = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BAR));

		bmp_endmuk = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENDMUK));
		bmp_endgee = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENDGEE));
		bmp_endbar = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ENDBAR));


		info[0] = { 600, 100 }; //ºó°Å
		info[1] = { 650, 100 }; //1Ãþ
		info[2] = { 700, 100 }; //2Ãþ
		info[3] = { 750, 100 }; //3Ãþ
		info[4] = { 800, 100 }; //¹Ð¾î¾ßµÇ´Â°Å
		info[5] = { 850, 100 }; //¸ñÀûÁö
		info[6] = { 900, 100 }; //Ä³¸¯ÅÍ

		info[7] = { 600, 200 }; //¹¬
		info[8] = { 650, 200 }; //Âî
		info[9] = { 700, 200 }; //ºü

		info[10] = { 600, 300 }; //¾Øµå ¹¬
		info[11] = { 650, 300 }; //¾Øµå Âî
		info[12] = { 700, 300 }; //¾Øµå ºü


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
				if (mapcheck[i][j] == 0) SelectObject(memdc2, bmp_white); //ºó°Å 
				else if (mapcheck[i][j] == 1) SelectObject(memdc2, bmp_floor1); //1Ãþ
				else if (mapcheck[i][j] == 2) SelectObject(memdc2, bmp_floor2); // 2Ãþ
				else if (mapcheck[i][j] == 3) SelectObject(memdc2, bmp_floor3); // 3Ãþ
				else if (mapcheck[i][j] == 4) SelectObject(memdc2, bmp_blue); // ¹Ð¾î¾ßµÇ´Â°Å
				else if (mapcheck[i][j] == 5) SelectObject(memdc2, bmp_end); //¸ñÀûÁö
				else if (mapcheck[i][j] == 6) SelectObject(memdc2, bmp_start); //Ä³¸¯ÅÍ
				else if (mapcheck[i][j] == 7) SelectObject(memdc2, bmp_muk); //Ä³¸¯ÅÍ
				else if (mapcheck[i][j] == 8) SelectObject(memdc2, bmp_gee); //Ä³¸¯ÅÍ
				else if (mapcheck[i][j] == 9) SelectObject(memdc2, bmp_bar); //Ä³¸¯ÅÍ

				else if (mapcheck[i][j] == 10) SelectObject(memdc2, bmp_endmuk); //Ä³¸¯ÅÍ
				else if (mapcheck[i][j] == 11) SelectObject(memdc2, bmp_endgee); //Ä³¸¯ÅÍ
				else if (mapcheck[i][j] == 12) SelectObject(memdc2, bmp_endbar); //Ä³¸¯ÅÍ



				else
					dotsize = 0;

				BitBlt(memdc, Paint[i][j].x + dotsize, Paint[i][j].y, Paint[i][j].x + 32, Paint[i][j].y + 32, memdc2, 0, 0, SRCCOPY);


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

		SelectObject(memdc2, bmp_white);		BitBlt(memdc, info[0].x, info[0].y, info[0].x + 32, info[0].y + 32, memdc2, 0, 0, SRCCOPY); // ºó°Å
		SelectObject(memdc2, bmp_floor1);		BitBlt(memdc, info[1].x, info[1].y, info[1].x + 32, info[1].y + 32, memdc2, 0, 0, SRCCOPY); // 1Ãþ
		SelectObject(memdc2, bmp_floor2);		BitBlt(memdc, info[2].x, info[2].y, info[2].x + 32, info[2].y + 32, memdc2, 0, 0, SRCCOPY); // 2Ãþ
		SelectObject(memdc2, bmp_floor3);		BitBlt(memdc, info[3].x, info[3].y, info[3].x + 32, info[3].y + 32, memdc2, 0, 0, SRCCOPY); // 3Ãþ
		SelectObject(memdc2, bmp_blue);		BitBlt(memdc, info[4].x, info[4].y, info[4].x + 32, info[4].y + 32, memdc2, 0, 0, SRCCOPY); // ¹Ð¾î¾ßµÇ´Â°Å
		SelectObject(memdc2, bmp_end);		BitBlt(memdc, info[5].x, info[5].y, info[5].x + 32, info[5].y + 32, memdc2, 0, 0, SRCCOPY); // ¸ñÀûÁö
		SelectObject(memdc2, bmp_start);		BitBlt(memdc, info[6].x, info[6].y, info[6].x + 32, info[6].y + 32, memdc2, 0, 0, SRCCOPY); // Ä³¸¯ÅÍ

		SelectObject(memdc2, bmp_muk);		BitBlt(memdc, info[7].x, info[7].y, info[7].x + 32, info[7].y + 32, memdc2, 0, 0, SRCCOPY); 
		SelectObject(memdc2, bmp_gee);		BitBlt(memdc, info[8].x, info[8].y, info[8].x + 32, info[8].y + 32, memdc2, 0, 0, SRCCOPY);
		SelectObject(memdc2, bmp_bar);		BitBlt(memdc, info[9].x, info[9].y, info[9].x + 32, info[9].y + 32, memdc2, 0, 0, SRCCOPY);

		SelectObject(memdc2, bmp_endmuk);		BitBlt(memdc, info[10].x, info[10].y, info[10].x + 32, info[10].y + 32, memdc2, 0, 0, SRCCOPY); 
		SelectObject(memdc2, bmp_endgee);		BitBlt(memdc, info[11].x, info[11].y, info[11].x + 32, info[11].y + 32, memdc2, 0, 0, SRCCOPY); 
		SelectObject(memdc2, bmp_endbar);		BitBlt(memdc, info[12].x, info[12].y, info[12].x + 32, info[12].y + 32, memdc2, 0, 0, SRCCOPY); 

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