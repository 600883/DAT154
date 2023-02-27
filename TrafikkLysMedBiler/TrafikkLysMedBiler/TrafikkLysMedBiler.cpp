// Assignment1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "commctrl.h"
#include "TrafikkLysMedBiler.h"
#include "Car.h"
#include <deque>

using namespace std;

#define MAX_LOADSTRING 100
#define IDT_TIMER_LIGHTS 10
#define IDT_TIMER_CARS 20
#define IDT_TIMER_CAR_SPAWN_TOP 30
#define IDT_TIMER_CAR_SPAWN_LEFT 40

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

INT LIGHT_STATE = 1;
deque<Car> HORIZONTAL_ROAD;
deque<Car> VERTICAL_ROAD;
HWND hWndComboBox;
HWND hWndComboBoxNorth;
TCHAR activePercentWest[10] = _T("50%");
TCHAR activePercentNorth[10] = _T("50%");

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_TRAFIKKLYSMEDBILER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRAFIKKLYSMEDBILER));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_TRAFIKKLYSMEDBILER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TRAFIKKLYSMEDBILER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
    hInst = hInstance; // Store instance handle in our global variable

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

void paintRoads(HDC hdc, HGDIOBJ hOrg, HGDIOBJ hOrgPen)
{
    HBRUSH hLightGreyBrush = CreateSolidBrush(RGB(200, 200, 200));
    SelectObject(hdc, hLightGreyBrush);
    Rectangle(hdc, 0, 200, 1200, 300); // Horizontal road
    Rectangle(hdc, 500, 0, 600, 600); // Vertical road

    HPEN hLightGreyPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    SelectObject(hdc, hLightGreyPen);

    // remove lines on vertical road
    MoveToEx(hdc, 501, 200, NULL);
    LineTo(hdc, 599, 200);
    MoveToEx(hdc, 501, 300, NULL);
    LineTo(hdc, 599, 300);

    // remove lines on horizontal road
    MoveToEx(hdc, 500, 201, NULL);
    LineTo(hdc, 500, 299);
    MoveToEx(hdc, 599, 201, NULL);
    LineTo(hdc, 599, 299);

    HPEN hWhitePen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
    SelectObject(hdc, hWhitePen);
    // white line on vertical road
    MoveToEx(hdc, 505, 200, NULL);
    LineTo(hdc, 595, 200);

    // white line on horizontal road
    MoveToEx(hdc, 500, 205, NULL);
    LineTo(hdc, 500, 295);

    //SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, hOrgPen);
    SelectObject(hdc, hOrg);
    DeleteObject(hWhitePen);
    DeleteObject(hLightGreyBrush);
}

