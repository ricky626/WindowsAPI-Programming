#include <Windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <time.h>

HINSTANCE g_hlnst;

LPCTSTR lpszClass = "window program 1-1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsh, WPARAM wParam, LPARAM lParam);

typedef struct LRTB{
	int left, right, top, bottom, r, g, b, top2, bottom2;
}LRTB;

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

	HDC hdc; //����� ������ ������ �� �ִ� Ÿ��  hdc ������ ����� ������ ������ ���� ������ ������ �� ����

	PAINTSTRUCT ps; //��µ� ������ ���� ������ ������ ����ü ������ ���� �ּ�

	HBRUSH hBrush, oldBrush;

	HPEN  hPen, oldPen;
	static LRTB  lrtb[5];
	static int count, info[5], keydown[5], xPos[5] = { 0, 0, 0, 0, 0 }, yPos[5] = { 0, 0, 0, 0, 0 };
	static int nowCount; //���絵������
	static int delNum[5] = { 0, 0, 0, 0, 0 }; //�����ȣ
	static bool empty[5];
	static SIZE size;

	int j = 0;

	int z = 1;

	int y = 0;



	switch (uMsg)

	{

	case WM_CREATE:
		srand((unsigned int)time(NULL));
		count = 0;
		nowCount = 0;
		for (int i = 0; i < 5; i++)
			empty[i] = false;
		break;

	case WM_CHAR:

		break;

	case WM_KEYUP:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (count == 5)
		{
			count = 0;
		}
		if (nowCount == 6)
			nowCount--;
		switch (wParam)
		{
		case 49: //1��
			for (int i = 0; i < 5; i++){
				if (keydown[i] == 1 && i != 0)
					keydown[i] = 0;
				if (keydown[0] == 0)
					keydown[0] = 1;
				else if (keydown[0] == 1)
					keydown[0] = 0;
			}
			break;
		case 50: //2��
			for (int i = 0; i < 5; i++){
				if (keydown[i] == 1 && i != 1)
					keydown[i] = 0;
				if (keydown[1] == 0)
					keydown[1] = 1;
				else if (keydown[1] == 1)
					keydown[1] = 0;
			}
			break;
		case 51: //3��
			for (int i = 0; i < 5; i++){
				if (keydown[i] == 1 && i != 2)
					keydown[i] = 0;
				if (keydown[2] == 0)
					keydown[2] = 1;
				else if (keydown[2] == 1)
					keydown[2] = 0;
			}
			break;
		case 52: //4��
			for (int i = 0; i < 5; i++){
				if (keydown[i] == 1 && i != 3)
					keydown[i] = 0;
				if (keydown[3] == 0)
					keydown[3] = 1;
				else if (keydown[3] == 1)
					keydown[3] = 0;
			}
			break;
		case 53: //5��
			for (int i = 0; i < 5; i++){
				if (keydown[i] == 1 && i != 4)
					keydown[i] = 0;
				if (keydown[4] == 0)
					keydown[4] = 1;
				else if (keydown[4] == 1)
					keydown[4] = 0;
			}
			break;

		case VK_DELETE: //�����
			if (nowCount >= 0)
			{
				for (int i = 0; i < 5; i++){
					if (keydown[i] == 1)
						delNum[i] = 1;
					else if (keydown[i] == 2)
						delNum[i] = 1;
					else if (keydown[i] == 3)
						delNum[i] = 1;
					else if (keydown[i] == 4)
						delNum[i] = 1;
					else if (keydown[i] == 5)
						delNum[i] = 1;
					
				}
				if (nowCount != 0)
					nowCount--;
			}
			
				
			empty[count] = false;
			break;

		case VK_LEFT:
			for (int i = 0; i < 5; i++)
				if (keydown[i] == 1)
					xPos[i] -= 5;
			break;
		case VK_RIGHT:
			for (int i = 0; i < 5; i++)
				if (keydown[i] == 1)
					xPos[i] += 5;
			break;
		case VK_UP:
			for (int i = 0; i < 5; i++)
				if (keydown[i] == 1)
					yPos[i] -= 5;
			break;
		case VK_DOWN:
			for (int i = 0; i < 5; i++)
				if (keydown[i] == 1)
					yPos[i] += 5;
			break;
		case 0x45://eŰ
			lrtb[count].left = rand() % 600 + 1;
			lrtb[count].right = rand() % 180 + 20;
			lrtb[count].top = rand() % 400 + 1;
			lrtb[count].bottom = rand() % 180 + 20;
			lrtb[count].r = rand() % 255;
			lrtb[count].g = rand() % 255;
			lrtb[count].b = rand() % 255;
			info[count] = 0; //��
			nowCount++;
			keydown[count] = 0;
			empty[count] = true;
			if (count < 5)
				count++;
			break;
		case 84://tŰ
			lrtb[count].left = rand() % 600 + 1;
			lrtb[count].right = rand() % 400 + 20;
			lrtb[count].top = rand() % 100 + 11;
			lrtb[count].bottom = rand() % 100 + 20;
			lrtb[count].top2 = rand() % 100 + 10;
			lrtb[count].bottom2 = rand() % 100 + 13;
			lrtb[count].r = rand() % 255;
			lrtb[count].g = rand() % 255;
			lrtb[count].b = rand() % 255;
			info[count] = 1; //�ﰢ��
			nowCount++;
			empty[count] = true;
			keydown[count] = 0;
			if (count < 5)
				count++;
			break;
		case 82://rŰ
			lrtb[count].left = rand() % 600 + 1;
			lrtb[count].right = rand() % 180 + 20;
			lrtb[count].top = rand() % 400 + 1;
			lrtb[count].bottom = rand() % 180 + 20;
			lrtb[count].r = rand() % 255;
			lrtb[count].g = rand() % 255;
			lrtb[count].b = rand() % 255;
			info[count] = 2; //�簢��
			keydown[count] = 0;
			nowCount++;
			empty[count] = true;
			if (count < 5)
				count++;
			break;
		default:
			keydown[count] = 0;
			break;
		}


		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 5; i++)
		{
			hBrush = CreateSolidBrush(RGB(lrtb[i].r, lrtb[i].g, lrtb[i].b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			if (info[i] == 0){ //���׸���
				if (keydown[i] == 1 && delNum[i] != 1){
					hPen = CreatePen(PS_DOT, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectPen(hdc, hPen);
					Ellipse(hdc, lrtb[i].left + xPos[i], lrtb[i].top + yPos[i], lrtb[i].left + lrtb[i].right + xPos[i], lrtb[i].top + lrtb[i].bottom + yPos[i]);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else if (delNum[i] != 1)
					Ellipse(hdc, lrtb[i].left + xPos[i], lrtb[i].top + yPos[i], lrtb[i].left + lrtb[i].right + xPos[i], lrtb[i].top + lrtb[i].bottom + yPos[i]);
				else if (delNum[i] == 1)
				{
					count = i;
					if (nowCount == 5)
						delNum[i] = 0;
				}
			}
			else if (info[i] == 2){
				if (keydown[i] == 1 && delNum[i] != 1){
					hPen = CreatePen(PS_DOT, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectPen(hdc, hPen);
					Rectangle(hdc, lrtb[i].left + xPos[i], lrtb[i].top + yPos[i], lrtb[i].left + lrtb[i].right + xPos[i], lrtb[i].top + lrtb[i].bottom + yPos[i]);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else if (delNum[i] != 1)
					Rectangle(hdc, lrtb[i].left + xPos[i], lrtb[i].top + yPos[i], lrtb[i].left + lrtb[i].right + xPos[i], lrtb[i].top + lrtb[i].bottom + yPos[i]);
				else if (delNum[i] == 1)
				{
					count = i;
					if (nowCount == 5)
						delNum[i] = 0;
				}
			}
			else if (info[i] == 1)
			{
				if (keydown[i] == 1 && delNum[i] != 1){
					hPen = CreatePen(PS_DOT, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectPen(hdc, hPen);
					POINT point[10] = { { lrtb[i].left + xPos[i], lrtb[i].right + yPos[i] }, { lrtb[i].left + lrtb[i].top + xPos[i], lrtb[i].right + lrtb[i].bottom + yPos[i] },
					{ lrtb[i].left + lrtb[i].top2 + xPos[i], lrtb[i].right + lrtb[i].bottom2 + yPos[i] } };
					Polygon(hdc, point, 3);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else if (delNum[i] != 1){
					POINT point[10] = { { lrtb[i].left + xPos[i], lrtb[i].right + yPos[i] }, { lrtb[i].left + lrtb[i].top + xPos[i], lrtb[i].right + lrtb[i].bottom + yPos[i] },
					{ lrtb[i].left + lrtb[i].top2 + xPos[i], lrtb[i].right + lrtb[i].bottom2 + yPos[i] } };
					Polygon(hdc, point, 3);
				}
				else if (delNum[i] == 1)
				{
					count = i;
					if (nowCount == 5)
						delNum[i] = 0;
				}
			}
			SelectObject(hdc, hBrush);
			DeleteObject(hBrush);

		}
		EndPaint(hWnd, &ps);

		break;



	case WM_DESTROY:



		DestroyCaret();

		PostQuitMessage(0);

		return 0;

		break;

	}



	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}