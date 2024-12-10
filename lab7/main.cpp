#include <windows.h> // ����������� ������������� ����� Windows API
#include <string>    // ����������� ������������� ����� ��� ������ �� ��������
#include <cstdlib>   // ����������� ������������� ����� ��� ����������� �������
#include "resource.h" // ����������� ������������� ����� ��������
#include "tchar.h"
#include <cstdio>
#include "lib77.h"

#pragma comment(lib,"Project1.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


// ���������� ����������
HINSTANCE hInst; // ���������� ���������� ����������
const wchar_t g_szClassName[] = L"CalcWnd"; // ��� ������ ��������� ����
const wchar_t g_szClassNameAbout[] = L"AboutWnd"; // ��� ������ ���� "� ���������"
HBITMAP Bkgnd; // ���������� ������� ����� ��� ����
bool AboutWndCreate = false; // ����, �����������, ������� �� ���� "� ���������"
int a = 0, b = 0, j = 0; // ���������� ��� ����������
std::wstring sbuf = L"0"; // ����� ��� �������� �������� �����

HCURSOR hCursor;
HICON hIcon;

// ����������� ���������� �������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // ������� ��������� ��������� ����
INT_PTR CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM); // ������� ��������� ���� "� ���������"
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); // ���������� ��������� ������������

// �������� ������� ���������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc; // ��������� ��� ����������� ������ ����
    HWND hwnd; // ���������� ��������� ����
    MSG Msg; // ��������� ��� �������� ���������

    HACCEL hAccelTable; // ���������

    hInst = hInstance; // ���������� ����������� ���������� ����������

    hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDI_MYCURSOR));
    if (hCursor == NULL) {
        MessageBox(NULL, L"�� ������� ��������� ������", L"������", MB_OK | MB_ICONERROR);
        return 1;
    }

    // ����������� ������ ����
    wc.cbSize = sizeof(WNDCLASSEX); // ������ ���������
    wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ����
    wc.lpfnWndProc = WndProc; // ��������� �� ������� ���������
    wc.cbClsExtra = 0; // �������������� ����� ��� ������
    wc.cbWndExtra = 0; // �������������� ����� ��� ����
    wc.hInstance = hInstance; // ���������� ���������� ����������
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON)); // �������� ������
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ������ ����
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ��� ����
    wc.lpszMenuName = NULL; // ��� ����
    wc.lpszClassName = g_szClassName; // ��� ������ ����
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // ��������� ������ ����

    if (!RegisterClassEx(&wc)) { // ����������� ������ ����
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK); // ��������� �� ������
        return 0;
    }


    // ����������� ������ ����-��������
    wc.lpfnWndProc = WndProc1; 
    wc.lpszClassName = g_szClassNameAbout; 
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
    wc.hbrBackground = NULL; 

    if (!RegisterClassEx(&wc)) { // ����������� ������ ���� "� ���������"
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK); // ��������� �� ������
        return 0;
    }

    // �������� �������� ����
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE, // ����������� ����� ����
        g_szClassName, // ��� ������ ����
        L"Progra", // ��������� ����
        WS_OVERLAPPEDWINDOW, // ����� ����
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, // ������� � ������� ����
        NULL, NULL, hInstance, NULL); // ����������� � �������������� ���������

    if (hwnd == NULL) { // �������� ���������� �������� ����
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK); // ��������� �� ������
        return 0;
    }

    ShowWindow(hwnd, nCmdShow); // ����������� ����
    UpdateWindow(hwnd); // ���������� ����
    Bkgnd = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP));
    if (Bkgnd == NULL) {
        MessageBox(NULL, L"�� ������� ��������� ������", L"������", MB_OK | MB_ICONERROR);
        return 1;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR)); // ���������
    if (hAccelTable == NULL) {
        MessageBox(NULL, L"�� ������� ��������� �����������", L"������", MB_OK | MB_ICONERROR);
        return 1;
    }

    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        if (!TranslateAccelerator(hwnd, hAccelTable, &Msg)) { // ���������
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }
    return Msg.wParam; // ������� ���� ������
}