void paintLights(HDC hdc, HBRUSH hBrush, HGDIOBJ hOrg)
{
    /*HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HGDIOBJ hOrg = SelectObject(hdc, hBrush);*/

    HBRUSH hBlack = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hGreyBrush = CreateSolidBrush(RGB(100, 100, 100));

    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));

    SelectObject(hdc, hBlack);
    // Top light
    int topBoxLeft = 460;
    int topBoxTop = 90;
    int topBoxRight = topBoxLeft + 40;
    int topBoxBottom = topBoxTop + 110;

    // Bulbs
    int bulbSize = 30;

    int bulbLeft = topBoxLeft + 5;
    int bulbTopTop = topBoxTop + 5;
    int bulbRight = bulbLeft + bulbSize;
    int bulbTopBottom = bulbTopTop + bulbSize;

    int bulbMidldeTop = bulbTopBottom + 5;
    int bulbMiddleBottom = bulbMidldeTop + bulbSize;

    int bulbBottomTop = bulbMiddleBottom + 5;
    int bulbBottomBottom = bulbBottomTop + bulbSize;

    RoundRect(hdc, topBoxLeft, topBoxTop, topBoxRight, topBoxBottom, 5, 10);

    // Bottom horizontal light
    // Box
    int bottomBoxLeft = 390;
    int bottomBoxTop = 300;
    int bottomBoxRight = topBoxRight;
    int bottomBoxBottom = bottomBoxTop + 40;
    RoundRect(hdc, bottomBoxLeft, bottomBoxTop, bottomBoxRight, bottomBoxBottom, 5, 10);

    // Bulbs
    // Left light
    int bottomBulb1Left = bottomBoxLeft + 5;
    int bottomBulb1Top = bottomBoxTop + 5;
    int bottomBulb1Right = bottomBulb1Left + bulbSize;
    int bottomBulb1Bottom = bottomBulb1Top + bulbSize;

    // Middle light
    int bottomBulb2Left = bottomBulb1Right + 5;
    int bottomBulb2Top = bottomBulb1Top;
    int bottomBulb2Right = bottomBulb2Left + bulbSize;
    int bottomBulb2Bottom = bottomBulb2Top + bulbSize;

    // Right light
    int bottomBul3Left = bottomBulb2Right + 5;
    int bottomBul3Top = bottomBulb1Top;
    int bottomBul3Right = bottomBul3Left + bulbSize;
    int bottomBul3Bottom = bottomBul3Top + bulbSize;

    SelectObject(hdc, hRedBrush);
    Ellipse(hdc, bottomBulb1Left, bottomBulb1Top, bottomBulb1Right, bottomBulb1Bottom);
    Ellipse(hdc, bottomBulb2Left, bottomBulb2Top, bottomBulb2Right, bottomBulb2Bottom);
    Ellipse(hdc, bottomBul3Left, bottomBul3Top, bottomBul3Right, bottomBul3Bottom);

    if (LIGHT_STATE == 1)
    {
        // Top box
        SelectObject(hdc, hRedBrush);
        Ellipse(hdc, bulbLeft, bulbTopTop, bulbRight, bulbTopBottom); // Top

        SelectObject(hdc, hGreyBrush);
        Ellipse(hdc, bulbLeft, bulbMidldeTop, bulbRight, bulbMiddleBottom); // Middle
        Ellipse(hdc, bulbLeft, bulbBottomTop, bulbRight, bulbBottomBottom); // Bottom

        // Bottom box
        Ellipse(hdc, bottomBulb2Left, bottomBulb2Top, bottomBulb2Right, bottomBulb2Bottom); // Middle
        Ellipse(hdc, bottomBul3Left, bottomBul3Top, bottomBul3Right, bottomBul3Bottom); // Right
        SelectObject(hdc, hGreenBrush);
        Ellipse(hdc, bottomBulb1Left, bottomBulb1Top, bottomBulb1Right, bottomBulb1Bottom); // Left 
    }
    else if (LIGHT_STATE == 2)
    {
        // Top box
        SelectObject(hdc, hRedBrush);
        Ellipse(hdc, bulbLeft, bulbTopTop, bulbRight, bulbTopBottom); // Top
        SelectObject(hdc, hYellowBrush);
        Ellipse(hdc, bulbLeft, bulbMidldeTop, bulbRight, bulbMiddleBottom); // Middle
        SelectObject(hdc, hGreyBrush);
        Ellipse(hdc, bulbLeft, bulbBottomTop, bulbRight, bulbBottomBottom); /// Bottom

        // Bottom box
        Ellipse(hdc, bottomBul3Left, bottomBul3Top, bottomBul3Right, bottomBul3Bottom); // Right
        Ellipse(hdc, bottomBulb1Left, bottomBulb1Top, bottomBulb1Right, bottomBulb1Bottom); // Left
        SelectObject(hdc, hYellowBrush);
        Ellipse(hdc, bottomBulb2Left, bottomBulb2Top, bottomBulb2Right, bottomBulb2Bottom); // Middle
    }
    else if (LIGHT_STATE == 3)
    {
        // Top box
        SelectObject(hdc, hGreyBrush);
        Ellipse(hdc, bulbLeft, bulbTopTop, bulbRight, bulbTopBottom); // Top
        Ellipse(hdc, bulbLeft, bulbMidldeTop, bulbRight, bulbMiddleBottom); // Middle
        SelectObject(hdc, hGreenBrush);
        Ellipse(hdc, bulbLeft, bulbBottomTop, bulbRight, bulbBottomBottom); // Bottom

        // Bottom box
        SelectObject(hdc, hGreyBrush);
        Ellipse(hdc, bottomBulb2Left, bottomBulb2Top, bottomBulb2Right, bottomBulb2Bottom); // Middle
        Ellipse(hdc, bottomBulb1Left, bottomBulb1Top, bottomBulb1Right, bottomBulb1Bottom); // Left
        SelectObject(hdc, hRedBrush);
        Ellipse(hdc, bottomBul3Left, bottomBul3Top, bottomBul3Right, bottomBul3Bottom); // Right
    }
    else if (LIGHT_STATE == 4)
    {
        // Top box
        SelectObject(hdc, hGreyBrush);
        Ellipse(hdc, bulbLeft, bulbTopTop, bulbRight, bulbTopBottom); // Top
        Ellipse(hdc, bulbLeft, bulbBottomTop, bulbRight, bulbBottomBottom); // Bottom
        SelectObject(hdc, hYellowBrush);
        Ellipse(hdc, bulbLeft, bulbMidldeTop, bulbRight, bulbMiddleBottom); // Middle

        // Bottom box
        Ellipse(hdc, bottomBulb2Left, bottomBulb2Top, bottomBulb2Right, bottomBulb2Bottom); // Middle
        SelectObject(hdc, hGreyBrush);
        Ellipse(hdc, bottomBulb1Left, bottomBulb1Top, bottomBulb1Right, bottomBulb1Bottom); // Left
        SelectObject(hdc, hRedBrush);
        Ellipse(hdc, bottomBul3Left, bottomBul3Top, bottomBul3Right, bottomBul3Bottom); // Right
    }
    SelectObject(hdc, hOrg);
    DeleteObject(hBlack);
    DeleteObject(hGreyBrush);
    DeleteObject(hRedBrush);
    DeleteObject(hYellowBrush);
    DeleteObject(hGreenBrush);
}

