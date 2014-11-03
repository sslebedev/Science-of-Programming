#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include <windows.h>

#include "resource.h"
#include "sl_text.h"

#define WM_START_BY_PARAMS (WM_USER + 0x0001)

// Window class name
static const char szClassName[] = "WC_T1";

// Windowed application supportive data
static struct WindowData
{
    int posVScroll; // vertical scroll position
    int posHScroll; // horisontal scroll position

    int wFontChar;  // fixed font letter meric - width
    int hFontChar;  // fixed font letter meric - heigth

    SCROLLINFO hSbInfo; // horz scrollbar SCROLLINFO - usually constant settings
    SCROLLINFO vSbInfo; // vert scrollbar SCROLLINFO - usually constant settings

    BOOL isLineWrapMode; // line wrap mode flag

    int curLine;        // in line wrap mode: current SOURCE line index
    int curLineStart;   // in line wrap mode: begining of current VIRTUAL line on the curLine

    int width;          // client rect width
    int heigth;         // client rect heith
    int nPageCols;      // for fixed fond: client rect column symbol metric
    int nPageRows;      // for fixed fond: client rect row symbol metric

    HWND   hdlg;          // modeless initial dialog
    char dFilename[260];  // dlg filename
    BOOL   dIsLineWrap;   // dlg is line wrap
} windowData;

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   BOOL CALLBACK
// Parameter: HWND hdlg
// Parameter: UINT message
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
BOOL CALLBACK StartupDlgProc( HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    LPSTR cmd;
    OPENFILENAME ofn;
    
    switch (message)
    {
    case WM_INITDIALOG:
        cmd = (LPSTR)lParam;
        if (strlen(cmd) > 3 && cmd[0] == '-' && cmd[1] == 'w' && cmd[2] == ' ') {
            windowData.dIsLineWrap = TRUE;
            strcpy(windowData.dFilename, cmd + 3);
        } else {
            windowData.dIsLineWrap = FALSE;
            strcpy(windowData.dFilename, cmd);
        }
        SetDlgItemText(hdlg, IDC_CMD, cmd);
        SendDlgItemMessage(hdlg, IDC_LINEWRAP, BM_SETCHECK, windowData.dIsLineWrap ? BST_CHECKED : BST_UNCHECKED, 0);
        SetDlgItemText(hdlg, IDC_FILENAME, windowData.dFilename);
        return TRUE;
    case WM_CLOSE:
        DestroyWindow(hdlg);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_OPEN:
            // open a standard dialog to get filename
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hdlg;
            ofn.lpstrFile = windowData.dFilename;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(windowData.dFilename);
            ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
            GetOpenFileName(&ofn);
            SetDlgItemText(hdlg, IDC_FILENAME, windowData.dFilename);
            return TRUE;
        case IDC_LINEWRAP:
            windowData.dIsLineWrap = !windowData.dIsLineWrap;
            SendDlgItemMessage(hdlg, IDC_LINEWRAP, BM_SETCHECK, windowData.dIsLineWrap ? BST_CHECKED : BST_UNCHECKED, 0);
            return TRUE;
        case IDC_OK:
            SendMessage(GetParent(hdlg), WM_START_BY_PARAMS, 0, 0);
            DestroyWindow(hdlg);
            return TRUE;
        }
   }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

//************************************
// Returns:   int WINAPI
// Parameter: HINSTANCE hThisInstance
// Parameter: HINSTANCE hPrevInstance
// Parameter: LPSTR lpszArgument
// Parameter: int nCmdShow
// Throws:    none
//************************************
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd, hdlg;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS | CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = (LPCSTR)"MainMenu";
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
        0,
        szClassName,
        "Code::Blocks Template Windows App",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        lpszArgument
        );

    DrawMenuBar(hwnd);
    ShowWindow (hwnd, nCmdShow);


    hdlg = CreateDialogParam(hThisInstance, "Startup_Dlg", hwnd, StartupDlgProc, (LPARAM)lpszArgument);
    ShowWindow(hdlg, SW_SHOW);
    windowData.hdlg = hdlg;

    while (GetMessage (&messages, NULL, 0, 0)) 
    { 
        if (!IsWindow(hdlg) || !IsDialogMessage(hdlg, &messages)) 
        { 
            TranslateMessage(&messages); 
            DispatchMessage(&messages); 
        } 
    }

    return messages.wParam;
}

