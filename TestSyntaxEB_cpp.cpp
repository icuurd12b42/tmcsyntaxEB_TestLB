// TestSyntaxEB_cpp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestSyntaxEB_cpp.h"
#include "LateBindingTmcSyntaxEB.h"
#include "resource.h"
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>
using namespace std;

#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE ghInst;                                // current instance
HWND ghWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EnterValueProc(HWND, UINT, WPARAM, LPARAM);

//Some test variables
TCHAR gTestTextValue[64000];
LPARAM gwParam = 0; //yeah, WPARAM is unsigned, this helps with keeping the occasional -1 values passed in wparam
LPARAM glParam = 0;
wstring glParamAsText = L"0";
wstring gInitialValue;

///https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start (in place)
static inline void ltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::wstring &s) {
	ltrim(s);
	rtrim(s);
}
void GetParam(const TCHAR * wpDesc, const TCHAR *lpDesc)
{
	gInitialValue = L"Value1(\r\n";
	gInitialValue += to_wstring( gwParam);
	gInitialValue += L"\r\n);";
	gInitialValue += wpDesc;
	gInitialValue += L"\r\n\r\nValue2(\r\n";
	gInitialValue += glParamAsText;
	gInitialValue += L"\r\n);";
	gInitialValue += lpDesc;
	DialogBox(ghInst, MAKEINTRESOURCE(IDD_ENTERVALUE), ghWnd, EnterValueProc);
	try {
		wstring TheWholeThing = gTestTextValue;
		size_t Val1Start = 9;
		size_t Val1End = TheWholeThing.find(L"\r\n);",Val1Start);
		size_t Val2Start = TheWholeThing.find(L"Value2(\r\n", Val1End) + 9;
		size_t Val2End = TheWholeThing.find(L"\r\n);", Val2Start);
		wstring Val1Str = TheWholeThing.substr(Val1Start, Val1End - Val1Start);
		wstring Val2Str = TheWholeThing.substr(Val2Start, Val2End - Val2Start);
		gwParam = stoi(Val1Str);
		glParamAsText = Val2Str;
		trim(glParamAsText);
		//this will fail if text
		glParam = stoi(Val2Str);

		

	}
	catch(...)
	{
		glParam = 0;
	}
}

