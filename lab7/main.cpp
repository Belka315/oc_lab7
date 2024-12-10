#include <windows.h> // Подключение заголовочного файла Windows API
#include <string>    // Подключение заголовочного файла для работы со строками
#include <cstdlib>   // Подключение заголовочного файла для стандартных функций
#include "resource.h" // Подключение заголовочного файла ресурсов
#include "tchar.h"
#include <cstdio>
#include "lib77.h"

#pragma comment(lib,"Project1.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


// Глобальные переменные
HINSTANCE hInst; // Дескриптор экземпляра приложения
const wchar_t g_szClassName[] = L"CalcWnd"; // Имя класса основного окна
const wchar_t g_szClassNameAbout[] = L"AboutWnd"; // Имя класса окна "О программе"
HBITMAP Bkgnd; // Дескриптор битовой карты для фона
bool AboutWndCreate = false; // Флаг, указывающий, создано ли окно "О программе"
int a = 0, b = 0, j = 0; // Переменные для вычислений
std::wstring sbuf = L"0"; // Буфер для хранения текущего ввода

HCURSOR hCursor;
HICON hIcon;

// Опережающие объявления функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Оконная процедура основного окна
INT_PTR CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM); // Оконная процедура окна "О программе"
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM); // Диалоговая процедура калькулятора

// Основная функция программы
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc; // Структура для регистрации класса окна
    HWND hwnd; // Дескриптор основного окна
    MSG Msg; // Структура для хранения сообщений

    HACCEL hAccelTable; // Добавлено

    hInst = hInstance; // Сохранение дескриптора экземпляра приложения

    hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDI_MYCURSOR));
    if (hCursor == NULL) {
        MessageBox(NULL, L"Не удалось загрузить курсор", L"Ошибка", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Регистрация класса окна
    wc.cbSize = sizeof(WNDCLASSEX); // Размер структуры
    wc.style = CS_HREDRAW | CS_VREDRAW; // Стили окна
    wc.lpfnWndProc = WndProc; // Указатель на оконную процедуру
    wc.cbClsExtra = 0; // Дополнительные байты для класса
    wc.cbWndExtra = 0; // Дополнительные байты для окна
    wc.hInstance = hInstance; // Дескриптор экземпляра приложения
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON)); // Загрузка иконки
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Курсор окна
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Фон окна
    wc.lpszMenuName = NULL; // Имя меню
    wc.lpszClassName = g_szClassName; // Имя класса окна
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // Маленькая иконка окна

    if (!RegisterClassEx(&wc)) { // Регистрация класса окна
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK); // Сообщение об ошибке
        return 0;
    }


    // Регистрация класса окна-заставки
    wc.lpfnWndProc = WndProc1; 
    wc.lpszClassName = g_szClassNameAbout; 
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
    wc.hbrBackground = NULL; 

    if (!RegisterClassEx(&wc)) { // Регистрация класса окна "О программе"
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK); // Сообщение об ошибке
        return 0;
    }

    // Создание главного окна
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE, // Расширенный стиль окна
        g_szClassName, // Имя класса окна
        L"Progra", // Заголовок окна
        WS_OVERLAPPEDWINDOW, // Стиль окна
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, // Размеры и позиция окна
        NULL, NULL, hInstance, NULL); // Дескрипторы и дополнительные параметры

    if (hwnd == NULL) { // Проверка успешности создания окна
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK); // Сообщение об ошибке
        return 0;
    }

    ShowWindow(hwnd, nCmdShow); // Отображение окна
    UpdateWindow(hwnd); // Обновление окна
    Bkgnd = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP));
    if (Bkgnd == NULL) {
        MessageBox(NULL, L"Не удалось загрузить битмап", L"Ошибка", MB_OK | MB_ICONERROR);
        return 1;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR)); // Добавлено
    if (hAccelTable == NULL) {
        MessageBox(NULL, L"Не удалось загрузить акселератор", L"Ошибка", MB_OK | MB_ICONERROR);
        return 1;
    }

    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        if (!TranslateAccelerator(hwnd, hAccelTable, &Msg)) { // Добавлено
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }
    return Msg.wParam; // Возврат кода выхода
}

// Оконная процедура класса основного окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) { // Обработка сообщений
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
        // Установка меню для основного окна
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
    case WM_DESTROY: // Сообщение о разрушении окна
        PostQuitMessage(0); // Отправка сообщения о выходе
        break;
    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT) {
            SetCursor(hCursor);
            return TRUE;
        }
        break;
    default: // Обработка других сообщений
        return DefWindowProc(hwnd, msg, wParam, lParam); // Обработка сообщений по умолчанию
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

// Оконная процедура класса "заставки"
INT_PTR CALLBACK WndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) { // Обработка сообщений
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);

        // Рисование битмапа
        DrawBitmap(hdc, Bkgnd, 0, 0, rect);

        // Рисование текста поверх битмапа
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 4, 4, L"Вариант 3", 9);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE: // Сообщение о закрытии окна
        EndDialog(hwnd, 0); // Завершение диалога
        break;
    default: // Обработка других сообщений
        return FALSE;
    }
    return TRUE;
}


// Диалоговая процедура диалога-калькулятора
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) { // Обработка сообщений
    //case WM_PAINT: {
    //    PAINTSTRUCT ps;
    //    HDC hdc = BeginPaint(hDlg, &ps);
    //    RECT rect;
    //    GetClientRect(hDlg, &rect);

    //    // Рисование битмапа
    //    DrawBitmap(hdc, Bkgnd, 0, 0, rect);

    //    // Рисование текста поверх битмапа
    //    SetBkMode(hdc, TRANSPARENT);
    //    TextOut(hdc, 4, 4, L"Вариант 3", 9);

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
    case WM_CLOSE: // Сообщение о закрытии окна
        EndDialog(hDlg, 0); // Завершение диалога
        break;
    default: // Обработка других сообщений
        return FALSE;
    }
    return TRUE;
}
