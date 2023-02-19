#pragma once

LPCWSTR lpszViewerSign = L"Offset(h)   00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F    Decoded text";
static int ixViewerSignPos = 20;
static int iyViewerSignPos = 10;

static int ixStartupWidth = 700;
static int iyStartupHeight = 500;

static int ixStartupDataWidth = 600;


DWORD dwFileSizeHigh;
DWORD dwBytesInBlock;

__int64 qwFileSize;
__int64 qwFileSizeConst;
__int64 qwFileOffset = 0;

DWORD64 dwByte = 0;



HMENU hmData = (HMENU)1;

struct
{
	int iIndex;
	const wchar_t* szLabel;
	const wchar_t* szDesc;
} sysmetrics[] =
{
		SM_CXSCREEN, L"SM_CXSCREEN", L"Screen width in pixels",
		SM_CYSCREEN, L"SM_CYSCREEN", L"Screen height in pixels",
		SM_CXVSCROLL, L"SM_CXVSCROLL", L"Vertical scroll arrow width",
		SM_CYHSCROLL, L"SM_CYHSCROLL", L"Horizontal scroll arrow height",
		SM_CYCAPTION, L"SM_CYCAPTION", L"Caption bar height",
		SM_CXBORDER, L"SM_CXBORDER", L"Window border width",
		SM_CYBORDER, L"SM_CYBORDER", L"Window border height",
		SM_CXDLGFRAME, L"SM_CXDLGFRAME", L"Dialog window frame width",
		SM_CYDLGFRAME, L"SM_CYDLGFRAME", L"Dialog window frame height",
		SM_CYVTHUMB, L"SM_CYVTHUMB", L"Vertical scroll thumb height",
		SM_CXHTHUMB, L"SM_CXHTHUMB", L"Horizontal scroll thumb width",
		SM_CXICON, L"SM_CXICON", L"Icon width",
		SM_CYICON, L"SM_CYICON", L"Icon height",
		SM_CXCURSOR, L"SM_CXCURSOR", L"Cursor width",
		SM_CYCURSOR, L"SM_CYCURSOR", L"Cursor height",
		SM_CYMENU, L"SM_CYMENU", L"Menu bar height",
		SM_CXFULLSCREEN, L"SM_CXFULLSCREEN", L"Full screen client area width",
		SM_CYFULLSCREEN, L"SM_CYFULLSCREEN", L"Full screen client area height",
		SM_CYKANJIWINDOW, L"SM_CYKANJIWINDOW", L"Kanji window height",
		SM_MOUSEPRESENT, L"SM_MOUSEPRESENT", L"Mouse present flag",
		SM_CYVSCROLL, L"SM_CYVSCROLL", L"Vertical scroll arrow height",
		SM_CXHSCROLL, L"SM_CXHSCROLL", L"Horizontal scroll arrow width",
		SM_DEBUG, L"SM_DEBUG", L"Debug version flag",
		SM_SWAPBUTTON, L"SM_SWAPBUTTON", L"Mouse buttons swapped flag",
		SM_RESERVED1, L"SM_RESERVED1", L"Reserved",
		SM_RESERVED2, L"SM_RESERVED2", L"Reserved",
		SM_RESERVED3, L"SM_RESERVED3", L"Reserved",
		SM_RESERVED4, L"SM_RESERVED4", L"Reserved",
		SM_CXMIN, L"SM_CXMIN", L"Minimum window width",
		SM_CYMIN, L"SM_CYMIN", L"Minimum window height",
		SM_CXSIZE, L"SM_CXSIZE", L"Minimize/Maximize icon width",
		SM_CYSIZE, L"SM_CYSIZE", L"Minimize/Maximize icon height",
		SM_CXFRAME, L"SM_CXFRAME", L"Window frame width",
		SM_CYFRAME, L"SM_CYFRAME", L"Window frame height",
		SM_CXMINTRACK, L"SM_CXMINTRACK", L"Minimum window tracking width",
		SM_CYMINTRACK, L"SM_CYMINTRACK", L"Minimum window tracking height",
		SM_CXDOUBLECLK, L"SM_CXDOUBLECLK", L"Double click x tolerance",
		SM_CYDOUBLECLK, L"SM_CYDOUBLECLK", L"Double click y tolerance",
		SM_CXICONSPACING, L"SM_CXICONSPACING", L"Horizontal icon spacing",
		SM_CYICONSPACING, L"SM_CYICONSPACING", L"Vertical icon spacing",
		SM_MENUDROPALIGNMENT, L"SM_MENUDROPALIGNENT", L"Left or right menu drop",
		SM_PENWINDOWS, L"SM_PENWINDOWS", L"Pen extensions installed",
		SM_DBCSENABLED, L"SM_DBCSENABLED",L"Double-Byte Char Set enabled",
		SM_CMOUSEBUTTONS, L"SM_CMOUSEBUTTONS", L"Number of mouse buttons",
		SM_SHOWSOUNDS, L"SM_SHOWSOUNDS", L"Present sounds visually"
};