void GetText(const TCHAR* t)
{
	gInitialValue = t;
	DialogBox(ghInst, MAKEINTRESOURCE(IDD_ENTERVALUE), ghWnd, EnterValueProc);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTSYNTAXEBCPP, szWindowClass, MAX_LOADSTRING);
	
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTSYNTAXEBCPP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTSYNTAXEBCPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTSYNTAXEBCPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   ghInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
#define ID_EDITCHILD 100
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ghWnd = hWnd;
	static HWND hwndEdit;
	static HFONT MyFont;
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindowEx(
			0, L"EDIT",   // predefined class 
			NULL,         // no window title 
			WS_CHILD | WS_VISIBLE |
			ES_LEFT,
			0, 0, 60, 30,   // set size in WM_SIZE message 
			hWnd,         // parent window 
			(HMENU)ID_EDITCHILD,   // edit control ID 
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);        // pointer not needed 
		hwndEdit = CreateWindowEx(
			0, L"EDIT",   // predefined class 
			NULL,         // no window title 
			WS_CHILD | WS_VISIBLE | 
			ES_LEFT |  ES_WANTRETURN | ES_NOHIDESEL | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | ES_AUTOHSCROLL,
			0, 0, 0, 0,   // set size in WM_SIZE message 
			hWnd,         // parent window 
			(HMENU)ID_EDITCHILD,   // edit control ID 
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);        // pointer not needed 

		//start off with a 12pt (16Pix) size courrier new font
		MyFont = CreateFont(-16, 0, 0, 0, FW_MEDIUM, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Courier New");
		//HFONT fnt = CreateFont(-16, 0, 0, 0, FW_MEDIUM, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Arial");
		SendMessage(hwndEdit, WM_SETFONT, (WPARAM)MyFont, 0);

		//THAT'S IT, ALL YOU NEED, you may want to set the EB limit higher and set the Hide Selection OFF, see the control messages
		TMC_SEB_LB_InitSyntaxModule();
		TMC_SEB_LB_LoadAllLanguageAndColorSpecFiles();
		//TMC_SEB_LB_SetHilite(hwndEdit,L"HKS + JScript 3.0 Syntax");
		///////////////

		return 0L;


	}

	case WM_COMMAND: //all the test messages and functions here
	{
		int wmId = LOWORD(wParam);
		// most the menu is testing if behaviour is identical to unhooked edit box. and some user message tests:
		switch (wmId)
		{

		case IDM_GETLANG:
		{
			MessageBox(GetActiveWindow(), TMC_SEB_LB_GetSelectedLanguage(), L"Get Language", 0);
			break;
		}
		case IDM_GETLANGA:
		{
			MessageBoxA(GetActiveWindow(), TMC_SEB_LB_GetSelectedLanguageA(), "Get LanguageA", 0);
			break;
		}
		case IDM_TEST_HOOKEDITBOX:
		{
			TMC_SEB_LB_SetHilite(hwndEdit, L"HKS + JScript 3.0 Syntax", L"Dark Night");
			break;
		}
		case IDM_TEST_UNHOOKEDITBOX:
		{
			TMC_SEB_LB_RemoveHilite(hwndEdit);
			break;
		}
		case IDM_TEST_WM_CLEAR:
		{
			SendMessage(hwndEdit, WM_CLEAR, 0, 0);
			break;
		}
		case IDM_TEST_WM_COPY:
		{
			SendMessage(hwndEdit, WM_COPY, 0, 0);
			break;
		}
		case IDM_TEST_WM_CUT:
		{
			SendMessage(hwndEdit, WM_CUT, 0, 0);
			break;
		}
		case IDM_TEST_WM_PASTE:
		{
			SendMessage(hwndEdit, WM_PASTE, 0, 0);
			break;
		}
		case IDM_TEST_WM_UNDO:
		{
			SendMessage(hwndEdit, WM_UNDO, 0, 0);
			break;
		}
		case IDM_TEST_EM_UNDO:
		{
			SendMessage(hwndEdit, EM_UNDO, 0, 0);
			break;
		}
		case IDM_TEST_UM_REDO:
		{
			SendMessage(hwndEdit, UM_REDO, 0, 0);
			break;
		}
		case IDM_TEST_EM_CANUNDO:
		{
			bool t = SendMessage(hwndEdit, EM_CANUNDO, 0, 0);
			stringstream ss;
			ss << "EM_CANUNDO: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_CANUNDO", 0);
			break;
		}
		case IDM_TEST_UM_CANREDO:
		{
			bool t = SendMessage(hwndEdit, UM_CANREDO, 0, 0);
			stringstream ss;
			ss << "UM_CANREDO: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_CANREDO", 0);
			break;
		}
		case IDM_TEST_EM_SETSEL_SELECT_ALL:
		{
			SendMessage(hwndEdit, EM_SETSEL, 0, -1);
			break;
		}
		case IDM_TEST_EM_SETSEL_SELECT_NONE:
		{
			SendMessage(hwndEdit, EM_SETSEL, -1, 0);
			break;
		}
		case IDM_TEST_UM_INDENT:
		{
			SendMessage(hwndEdit, UM_BLOCKINDENT, 1, 0);
			break;
		}
		case IDM_TEST_UM_UNINDENT:
		{
			SendMessage(hwndEdit, UM_BLOCKINDENT, 0, 0);
			break;
		}
		case IDM_TEST_UM_BLOCKCOMMENT:
		{
			SendMessage(hwndEdit, UM_BLOCKCOMMENT, 1, 0);
			break;
		}
		case IDM_TEST_UM_BLOCKUNCOMMENT:
		{
			SendMessage(hwndEdit, UM_BLOCKCOMMENT, 0, 0);
			break;
		}
		case IDM_TEST_UM_MATCHBRACKET:
		{
			SendMessage(hwndEdit, UM_MATCHBRACKET, -1, 0);
			break;
		}
		case IDM_TEST_UM_SETHILIGHT_ON:
		{
			SendMessage(hwndEdit, UM_SETHILIGHT, 1, 0);
			break;
		}
		case IDM_TEST_UM_SETHILIGHT_OFF:
		{
			SendMessage(hwndEdit, UM_SETHILIGHT, 0, 0);
			break;
		}
		case IDM_TEST_UM_GETHILIGHT:
		{
			SendMessage(hwndEdit, UM_GETHILIGHT, 0, 0);
			break;
		}
		case IDM_TEST_UM_SETLINEMARGIN_ON:
		{
			SendMessage(hwndEdit, UM_SETLINEMARGIN, 1, 0);
			break;
		}
		case IDM_TEST_UM_SETLINEMARGIN_OFF:
		{
			SendMessage(hwndEdit, UM_SETLINEMARGIN, 0, 0);
			break;
		}
		case IDM_TEST_UM_GETLINEMARGIN:
		{
			size_t t = SendMessage(hwndEdit, UM_GETLINEMARGIN, 0, 0);
			stringstream ss;
			ss << "UM_GETLINEMARGIN: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_GETLINEMARGIN", 0);
			break;
		}
		case IDM_TEST_UM_SEARCHSEL:
		{
			GetParam(L"1 forward, -1 backward", L"0 Any, 1 Sensitive, 2 Whole Word, 3 Sensitive Whole Word");
			SendMessage(hwndEdit, UM_SEARCHSEL, gwParam, glParam);
			break;
		}
		case IDM_TEST_UM_CANSEARCHSEL:
		{
			size_t t = SendMessage(hwndEdit, UM_CANSEARCHSEL, 0, 0);
			stringstream ss;
			ss << "UM_CANSEARCHSEL: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_CANSEARCHSEL", 0);
			break;
		}
		case IDM_TEST_EM_CHARFROMPOS:
		{
			GetParam(L"x coord", L"y coord");
			size_t t = SendMessage(hwndEdit, EM_CHARFROMPOS, 0, MAKELPARAM(gwParam, glParam));
			stringstream ss;
			ss << "EM_CHARFROMPOS: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_CHARFROMPOS", 0);
			break;
		}
		case IDM_TEST_EM_EMPTYUNDOBUFFER:
		{
			SendMessage(hwndEdit, EM_EMPTYUNDOBUFFER, 0, 0);
			break;
		}
		case IDM_TEST_EM_FMTLINES_TRUE:
		{
			SendMessage(hwndEdit, EM_FMTLINES, 1, 0);
			break;
		}
		case IDM_TEST_EM_FMTLINES_FALSE:
		{
			SendMessage(hwndEdit, EM_FMTLINES, 0, 0);
			break;
		}

		case IDM_TEST_EM_GETFIRSTVISIBLELINE:
		{
			size_t t = SendMessage(hwndEdit, EM_GETFIRSTVISIBLELINE, 0, 0);
			stringstream ss;
			ss << "EM_GETFIRSTVISIBLELINE: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_GETFIRSTVISIBLELINE", 0);
			break;
		}
		case IDM_TEST_EM_GETLIMITTEXT:
		{
			size_t t = SendMessage(hwndEdit, EM_GETLIMITTEXT, 0, 0);
			stringstream ss;
			ss << "EM_GETLIMITTEXT: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_GETLIMITTEXT", 0);
			break;
		}
		case IDM_TEST_EM_GETLINE:
		{
			GetParam(L"Line Number", L"Not Used");
			WORD *size = (WORD *)gTestTextValue;
			*size = 64000;
			size_t t = SendMessage(hwndEdit, EM_GETLINE, gwParam, (LPARAM)gTestTextValue);
			gTestTextValue[t] = 0; //must null terminate the bufefer according to returned size
			MessageBox(GetActiveWindow(), gTestTextValue, L"EM_GETLINE", 0);
			break;
		}
		case IDM_TEST_EM_GETLINECOUNT:
		{
			size_t t = SendMessage(hwndEdit, EM_GETLINECOUNT, 0, 0);
			stringstream ss;
			ss << "EM_GETLINECOUNT: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_GETLINECOUNT", 0);
			break;
		}
		case IDM_TEST_EM_GETMARGINS:
		{
			size_t t = SendMessage(hwndEdit, EM_GETMARGINS, 0, 0);
			stringstream ss;
			ss << "EM_POSFROMCHAR: Left,Right Margins (" << (short)LOWORD(t) << "," << (short)HIWORD(t) << ")";
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_GETMARGINS", 0);
			break;
		}
		case IDM_TEST_EM_GETMODIFY:
		{
			size_t t = SendMessage(hwndEdit, EM_GETMODIFY, 0, 0);
			stringstream ss;
			ss << "EM_GETMODIFY: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_GETMODIFY", 0);
			break;
		}
		case IDM_TEST_EM_GETSEL:
		{
			DWORD SelStart, SelEnd;
			size_t t = SendMessage(hwndEdit, EM_GETSEL, (WPARAM)&SelStart, (LPARAM)&SelEnd);
			stringstream ss;
			ss << "EM_GETSEL: " << t << " Start: " << SelStart << " End: " << SelEnd;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_GETSEL", 0);
			break;
		}
		case IDM_TEST_EM_GETTHUMB:
		{
			size_t t = SendMessage(hwndEdit, EM_GETTHUMB, 0, 0);
			stringstream ss;
			ss << "EM_GETTHUMB: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_GETTHUMB", 0);
			break;
		}
		case IDM_TEST_EM_LIMITTEXT:
		{
			GetParam(L"Max Characters", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_LIMITTEXT, gwParam, 0);
			break;
		}
		case IDM_TEST_EM_LINEFROMCHAR:
		{
			GetParam(L"Char Index or -1", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_LINEFROMCHAR, gwParam, 0);
			stringstream ss;
			ss << "EM_LINEFROMCHAR: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_LINEFROMCHAR", 0);
			break;
		}
		case IDM_TEST_EM_LINEINDEX:
		{
			GetParam(L"Line Number or -1", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_LINEINDEX, gwParam, 0);
			stringstream ss;
			ss << "EM_LINEINDEX Char Inder: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_LINEINDEX", 0);
			break;
		}
		case IDM_TEST_EM_LINELENGTH:
		{
			GetParam(L"CHARACTER INDEX, NOT LINE NUMBER or -1 for Carret Position", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_LINELENGTH, (WPARAM)gwParam, 0);
			stringstream ss;
			ss << "EM_LINELENGTH: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_LINELENGTH", 0);
			break;
		}
		case IDM_TEST_EM_LINESCROLL:
		{
			GetParam(L"+-CharHScroll", L"+-LineVScroll");
			size_t t = SendMessage(hwndEdit, EM_LINESCROLL, gwParam, glParam);

			break;
		}
		case IDM_TEST_EM_POSFROMCHAR:
		{
			GetParam(L"Char Index", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_POSFROMCHAR, gwParam, 0);
			stringstream ss;
			ss << "EM_POSFROMCHAR: Pos(" << (short)LOWORD(t) << "," << (short)HIWORD(t) << ")";
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "EM_POSFROMCHAR", 0);
			break;
		}
		case IDM_TEST_EM_REPLACESEL:
		{
			GetParam(L"Change is Undoable? 0 No, 1 Yes", L"Not Used");
			GetText(L"Text To Use");
			SendMessage(hwndEdit, EM_REPLACESEL, gwParam, (LPARAM)gTestTextValue);

			break;
		}

		case IDM_TEST_EM_SCROLL:
		{
			GetParam(L"0 to 8", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_SCROLL, gwParam, 0);

			break;
		}
		case IDM_TEST_EM_SCROLLCARET:
		{
			size_t t = SendMessage(hwndEdit, EM_SCROLLCARET, gwParam, 0);

			break;
		}
		case IDM_TEST_EM_SETLIMITTEXT:
		{
			GetParam(L"Max Text Limit", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_SETLIMITTEXT, gwParam, 0);
			break;
		}
		case IDM_TEST_EM_SETMARGINS:
		{
			GetParam(L"Left Margin", L"Right Margin");
			size_t t = SendMessage(hwndEdit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(gwParam, glParam));
			break;
		}
		case IDM_TEST_EM_SETMODIFY:
		{
			GetParam(L"0 Clear, 1 Set Modify", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_SETMODIFY, gwParam, 0);
			break;
		}
		case IDM_TEST_EM_SETREADONLY:
		{
			GetParam(L"0 Read Write, 1 Read Only", L"Not Used");
			size_t t = SendMessage(hwndEdit, EM_SETREADONLY, gwParam, 0);
			break;
		}
		case IDM_TEST_EM_SETSEL:
		{
			GetParam(L"Char Index SelStart -1 Deselect", L"Char Index SelEnd -1 Select All");
			size_t t = SendMessage(hwndEdit, EM_SETSEL, gwParam, glParam);
			break;
		}
		case IDM_TEST_WM_SETTEXT:
		{
			GetText(L"Text to set");
			size_t t = SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)gTestTextValue);
			break;
		}
		case IDM_TEST_WM_GETTEXT:
		{
			SendMessage(hwndEdit, WM_GETTEXT, 64000, (LPARAM)gTestTextValue);

			MessageBox(GetActiveWindow(), gTestTextValue, L"WM_GETTEXT", 0);
			break;
		}
		case IDM_TEST_SETWINDOWTEXT:
		{
			GetText(L"Text to set");
			SetWindowText(hwndEdit, gTestTextValue);
			break;
		}
		case IDM_TEST_GETWINDOWTEXT:
		{
			GetWindowText(hwndEdit, gTestTextValue, 64000);

			MessageBox(GetActiveWindow(), gTestTextValue, L"GetWindowText", 0);
			break;
		}
		case IDM_TEST_ENABLEWINDOW:
		{
			GetParam(L"Enable 1, Disabled 0", L"Not Used");
			EnableWindow(hwndEdit, (BOOL)gwParam);
			break;
		}
		case IDM_TEST_UM_CHARFROMPOS64k:
		{
			GetParam(L"XCoord Relative To Box", L"YCoord Relative To Box");
			size_t t = SendMessage(hwndEdit, UM_CHARFROMPOS64k, gwParam, glParam);
			stringstream ss;
			ss << "UM_CHARFROMPOS64k: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_CHARFROMPOS64k", 0);
			break;
		}
		case IDM_TEST_UM_SETNOHIDESEL:
		{
			GetParam(L"0 - Hide, 1 - Don't Hide Selection When Not Focussed", L"Not Used");
			SendMessage(hwndEdit, UM_SETNOHIDESEL, gwParam, 0);
			break;
		}
		case IDM_TEST_UM_SETINSERTMODE:
		{
			GetParam(L"0 - Overwrite, 1 - Insert", L"Not Used");
			SendMessage(hwndEdit, UM_SETINSERTMODE, gwParam, 0);
			break;
		}
		case IDM_TEST_UM_GETINSERTMODE:
		{
			size_t t = SendMessage(hwndEdit, UM_GETINSERTMODE, 0, 0);
			stringstream ss;
			ss << "UM_GETINSERTMODE: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_GETINSERTMODE", 0);
			break;
		}
		case IDM_TEST_UM_GETFONTSIZE:
		{
			GetParam(L"0 - In Pixel (Height), 1 - Int Pt", L"Not Used");
			size_t t = SendMessage(hwndEdit, UM_GETFONTSIZE, gwParam, 0);
			stringstream ss;
			ss << "UM_GETFONTSIZE: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_GETFONTSIZE", 0);
			break;
		}
		case IDM_TEST_UM_SETFONTSIZE:
		{
			GetParam(L"0 - In Pixel (Height), 1 - Int Pt", L"Size");
			size_t t = SendMessage(hwndEdit, UM_SETFONTSIZE, gwParam, glParam);
			stringstream ss;
			ss << "UM_SETFONTSIZE: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_SETFONTSIZE", 0);
			break;
		}
		case IDM_TEST_UM_ZOOMFONT:
		{
			GetParam(L"1 Zomm in, -1 Zoon Out", L"0 - Return Value in height, 0 - Return Values is in Pt");
			size_t t = SendMessage(hwndEdit, UM_ZOOMFONT, gwParam, glParam);
			stringstream ss;
			ss << "UM_ZOOMFONT: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_ZOOMFONT", 0);
			break;
		}
		case IDM_TEST_WM_SETFONT:
		{
			GetParam(L"Font Size (Height In Pixels)", L"Font Name, dont use quotes");
			//get current one
			HFONT CurrentFont = (HFONT)SendMessage(hwndEdit, WM_GETFONT, 0, 0);
			HFONT NewFont = CreateFont(-gwParam, 0, 0, 0, FW_MEDIUM, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, glParamAsText.c_str());
			//HFONT fnt = CreateFont(-16, 0, 0, 0, FW_MEDIUM, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Arial");
			//success with new font
			if (NewFont)
			{
				//set it
				SendMessage(hwndEdit, WM_SETFONT, (WPARAM)NewFont, 1);
				//cleanup if old is mine and not system font
				if (CurrentFont != 0 && CurrentFont == MyFont)
					DeleteObject(MyFont);
				MyFont = NewFont;
			}
			break;
		}
		case IDM_TEST_UM_SHOWSEARCHREPLACE:
		{
			GetParam(L"0-Search, 1-Replace", L"0-Hide, 1-Show");
			//get current one
			SendMessage(hwndEdit, UM_SHOWSEARCHREPLACE, gwParam, glParam);
			
			break;
		}
		case IDM_TEST_UM_SETSCROLLBAR:
		{
			GetParam(L"0-VScrollBar, 1-HScrollBar", L"0-Off, 1-On");
			SendMessage(hwndEdit, UM_SETSCROLLBAR, gwParam, glParam);
			
			break;
		}
		case IDM_TEST_UM_GETSCROLLBAR:
		{
			GetParam(L"0-VScrollBar, 1-HScrollBar", L"Not Used");
			size_t t = SendMessage(hwndEdit, UM_GETSCROLLBAR, gwParam, 0);
			stringstream ss;
			ss << "UM_GETSCROLLBAR: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_GETSCROLLBAR", 0);
			break;
		}
		case IDM_TEST_UM_GETWORDWRAP:
		{
			size_t t = SendMessage(hwndEdit, UM_GETWORDWRAP, 0, 0);
			stringstream ss;
			ss << "UM_GETWORDWRAP: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_GETWORDWRAP", 0);
			break;
		}
		case IDM_TEST_UM_SETWORDWRAP:
		{
			GetParam(L"0-Off, 1-On", L"Not Used\r\n\r\nWARNING THIS FEATURE DOES NOT WORK\r\nTHE UM_GETWORDWARP EQUIVALENT VALUE\r\nIS ALSO MEANINGLESS ");
			SendMessage(hwndEdit, UM_SETWORDWRAP, gwParam, 0);
			
			break;
		}
		case IDM_TEST_UM_GETNOHIDESEL:
		{
			size_t t = SendMessage(hwndEdit, UM_GETNOHIDESEL, 0, 0);
			stringstream ss;
			ss << "UM_GETNOTHIDESEL: " << t;
			MessageBoxA(GetActiveWindow(), ss.str().c_str(), "UM_GETNOTHIDESEL", 0);
			break;
		}
		case IDM_TEST_UM_PERFORMAUTOINDENT:
		{
			GetParam(L"0-K&R, 1-Allman, 2-GNU, 3-Whitesmiths, 4-Horstman, 5-Pico, 6-Ratliff, 7-Lisp", L"Not Used\r\n\r\n");
			SendMessage(hwndEdit, UM_PERFORMAUTOINDENT, gwParam, 0);

			break;
		}
		case IDM_EXIT:
		{
			DestroyWindow(hWnd);
			if (MyFont != 0)
			{
				DeleteObject(MyFont);
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
	{
		TMC_SEB_LB_CleanUp();
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE:
	{
		TMC_SEB_LB_RemoveHilite(hwndEdit);
		PostQuitMessage(0);
		break;
	}
	case WM_SETFOCUS:
	{
		SetFocus(hwndEdit);
		return 0;
	}
	case WM_SIZE:
	{
		// Make the edit control the size of the window's client area. 

		MoveWindow(hwndEdit,
			100, 0,                  // starting x- and y-coordinates 
			LOWORD(lParam) - 100,        // width of client area 
			HIWORD(lParam),        // height of client area 
			TRUE);                 // repaint window 
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}

	return 0;

}

// Message handler for about box.
INT_PTR CALLBACK EnterValueProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
	{
		SetDlgItemText(hDlg, IDC_TEXT_VALUE, gInitialValue.c_str());
		return (INT_PTR)TRUE;
	}
    case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			GetDlgItemText(hDlg, IDC_TEXT_VALUE, gTestTextValue, 64000);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
    }
    return (INT_PTR)FALSE;
}
