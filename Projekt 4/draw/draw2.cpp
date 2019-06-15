// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <cstdio>
#include <iostream>
#include <math.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <algorithm>

#define MAX_LOADSTRING 100
#define TMR_1 1
#define TMR_2 2
#define TMR_3 3

#define M_PI 3.14159265358979323846
#define liczba_danych_w_probce 12
#define promien_kola_wskazowek 80
#define liczba_pomiarow 625

#define ilosc_pieter 5
#define maksymalna_ilosc_oczekujacych 10
#define maksymalny_udzwig_windy 600                       //w kg
#define srednia_waga_czlowieka 70                         //w kg
#define czas_po_ktorym_pusta_winda_zjezdza_na_parter 5000 //w ms

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
bool pauza = true;
int liczba_zbednych_pomiarow = 0;
int osie = 0;
double blad = 0;
long double zmiana_przedzia³ki = 0.025;

int tablica_oczekujacych[ilosc_pieter][maksymalna_ilosc_oczekujacych];
std::vector<int> tablica_pasazerow;
int aktualny_numer_pietra_ludzik = 0;
int pozycja_w_kolejce = 0;
int aktualna_pozycja_windy = 750;
int docelowe_pietro_windy;
bool powrot = false;




// Dane z pliku
std::vector<double> dane;
using namespace std;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);
/*
void Rysowanie_danych(HDC hdc, double angle)
{
	Graphics graphics(hdc);
	
	Gdiplus::Font *czcionka = new Font(L"Times new roman", 16);
	PointF A = PointF(460, 120);
	PointF B = PointF(460, 145);
	PointF C = PointF(460, 70);
	PointF D = PointF(460, 95);

	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	graphics.DrawString(L"Aktualny k¹t (po kalibracji):", 29, czcionka, A, &cyfry);

	std::wstring kat;

	graphics.DrawString(L"Aktualny k¹t (przed kalibracj¹):", 32, czcionka, C, &cyfry);

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

	graphics.DrawString(L"Liczba zbêdnych próbek:", 23, czcionka, A, &cyfry);

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

	graphics.DrawString(L"Wybór osi:", 11, czcionka, A, &cyfry);
}

void Rysowanie_przedzialki_czasowej(HDC hdc)
{
	Graphics graphics(hdc);

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 16);

	PointF A = PointF(460, 5);
	PointF B = PointF(725, 5);
	PointF C = PointF(640, 5);
	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	graphics.DrawString(L"Przedzia³ka czasowa:", 22, czcionka, A, &cyfry);
	graphics.DrawString(L"s", 1, czcionka, B, &cyfry);

	std::wstring liczba_string;

	liczba_string = std::to_wstring(zmiana_przedzia³ki);

	graphics.DrawString(liczba_string.c_str(), liczba_string.length(), czcionka, C, &cyfry);
}
*/