// ������� ��������� ������ ��������� ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) { // ��������� ���������
    case WM_PAINT:
    {
        char brd;
        brd = 'a';
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        bool iTextLen;
        TCHAR szText[1024];
        /*iTextLen = _stprintf_s(szText, TEXT("lll"), procd3("aert", brd));
            TextOut(hdc, 16, 16, szText, iTextLen);*/
    }
    case WM_CREATE:
        // ��������� ���� ��� ��������� ����
        SetMenu(hwnd, LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1)));
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case ID_CALCULATOR:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_CALCULATOR), hwnd, DlgProc);
            break;
        case ID_SCREENSAVER:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_SCREENSAVER), hwnd, WndProc1);
            break;
        case ID_DLL: {
            const char* str = "Hella";
            bool result = procd3(str, 'a');
            /*if (result) {
                MessageBox(hwnd, L"Function call succeeded!", L"Success", MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBox(hwnd, L"Function call failed!", L"Error", MB_OK | MB_ICONERROR);
            }
            break;*/
        }
        }

        return 0;
    case WM_DESTROY: // ��������� � ���������� ����
        PostQuitMessage(0); // �������� ��������� � ������
        break;
    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT) {
            SetCursor(hCursor);
            return TRUE;
        }
        break;
    default: // ��������� ������ ���������
        return DefWindowProc(hwnd, msg, wParam, lParam); // ��������� ��������� �� ���������
    }
    return 0;
}

void DrawBitmap(HDC hdc, HBITMAP hBitmap, int xs, int ys, RECT rect) {
    SetStretchBltMode(hdc, COLORONCOLOR);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
    BITMAP bm;
    GetObject(hBitmap, sizeof(bm), &bm);
    StretchBlt(hdc, xs, ys, rect.right, rect.bottom, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    SelectObject(hdcMem, hOldBitmap);
    DeleteDC(hdcMem);
}

// ������� ��������� ������ "��������"
INT_PTR CALLBACK WndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) { // ��������� ���������
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);

        // ��������� �������
        DrawBitmap(hdc, Bkgnd, 0, 0, rect);

        // ��������� ������ ������ �������
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 4, 4, L"������� 3", 9);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE: // ��������� � �������� ����
        EndDialog(hwnd, 0); // ���������� �������
        break;
    default: // ��������� ������ ���������
        return FALSE;
    }
    return TRUE;
}


// ���������� ��������� �������-������������
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) { // ��������� ���������
    //case WM_PAINT: {
    //    PAINTSTRUCT ps;
    //    HDC hdc = BeginPaint(hDlg, &ps);
    //    RECT rect;
    //    GetClientRect(hDlg, &rect);

    //    // ��������� �������
    //    DrawBitmap(hdc, Bkgnd, 0, 0, rect);

    //    // ��������� ������ ������ �������
    //    SetBkMode(hdc, TRANSPARENT);
    //    TextOut(hdc, 4, 4, L"������� 3", 9);

    //    EndPaint(hDlg, &ps);
    //    break;
    //}
    case WM_COMMAND: {
        if (HIWORD(wParam) == BN_CLICKED) {
            switch (LOWORD(wParam)) {
            case IDC_BUTTON0: case IDC_BUTTON1: case IDC_BUTTON2: case IDC_BUTTON3: case IDC_BUTTON4:
            case IDC_BUTTON5: case IDC_BUTTON6: case IDC_BUTTON7: case IDC_BUTTON8: case IDC_BUTTON9: {
                if (sbuf == L"0") sbuf = L"";
                sbuf += std::to_wstring(LOWORD(wParam) - IDC_BUTTON0);
                SetDlgItemText(hDlg, IDC_EDIT, sbuf.c_str());
                break;
            }
            case IDC_BUTTONSUM: case IDC_BUTTONSUB: case IDC_BUTTONMUL: case IDC_BUTTONDIV: case IDC_BUTTONEQU: {
                b = std::stoi(sbuf);
                if (j > 3) a = b;
                if (j == 0) a += b;
                if (j == 1) a -= b;
                if (j == 2) a *= b;
                if (j == 3) a /= b;
                sbuf = std::to_wstring(a);
                SetDlgItemText(hDlg, IDC_EDIT, sbuf.c_str());
                sbuf = L"0";
                if (LOWORD(wParam) == IDC_BUTTONSUM) j = 0;
                if (LOWORD(wParam) == IDC_BUTTONSUB) j = 1;
                if (LOWORD(wParam) == IDC_BUTTONMUL) j = 2;
                if (LOWORD(wParam) == IDC_BUTTONDIV) j = 3;
                if (LOWORD(wParam) == IDC_BUTTONEQU) {
                    j = 5;
                    a = 0;
                }
                break;
            }
            case IDC_BUTTONCLEAR: {
                sbuf = L"0";
                j = 0;
                a = 0;
                b = 0;
                SetDlgItemText(hDlg, IDC_EDIT, sbuf.c_str());
                break;
            }
            }
        }
        break;
    }
    case WM_CLOSE: // ��������� � �������� ����
        EndDialog(hDlg, 0); // ���������� �������
        break;
    default: // ��������� ������ ���������
        return FALSE;
    }
    return TRUE;
}