void reDrawHorizontalRoad(HWND hWnd)
{
    tagRECT rectHorizontalRoad;
    rectHorizontalRoad.left = 0;
    rectHorizontalRoad.top = 200;
    rectHorizontalRoad.right = 1200;
    rectHorizontalRoad.bottom = 300;
    InvalidateRect(hWnd, &rectHorizontalRoad, false);
}

void reDrawVerticalRoad(HWND hWnd)
{
    tagRECT rectVerticalRoad;
    rectVerticalRoad.left = 500;
    rectVerticalRoad.top = 0;
    rectVerticalRoad.right = 600;
    rectVerticalRoad.bottom = 600;
    InvalidateRect(hWnd, &rectVerticalRoad, false);
}

void moveCarHorizontal(deque<Car>::iterator car)
{
    if (LIGHT_STATE <= 2 || car->posRight != 500) // green or yellow, or not at intersection
    {
        if (car->posRight == 535) // possible collision with vertical road
        {
            for (const auto& carVer : VERTICAL_ROAD)
            {
                if (((carVer.posTop > 200) && (carVer.posTop < 260)) || ((carVer.posBottom > 200) && (carVer.posBottom < 300)))
                { // doesnt work properly 
                    return;
                }
            }
        }
        car->posLeft += 5;
        car->posRight += 5;
    }
}

void moveCarVertical(deque<Car>::iterator car)
{
    if (LIGHT_STATE >= 3 || car->posBottom != 200) // green or yellow, or not at intersection
    {
        if (car->posBottom == 200) // possible collision with vertical road
        {
            for (const auto& carHor : HORIZONTAL_ROAD)
            {
                if (((carHor.posLeft > 500) && (carHor.posLeft < 560)) || ((carHor.posBottom > 500) && (carHor.posBottom < 600)))
                { // doesnt work properly 
                    return;
                }
            }
        }
        car->posTop += 5;
        car->posBottom += 5;
    }
}