void rysowanie_ludzika(HDC hdc, int jakis_x, int jakis_y, int numer_ludzika)
{
	Graphics graphics(hdc);

	Pen ludzik(Color(255, 0, 0, 0));
	Pen cyfry(Color(255, 255, 255, 255));

	for (int i = 0; i < 20; i++)graphics.DrawEllipse(&ludzik, jakis_x+15 - (i / 2), jakis_y - (i / 2), i, i);

	for (int j = 0; j <= 5; j++)
	{
		graphics.DrawRectangle(&ludzik, jakis_x, jakis_y + 10, 30, j);
		graphics.DrawRectangle(&ludzik, jakis_x, jakis_y + 10, j, 20);
		graphics.DrawRectangle(&ludzik, jakis_x+25, jakis_y + 10, j, 20);
		graphics.DrawRectangle(&ludzik, jakis_x + 8, jakis_y + 35, j, 25);
		graphics.DrawRectangle(&ludzik, jakis_x + 17, jakis_y + 35, j, 25);
	}

	for (int j = 0; j <= 14; j++)graphics.DrawRectangle(&ludzik, jakis_x + 8, jakis_y + 15, j, 20);

	if (numer_ludzika == 0)
	{
		for (int i = 4; i < 7; i++)graphics.DrawArc(&cyfry, jakis_x + 13 - (i / 2), jakis_y + 18 - (i / 2), 4 + i, 8 + i, 0, 360);
	};

	if (numer_ludzika == 1)
	{
		for (int i = 0; i < 2; i++)graphics.DrawRectangle(&cyfry, jakis_x + 17, jakis_y + 18, i, 12);
		for(int i = 0; i<3; i++)graphics.DrawLine(&cyfry, jakis_x + 13, jakis_y + 22+ i, jakis_x + 18, jakis_y + 17 + i);
	};

	if (numer_ludzika == 2)
	{
		for (int i = 4; i < 7; i++)graphics.DrawArc(&cyfry, jakis_x + 13 - (i / 2), jakis_y + 18 - (i / 2), 4 + i, 4 + i, 180, 180);
		for (int i = 0; i < 2; i++)graphics.DrawRectangle(&cyfry, jakis_x + 11, jakis_y + 28, 9, i);
		for (int i = 0; i < 3; i++)graphics.DrawLine(&cyfry, jakis_x + 12, jakis_y + 27 + i, jakis_x + 20, jakis_y + 19 + i);
	};

	if (numer_ludzika == 3)
	{
		for (int i = 4; i < 7; i++)graphics.DrawArc(&cyfry, jakis_x + 13 - (i / 2), jakis_y + 16 - (i / 2), 4 + i, 4 + i, 190, 270);
		for (int i = 4; i < 7; i++)graphics.DrawArc(&cyfry, jakis_x + 13 - (i / 2), jakis_y + 25 - (i / 2), 4 + i, 4 + i, 270, 270);
	};

	if (numer_ludzika == 4)
	{
		for (int i = 0; i < 2; i++)
		{
			graphics.DrawRectangle(&cyfry, jakis_x + 17, jakis_y + 20, i, 12);
			graphics.DrawRectangle(&cyfry, jakis_x + 12, jakis_y + 26, 8, i);
		}
		for (int i = 0; i < 3; i++)graphics.DrawLine(&cyfry, jakis_x + 12, jakis_y + 26 + i, jakis_x + 18, jakis_y + 17 + i);
	};
};

void rysowanie_pieter(HDC hdc)
{
	Graphics graphics(hdc);

	Pen pietra(Color(255, 0, 0, 0));

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 12);

	Gdiplus::SolidBrush litery(Gdiplus::Color(255, 0, 0, 0));

	PointF A;
	
	for (int i = 0; i < 5; i++)
	{
		A = PointF(5, 15 + 150 * i);
		graphics.DrawString(L"Wybór piêtra:", 13, czcionka, A, &litery);
	}
		A = PointF(35 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, 15);
		graphics.DrawString(L"4 Piêtro", 8, czcionka, A, &litery);
		A = PointF(35 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, 165);
		graphics.DrawString(L"3 Piêtro", 8, czcionka, A, &litery);
		A = PointF(35 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, 315);
		graphics.DrawString(L"2 Piêtro", 8, czcionka, A, &litery);
		A = PointF(35 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, 465);
		graphics.DrawString(L"1 Piêtro", 8, czcionka, A, &litery);
		A = PointF(35 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, 615);
		graphics.DrawString(L"Parter", 6, czcionka, A, &litery);

//Podloga oczekujacych
	for (int i = 0; i <= 600; i = i + 150)
	{
		for (int j = 0; j < 6; j++)graphics.DrawRectangle(&pietra, 0, 750 - i, 10 + maksymalna_ilosc_oczekujacych * 40, j);
	};

//Szyb
	for (int i = 0; i < 6; i++)
	{
		graphics.DrawRectangle(&pietra, 10 + maksymalna_ilosc_oczekujacych * 40, 0, i, 755);
		graphics.DrawRectangle(&pietra, 25 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy/srednia_waga_czlowieka)*40, 0, i, 755);
	}
	
//Podloga wychodzacych
	for (int i = 0; i <= 600; i = i + 150)
	{
		for (int j = 0; j < 6; j++)graphics.DrawRectangle(&pietra, 30 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, 750 - i, 130, j);
	};
	
};

