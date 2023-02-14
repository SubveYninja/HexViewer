#include <Windows.h>
#include "Info.h"
#include "resource.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const int lines = sizeof(sysmetrics) / sizeof(sysmetrics[0]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "IDR_MENU";
	HWND hwnd;
	HMENU hMenu;
	MSG msg;
	WNDCLASSEX wndclass{};

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = (LPCWSTR)szAppName;
	wndclass.lpszClassName = (LPCWSTR)szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hMenu = LoadMenu(hInstance, L"IDR_MENU");

	hwnd = CreateWindow(
		(LPCWSTR)szAppName, // window class name 
		L"The Hello Program", // window caption 
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, // window style 
		CW_USEDEFAULT, // initial x position 
		CW_USEDEFAULT, // initial y position 
		CW_USEDEFAULT, // initial x size 
		CW_USEDEFAULT, // initial y size 
		NULL, // parent window handle 
		hMenu, // window menu handle 
		hInstance, // program instance handle 
		NULL
	); // creation parameters 

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	//RECT rect;
	TEXTMETRIC tm{};

	static int cxChar, cyChar, cxCaps;
	static int cyClient, cxClient, iMaxWidth;
	static int iVscrollPos, iVscrollMax, iHscrollPos, iHscrollMax;

	wchar_t szBuffer[10]{};

	int iVscrollInc, iHscrollInc;
	int iPaintBeg, iPaintEnd;

	switch (iMsg)
	{
	case WM_CREATE:

		hdc = BeginPaint(hwnd, &ps);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		EndPaint(hwnd, &ps);

		iMaxWidth = 40 * cxCaps + 22 * cxCaps;

		return 0;

	case WM_SIZE:

		cyClient = HIWORD(lParam);
		cxClient = HIWORD(lParam);

		iVscrollMax = max(0, lines + 2 - cyClient / cyChar);
		iVscrollPos = min(iVscrollPos, iVscrollMax);

		SetScrollRange(hwnd, SB_VERT, 0, iVscrollMax, FALSE);
		SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);

		iHscrollMax = max(0, 2 + (iMaxWidth - cxClient) / cxChar);
		iHscrollPos = min(iHscrollPos, iHscrollMax);

		SetScrollRange(hwnd, SB_HORZ, 0, iHscrollMax, FALSE);
		SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);

		return 0;

	case WM_VSCROLL:

		switch (LOWORD(wParam))
		{
		case SB_TOP:

			iVscrollInc = -iVscrollPos;
			break;

		case SB_BOTTOM:

			iVscrollInc = iVscrollMax - iVscrollPos;
			break;

		case SB_LINEUP:

			iVscrollInc = -1;
			break;

		case SB_LINEDOWN:

			iVscrollInc = 1;
			break;

		case SB_PAGEUP:

			iVscrollInc = min(-1, -cyClient / cyChar);
			break;

		case SB_PAGEDOWN:

			iVscrollInc = max(1, cyClient / cyChar);
			break;

		case SB_THUMBTRACK:

			iVscrollInc = HIWORD(wParam) - iVscrollPos;
			break;

		default:

			iVscrollInc = 0;
			break;
		}

		iVscrollInc = max(-iVscrollPos, min(iVscrollInc, iVscrollMax - iVscrollPos));

		if (iVscrollInc != 0)
		{
			iVscrollPos += iVscrollInc;
			ScrollWindow(hwnd, 0, -cyChar * iVscrollInc, NULL, NULL);
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_HSCROLL:

		switch (LOWORD(wParam))
		{
		case SB_LINEUP:

			iHscrollInc = -1;
			break;

		case SB_LINEDOWN:

			iHscrollInc = 1;
			break;

		case SB_PAGEUP:

			iHscrollInc = min(-1, -cxClient / cxChar);
			break;

		case SB_PAGEDOWN:

			iHscrollInc = max(1, cxClient / cxChar);
			break;

		case SB_THUMBTRACK:

			iHscrollInc = HIWORD(wParam) - iHscrollPos;
			break;

		default:

			iHscrollInc = 0;
			break;
		}

		iHscrollInc = max(-iHscrollPos, min(iHscrollInc, iHscrollMax - iHscrollPos));

		if (iHscrollInc != 0)
		{
			iHscrollPos += iHscrollInc;
			ScrollWindow(hwnd, -cxChar * iHscrollInc, 0, NULL, NULL);
			SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		iPaintBeg = max(0, iVscrollPos + ps.rcPaint.top / cyChar - 1);
		iPaintEnd = min(lines, iVscrollPos + ps.rcPaint.bottom / cyChar);

		for (int x, y, i = iPaintBeg; i < iPaintEnd; i++)
		{
			x = cxChar * (1 - iHscrollPos);
			y = cyChar * (1 - iVscrollPos + i);

			TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlenW(sysmetrics[i].szLabel));
			TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlenW(sysmetrics[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintfW(szBuffer, L"%5d", GetSystemMetrics(sysmetrics[i].iIndex)));
			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}