void moveCars(HWND hWnd, string direction, deque<Car>* road)
{
    if (road->size() == 0)
    {
        return;
    }

    if (direction == "horizontal")
    {
        reDrawHorizontalRoad(hWnd);
    }
    else // vertical
    {
        reDrawVerticalRoad(hWnd);
    }


    auto currCar = road->begin();
    auto prevCar = currCar;
    bool removeCar = false;

    for (int i = 0; currCar != road->end(); currCar++)
    {
        if (currCar->posLeft > 1200 || currCar->posTop > 600) // car out of frame, delete
        {
            removeCar = true;
        }
        else
        {
            if (direction == "horizontal")
            {
                if ((currCar->posLeft == prevCar->posLeft) || (currCar->posRight + 5) < (prevCar->posLeft)) // prevent collision
                {
                    moveCarHorizontal(currCar);
                }
            }
            else // vertical
            {
                if ((currCar->posTop == prevCar->posTop) || (currCar->posBottom + 5) < (prevCar->posTop)) // prevent collision
                {
                    moveCarVertical(currCar);
                }
            }
        }
        i++;
        prevCar = currCar;
    }
    if (removeCar)
    {
        road->pop_front();
    }
}

void getComboBoxItem(HWND box, TCHAR* percent)
{
    int comboBoxActiveIndex = (int)SendMessageW(box, CB_GETCURSEL, 0, 0);
    if (comboBoxActiveIndex != CB_ERR)
    {
        int textLength = (int)SendMessageW(box, CB_GETLBTEXTLEN, comboBoxActiveIndex, 0);
        if (textLength != CB_ERR)
        {
            SendMessageW(box, CB_GETLBTEXT, comboBoxActiveIndex, (LPARAM)percent);

        }
    }
}