void rysowanie_windy(HDC hdc)
{
	Graphics graphics(hdc);

	Pen pietra(Color(255, 0, 0, 0));

	//Podloga windy
	for (int i = 0; i < 6; i++)graphics.DrawRectangle(&pietra, 15 + maksymalna_ilosc_oczekujacych * 40, aktualna_pozycja_windy, 10 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, i);
	//Sufit windy
	for (int i = 0; i < 6; i++)graphics.DrawRectangle(&pietra, 15 + maksymalna_ilosc_oczekujacych * 40, aktualna_pozycja_windy - 150, 10 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, i);
};

void otwieranie_drzwi_windy(HDC hdc, int pozycja_x, int pozycja_y)
{
	Graphics graphics(hdc);

	Pen drzwi(Color(255, 255, 255, 255));

	graphics.DrawLine(&drzwi, pozycja_x, pozycja_y, pozycja_x + 5, pozycja_y);
};

void zamykanie_drzwi_windy(HDC hdc, int pozycja_x, int pozycja_y)
{
	Graphics graphics(hdc);

	Pen drzwi(Color(255, 0, 0, 0));

	graphics.DrawLine(&drzwi, pozycja_x, pozycja_y, pozycja_x + 5, pozycja_y);
};
/*
void rysowanie_kompasu(HDC hdc, double angle_1)
{
	angle_1 = angle_1 + blad;

	if (angle_1 > 180)angle_1 = angle_1 - 360;
	if (angle_1 < -180)angle_1 = angle_1 + 360;

	double angle = angle_1;
	
	Graphics kompas(hdc);
	SwapBuffers(hdc);

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
*/
void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea==NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, NULL, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);

	rysowanie_pieter(hdc);
	rysowanie_windy(hdc);
	
	EndPaint(hWnd, &ps);
}

void repaint_ludzik(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	RECT drawArea;

	drawArea = { (maksymalna_ilosc_oczekujacych - pozycja_w_kolejce) * 40 - 30, 675 - aktualny_numer_pietra_ludzik * 150, (maksymalna_ilosc_oczekujacych - pozycja_w_kolejce) * 40 + 10, 745 - aktualny_numer_pietra_ludzik * 150 };
				
	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);

	rysowanie_ludzika(hdc,(maksymalna_ilosc_oczekujacych - pozycja_w_kolejce )*40 - 30, 685-aktualny_numer_pietra_ludzik*150, tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce]);

	EndPaint(hWnd, &ps);
}

/*
void repaint_wsiadajacy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, int ilosc_wsiadajacych, int pozycja_x)
{
	RECT drawArea;

	drawArea = { (maksymalna_ilosc_oczekujacych - ilosc_wsiadajacych) * 40 - 30 + pozycja_x, aktualna_pozycja_windy - 75, (maksymalna_ilosc_oczekujacych) * 40 + 10 + pozycja_x, aktualna_pozycja_windy - 4 };

	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);

	for (int i = 0; i < ilosc_wsiadajacych; i++)rysowanie_ludzika(hdc, (maksymalna_ilosc_oczekujacych - i) * 40 - 30 + pozycja_x, aktualna_pozycja_windy - 65, tablica_oczekujacych[ilosc_pieter - (aktualna_pozycja_windy / 150)][i]);

	Sleep(100);

	EndPaint(hWnd, &ps);
}
*/

void wysiadanie_pasazerow()
{
	for (int i = 0; i < tablica_pasazerow.size(); i++)
	{
		if (tablica_pasazerow[i] == docelowe_pietro_windy)
		{
			tablica_pasazerow.erase(tablica_pasazerow.begin() + i);
			i--;
		}
	}
}

int sortowanie_wysiadaj¹cych()
{
	int rozmiar = 0;
	for (int i = 0; i < tablica_pasazerow.size(); i++)
	{
		if (tablica_pasazerow[i] == docelowe_pietro_windy)
		{
			int j = 0;
			rozmiar++;
			while (tablica_pasazerow[j] == docelowe_pietro_windy && j < i)j++;
			
			int swap_tab = tablica_pasazerow[j];
			tablica_pasazerow[j] = tablica_pasazerow[i];
			tablica_pasazerow[i] = swap_tab;
		}
	}

	return rozmiar;
}

