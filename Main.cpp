#include <Windows.h>
#include "Info.h"
#include "resource.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const int lines = sizeof(sysmetrics) / sizeof(sysmetrics[0]);

HWND hwndData;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	LPCWSTR lpszAppName = L"HexViewer";
	LPCWSTR lpszMenuName = L"IDR_MENU";
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
	wndclass.lpszMenuName = lpszMenuName;
	wndclass.lpszClassName = lpszAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hMenu = LoadMenu(hInstance, lpszMenuName);

	hwnd = CreateWindow(
		lpszAppName, // window class name 
		(LPCWSTR)L"TEST", // window caption 
		WS_OVERLAPPEDWINDOW | WS_VSCROLL, // window style 
		CW_USEDEFAULT, // initial x position 
		CW_USEDEFAULT, // initial y position 
		ixStartupWidth, // initial x size 
		iyStartupHeight, // initial y size 
		NULL, // parent window handle 
		hMenu, // window menu handle 
		hInstance, // program instance handle 
		NULL
	); // creation parameters 

	hwndData = CreateWindow(
		(LPCWSTR)L"edit",
		L"NULL",
		WS_CHILD,
		0,
		0,
		0,
		0,
		hwnd,
		hmData,
		hInstance,
		NULL);

	ShowWindow(hwndData, iCmdShow);
	UpdateWindow(hwnd);

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
	TEXTMETRIC tm{};
	SCROLLINFO si;


	RECT rectWnd, rectData;

	static int cxChar, cyChar, cxCaps;
	static int cyClient, cxClient, iMaxWidth;
	static int iVscrollPos, iHscrollPos;

	wchar_t szBuffer[10]{};

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

		iMaxWidth = 40 * cxChar + 22 * cxCaps;

		return 0;

	case WM_SIZE:

		cyClient = HIWORD(lParam);
		cxClient = LOWORD(lParam);

		MoveWindow(hwndData, 2 * cxChar, 2 * cyChar, ixStartupDataWidth, cyClient - 4 * cyChar, TRUE);

		GetClientRect(hwndData, &rectData);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = lines - 1;
		si.nPage = rectData.bottom / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + iMaxWidth / cxChar;
		si.nPage = (cxClient - cxChar * 2) / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return 0;

	//case WM_COMMAND:
	//	GetWindowRect(hwnd, &rectWnd);

	//	switch (wParam)
	//	{
	//	case 
	//	}

	//	return 0;

	case WM_VSCROLL:

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;

		GetScrollInfo(hwnd, SB_VERT, &si);
		iVscrollPos = si.nPos;

		switch (LOWORD(wParam))
		{
		case SB_TOP:

			si.nPos = si.nMin;
			break;

		case SB_BOTTOM:

			si.nPos = si.nMax;
			break;

		case SB_LINEUP:

			si.nPos -= 1;
			break;

		case SB_LINEDOWN:

			si.nPos += 1;
			break;

		case SB_PAGEUP:

			si.nPos -= si.nPage;
			break;

		case SB_PAGEDOWN:

			si.nPos += si.nPage;
			break;

		case SB_THUMBTRACK:

			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &si);

		if (si.nPos != iVscrollPos)
		{
			GetClientRect(hwnd, &rectWnd);
			InvalidateRect(hwnd, &rectWnd, TRUE);

			ScrollWindow(hwndData, 0, cyChar * (iVscrollPos - si.nPos), NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_HSCROLL:

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;

		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHscrollPos = si.nPos;

		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:

			si.nPos -= 1;
			break;

		case SB_LINERIGHT:

			si.nPos += 1;
			break;

		case SB_PAGELEFT:

			si.nPos -= si.nPage;
			break;

		case SB_PAGERIGHT:

			si.nPos += si.nPage;
			break;

		case SB_THUMBTRACK:

			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &si);

		if (si.nPos != iHscrollPos)
		{
			GetClientRect(hwnd, &rectWnd);
			InvalidateRect(hwnd, &rectWnd, TRUE);

			ScrollWindow(hwndData, cxChar * (iHscrollPos - si.nPos), 0, NULL, NULL);
			UpdateWindow(hwnd);
		}

		return 0;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, ixViewerSignPos, iyViewerSignPos, lpszViewerSign, lstrlen(lpszViewerSign));

		EndPaint(hwnd, &ps);
		ReleaseDC(hwnd, hdc);

		hdc = BeginPaint(hwndData, &ps);

		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVscrollPos = si.nPos;

		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHscrollPos = si.nPos;

		iPaintBeg = max(0, iVscrollPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(lines - 1, iVscrollPos + ps.rcPaint.bottom / cyChar);

		for (int x, y, i = iPaintBeg; i <= iPaintEnd; i++)
		{
			x = cxChar * (1 - iHscrollPos);
			y = cyChar * (i - iVscrollPos);

			TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, L"%5d", GetSystemMetrics(sysmetrics[i].iIndex)));
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


