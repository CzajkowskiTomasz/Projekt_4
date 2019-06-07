// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <math.h>
#include <gdiplus.h>
#include <string>

#define MAX_LOADSTRING 100
#define TMR_1 1

#define M_PI 3.14159265358979323846
#define liczba_danych_w_probce 12
#define promien_kola_wskazowek 80
#define liczba_pomiarow 625

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;

// buttons
HWND hwndButton;

// sent data
RECT drawArea1 = { 150, 0, 450, 400 };
RECT drawArea2 = { 220, 120, 380, 280};
RECT drawArea3 = { 460,70,735,165 };
RECT drawArea_liczba_zbednych_pomiarow = { 5, 5, 270, 30 };
RECT drawArea_zegar = { 5,80,65,105 };
RECT drawArea_os = { 5,200,110,230 };
RECT drawArea_przedzialka_czasowa = { 460,5,740,30 };

//Zmienne globalne
std::fstream plik_dane;
bool pauza = true;
int liczba_zbednych_pomiarow = 0;
int osie = 0;
double blad = 0;
long double zmiana_przedzia≥ki = 0.025;

// Dane z pliku
std::vector<double> dane;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

void Rysowanie_danych(HDC hdc, double angle)
{
	Graphics graphics(hdc);
	
	Gdiplus::Font *czcionka = new Font(L"Times new roman", 16);
	PointF A = PointF(460, 120);
	PointF B = PointF(460, 145);
	PointF C = PointF(460, 70);
	PointF D = PointF(460, 95);

	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	graphics.DrawString(L"Aktualny kπt (po kalibracji):", 29, czcionka, A, &cyfry);

	std::wstring kat;

	graphics.DrawString(L"Aktualny kπt (przed kalibracjπ):", 32, czcionka, C, &cyfry);

	kat = std::to_wstring(angle);

	graphics.DrawString(kat.c_str(), kat.length(), czcionka, D, &cyfry);

	if (value > 1)
	{
		angle = angle + blad;
		if (angle > 180)angle = angle - 360;
		if (angle < -180)angle = angle + 360;
	}
	kat = std::to_wstring(angle);

	graphics.DrawString(kat.c_str(), kat.length(), czcionka, B, &cyfry);
}

void Rysowanie_zbednych_pomiarow(HDC hdc)
{
	Graphics graphics(hdc);

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 16);
	PointF A = PointF(5, 5);
	PointF B = PointF(220, 5);
	PointF C = PointF(5, 200);
	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	graphics.DrawString(L"Liczba zbÍdnych prÛbek:", 23, czcionka, A, &cyfry);

	std::wstring liczba_string;

	liczba_string = std::to_wstring(liczba_zbednych_pomiarow);

	graphics.DrawString(liczba_string.c_str(), liczba_string.length(), czcionka, B, &cyfry);
}

void Rysowanie_zegar(HDC hdc)
{
	Graphics graphics(hdc);

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 16);
	
	PointF A = PointF(5, 80);
	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	graphics.DrawString(L"Zegar:", 6, czcionka, A, &cyfry);
}

void Rysowanie_osi(HDC hdc)
{
	Graphics graphics(hdc);

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 16);

	PointF A = PointF(5, 200);
	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	graphics.DrawString(L"WybÛr osi:", 11, czcionka, A, &cyfry);
}

void Rysowanie_przedzialki_czasowej(HDC hdc)
{
	Graphics graphics(hdc);

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 16);

	PointF A = PointF(460, 5);
	PointF B = PointF(725, 5);
	PointF C = PointF(640, 5);
	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	graphics.DrawString(L"Przedzia≥ka czasowa:", 22, czcionka, A, &cyfry);
	graphics.DrawString(L"s", 1, czcionka, B, &cyfry);

	std::wstring liczba_string;

	liczba_string = std::to_wstring(zmiana_przedzia≥ki);

	graphics.DrawString(liczba_string.c_str(), liczba_string.length(), czcionka, C, &cyfry);
}