bool sprawdzanie_czy_ktos_wysiada()
{
	bool wysiada = false;
	for (int i = 0; i < tablica_pasazerow.size() && wysiada == false; i++)
	{
		if (tablica_pasazerow[i] == docelowe_pietro_windy)wysiada = true;
	}

	return wysiada;
}

void repaint_winda(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	RECT drawArea;
	drawArea = { 16 + maksymalna_ilosc_oczekujacych * 40, aktualna_pozycja_windy + 11, 25 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, aktualna_pozycja_windy - 157 };

	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);

	rysowanie_windy(hdc);

	for (int i = 0; i < tablica_pasazerow.size(); i++)rysowanie_ludzika(hdc, maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 - 20 - i * 40, aktualna_pozycja_windy - 65, tablica_pasazerow[i]);
	Sleep(50);
	EndPaint(hWnd, &ps);
}

void repaint_otwieranie_drzwi_windy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, int pozycja_x)
{
	RECT drawArea;

	for (int i = 0; i < 144; i++)
	{
		drawArea = { pozycja_x, aktualna_pozycja_windy - i, pozycja_x + 6, aktualna_pozycja_windy - i - 1 };

		InvalidateRect(hWnd, &drawArea, TRUE);
		hdc = BeginPaint(hWnd, &ps);

		otwieranie_drzwi_windy(hdc, pozycja_x, aktualna_pozycja_windy - i);
		Sleep(5);

		EndPaint(hWnd, &ps);
	}
}

void repaint_zamykanie_drzwi_windy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, int pozycja_x)
{
	RECT drawArea;

	for (int i = 144; i > 0; i--)
	{
		drawArea = { pozycja_x, aktualna_pozycja_windy - i, pozycja_x + 6, aktualna_pozycja_windy - i + 1 };

		InvalidateRect(hWnd, &drawArea, TRUE);
		hdc = BeginPaint(hWnd, &ps);

		zamykanie_drzwi_windy(hdc, pozycja_x, aktualna_pozycja_windy - i);
		Sleep(5);

		EndPaint(hWnd, &ps);
	}
}

int sprawdzanie_ilosci_oczekujacych_na_pietrze(int pietro)
{
	int i = 0;
	while (tablica_oczekujacych[pietro][i] >= 0 && i < maksymalna_ilosc_oczekujacych)i++;
	return i;

}

void dodawanie_docelowych_pietr_windy(HWND hWnd)
{
	int aktualne_pietro_windy = ilosc_pieter - aktualna_pozycja_windy / 150;

	int preferencja = 0;			//"+" -> góra; "-" -> dó³;
	
	docelowe_pietro_windy = aktualne_pietro_windy;

	for (int i = 1; i < 4 && docelowe_pietro_windy == aktualne_pietro_windy; i++)
	{
		for (int j = 0; j < tablica_pasazerow.size() && docelowe_pietro_windy == aktualne_pietro_windy; j++)
		{
			if (abs(tablica_pasazerow[j] - aktualne_pietro_windy) == i)docelowe_pietro_windy = tablica_pasazerow[j];
		}

		if (docelowe_pietro_windy == aktualne_pietro_windy && i == 1)
		{	
			for (int j = 0; j < tablica_pasazerow.size(); j++)
			{
				if (tablica_pasazerow[j] > aktualne_pietro_windy)preferencja++;
				else if (tablica_pasazerow[j] < aktualne_pietro_windy)preferencja--;
			}
		}

		if (aktualne_pietro_windy - i >= 0 && aktualne_pietro_windy - i <= 4 && docelowe_pietro_windy == aktualne_pietro_windy && preferencja <= 0)
		{
			if(sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualne_pietro_windy - i) > 0)docelowe_pietro_windy = aktualne_pietro_windy - i;
		}

		if (aktualne_pietro_windy + i >= 0 && aktualne_pietro_windy + i <= 4 && docelowe_pietro_windy == aktualne_pietro_windy && preferencja >= 0)
		{
			if (sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualne_pietro_windy + i) > 0)docelowe_pietro_windy = aktualne_pietro_windy + i;
		}
	}
	
	if (aktualna_pozycja_windy != 750 && docelowe_pietro_windy == aktualne_pietro_windy)
	{
		SetTimer(hWnd, TMR_2, czas_po_ktorym_pusta_winda_zjezdza_na_parter, 0);
		KillTimer(hWnd, TMR_1);
	}
}

