#include <Windows.h>
#include <string.h>
#include <stdlib.h>

int fct(int* index, int num, char*str);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	SIZE size;
	static char str[50];
	static int count, yPos, index;
	static int x1, y1, x2, y2;
	static int mode;
	RECT rc;
	ZeroMemory(&rc, sizeof(RECT));
	GetClientRect(hWnd, &rc);

	static char tEllipse[] = "Ellipse";

	switch (iMessage)
	{
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);

		x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		count = 0;
		mode = -1;
		break;

	case WM_CHAR:
		hdc = GetDC(hWnd);

		if (wParam == VK_BACK){
			if (count > 0)
				count--;
		}

		else if (wParam == VK_RETURN){
			if (str[0] == 'E'){
				mode = 0;
				index = 8;
			}

			else if (str[0] == 'L'){
				mode = 1;
				index = 5;
			}

			else if (str[0] == 'R'){
				mode = 2;
				index = 10;
			}

			InvalidateRect(hWnd, NULL, TRUE);
		}

		else
			str[count++] = wParam;

		str[count] = '\0';

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetTextExtentPoint(hdc, str, strlen(str), &size);
		Rectangle(hdc, 0, 470, 490, 490);

		TextOut(hdc, 0, 470, str, strlen(str));
		SetCaretPos(size.cx, yPos + 470);

		if (mode != -1){
			x1 = fct(&index, -1, str);
			y1 = fct(&index, x1, str);
			x2 = fct(&index, y1, str);
			y2 = fct(&index, x2, str);

			if (mode = 0)
				Ellipse(hdc, x1, y1, x2, y2);

			else if (mode = 1){
				MoveToEx(hdc, x1, y1, NULL);
				LineTo(hdc, x2, y2);
			}

			else if (mode == 2)
				Rectangle(hdc, x1, y1, x2, y2);

		}

		EndPaint(hWnd, &ps);

		break;

	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);

		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}


int fct(int* index, int num, char*str)
{
	int ret = 0;
	if (num >= 100)
		*index += 4;
	else if (num >= 10)
		*index += 3;
	else if (num >= 0)
		*index += 2;

	if (str[(*index) + 1] > 57 || str[(*index) + 1] < 48)
		ret = str[(*index)] - 48;
	else if (str[(*index) + 2] > 57 || str[(*index) + 2] < 48)
		ret = ((str[*index] - 48) * 10 + (str[*index] - 48));
	else if (str[(*index + 3)] > 57 || str[(*index + 3)] < 48)
		ret = ((str[*index] - 48) * 100 + (str[*index] - 48) * 10 + (str[*index] - 48));
	return ret;
}