void rysowanie_kompasu(HDC hdc, double angle_1)
{
	angle_1 = angle_1 + blad;

	if (angle_1 > 180)angle_1 = angle_1 - 360;
	if (angle_1 < -180)angle_1 = angle_1 + 360;

	double angle = angle_1;
	
	Graphics kompas(hdc);

	Pen ramka(Color(255, 150, 150, 150));
	Pen tarcza(Color(255, 200, 200, 200));
	Pen srodek(Color(255, 0, 0, 0));
	Pen litery(Color(255, 50, 50, 50));
	Pen wskazowka_1(Color(255, 255, 20, 20));
	Pen wskazowka_2(Color(255, 255, 255, 255));

	//Rysowanie uchwytu
	for (int i = 30; i < 60; i++)kompas.DrawEllipse(&ramka, 300 - (i / 2), 35 - (i / 2), i, i);

	//Rysowanie tarczy i ramki
	for (int i = 270; i < 300; i++)kompas.DrawEllipse(&ramka, 300 - (i / 2), 200 - (i / 2), i, i);
	//for (int i = 10; i < 270; i++)kompas.DrawEllipse(&tarcza, 300 - (i / 2), 200 - (i / 2), i, i);

	//Rysowanie liter na tarczy
	
	//N
	for (int i = 0; i < 7; i++)
	{
		kompas.DrawLine(&litery, 290 + i, 110, 290 + i, 80);
		kompas.DrawLine(&litery, 290 + i, 80, 303 + i, 110);
		kompas.DrawLine(&litery, 310 - i, 110, 310 - i, 80);
	}

	//S
	for (int i = 0; i < 12; i++)
	{
		kompas.DrawArc(&litery, 294 - (i / 2), 294 - (i / 2), 9 + i, 9 + i, 90, 270);
		kompas.DrawArc(&litery, 294-(i/2), 310-(i/2), 12 + i, 12 + i, 270, 270);
	}

	//W
	for (int i = 0; i < 7; i++)
	{
		kompas.DrawLine(&litery, 180 + i, 182, 185 + i, 218);
		kompas.DrawLine(&litery, 185 + i, 218, 195 + i, 200);
		kompas.DrawLine(&litery, 195 + i, 200, 205 + i, 218);
		kompas.DrawLine(&litery, 205 + i, 218, 210 + i, 182);
	}

	//E
	for (int i = -3; i < 3; i++)
	{
		kompas.DrawLine(&litery, 400 + i, 182, 400 + i, 217);
		kompas.DrawLine(&litery, 400, 185 + i, 418, 185 + i);
		kompas.DrawLine(&litery, 400, 200 + i, 418, 200 + i);
		kompas.DrawLine(&litery, 400, 215 + i, 418, 215 + i);
	}

	//Wyliczanie polozenia wskazowek

	int jakis_x = 0;
	int jakis_y = 0;
	
	if (0 < angle && angle < 90)
	{
		if (angle <= 80)
		{
			jakis_x = promien_kola_wskazowek / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
			jakis_y = jakis_x * tan((angle / 180)*(M_PI));

			jakis_y = -jakis_y;
		}
		else
		{
			jakis_x = 13 - (angle - 80) * (13 / 10);
			jakis_y = -73 - (angle - 80) * (7 / 10);
		}
	}
	
	if (90 < angle && angle < 180)
	{
		if (angle >= 100)
		{
			angle = -angle + 180;

			jakis_x = promien_kola_wskazowek / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
			jakis_y = jakis_x * tan((angle / 180)*(M_PI));

			jakis_x = -jakis_x;
			jakis_y = -jakis_y;
		}
		else
		{
			jakis_x = -13 - (angle - 100) * (13 / 10);
			jakis_y = -73 + (angle - 100) * (7 / 10);
		}
		
		
	}

	if (-90 < angle && angle < 0)
	{
		if (angle >= -80)
		{
			angle = -angle;

			jakis_x = promien_kola_wskazowek / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
			jakis_y = jakis_x * tan((angle / 180)*(M_PI));
		}
		else
		{
			jakis_x = 13 + (angle + 80) * ( 13 / 10) ;
			jakis_y = 73 - (angle + 80) * (7 / 10) ;
		}
	}

	if (-180 < angle && angle < -90)
	{
		if (angle <= -100)
		{
			angle = angle + 180;

			jakis_x = promien_kola_wskazowek / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
			jakis_y = jakis_x * tan((angle / 180)*(M_PI));

			jakis_x = -jakis_x;
		}
		else 
		{
			jakis_x = -13 + (angle + 100) * (13 / 10);
			jakis_y = 73 + (angle + 100) * (7 / 10);
		}
	}

	angle = angle_1;

	if (angle == 180 || angle == -180)
	{
		jakis_x = -promien_kola_wskazowek;
		jakis_y = 0;
	}

	if (angle == 0)
	{
		jakis_x = promien_kola_wskazowek;
		jakis_y = 0;
	}


	if(angle == 90)
	{
		jakis_x = 0;
		jakis_y = -promien_kola_wskazowek;
	}

	if (angle == -90)
	{
		jakis_x = 0;
		jakis_y = promien_kola_wskazowek;
	}
	
	jakis_x = jakis_x + 300;
	jakis_y = jakis_y + 200;

	//Rysowanie wskazowki

	double x_2 = 0;
	double y_2 = 0;

	angle = angle_1 + 90;

		for (int i = -15; i < 15; i++)
		{
			if (0 < angle && angle < 90)
			{
				x_2 = i / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
				y_2 = x_2 * tan((angle / 180)*(M_PI));

				y_2 = -y_2;
			}

			if (90 < angle && angle < 180)
			{
				x_2 = i / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
				y_2 = x_2 * tan((angle / 180)*(M_PI));

				x_2 = -x_2;
			}

			if (180 < angle && angle < 270)
			{
				x_2 = i / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
				y_2 = x_2 * tan((angle / 180)*(M_PI));

				x_2 = -x_2;
			}

			if (-90 < angle && angle < 0)
			{
				x_2 = i / (sqrt(1 + tan((angle / 180)*(M_PI))*tan((angle / 180)*(M_PI))));
				y_2 = x_2 * tan((angle / 180)*(M_PI));

				y_2 = -y_2;
			}

			if (angle == 180 || angle == -180)
			{
				x_2 = -i;
				y_2 = 0;
			}

			if (angle == 0)
			{
				x_2 = i;
				y_2 = 0;
			}


			if (angle == 90)
			{
				x_2 = 0;
				y_2 = i;
			}

			if (angle == -90 && angle == 270)
			{
				x_2 = 0;
				y_2 = -1 * i;
			}

			x_2 = x_2 + 300;
			y_2 = y_2 + 200;

			kompas.DrawLine(&wskazowka_1, jakis_x, jakis_y, x_2, y_2);
			kompas.DrawLine(&tarcza, 300 + (300 - jakis_x), 200 + (200 - jakis_y), x_2, y_2);
		}
	
	//Rysowanie srodka
	for (int i = 0; i < 10; i++)kompas.DrawEllipse(&srodek, 300 - (i / 2), 200 - (i / 2), i, i);
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea==NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	
	if (dane.size() > value)
	{
		if (dane.size() > 2)
		{
			blad = 90 - dane[2];
		}

		rysowanie_kompasu(hdc, dane[value]);
	}
	

	EndPaint(hWnd, &ps);
}