void wsiadanie_pasazerow()
{
	int rozmiar_tablicy_przed = tablica_pasazerow.size();

	if (sprawdzanie_ilosci_oczekujacych_na_pietrze(docelowe_pietro_windy) < (maksymalny_udzwig_windy / srednia_waga_czlowieka)- rozmiar_tablicy_przed)
	{
		for (int i = 0; i < sprawdzanie_ilosci_oczekujacych_na_pietrze(docelowe_pietro_windy); i++)tablica_pasazerow.push_back(tablica_oczekujacych[docelowe_pietro_windy][i]);
	}
	else
	{
		for (int i = 0; i < (maksymalny_udzwig_windy / srednia_waga_czlowieka) - rozmiar_tablicy_przed; i++)tablica_pasazerow.push_back(tablica_oczekujacych[docelowe_pietro_windy][i]);
	}
	
	for (int i = 0; i < (maksymalny_udzwig_windy / srednia_waga_czlowieka) - tablica_pasazerow.size() + rozmiar_tablicy_przed; i++)
	{
		tablica_oczekujacych[docelowe_pietro_windy][i] = tablica_oczekujacych[docelowe_pietro_windy][i + tablica_pasazerow.size() - rozmiar_tablicy_przed];
	}

	for (int i = (maksymalny_udzwig_windy / srednia_waga_czlowieka) - 1; i >= (maksymalny_udzwig_windy / srednia_waga_czlowieka) - (tablica_pasazerow.size() - rozmiar_tablicy_przed); i--)
	{
		tablica_oczekujacych[docelowe_pietro_windy][i] = -1;
	}
}

void repaint_pozostali_oczekujacy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, int ilosc_wsiadajacych)
{
	int ilosc_pozosta³ych_oczekujacych = sprawdzanie_ilosci_oczekujacych_na_pietrze(docelowe_pietro_windy);
	
	if (ilosc_pozosta³ych_oczekujacych > 0)
	{
		RECT drawArea;

		drawArea = { 0, aktualna_pozycja_windy - 75, (maksymalna_ilosc_oczekujacych) * 40 + 10, aktualna_pozycja_windy - 4 };

		InvalidateRect(hWnd, &drawArea, TRUE);
		hdc = BeginPaint(hWnd, &ps);

		for (int j = 0; j < ilosc_pozosta³ych_oczekujacych; j++)rysowanie_ludzika(hdc, (maksymalna_ilosc_oczekujacych - j) * 40 - 30, aktualna_pozycja_windy - 65, tablica_oczekujacych[ilosc_pieter - (aktualna_pozycja_windy / 150)][j]);

		Sleep(100);

		EndPaint(hWnd, &ps);
	}
}

void repaint_wsiadajacy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	int ilosc_wsiadajacych = sprawdzanie_ilosci_oczekujacych_na_pietrze(docelowe_pietro_windy);

	if (ilosc_wsiadajacych > 0)
	{
		repaint_otwieranie_drzwi_windy(hWnd, hdc, ps, maksymalna_ilosc_oczekujacych * 40 + 10);

		if (ilosc_wsiadajacych > (maksymalny_udzwig_windy / srednia_waga_czlowieka) - tablica_pasazerow.size())
		{
			ilosc_wsiadajacych = (maksymalny_udzwig_windy / srednia_waga_czlowieka) - tablica_pasazerow.size();
		}

		RECT drawArea;

		for (int i = 0; i < ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - tablica_pasazerow.size()) * 40 + 15; i = i + 10)
		{
			drawArea = { (maksymalna_ilosc_oczekujacych - ilosc_wsiadajacych) * 40 + i, aktualna_pozycja_windy - 75, (maksymalna_ilosc_oczekujacych) * 40 + 10 + i, aktualna_pozycja_windy - 4 };

			InvalidateRect(hWnd, &drawArea, TRUE);
			hdc = BeginPaint(hWnd, &ps);

			for (int j = 0; j < ilosc_wsiadajacych; j++)rysowanie_ludzika(hdc, (maksymalna_ilosc_oczekujacych - j) * 40 - 30 + i, aktualna_pozycja_windy - 65, tablica_oczekujacych[ilosc_pieter - (aktualna_pozycja_windy / 150)][j]);

			Sleep(100);

			EndPaint(hWnd, &ps);
		};

		repaint_zamykanie_drzwi_windy(hWnd, hdc, ps, maksymalna_ilosc_oczekujacych * 40 + 10);

		wsiadanie_pasazerow();

		repaint_pozostali_oczekujacy(hWnd, hdc, ps, ilosc_wsiadajacych);
	}
}