double convertPerscentStringToPercentageValue(TCHAR* percent)
{
    int nLength = _tcsclen(percent);
    percent[nLength - 1] = _T('\0');
    return (double)_tcstol(percent, NULL, 10) / 100;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case CBN_SELCHANGE:
        {
            MessageBox(NULL, _T("Hello, World!"), _T("Message"), MB_OK);
            int ItemIndex = (int)SendMessage((HWND)hWndComboBox, (UINT)CB_GETCURSEL,
                (WPARAM)0, (LPARAM)0);

            TCHAR  ListItem[256];
            (TCHAR)SendMessage((HWND)hWnd, (UINT)CB_GETLBTEXT,
                (WPARAM)ItemIndex, (LPARAM)ListItem);

            MessageBox(hWndComboBox, (LPCWSTR)ListItem, TEXT("Item Selected"), MB_OK);
        }
        break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...

        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
        HGDIOBJ hOrg = SelectObject(hdc, hBrush);
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HGDIOBJ hOrgPen = SelectObject(hdc, hPen);

        paintRoads(hdc, hOrg, hOrgPen);
        paintLights(hdc, hBrush, hOrg);

        //HBRUSH hCarBrush = NULL;
        for (const auto& car : HORIZONTAL_ROAD)
        {
            SelectObject(hdc, car.hCarBrush);
            Rectangle(hdc, car.posLeft, car.posTop, car.posRight, car.posBottom);
        }

        for (const auto& car : VERTICAL_ROAD)
        {
            SelectObject(hdc, car.hCarBrush);
            Rectangle(hdc, car.posLeft, car.posTop, car.posRight, car.posBottom);
        }

        SelectObject(hdc, hOrg);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        if (HORIZONTAL_ROAD.size() > 0)
        {
            if (HORIZONTAL_ROAD.back().posLeft > 50) // no collision at creation
            {
                tagRECT rectCar;
                rectCar.left = 0;
                rectCar.top = 240;
                rectCar.right = 1200;
                rectCar.bottom = 260;
                InvalidateRect(hWnd, &rectCar, false);

                Car carLeft("left");
                HORIZONTAL_ROAD.push_back(carLeft);
            }
        }
        else // first car
        {
            Car carLeft("left");
            HORIZONTAL_ROAD.push_back(carLeft);
        }
    }
    break;
    case WM_RBUTTONDOWN:
    {
        if (VERTICAL_ROAD.size() > 0)
        {
            if (VERTICAL_ROAD.back().posTop > 50) // no collision at creation
            {
                tagRECT rectCar;
                rectCar.left = 540;
                rectCar.top = 0;
                rectCar.right = 560;
                rectCar.bottom = 600;
                InvalidateRect(hWnd, &rectCar, false);

                Car carTop("top");
                VERTICAL_ROAD.push_back(carTop);
            }
        }
        else
        {
            Car carTop("top");
            VERTICAL_ROAD.push_back(carTop);
        }
    }
    break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
        {
            TCHAR percent[256];
            wcscpy_s(percent, activePercentWest);
            int nLength = _tcsclen(activePercentWest);
            //percent[nLength - 1] = _T('\0');

            int percentInt = _tcstol(percent, NULL, 10);

            if (percentInt > 10)
            {
                percentInt -= 10;

                int index = (int)(percentInt / 10) - 1;

                wsprintf(percent, _T("%d"), index); // add % to number 
                //MessageBox(NULL, percent, _T("Message"), MB_OK);

                SendMessageW(hWndComboBox, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
            }
        }
        break;
        case VK_RIGHT:
        {
            TCHAR percent[256];
            wcscpy_s(percent, activePercentWest);
            int nLength = _tcsclen(activePercentWest);
            //percent[nLength - 1] = _T('\0');

            int percentInt = _tcstol(percent, NULL, 10);

            if (percentInt < 100)
            {
                percentInt += 10;

                int index = (int)(percentInt / 10) - 1;

                wsprintf(percent, _T("%d"), index); // add % to number 
                //MessageBox(NULL, percent, _T("Message"), MB_OK);

                SendMessageW(hWndComboBox, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
            }
        }
        break;
        case VK_UP:
        {
            TCHAR percent[256];
            wcscpy_s(percent, activePercentNorth);
            int nLength = _tcsclen(activePercentNorth);
            //percent[nLength - 1] = _T('\0');

            int percentInt = _tcstol(percent, NULL, 10);

            if (percentInt < 100)
            {
                percentInt += 10;

                int index = (int)(percentInt / 10) - 1;

                wsprintf(percent, _T("%d"), index); // add % to number 
                //MessageBox(NULL, percent, _T("Message"), MB_OK);

                SendMessageW(hWndComboBoxNorth, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
            }
        }
        break;
        case VK_DOWN:
        {
            TCHAR percent[256];
            wcscpy_s(percent, activePercentNorth);
            int nLength = _tcsclen(activePercentNorth);
            //percent[nLength - 1] = _T('\0');

            int percentInt = _tcstol(percent, NULL, 10);

            if (percentInt > 10)
            {
                percentInt -= 10;

                int index = (int)(percentInt / 10) - 1;

                wsprintf(percent, _T("%d"), index); // add % to number 
                //MessageBox(NULL, percent, _T("Message"), MB_OK);

                SendMessageW(hWndComboBoxNorth, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
            }
        }
        break;
        default:
            break;
        }
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case IDT_TIMER_LIGHTS:
        {
            RECT rectTopBox;
            rectTopBox.left = 460;
            rectTopBox.top = 90;
            rectTopBox.right = 460 + 40;
            rectTopBox.bottom = 90 + 110;

            tagRECT rectBottomBox;
            rectBottomBox.left = 390;
            rectBottomBox.top = 300;
            rectBottomBox.right = 390 + 110;
            rectBottomBox.bottom = 300 + 40;

            InvalidateRect(hWnd, &rectTopBox, false);
            InvalidateRect(hWnd, &rectBottomBox, false);
            LIGHT_STATE = (LIGHT_STATE % 4) + 1;
        }
        break;
        case IDT_TIMER_CARS:
        {
            moveCars(hWnd, "horizontal", &HORIZONTAL_ROAD);
            moveCars(hWnd, "vertical", &VERTICAL_ROAD);
        }
        break;
        case IDT_TIMER_CAR_SPAWN_LEFT:
        {
            getComboBoxItem(hWndComboBox, activePercentWest);

            double percentage = convertPerscentStringToPercentageValue(activePercentWest);

            double pw = (double)rand() / RAND_MAX;

            if (pw < percentage)
            {
                if (HORIZONTAL_ROAD.size() > 0)
                {
                    if (HORIZONTAL_ROAD.back().posLeft > 50) // no collision at creation
                    {
                        tagRECT rectCar;
                        rectCar.left = 0;
                        rectCar.top = 240;
                        rectCar.right = 1200;
                        rectCar.bottom = 260;
                        InvalidateRect(hWnd, &rectCar, false);

                        Car carLeft("left");
                        HORIZONTAL_ROAD.push_back(carLeft);
                    }
                }
                else // first car
                {
                    Car carLeft("left");
                    HORIZONTAL_ROAD.push_back(carLeft);
                }
            }
        }
        break;
        case IDT_TIMER_CAR_SPAWN_TOP:
        {
            getComboBoxItem(hWndComboBoxNorth, activePercentNorth);

            double percentage = convertPerscentStringToPercentageValue(activePercentNorth);

            double pn = (double)rand() / RAND_MAX;

            if (pn < percentage)
            {
                if (VERTICAL_ROAD.size() > 0)
                {
                    if (VERTICAL_ROAD.back().posTop > 50) // no collision at creation
                    {
                        tagRECT rectCar;
                        rectCar.left = 540;
                        rectCar.top = 0;
                        rectCar.right = 560;
                        rectCar.bottom = 600;
                        InvalidateRect(hWnd, &rectCar, false);

                        Car carTop("top");
                        VERTICAL_ROAD.push_back(carTop);
                    }
                }
                else
                {
                    Car carTop("top");
                    VERTICAL_ROAD.push_back(carTop);
                }
            }
        }
        break;
        }
        break;
    case WM_CREATE:
    {
        // craete timers
        SetTimer(hWnd, IDT_TIMER_LIGHTS, 3000, (TIMERPROC)NULL);
        SetTimer(hWnd, IDT_TIMER_CARS, 50, (TIMERPROC)NULL);
        SetTimer(hWnd, IDT_TIMER_CAR_SPAWN_TOP, 1000, (TIMERPROC)NULL);
        SetTimer(hWnd, IDT_TIMER_CAR_SPAWN_LEFT, 1000, (TIMERPROC)NULL);

        hWndComboBox = CreateWindow(
            WC_COMBOBOX,
            _T(""),
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_VISIBLE | WS_CHILD | WS_BORDER,
            10, 20, 70, 200,
            hWnd, NULL, NULL, NULL);

        hWndComboBoxNorth = CreateWindow(
            WC_COMBOBOX,
            _T(""),
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_VISIBLE | WS_CHILD | WS_BORDER,
            100, 20, 70, 200,
            hWnd, NULL, NULL, NULL);

        int percent = 10;
        TCHAR item[256];

        for (int i = 0; i < 10; i++)
        {
            wsprintf(item, _T("%d%%"), percent); // add % to number 

            SendMessageW(
                hWndComboBox,
                (UINT)CB_ADDSTRING,
                (WPARAM)0,
                (LPARAM)item);

            SendMessageW(
                hWndComboBoxNorth,
                (UINT)CB_ADDSTRING,
                (WPARAM)0,
                (LPARAM)item);

            percent += 10;
        }

        SendMessageW(hWndComboBox, CB_SETCURSEL, (WPARAM)4, (LPARAM)0);
        SendMessageW(hWndComboBoxNorth, CB_SETCURSEL, (WPARAM)4, (LPARAM)0);

        // create brushes
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, IDT_TIMER_LIGHTS);
        KillTimer(hWnd, IDT_TIMER_CARS);
        KillTimer(hWnd, IDT_TIMER_CAR_SPAWN_LEFT);
        KillTimer(hWnd, IDT_TIMER_CAR_SPAWN_TOP);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}