void repaintData(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	InvalidateRect(hWnd, &drawArea3, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	Rysowanie_danych(hdc, dane[value]);
	EndPaint(hWnd, &ps);
}

void repaintLiczba_zbednych_pomiarow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	InvalidateRect(hWnd, &drawArea_liczba_zbednych_pomiarow, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	Rysowanie_zbednych_pomiarow(hdc);
	EndPaint(hWnd, &ps);
}

void repaintPrzedzialka_czasowa(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	InvalidateRect(hWnd, &drawArea_przedzialka_czasowa, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	Rysowanie_przedzialki_czasowej(hdc);
	EndPaint(hWnd, &ps);
}

void repaintDane_niezalezne_od_czasu(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	InvalidateRect(hWnd, &drawArea_zegar, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	Rysowanie_zegar(hdc);
	EndPaint(hWnd, &ps);

	InvalidateRect(hWnd, &drawArea_os, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	Rysowanie_osi(hdc);
	EndPaint(hWnd, &ps);
}

int otwieranie_pliku(std::fstream &plik_dane)
{
	if (!plik_dane)
	{
		//std::cout << "Plik wejsciowy klienci.txt nie zostal otwarty. :(" << std::endl;
		return 1;
	}
	else
	{
		double dana;
		int j = 0;

		plik_dane.seekg(0);
		
		while(plik_dane.eof() == false && j <= liczba_zbednych_pomiarow )
		{
			if(j == 0)plik_dane >> dana;
			else for (int i = 0; i < liczba_danych_w_probce ; i++)plik_dane >> dana;
			j++;
		}

		if (osie == 2)
		{
			plik_dane >> dana;
			plik_dane >> dana;
		}
		else if(osie == 1)
		{
			plik_dane >> dana;
		}
		
		dane.push_back(dana);

		while (plik_dane.eof() == false)
		{
			for (int i = 0; i < liczba_danych_w_probce ; i++)plik_dane >> dana;
			dane.push_back(dana);
		}
		dane.erase(dane.end() - 1);

		return 0;
	}
}

int OnCreate(HWND window)
{
	plik_dane.open("D:\\outputCatapult01.log", std::ios::in);
	dane.push_back(90);
	dane.push_back(90);
	return 0;
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// OTWIERANIE PLIKU:
	
	
	if (!plik_dane)
	{
		return 1;
	}

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		100, 100, 780, 450, NULL, NULL, hInstance, NULL);
	// create button and store the handle                                                       
	
	//Przyciski wyboru poczatkowego pomiaru
	
	hwndButton = CreateWindow(TEXT("button"), TEXT("<<"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 30, 30, 30, hWnd, (HMENU)ID_BUTTON1, GetModuleHandle(NULL), NULL);
	
	hwndButton = CreateWindow(TEXT("button"), TEXT("<"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 45, 30, 30, 30, hWnd, (HMENU)ID_BUTTON2, GetModuleHandle(NULL), NULL);
	
	hwndButton = CreateWindow(TEXT("button"), TEXT(">"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 80, 30, 30, 30, hWnd, (HMENU)ID_BUTTON3, GetModuleHandle(NULL), NULL);
	
	hwndButton = CreateWindow(TEXT("button"), TEXT(">>"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 115, 30, 30, 30, hWnd, (HMENU)ID_BUTTON4, GetModuleHandle(NULL), NULL);
	
	//Przycisk START/STOP

	hwndButton = CreateWindow(TEXT("button"), TEXT("START/PAUZA"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 110, 100, 30, hWnd, (HMENU)ID_BUTTON5, GetModuleHandle(NULL), NULL);

	//Przycisk Reset

	hwndButton = CreateWindow(TEXT("button"), TEXT("RESET"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 145, 100, 30, hWnd, (HMENU)ID_BUTTON6, GetModuleHandle(NULL), NULL);

	//Przyciski Osi
	
	hwndButton = CreateWindow(TEXT("button"), TEXT("Oú X"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 230, 100, 30, hWnd, (HMENU)ID_BUTTON7, GetModuleHandle(NULL), NULL);
	hwndButton = CreateWindow(TEXT("button"), TEXT("Oú Y"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 265, 100, 30, hWnd, (HMENU)ID_BUTTON8, GetModuleHandle(NULL), NULL);
	hwndButton = CreateWindow(TEXT("button"), TEXT("Oú Z"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, 300, 100, 30, hWnd, (HMENU)ID_BUTTON9, GetModuleHandle(NULL), NULL);
	hwndButton = CreateWindow(TEXT("button"), TEXT("Zatwierdü"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 335, 100, 30, hWnd, (HMENU)ID_BUTTON10, GetModuleHandle(NULL), NULL);

	//Przyciski wyboru przedzia≥ki czasowej

	hwndButton = CreateWindow(TEXT("button"), TEXT("<<"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 470, 30, 30, 30, hWnd, (HMENU)ID_BUTTON11, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("<"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 505, 30, 30, 30, hWnd, (HMENU)ID_BUTTON12, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT(">"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 30, 30, 30, hWnd, (HMENU)ID_BUTTON13, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT(">>"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 575, 30, 30, 30, hWnd, (HMENU)ID_BUTTON14, GetModuleHandle(NULL), NULL);
	
	OnCreate(hWnd);

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
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1 :
			if (liczba_zbednych_pomiarow >= 5)
			{
				liczba_zbednych_pomiarow = liczba_zbednych_pomiarow - 5;
				repaintLiczba_zbednych_pomiarow(hWnd, hdc, ps);
			}
			break;
		case ID_BUTTON2:
			if (liczba_zbednych_pomiarow > 0)
			{
				liczba_zbednych_pomiarow = liczba_zbednych_pomiarow - 1;
				repaintLiczba_zbednych_pomiarow(hWnd, hdc, ps);
			}
			break;
		case ID_BUTTON3:
			if (liczba_zbednych_pomiarow < liczba_pomiarow)
			{
				liczba_zbednych_pomiarow = liczba_zbednych_pomiarow + 1;
				repaintLiczba_zbednych_pomiarow(hWnd, hdc, ps);
			}
			break;
		case ID_BUTTON4:
			if ((liczba_zbednych_pomiarow+4) < liczba_pomiarow)
			{
				liczba_zbednych_pomiarow = liczba_zbednych_pomiarow + 5;
				repaintLiczba_zbednych_pomiarow(hWnd, hdc, ps);
			}
			break;
		case ID_BUTTON5:
			if (pauza == true)
			{
				pauza = false;
				SetTimer(hWnd, TMR_1, 1, 0);
			}
			else
			{
				pauza = true;
				KillTimer(hWnd, TMR_1);
			}
			break;
		case ID_BUTTON6:
			pauza = true;
			KillTimer(hWnd, TMR_1);
			dane.erase(dane.begin() + 2, dane.end()); 
			value = 0;
			plik_dane.close();
			plik_dane.open("D:\\outputCatapult01.log", std::ios::in);
			break;
		case ID_BUTTON7:
			osie = 0;
			break;
		case ID_BUTTON8:
			osie = 1;
			break;
		case ID_BUTTON9:
			osie = 2;
			break;
		case ID_BUTTON10:
			otwieranie_pliku(plik_dane);
			break;
		case ID_BUTTON11:
			if(zmiana_przedzia≥ki >= 0.15000)
			{
				zmiana_przedzia≥ki = zmiana_przedzia≥ki - 0.125;
				repaintPrzedzialka_czasowa(hWnd, hdc, ps);
			}
			break;
		case ID_BUTTON12:
			if(zmiana_przedzia≥ki >= 0.05000)
			{
				zmiana_przedzia≥ki = zmiana_przedzia≥ki - 0.025;
				repaintPrzedzialka_czasowa(hWnd, hdc, ps);
			}
			break;
		case ID_BUTTON13:
			if ((liczba_pomiarow - liczba_zbednych_pomiarow) / (value + (zmiana_przedzia≥ki + 0.025) / 0.025) > 1)
			{
				zmiana_przedzia≥ki = zmiana_przedzia≥ki + 0.025;
				repaintPrzedzialka_czasowa(hWnd, hdc, ps);
			}
			break;
		case ID_BUTTON14:
			if ((liczba_pomiarow - liczba_zbednych_pomiarow) / (value + (zmiana_przedzia≥ki + 0.125) / 0.025) > 1)
			{
				zmiana_przedzia≥ki = zmiana_przedzia≥ki + 0.125;
				repaintPrzedzialka_czasowa(hWnd, hdc, ps);
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		repaintWindow(hWnd, hdc, ps, NULL);
		//repaintData(hWnd, hdc, ps);
		repaintDane_niezalezne_od_czasu(hWnd, hdc, ps);
		repaintLiczba_zbednych_pomiarow(hWnd, hdc, ps);
		repaintPrzedzialka_czasowa(hWnd, hdc, ps);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			
			if (dane.size() > value)
			{
				if(value > 2)repaintWindow(hWnd, hdc, ps, &drawArea2);
				repaintData(hWnd, hdc, ps);
				Sleep(24 + (zmiana_przedzia≥ki-0.025)*1000);
				value=value+(zmiana_przedzia≥ki/0.025);
			}
			else KillTimer(hWnd, TMR_1);
			
			break;
		}

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