void repaint_wysiadajacy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	if (sprawdzanie_czy_ktos_wysiada() == true)
	{
		Sleep(200);
		int ilosc_wysiadajacych = sortowanie_wysiadaj¹cych();
		repaint_winda(hWnd, hdc, ps);
		Sleep(200);
		repaint_otwieranie_drzwi_windy(hWnd, hdc, ps, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 25);


		RECT drawArea;

		for (int i = 0; i < 150 + ilosc_wysiadajacych * 40; i = i + 5)
		{
			drawArea = { ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - ilosc_wysiadajacych) * 40 + maksymalna_ilosc_oczekujacych * 40 + 15 + i, aktualna_pozycja_windy - 75, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 15 + i, aktualna_pozycja_windy - 4 };

			InvalidateRect(hWnd, &drawArea, TRUE);
			hdc = BeginPaint(hWnd, &ps);

			for (int j = 0; j < ilosc_wysiadajacych; j++)rysowanie_ludzika(hdc, ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - j) * 40 + maksymalna_ilosc_oczekujacych * 40 - 20 + i, aktualna_pozycja_windy - 65, tablica_pasazerow[j]);

			Sleep(100);
		}

		wysiadanie_pasazerow();

		repaint_winda(hWnd, hdc, ps);

		repaint_zamykanie_drzwi_windy(hWnd, hdc, ps, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 25);

		EndPaint(hWnd, &ps);
	}


}

