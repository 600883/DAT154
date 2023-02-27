// Oblig1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Oblig1.h"

#define MAX_LOADSTRING 100
#define trafficLight 1

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int step = 0;
UINT_PTR timerdelay = 500;

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
    LoadStringW(hInstance, IDC_OBLIG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBLIG1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OBLIG1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OBLIG1);
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
    case WM_TIMER:
        switch (wParam)
        {
        case trafficLight:
        step++;

        
        if (step == 2 || step == 0) {
            timerdelay = 5000;
        }
        else if (step == 3) {
            timerdelay = 500;
        }
        if (step == 4) {
            step = 0;
            timerdelay = 5000;
        }
        }
        KillTimer(hWnd, trafficLight);
        SetTimer(hWnd, trafficLight, timerdelay, NULL);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    
            

    case WM_CREATE:
    {
        SetTimer(hWnd, trafficLight, 5000, NULL);

    }
        
    break;

    case WM_LBUTTONDOWN:
        step++;
    {
            if (step == 4) {
                step = 0;
            }
            InvalidateRect(hWnd, NULL, TRUE);

    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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

            HBRUSH firkant_svart = CreateSolidBrush(RGB(0, 0, 0));
            HBRUSH hBrush_Rod = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH hBrush_Gul = CreateSolidBrush(RGB(255, 255, 0));
            HBRUSH hBrush_Gronn = CreateSolidBrush(RGB(0, 255, 0));
            HBRUSH hBrush_Graa = CreateSolidBrush(RGB(128, 128, 128));

            HGDIOBJ hOrg = SelectObject(hdc, firkant_svart);
            Rectangle(hdc, 395, 65, 445, 195);

            Rectangle(hdc, 315, 305, 445, 355);
            hOrg = SelectObject(hdc, hBrush_Graa);

            if(step == 0) {

                //Trafikklys loddrett
               
                hOrg = SelectObject(hdc, hBrush_Rod);
                Ellipse(hdc, 405, 70, 435, 100);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 115, 435, 145);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 160, 435, 190);

                //Traffikklys vannrett

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 320, 315, 350, 345);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 365, 315, 395, 345);

                hOrg = SelectObject(hdc, hBrush_Gronn);
                Ellipse(hdc, 410, 315, 440, 345);




            }
            
            else if(step == 1) {

                //Trafikklys loddrett
               
                hOrg = SelectObject(hdc, hBrush_Rod);
                Ellipse(hdc, 405, 70, 435, 100);

                hOrg = SelectObject(hdc, hBrush_Gul);
                Ellipse(hdc, 405, 115, 435, 145);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 160, 435, 190);

                //Traffikklys vannrett

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 320, 315, 350, 345);

                hOrg = SelectObject(hdc, hBrush_Gul);
                Ellipse(hdc, 365, 315, 395, 345);

                hOrg = SelectObject(hdc, hBrush_Gronn);
                Ellipse(hdc, 410, 315, 440, 345);


            }

            else if(step == 2) {

                //Trafikklys loddrett
              
                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 70, 435, 100);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 115, 435, 145);

                hOrg = SelectObject(hdc, hBrush_Gronn);
                Ellipse(hdc, 405, 160, 435, 190);

                //Traffikklys vannrett

                hOrg = SelectObject(hdc, hBrush_Rod);
                Ellipse(hdc, 320, 315, 350, 345);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 365, 315, 395, 345);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 410, 315, 440, 345);


            }

            else if (step == 3) {

                //Trafikklys loddrett

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 70, 435, 100);

                hOrg = SelectObject(hdc, hBrush_Gul);
                Ellipse(hdc, 405, 115, 435, 145);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 160, 435, 190);

                //Traffikklys vannrett

                hOrg = SelectObject(hdc, hBrush_Rod);
                Ellipse(hdc, 320, 315, 350, 345);

                hOrg = SelectObject(hdc, hBrush_Gul);
                Ellipse(hdc, 365, 315, 395, 345);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 410, 315, 440, 345);
               
            }
            else if (step == 4) {

                //Trafikklys loddrett
                
                hOrg = SelectObject(hdc, hBrush_Rod);
                Ellipse(hdc, 405, 70, 435, 100);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 115, 435, 145);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 405, 160, 435, 190);

                //Traffikklys vannrett

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 320, 315, 350, 345);

                hOrg = SelectObject(hdc, hBrush_Graa);
                Ellipse(hdc, 365, 315, 395, 345);

                hOrg = SelectObject(hdc, hBrush_Gronn);
                Ellipse(hdc, 410, 315, 440, 345);

            }

            HBRUSH hBrush_Roadx = CreateSolidBrush(RGB(211, 211, 211));
            hOrg = SelectObject(hdc, hBrush_Roadx);
            Rectangle(hdc, 0, 200, 1000, 300);

            HBRUSH hBrush_Roady = CreateSolidBrush(RGB(211, 211, 211));
            hOrg = SelectObject(hdc, hBrush_Roady);
            Rectangle(hdc, 450, 0, 550 , 1000);

           

            // Sletting
            SelectObject(hdc, firkant_svart);
            SelectObject(hdc, hBrush_Rod);
            SelectObject(hdc, hBrush_Gul);
            SelectObject(hdc, hBrush_Gronn);
            SelectObject(hdc, hBrush_Roadx);
            SelectObject(hdc, hBrush_Roady);
            SelectObject(hdc, hBrush_Graa);

            DeleteObject(firkant_svart);
            DeleteObject(hBrush_Rod);
            DeleteObject(hBrush_Gul);
            DeleteObject(hBrush_Gronn);
            DeleteObject(hBrush_Roadx);
            DeleteObject(hBrush_Roady);
            DeleteObject(hBrush_Graa); 
            
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
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
