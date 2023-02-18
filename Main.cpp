#include <Windows.h>
#include "Info.h"
#include "resource.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//VOID DrawContent(HDC hdcData);
//VOID LoadDataFromFile(LPCWSTR cPath);
//void gGetFileSize(HANDLE hFile, LPCWSTR path);

//const int lines = sizeof(sysmetrics) / sizeof(sysmetrics[0]);

HWND hwndData;
PBYTE pbFile;

//VOID LoadDataFromFile(LPCWSTR cPath) {
//
//	HANDLE hFileToLoad = CreateFile(
//		cPath,
//		GENERIC_READ,
//		NULL,
//		NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_READONLY,
//		NULL);
//
//	HANDLE hMapFile = CreateFileMapping(
//		hFileToLoad,
//		NULL,
//		PAGE_READONLY,
//		0,
//		0,
//		NULL);
//
//	DWORD dwFileSizeHigh;
//	__int64 qwFileSize = GetFileSize(hFileToLoad, &dwFileSizeHigh);
//	qwFileSize += (((__int64)dwFileSizeHigh) << 32);
//
//	__int64 qwFileOffset = 0;
//
//
//	PBYTE pbFile = (PBYTE)MapViewOfFile(
//		hMapFile,
//		FILE_MAP_READ,
//		0,
//		0,
//		0);
//
//
//
//	//INT iSaveLength = 16;
//
//	//DWORD bytesIter;
//	//ReadFile(hFileToLoad, Buffer, TextBufferSize, &bytesIter, NULL);
//
//	UnmapViewOfFile(pbFile);
//	CloseHandle(hMapFile);
//	CloseHandle(hFileToLoad);
//}



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

	hwndData = CreateWindow(
		(LPCWSTR)L"edit",
		L"NULL",
		WS_CHILD | WS_THICKFRAME,
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
	static SYSTEM_INFO sinf;

	static HANDLE hFile;
	static HANDLE hFileMapping;
	LPCWSTR cPath = (LPCWSTR)L"C:/Users/1metr/Downloads/68015000878_HC_enus_MTP850_FuG_Product_Information_Manual.pdf";

	RECT rectWnd, rectData;

	static int cxChar, cyChar, cxCaps;
	static int cyClient, cxClient, iMaxWidth;
	static int iVscrollPos, iVscrollMax, iHscrollPos, iHscrollMax;

	wchar_t szBuffer[10]{};

	int iVscrollInc, iHscrollInc;
	int iPaintBeg, iPaintEnd;

	switch (iMsg)
	{
	case WM_CREATE:

		GetSystemInfo(&sinf);

		hFile = CreateFile(
			cPath,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

		hFileMapping = CreateFileMapping(
			hFile, 
			NULL, 
			PAGE_READONLY, 
			0, 
			0, 
			NULL);

		qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
		qwFileSize += (((__int64)dwFileSizeHigh) << 32);
		qwFileSizeConst = qwFileSize;

		if (qwFileSize > 0)
		{
			dwBytesInBlock = sinf.dwAllocationGranularity;
			if (qwFileSize < sinf.dwAllocationGranularity)
			{
				dwBytesInBlock = (DWORD)qwFileSize;
			}

			pbFile = (PBYTE)MapViewOfFile(
				hFileMapping,
				FILE_MAP_READ,
				(DWORD)(qwFileOffset >> 32),
				(DWORD)(qwFileOffset & 0xFFFFFFFF),
				dwBytesInBlock);

			qwFileOffset += dwBytesInBlock;
			qwFileSize -= dwBytesInBlock;
		}

		hdc = BeginPaint(hwnd, &ps);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		EndPaint(hwnd, &ps);

		iMaxWidth = ixStartupDataWidth;

		return 0;

	case WM_SIZE:

		cyClient = HIWORD(lParam);
		cxClient = HIWORD(lParam);

		iVscrollMax = max(0, qwFileSizeConst / 16 + 2 - cyClient / cyChar);
		iVscrollPos = min(iVscrollPos, iVscrollMax);

		SetScrollRange(hwnd, SB_VERT, 0, iVscrollMax, FALSE);
		SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = qwFileSizeConst / 16 - 1;
		si.nPage = rectData.bottom / cyChar;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		SetScrollRange(hwnd, SB_HORZ, 0, iHscrollMax, FALSE);
		SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);

		return 0;

	case WM_COMMAND:
		GetWindowRect(hwnd, &rectWnd);

		switch (wParam)
		{
		case ID_FILE_OPEN:
			break;

		case ID_FILE_CLOSE:
			break;

		case ID_SEARCH_FIND:
			break;

		case ID_SEARCH_GO:
			break;

		case ID_EXIT:
			break;

		default:
			break;
		}

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
		iPaintEnd = min(qwFileSizeConst - 1, iVscrollPos + ps.rcPaint.bottom / cyChar);

		for (int x, y, i = iPaintBeg; i < iPaintEnd; i++)
		{

			x = cxChar * (1 - iHscrollPos);
			y = cyChar * (1 - iVscrollPos + i);

			//SetTextAlign(hdc, TA_LEFT | TA_TOP);
			TextOut(hdc, x, y, szBuffer, wsprintf(szBuffer, L"%08X", dwByte));

			for (int j = 0; (j < 16); j++)
			{
				TextOut(hdc, 150 + x + j * cxChar * 3, y, szBuffer, wsprintf(szBuffer, L"%02X", pbFile[dwByte]));
				dwByte++;

				if ((dwByte >= dwBytesInBlock - 1) && (qwFileSize != 0))
				{
					UnmapViewOfFile(pbFile);

					qwFileOffset += dwBytesInBlock;
					qwFileSize -= dwBytesInBlock;

					dwBytesInBlock = sinf.dwAllocationGranularity;
					if (qwFileSize < sinf.dwAllocationGranularity)
					{
						dwBytesInBlock = (DWORD)qwFileSize;
					}

					pbFile = (PBYTE)MapViewOfFile(
						hFileMapping,
						FILE_MAP_READ,
						(DWORD)(qwFileOffset >> 32),
						(DWORD)(qwFileOffset & 0xFFFFFFFF),
						dwBytesInBlock);

					dwByte = 0;
				}

			}

			if ((dwByte >= dwBytesInBlock - 1) && (qwFileSize != 0))
			{
				UnmapViewOfFile(pbFile);

				qwFileOffset += dwBytesInBlock;
				qwFileSize -= dwBytesInBlock;

				dwBytesInBlock = sinf.dwAllocationGranularity;
				if (qwFileSize < sinf.dwAllocationGranularity)
				{
					dwBytesInBlock = (DWORD)qwFileSize;
				}

				pbFile = (PBYTE)MapViewOfFile(
					hFileMapping,
					FILE_MAP_READ,
					(DWORD)(qwFileOffset >> 32),
					(DWORD)(qwFileOffset & 0xFFFFFFFF),
					dwBytesInBlock);

				dwByte = 0;
			}

		}

		if ((qwFileSize != 0))
		{
			dwByte = iVscrollPos % dwBytesInBlock * 16;

		}
		EndPaint(hwnd, &ps);





		//////hdc = BeginPaint(hwndData, &ps);

		//////si.cbSize = sizeof(si);
		//////si.fMask = SIF_POS;
		//////GetScrollInfo(hwnd, SB_VERT, &si);
		//////iVscrollPos = si.nPos;

		//////GetScrollInfo(hwnd, SB_HORZ, &si);
		//////iHscrollPos = si.nPos;

		//////iPaintBeg = max(0, iVscrollPos + ps.rcPaint.top / cyChar);
		//////iPaintEnd = min(lines - 1, iVscrollPos + ps.rcPaint.bottom / cyChar);

		//////for (int x, y, i = iPaintBeg; i <= iPaintEnd; i++)
		//////{
		//////	x = cxChar * (1 - iHscrollPos);
		//////	y = cyChar * (i - iVscrollPos);

		//////	TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
		//////	TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
		//////	SetTextAlign(hdc, TA_RIGHT | TA_TOP);

		//////	TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, L"%5d", GetSystemMetrics(sysmetrics[i].iIndex)));
		//////	SetTextAlign(hdc, TA_LEFT | TA_TOP);
		//////}

		//////EndPaint(hwnd, &ps);
		
		return 0;

	case WM_DESTROY:

		CloseHandle(hFile);
		CloseHandle(hFileMapping);
		UnmapViewOfFile(pbFile);

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}


//VOID DrawContentFromFile(HWND hwndData, LPCWSTR cPath)
//{
//	SYSTEM_INFO sinf;
//	GetSystemInfo(&sinf);
//
//	HANDLE hFileToLoad = CreateFile(
//		cPath,
//		GENERIC_READ,
//		NULL,
//		NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_READONLY,
//		NULL);
//
//	HANDLE hMapFile = CreateFileMapping(
//		hFileToLoad,
//		NULL,
//		PAGE_READONLY,
//		0,
//		0,
//		NULL);
//
//	DWORD dwFileSizeHigh;
//	__int64 qwFileSize = GetFileSize(hFileToLoad, &dwFileSizeHigh);
//	qwFileSize += (((__int64)dwFileSizeHigh) << 32);
//
//	__int64 qwFileOffset = 0;
//
//
//
//	PBYTE pbFile = (PBYTE)MapViewOfFile(
//		hMapFile,
//		FILE_MAP_READ,
//		0,
//		0,
//		0);
//
//
//
//
//
//	GetTextMetrics(hdc, &tm);
//	cxChar = tm.tmAveCharWidth;
//	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
//	cyChar = tm.tmHeight + tm.tmExternalLeading;
//
//	static int iPaintBeg = max(0, iVscrollPos + ps.rcPaint.top / cyChar);
//
//
//
//	//INT iSaveLength = 16;
//
//	//DWORD bytesIter;
//	//ReadFile(hFileToLoad, Buffer, TextBufferSize, &bytesIter, NULL);
//
//	UnmapViewOfFile(pbFile);
//	CloseHandle(hMapFile);
//	CloseHandle(hFileToLoad);
//}


//VOID LoadDataFromFile(LPCWSTR cPath) {
//
//	HANDLE hFileToLoad = CreateFile(
//		cPath,
//		GENERIC_READ,
//		NULL,
//		NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_READONLY,
//		NULL);
//
//	INT iSaveLength = 16;
//
//	DWORD bytesIter;
//	ReadFile(hFileToLoad, Buffer, TextBufferSize, &bytesIter, NULL);
//
//
//
//}