int OnCreate(HWND window)
{
	for (int i = 0; i < ilosc_pieter; i++)
	{
		for (int j = 0; j < maksymalna_ilosc_oczekujacych; j++)tablica_oczekujacych[i][j] = -1;
	}
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
		0, 0, 160 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, 815, NULL, NULL, hInstance, NULL);
	// create button and store the handle                                                       

	//Przyciski Piêter

	//Parter
	hwndButton = CreateWindow(TEXT("button"), TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 615, 20, 20, hWnd, (HMENU)ID_BUTTON_01, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 125, 615, 20, 20, hWnd, (HMENU)ID_BUTTON_02, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 615, 20, 20, hWnd, (HMENU)ID_BUTTON_03, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 615, 20, 20, hWnd, (HMENU)ID_BUTTON_04, GetModuleHandle(NULL), NULL);

	//1 Pietro
	hwndButton = CreateWindow(TEXT("button"), TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 465, 20, 20, hWnd, (HMENU)ID_BUTTON_10, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 125, 465, 20, 20, hWnd, (HMENU)ID_BUTTON_12, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 465, 20, 20, hWnd, (HMENU)ID_BUTTON_13, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 465, 20, 20, hWnd, (HMENU)ID_BUTTON_14, GetModuleHandle(NULL), NULL);

	//2 Pietro
	hwndButton = CreateWindow(TEXT("button"), TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 315, 20, 20, hWnd, (HMENU)ID_BUTTON_20, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 125, 315, 20, 20, hWnd, (HMENU)ID_BUTTON_21, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 315, 20, 20, hWnd, (HMENU)ID_BUTTON_23, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 315, 20, 20, hWnd, (HMENU)ID_BUTTON_24, GetModuleHandle(NULL), NULL);

	//3 Pietro
	hwndButton = CreateWindow(TEXT("button"), TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 165, 20, 20, hWnd, (HMENU)ID_BUTTON_30, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 125, 165, 20, 20, hWnd, (HMENU)ID_BUTTON_31, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 165, 20, 20, hWnd, (HMENU)ID_BUTTON_32, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("4"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 165, 20, 20, hWnd, (HMENU)ID_BUTTON_34, GetModuleHandle(NULL), NULL);
	
	//4 Pietro
	hwndButton = CreateWindow(TEXT("button"), TEXT("0"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 15, 20, 20, hWnd, (HMENU)ID_BUTTON_40, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 125, 15, 20, 20, hWnd, (HMENU)ID_BUTTON_41, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 15, 20, 20, hWnd, (HMENU)ID_BUTTON_42, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 15, 20, 20, hWnd, (HMENU)ID_BUTTON_43, GetModuleHandle(NULL), NULL);

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
		
//Parter
		case ID_BUTTON_01:
			aktualny_numer_pietra_ludzik = 0;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 1;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;
		
		case ID_BUTTON_02:
			aktualny_numer_pietra_ludzik = 0;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 2;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;
		
		case ID_BUTTON_03:
			aktualny_numer_pietra_ludzik = 0;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 3;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;
		
		case ID_BUTTON_04:
			aktualny_numer_pietra_ludzik = 0;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 4;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0); 
			break;

//1 Pietro
		case ID_BUTTON_10:
			aktualny_numer_pietra_ludzik = 1;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 0;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_12:
			aktualny_numer_pietra_ludzik = 1;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 2;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_13:
			aktualny_numer_pietra_ludzik = 1;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 3;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_14:
			aktualny_numer_pietra_ludzik = 1;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 4;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

//2 Pietro
		case ID_BUTTON_20:
			aktualny_numer_pietra_ludzik = 2;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 0;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_21:
			aktualny_numer_pietra_ludzik = 2;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 1;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_23:
			aktualny_numer_pietra_ludzik = 2;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 3;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_24:
			aktualny_numer_pietra_ludzik = 2;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 4;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

//3 Pietro
		case ID_BUTTON_30:
			aktualny_numer_pietra_ludzik = 3;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 0;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_31:
			aktualny_numer_pietra_ludzik = 3;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 1;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_32:
			aktualny_numer_pietra_ludzik = 3;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 2;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_34:
			aktualny_numer_pietra_ludzik = 3;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 4;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

//4 Pietro
		case ID_BUTTON_40:
			aktualny_numer_pietra_ludzik = 4;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 0;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_41:
			aktualny_numer_pietra_ludzik = 4;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 1;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_42:
			aktualny_numer_pietra_ludzik = 4;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 2;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
			break;

		case ID_BUTTON_43:
			aktualny_numer_pietra_ludzik = 4;
			pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
			if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = 3;
				repaint_ludzik(hWnd, hdc, ps);
			}
			dodawanie_docelowych_pietr_windy(hWnd);
			SetTimer(hWnd, TMR_1, 1, 0);
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
		//repaintDane_niezalezne_od_czasu(hWnd, hdc, ps);
		//repaintLiczba_zbednych_pomiarow(hWnd, hdc, ps);
		//repaintPrzedzialka_czasowa(hWnd, hdc, ps);

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
			
			if (aktualna_pozycja_windy > (ilosc_pieter - docelowe_pietro_windy)*150)
			{
				aktualna_pozycja_windy = aktualna_pozycja_windy - 5;
				repaint_winda(hWnd, hdc, ps);
			}
			else if (aktualna_pozycja_windy < (ilosc_pieter - docelowe_pietro_windy) * 150)
			{
				aktualna_pozycja_windy = aktualna_pozycja_windy + 5;
				repaint_winda(hWnd, hdc, ps);
			}
			else
			{
				
				repaint_wysiadajacy(hWnd, hdc, ps);
				
				repaint_wsiadajacy(hWnd, hdc, ps);

				dodawanie_docelowych_pietr_windy(hWnd);
			}
			break;

		case TMR_2:
			//force window to repaint
			if (docelowe_pietro_windy == ilosc_pieter - aktualna_pozycja_windy / 150)
			{
				docelowe_pietro_windy = 0;
				SetTimer(hWnd, TMR_1, 1, 0);
			}
			KillTimer(hWnd, TMR_2);
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