//////////////////////////////////////////////////////////////////////////

LRESULT OnCreate        ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnDestroy       ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnPaint         ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnSize          ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnVScroll       ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnHScroll       ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnKeydown       ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnMouseWheel    ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnCommand       ( HWND hwnd, WPARAM wParam, LPARAM lParam );
LRESULT OnStartByParams ( HWND hwnd, WPARAM wParam, LPARAM lParam );

//************************************
// Returns:   LRESULT CALLBACK
// Parameter: HWND hwnd
// Parameter: UINT message
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Current state
    switch (message)
    {
    case WM_CREATE:
        return OnCreate(hwnd, wParam, lParam);
    case WM_DESTROY:
        return OnDestroy(hwnd, wParam, lParam);
    case WM_PAINT:
        return OnPaint(hwnd, wParam, lParam);
    case WM_SIZE:
        return OnSize(hwnd, wParam, lParam);
    case WM_VSCROLL:
        return OnVScroll(hwnd, wParam, lParam);
    case WM_HSCROLL:
        return OnHScroll(hwnd, wParam, lParam);
    case WM_KEYDOWN:
        return OnKeydown(hwnd, wParam, lParam);
    case WM_MOUSEWHEEL:
        return OnMouseWheel(hwnd, wParam, lParam);
    case WM_COMMAND:
        return OnCommand(hwnd, wParam, lParam);
    case WM_START_BY_PARAMS:
        return OnStartByParams(hwnd, wParam, lParam);
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
}

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   void
// Parameter: LPCSTR filename
// Throws:    none
//************************************
void InitFileByName( LPCSTR filename )
{
    FILE *file;

    // try to open file
    file = fopen(filename, "rb");

    // no file: do nothing
    if (!file) {
        return;
    }

    // deinit old text
    DeinitText(); // safe to be not initialized
    // file found: init text
    InitText(file);

    // no need file anymore
    fclose(file);
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void InitFileByDialog( HWND hwnd )
{
    OPENFILENAME ofn;
    char         filename[260];
    FILE         *file;

    // open a standard dialog to get filename
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = filename;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
    GetOpenFileName(&ofn);

    // try to open
    file = fopen(filename, "rb");
    if (!file) {
        return;
    }

    // deinit old text
    DeinitText(); // safe to be not initialized
    // init new text
    InitText(file);

    fclose(file);
}

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void SetupDC( HWND hwnd )
{
    HDC hDC;
    HFONT hfont;
    TEXTMETRIC textMetric;

    hDC = GetDC(hwnd);

    // Setup font
    hfont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
    SelectObject(hDC, hfont);
    GetTextMetrics(hDC, &textMetric);
    // - save font metrics
    windowData.wFontChar = textMetric.tmAveCharWidth;
    windowData.hFontChar = textMetric.tmHeight;

    ReleaseDC(hwnd, hDC);
}

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnCreate( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    // CREATESTRUCT *cs;

    // Open file from commandline if possible
    // cs = (CREATESTRUCT*)lParam;
    // InitFileByCreateStruct(cs); - moved to dialog, for some logic

    // Setup DC params, expected CS_CLASSDC or CS_OWNDC
    SetupDC(hwnd);

    // Line wrap is disabled by default
    windowData.isLineWrapMode = FALSE;

    // Fill stable SCROLLINFO params
    windowData.hSbInfo.cbSize = sizeof(windowData.hSbInfo);
    windowData.hSbInfo.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
    windowData.hSbInfo.nMin  = 0;
    windowData.hSbInfo.nPos  = windowData.posHScroll = 0;
    windowData.hSbInfo.nPage = 0;
    windowData.hSbInfo.nMax  = 0;
    SetScrollInfo (hwnd, SB_HORZ, &windowData.hSbInfo, TRUE);

    windowData.vSbInfo.cbSize = sizeof(windowData.vSbInfo);
    windowData.vSbInfo.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
    windowData.vSbInfo.nMin  = 0;
    windowData.vSbInfo.nPos  = windowData.posHScroll = 0;
    windowData.vSbInfo.nPage = 0;
    windowData.vSbInfo.nMax  = 0;
    SetScrollInfo (hwnd, SB_VERT, &windowData.vSbInfo, TRUE);

    return 0;
}

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnDestroy( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    PostQuitMessage(0);
    DeinitText();

    return 0;
}

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   void
// Parameter: HDC hDC
// Throws:    none
//************************************
void WriteText( HDC hDC )
{
    char *buf;
    int  bufLen;
    Line *lines;
    int  linesLength;
    int  maxLength;
    int  i;

    // Get to text data
    GetContent(&buf, &bufLen);
    GetLines(&lines, &linesLength, &maxLength);

    // no text - do nothing
    if (!buf) {
        return;
    }

    if (windowData.isLineWrapMode) {
        int curLine = windowData.curLine;
        int curLineStart = windowData.curLineStart;

        // find begining, specifyed by PrepareCurLine function
        for (i = 0; i < min(windowData.nPageRows, windowData.vSbInfo.nMax); ++i) {
            // not this line - go next text line
            if (curLineStart > GetLineLegth(curLine)) {
                ++curLine;
                curLineStart = 0;
            }
            // print this virtual line
            TextOut(hDC, 0, i * windowData.hFontChar, buf + lines[curLine].start + curLineStart, max(GetLineLegth(curLine) - curLineStart, 0));
            // advance virtual line
            curLineStart += windowData.nPageCols + ((windowData.width % windowData.wFontChar) > 0);
        }
    } else {
        // first line not need to be counted by curLine
        int iFirstLine = windowData.posVScroll;

        for (i = iFirstLine; i < min(iFirstLine + windowData.nPageRows, linesLength); ++i) {
            // leftest line symbol
            int shift = windowData.posHScroll;
            // print
            TextOut(hDC, 0, (i - iFirstLine) * windowData.hFontChar, buf + lines[i].start + shift, max(GetLineLegth(i) - shift, 0));
        }
    }
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnPaint( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT PaintStruct;
    HDC hDC;

    hDC = BeginPaint(hwnd, &PaintStruct);

    WriteText(hDC);

    EndPaint(hwnd, &PaintStruct);

    return 0;
}

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   void
// Throws:    none
//************************************
void PrepareCurLine()
{
    int i, j;
    int maxLength, linesLength;
    Line *lines;

    // not used only in line wrap mode
    if (!windowData.isLineWrapMode) {
        return;
    }

    // Get to text data
    GetLines(&lines, &linesLength, &maxLength);

    // Windows notepad like
    // supports line nuber by constant VSCROLL pos as resize invariant
    // garanties return to previous state when resized back
    j = 0;
    for (i = 0; i < linesLength; ++i) {
        int delta = GetLineLegth(i) / windowData.nPageCols + (GetLineLegth(i) % windowData.nPageCols > 0);

        if (j <= windowData.posVScroll && windowData.posVScroll < j + delta) {
            // curLine and curLineStart as a result
            windowData.curLine      = i;
            windowData.curLineStart = (windowData.posVScroll - j) * windowData.nPageCols;
            break;
        }
        j += delta;
    }
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void ApplyResize( HWND hwnd )
{
    int maxLength, linesLength;
    Line *lines;
    RECT clientRect;
    int i;

    GetClientRect(hwnd, &clientRect);
    // save useful metrics and recount caps
    windowData.width  = clientRect.right - clientRect.left;
    windowData.heigth = clientRect.bottom - clientRect.top;
    windowData.nPageCols  = windowData.width  / windowData.wFontChar;
    windowData.nPageRows = windowData.heigth / windowData.hFontChar;

    // get to text data
    GetLines(&lines, &linesLength, &maxLength);

    // no text, fill dummy and return
    if (lines == NULL) {
        // - set dummy info
        windowData.hSbInfo.nPos  = windowData.posHScroll = 0;
        windowData.hSbInfo.nPage = 0;
        windowData.hSbInfo.nMax  = 0;
        SetScrollInfo (hwnd, SB_HORZ, &windowData.hSbInfo, TRUE);

        windowData.vSbInfo.nPos  = windowData.posHScroll = 0;
        windowData.vSbInfo.nPage = 0;
        windowData.vSbInfo.nMax  = 0;
        SetScrollInfo (hwnd, SB_VERT, &windowData.vSbInfo, TRUE);

        // save updated positions
        windowData.posVScroll = GetScrollPos(hwnd, SB_VERT);
        windowData.posHScroll = GetScrollPos(hwnd, SB_HORZ);

        return;
    }


    if (windowData.isLineWrapMode) {
        // count virtual lines
        int countLines = 0;
        for (i = 0; i < linesLength; ++i) {
            countLines += GetLineLegth(i) / windowData.nPageCols + (GetLineLegth(i) % windowData.nPageCols > 0);
        }

        // set updated info
        windowData.hSbInfo.nPos  = windowData.posHScroll = 0;
        windowData.hSbInfo.nPage = 0;
        windowData.hSbInfo.nMax  = 0;
        SetScrollInfo (hwnd, SB_HORZ, &windowData.hSbInfo, TRUE);

        windowData.vSbInfo.nPos  = windowData.posVScroll;
        windowData.vSbInfo.nPage = windowData.nPageRows;
        windowData.vSbInfo.nMax  = countLines;
        SetScrollInfo (hwnd, SB_VERT, &windowData.vSbInfo, TRUE);

        // save updated positions
        windowData.posVScroll = GetScrollPos(hwnd, SB_VERT);
        windowData.posHScroll = GetScrollPos(hwnd, SB_HORZ);

        // recount curLine and curLineStart
        PrepareCurLine();
    } else {
        // set updated info
        windowData.hSbInfo.nPos  = windowData.posHScroll;
        windowData.hSbInfo.nPage = windowData.nPageCols;
        windowData.hSbInfo.nMax  = maxLength;
        SetScrollInfo (hwnd, SB_HORZ, &windowData.hSbInfo, TRUE);

        windowData.vSbInfo.nPos  = windowData.posVScroll;
        windowData.vSbInfo.nPage = windowData.nPageRows;
        windowData.vSbInfo.nMax  = linesLength;
        SetScrollInfo (hwnd, SB_VERT, &windowData.vSbInfo, TRUE);

        // save updated positions
        windowData.posVScroll = GetScrollPos(hwnd, SB_VERT);
        windowData.posHScroll = GetScrollPos(hwnd, SB_HORZ);
    }
}



//************************************
// Returns:   void
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnSize( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    ApplyResize(hwnd);

    return DefWindowProc(hwnd, WM_SIZE, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void ScrollLeft( HWND hwnd )
{
    if (windowData.posHScroll > 0) {
        --windowData.posHScroll;
        SetScrollPos(hwnd, SB_HORZ, windowData.posHScroll, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void ScrollRight( HWND hwnd )
{
    if (windowData.posHScroll < windowData.hSbInfo.nMax - (int)windowData.hSbInfo.nPage + 1) {
        ++windowData.posHScroll;
        SetScrollPos(hwnd, SB_HORZ, windowData.posHScroll, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void ScrollUp( HWND hwnd )
{
    if (windowData.posVScroll > 0) {
        --windowData.posVScroll;
        SetScrollPos(hwnd, SB_VERT, windowData.posVScroll, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void ScrollDown( HWND hwnd )
{
    if (windowData.posVScroll < windowData.vSbInfo.nMax - (int)windowData.vSbInfo.nPage + 1) {
        ++windowData.posVScroll;
        SetScrollPos(hwnd, SB_VERT, windowData.posVScroll, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnVScroll( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    switch (LOWORD(wParam))
    {
    case SB_LINEUP:
        ScrollUp(hwnd);
        PrepareCurLine();
        break;
    case SB_LINEDOWN:
        ScrollDown(hwnd);
        PrepareCurLine();
        break;
    case SB_THUMBTRACK:
        windowData.posVScroll = HIWORD(wParam);
        SetScrollPos(hwnd, SB_VERT, windowData.posVScroll, TRUE);
        PrepareCurLine();
        InvalidateRect(hwnd, NULL, TRUE);
    default:
        break;
    }

    return 0;
}

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnHScroll( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    switch (LOWORD(wParam))
    {
    case SB_LINEUP:
        ScrollLeft(hwnd);
        PrepareCurLine();
        break;
    case SB_LINEDOWN:
        ScrollRight(hwnd);
        PrepareCurLine();
        break;
    case SB_THUMBTRACK:
        windowData.posHScroll = HIWORD(wParam);
        SetScrollPos(hwnd, SB_HORZ, windowData.posHScroll, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
        PrepareCurLine();
    default:
        break;
    }

    return 0;
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void PageUp( HWND hwnd )
{
    int  nPageLines;

    nPageLines = (int)windowData.vSbInfo.nPage;

    while (nPageLines > 0)
    {
        ScrollUp(hwnd);
        --nPageLines;
    }
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void PageDown( HWND hwnd )
{
    int  nPageLines;

    nPageLines = (int)windowData.vSbInfo.nPage;

    while (nPageLines > 0)
    {
        ScrollDown(hwnd);
        --nPageLines;
    }
}

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnKeydown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    switch (wParam)
    {
    case VK_LEFT:
        ScrollLeft(hwnd);
        PrepareCurLine();
        break;
    case VK_RIGHT:
        ScrollRight(hwnd);
        PrepareCurLine();
        break;
    case VK_UP:
        ScrollUp(hwnd);
        PrepareCurLine();
        break;
    case VK_DOWN:
        ScrollDown(hwnd);
        PrepareCurLine();
        break;
    case VK_PRIOR:
        PageUp(hwnd);
        PrepareCurLine();
        break;
    case VK_NEXT:
        PageDown(hwnd);
        PrepareCurLine();
        break;
    default:
        break;
    }

    return 0;
}

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnMouseWheel ( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

    if (zDelta > 0) {
        ScrollUp(hwnd);
        PrepareCurLine();
    } else {
        ScrollDown(hwnd);
        PrepareCurLine();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

void OnCommandOpen     ( HWND hwnd );
void OnCommandLineWrap ( HWND hwnd );
void OnCommandExit     ( HWND hwnd );

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnCommand ( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    switch (wParam)
    {
    case IDM_OPEN:
        if (windowData.hdlg) {
            DestroyWindow(windowData.hdlg);
            windowData.hdlg = NULL;
        }
        OnCommandOpen(hwnd);
        break;
    case IDM_LINEWRAP:
        if (windowData.hdlg) {
            DestroyWindow(windowData.hdlg);
            windowData.hdlg = NULL;
        }
        OnCommandLineWrap(hwnd);
        break;
    case IDM_EXIT:
        OnCommandExit(hwnd);
        break;
    default:
        break;
    }

    return 0;
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void OnCommandOpen( HWND hwnd )
{
    InitFileByDialog(hwnd);

    // reset scroll positions
    SetScrollPos(hwnd, SB_HORZ, windowData.posHScroll = 0, 0);
    SetScrollPos(hwnd, SB_VERT, windowData.posVScroll = 0, 0);

    // scroll correction
    ApplyResize(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void OnCommandLineWrap( HWND hwnd )
{
    HMENU hMenu;

    // Invert mode
    windowData.isLineWrapMode = !windowData.isLineWrapMode;

    // Set menu item checked/unchecked by mode
    hMenu = GetMenu(hwnd);
    CheckMenuItem(hMenu, IDM_LINEWRAP, windowData.isLineWrapMode ? MF_CHECKED : MF_UNCHECKED);

    DrawMenuBar(hwnd);
    ApplyResize(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);
}

//************************************
// Returns:   void
// Parameter: HWND hwnd
// Throws:    none
//************************************
void OnCommandExit( HWND hwnd )
{
    PostMessage(hwnd, WM_CLOSE, 0, 0);
}

//////////////////////////////////////////////////////////////////////////

//************************************
// Returns:   LRESULT
// Parameter: HWND hwnd
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
// Throws:    none
//************************************
LRESULT OnStartByParams( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
    HMENU hMenu;

    windowData.isLineWrapMode = windowData.dIsLineWrap;
    
    // Set menu item checked/unchecked by mode
    hMenu = GetMenu(hwnd);
    CheckMenuItem(hMenu, IDM_LINEWRAP, windowData.isLineWrapMode ? MF_CHECKED : MF_UNCHECKED);
    DrawMenuBar(hwnd);

    // Open command
    InitFileByName(windowData.dFilename);

    // reset scroll positions
    SetScrollPos(hwnd, SB_HORZ, windowData.posHScroll = 0, 0);
    SetScrollPos(hwnd, SB_VERT, windowData.posVScroll = 0, 0);

    // scroll correction
    ApplyResize(hwnd);
    InvalidateRect(hwnd, NULL, TRUE);

    return 0;
}