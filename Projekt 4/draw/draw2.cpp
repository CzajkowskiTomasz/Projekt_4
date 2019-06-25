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
#define TMR_4 4

#define M_PI 3.14159265358979323846
#define liczba_danych_w_probce 12
#define promien_kola_wskazowek 80
#define liczba_pomiarow 625

#define ilosc_pieter 5
#define maksymalna_ilosc_oczekujacych 10
#define maksymalny_udzwig_windy 600                       //w kg
#define srednia_waga_czlowieka 70                         //w kg
#define czas_po_ktorym_pusta_winda_zjezdza_na_parter 5    //w s

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;

// buttons
HWND hwndButton;

//Zmienne globalne
int tablica_oczekujacych[ilosc_pieter][maksymalna_ilosc_oczekujacych];
std::vector<int> tablica_pasazerow;
int aktualny_numer_pietra_ludzik = 0;
int pozycja_w_kolejce = 0;
int aktualna_pozycja_windy = 750;
int docelowe_pietro_windy = 0;
int czas_do_powrotu_windy = czas_po_ktorym_pusta_winda_zjezdza_na_parter;

int koncowa_pozycja_x_animacji = 0;
int pozycja_animacji_x = 0;
int ilosc_osob_animacja = 0;
bool wsiadanie = false;
bool wysiadanie = false;
bool dodawanie_pietr = false;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

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
	
	for (int i = 1; i < 5; i++)
	{
		A = PointF(5, 15 + 150 * i);
		graphics.DrawString(L"Wybór piêtra:", 13, czcionka, A, &litery);
	}

	A = PointF(5, 40);
	graphics.DrawString(L"Wybór piêtra:", 13, czcionka, A, &litery);

	A = PointF(5, 2);
	graphics.DrawString(L"Czas pozosta³y do zjazdu na parter:", 35, czcionka, A, &litery);

	A = PointF(256, 2);
	graphics.DrawString(L"s", 1, czcionka, A, &litery);

	A = PointF(5, 18);
	graphics.DrawString(L"Aktualna waga pasa¿erów:", 24, czcionka, A, &litery);

	A = PointF(215, 18);
	graphics.DrawString(L"kg", 2, czcionka, A, &litery);

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

void repaint_ludzik(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	RECT drawArea;

	drawArea = { (maksymalna_ilosc_oczekujacych - pozycja_w_kolejce) * 40 - 30, 675 - aktualny_numer_pietra_ludzik * 150, (maksymalna_ilosc_oczekujacych - pozycja_w_kolejce) * 40 + 10, 745 - aktualny_numer_pietra_ludzik * 150 };
				
	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);

	rysowanie_ludzika(hdc,(maksymalna_ilosc_oczekujacych - pozycja_w_kolejce )*40 - 30, 685-aktualny_numer_pietra_ludzik*150, tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce]);

	EndPaint(hWnd, &ps);
}

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

void rysowanie_wagi_pasazerow(HDC hdc, int waga_pasazerow)
{
	Graphics graphics(hdc);

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 13);
	PointF A = PointF(185, 18);

	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	std::wstring string_waga_pasazerow;
	
	string_waga_pasazerow = std::to_wstring(waga_pasazerow);

	graphics.DrawString(string_waga_pasazerow.c_str(), string_waga_pasazerow.length(), czcionka, A, &cyfry);
}

void rysowanie_czasu_do_powrotu_windy(HDC hdc, int czas_do_powrotu_windy)
{
	Graphics graphics(hdc);

	Gdiplus::Font *czcionka = new Font(L"Times new roman", 13);
	PointF A = PointF(245, 2);

	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	std::wstring string_czas_do_powrotu_windy;

	string_czas_do_powrotu_windy = std::to_wstring(czas_do_powrotu_windy);

	graphics.DrawString(string_czas_do_powrotu_windy.c_str(), string_czas_do_powrotu_windy.length(), czcionka, A, &cyfry);
}

void repaint_rysowanie_czasu_do_powrotu_windy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, int czas_do_powrotu_windy)
{
	RECT drawArea;
	drawArea = { 240, 0, 260, 20 };

	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	rysowanie_czasu_do_powrotu_windy(hdc, czas_do_powrotu_windy);
	EndPaint(hWnd, &ps);
}

void repaint_winda(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	RECT drawArea;
	drawArea = { 16 + maksymalna_ilosc_oczekujacych * 40, aktualna_pozycja_windy + 11, 25 + maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40, aktualna_pozycja_windy - 157 };

	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);

	rysowanie_windy(hdc);

	for (int i = 0; i < tablica_pasazerow.size(); i++)rysowanie_ludzika(hdc, maksymalna_ilosc_oczekujacych * 40 + (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 - 20 - i * 40, aktualna_pozycja_windy - 65, tablica_pasazerow[i]);
	
	EndPaint(hWnd, &ps);

	drawArea = { 180, 20, 215, 35 };
	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	if (tablica_pasazerow.size() == 0)
	{
		rysowanie_wagi_pasazerow(hdc, 0);
	}
	else 
	{
		rysowanie_wagi_pasazerow(hdc, tablica_pasazerow.size()*srednia_waga_czlowieka);
	}
	EndPaint(hWnd, &ps);
	Sleep(50);
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
		Sleep(2);

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
		Sleep(2);

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

	for (int i = 1; i <= 4 && docelowe_pietro_windy == aktualne_pietro_windy; i++)
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
	dodawanie_pietr = true;
	//if (aktualna_pozycja_windy != 750 && docelowe_pietro_windy == aktualne_pietro_windy)
	//{
		//SetTimer(hWnd, TMR_2, 1000, 0);
		//KillTimer(hWnd, TMR_1);
		//powrot = true;
	//}
}

void wsiadanie_pasazerow()
{
	int rozmiar_tablicy_przed = tablica_pasazerow.size();
	int ilosc_oczekujacych = sprawdzanie_ilosci_oczekujacych_na_pietrze(docelowe_pietro_windy);
	int ilosc_wolnych_miejsc = (maksymalny_udzwig_windy / srednia_waga_czlowieka) - rozmiar_tablicy_przed;

	if (ilosc_oczekujacych <= ilosc_wolnych_miejsc)
	{
		for (int i = 0; i < ilosc_oczekujacych; i++)tablica_pasazerow.push_back(tablica_oczekujacych[docelowe_pietro_windy][i]);
	}
	else
	{
		for (int i = 0; i < ilosc_wolnych_miejsc; i++)tablica_pasazerow.push_back(tablica_oczekujacych[docelowe_pietro_windy][i]);
	}
	
	for (int i = 0; i < ilosc_oczekujacych - ilosc_wolnych_miejsc; i++)
	{
		tablica_oczekujacych[docelowe_pietro_windy][i] = tablica_oczekujacych[docelowe_pietro_windy][i + ilosc_wolnych_miejsc];
	}

	for (int i = ilosc_oczekujacych - ilosc_wolnych_miejsc; i < ilosc_oczekujacych; i++)
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
	ilosc_osob_animacja = sprawdzanie_ilosci_oczekujacych_na_pietrze(docelowe_pietro_windy);

	if (ilosc_osob_animacja > 0)
	{
		repaint_otwieranie_drzwi_windy(hWnd, hdc, ps, maksymalna_ilosc_oczekujacych * 40 + 10);

		if (ilosc_osob_animacja > (maksymalny_udzwig_windy / srednia_waga_czlowieka) - tablica_pasazerow.size())
		{
			ilosc_osob_animacja = (maksymalny_udzwig_windy / srednia_waga_czlowieka) - tablica_pasazerow.size();
		}

		koncowa_pozycja_x_animacji = ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - tablica_pasazerow.size()) * 40 + 10;

		KillTimer(hWnd, TMR_1);
		//KillTimer(hWnd, TMR_2);
		SetTimer(hWnd, TMR_3, 1, 0);

		/*
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
		*/
	}
	else wsiadanie = true;
}

void repaint_wysiadajacy(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	if (sprawdzanie_czy_ktos_wysiada() == true)
	{
		//powrot = false;
		
		ilosc_osob_animacja = sortowanie_wysiadaj¹cych();
		repaint_winda(hWnd, hdc, ps);
		Sleep(200);
		repaint_otwieranie_drzwi_windy(hWnd, hdc, ps, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 25);
	
		koncowa_pozycja_x_animacji = 150 + ilosc_osob_animacja * 40;

		KillTimer(hWnd, TMR_1);
		//KillTimer(hWnd, TMR_2);
		SetTimer(hWnd, TMR_4, 1, 0);
	/*
		RECT drawArea;

		for (int i = 0; i < 150 + ilosc_wysiadajacych * 40; i = i + 5)
		{
			drawArea = { ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - ilosc_wysiadajacych) * 40 + maksymalna_ilosc_oczekujacych * 40 + 16 + i, aktualna_pozycja_windy - 75, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 15 + i, aktualna_pozycja_windy - 4 };

			InvalidateRect(hWnd, &drawArea, TRUE);
			hdc = BeginPaint(hWnd, &ps);

			for (int j = 0; j < ilosc_wysiadajacych; j++)rysowanie_ludzika(hdc, ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - j) * 40 + maksymalna_ilosc_oczekujacych * 40 - 19 + i, aktualna_pozycja_windy - 65, tablica_pasazerow[j]);

			EndPaint(hWnd, &ps);
			Sleep(100);
		}

		wysiadanie_pasazerow();

		repaint_winda(hWnd, hdc, ps);

		repaint_zamykanie_drzwi_windy(hWnd, hdc, ps, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 25);
	*/
	}
	else wysiadanie = true;
}

void funkcja_przycisk(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, const int numer_1, const int numer_2)
{
	aktualny_numer_pietra_ludzik = numer_1;
	pozycja_w_kolejce = sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik);
	if (pozycja_w_kolejce < maksymalna_ilosc_oczekujacych)
	{
		if (aktualna_pozycja_windy != 750 || numer_1 != 0)
		{
			if (aktualny_numer_pietra_ludzik != ilosc_pieter - aktualna_pozycja_windy / 150 || wysiadanie == false)
			{
				tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = numer_2;
				repaint_ludzik(hWnd, hdc, ps);
			}
		}
		else if (pozycja_w_kolejce == 0)
		{
			tablica_oczekujacych[aktualny_numer_pietra_ludzik][pozycja_w_kolejce] = numer_2;
			repaint_ludzik(hWnd, hdc, ps);
		}
	}
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps)
{
	InvalidateRect(hWnd, NULL, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);

	rysowanie_pieter(hdc);
	rysowanie_windy(hdc);
	EndPaint(hWnd, &ps);

	RECT drawArea = { 180, 20, 215, 35 };
	InvalidateRect(hWnd, &drawArea, TRUE);
	hdc = BeginPaint(hWnd, &ps);
	rysowanie_wagi_pasazerow(hdc, 0);
	EndPaint(hWnd, &ps);
	repaint_rysowanie_czasu_do_powrotu_windy(hWnd, hdc, ps, czas_do_powrotu_windy);
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
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 40, 20, 20, hWnd, (HMENU)ID_BUTTON_40, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 125, 40, 20, 20, hWnd, (HMENU)ID_BUTTON_41, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 40, 20, 20, hWnd, (HMENU)ID_BUTTON_42, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("3"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 40, 20, 20, hWnd, (HMENU)ID_BUTTON_43, GetModuleHandle(NULL), NULL);

	OnCreate(hWnd);
	SetTimer(hWnd, TMR_1, 1, 0);
	SetTimer(hWnd, TMR_2, 1000, 0);

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
			funkcja_przycisk(hWnd, hdc, ps, 0, 1);
			break;
		
		case ID_BUTTON_02:
			funkcja_przycisk(hWnd, hdc, ps, 0, 2);
			break;
		
		case ID_BUTTON_03:
			funkcja_przycisk(hWnd, hdc, ps, 0, 3);
			break;
		
		case ID_BUTTON_04:
			funkcja_przycisk(hWnd, hdc, ps, 0, 4);
			break;

//1 Pietro
		case ID_BUTTON_10:
			funkcja_przycisk(hWnd, hdc, ps, 1, 0);
			break;

		case ID_BUTTON_12:
			funkcja_przycisk(hWnd, hdc, ps, 1, 2);
			break;

		case ID_BUTTON_13:
			funkcja_przycisk(hWnd, hdc, ps, 1, 3);
			break;

		case ID_BUTTON_14:
			funkcja_przycisk(hWnd, hdc, ps, 1, 4);
			break;

//2 Pietro
		case ID_BUTTON_20:
			funkcja_przycisk(hWnd, hdc, ps, 2, 0);
			break;

		case ID_BUTTON_21:
			funkcja_przycisk(hWnd, hdc, ps, 2, 1);
			break;

		case ID_BUTTON_23:
			funkcja_przycisk(hWnd, hdc, ps, 2, 3);
			break;

		case ID_BUTTON_24:
			funkcja_przycisk(hWnd, hdc, ps, 2, 4);
			break;

//3 Pietro
		case ID_BUTTON_30:
			funkcja_przycisk(hWnd, hdc, ps, 3, 0);
			break;

		case ID_BUTTON_31:
			funkcja_przycisk(hWnd, hdc, ps, 3, 1);
			break;

		case ID_BUTTON_32:
			funkcja_przycisk(hWnd, hdc, ps, 3, 2);
			break;

		case ID_BUTTON_34:
			funkcja_przycisk(hWnd, hdc, ps, 3, 4);
			break;

//4 Pietro
		case ID_BUTTON_40:
			funkcja_przycisk(hWnd, hdc, ps, 4, 0);
			break;

		case ID_BUTTON_41:
			funkcja_przycisk(hWnd, hdc, ps, 4, 1);

		case ID_BUTTON_42:
			funkcja_przycisk(hWnd, hdc, ps, 4, 2);
			break;

		case ID_BUTTON_43:
			funkcja_przycisk(hWnd, hdc, ps, 4, 3);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		repaintWindow(hWnd, hdc, ps);

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
			if (tablica_pasazerow.size() < maksymalny_udzwig_windy/srednia_waga_czlowieka && aktualna_pozycja_windy % 150 == 0 && sprawdzanie_ilosci_oczekujacych_na_pietrze(ilosc_pieter - aktualna_pozycja_windy / 150) > 0)
			{
				docelowe_pietro_windy = ilosc_pieter - aktualna_pozycja_windy / 150;
			}
			
			if (docelowe_pietro_windy == 0 && aktualny_numer_pietra_ludzik > 0 && sprawdzanie_ilosci_oczekujacych_na_pietrze(aktualny_numer_pietra_ludzik) > 0 && tablica_pasazerow.size()==0 && aktualna_pozycja_windy != 750)
			{
				docelowe_pietro_windy = aktualny_numer_pietra_ludzik;
			}
			
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
				if(wsiadanie == false && wysiadanie == false)repaint_wysiadajacy(hWnd, hdc, ps);
				
				if(wsiadanie == false && wysiadanie == true)repaint_wsiadajacy(hWnd, hdc, ps);

				if (wsiadanie == true && wysiadanie == true)
				{
					dodawanie_docelowych_pietr_windy(hWnd);
				}

				if ((docelowe_pietro_windy != ilosc_pieter - aktualna_pozycja_windy / 150) || aktualna_pozycja_windy == 750)
				{
					wsiadanie = false;
					wysiadanie = false;
					dodawanie_pietr = false;
				}
			}
			break;

		case TMR_2:
			//force window to repaint
			if (dodawanie_pietr == true && wsiadanie == true && wysiadanie == true && docelowe_pietro_windy == ilosc_pieter - (aktualna_pozycja_windy / 150) && tablica_pasazerow.size() == 0)
			{
				if (docelowe_pietro_windy != 0 && czas_do_powrotu_windy > 0)czas_do_powrotu_windy--;
				repaint_rysowanie_czasu_do_powrotu_windy(hWnd, hdc, ps, czas_do_powrotu_windy);
				if (czas_do_powrotu_windy == 0)
				{
					docelowe_pietro_windy = 0;
					wsiadanie = false;
					wysiadanie = false;
					//SetTimer(hWnd, TMR_1, 1, 0);
					//KillTimer(hWnd, TMR_2);
				}
			}
			else
			{
				czas_do_powrotu_windy = czas_po_ktorym_pusta_winda_zjezdza_na_parter;
				repaint_rysowanie_czasu_do_powrotu_windy(hWnd, hdc, ps, czas_do_powrotu_windy);
			}

			break;
		
		case TMR_3:
			//force window to repaint
			RECT drawArea_animacja_wsiadania;

			drawArea_animacja_wsiadania = { (maksymalna_ilosc_oczekujacych - ilosc_osob_animacja) * 40 + pozycja_animacji_x, aktualna_pozycja_windy - 75, (maksymalna_ilosc_oczekujacych) * 40 + 10 + pozycja_animacji_x, aktualna_pozycja_windy - 4 };

			InvalidateRect(hWnd, &drawArea_animacja_wsiadania, TRUE);
			hdc = BeginPaint(hWnd, &ps);

			for (int j = 0; j < ilosc_osob_animacja; j++)rysowanie_ludzika(hdc, (maksymalna_ilosc_oczekujacych - j) * 40 - 30 + pozycja_animacji_x, aktualna_pozycja_windy - 65, tablica_oczekujacych[ilosc_pieter - (aktualna_pozycja_windy / 150)][j]);

			Sleep(100);

			EndPaint(hWnd, &ps);

			if (pozycja_animacji_x < koncowa_pozycja_x_animacji)pozycja_animacji_x = pozycja_animacji_x + 10;
			else
			{
				wsiadanie = true;
				repaint_zamykanie_drzwi_windy(hWnd, hdc, ps, maksymalna_ilosc_oczekujacych * 40 + 10);
				pozycja_animacji_x = 0;

				wsiadanie_pasazerow();

				repaint_pozostali_oczekujacy(hWnd, hdc, ps, ilosc_osob_animacja);

				SetTimer(hWnd, TMR_1, 1, 0);
				KillTimer(hWnd, TMR_3);
			}
			
			break;

		case TMR_4:
			//force window to repaint
			RECT drawArea_animacja_wysiadania;
			
			drawArea_animacja_wysiadania = { ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - ilosc_osob_animacja) * 40 + maksymalna_ilosc_oczekujacych * 40 + 16 + pozycja_animacji_x, aktualna_pozycja_windy - 75, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 20 + pozycja_animacji_x, aktualna_pozycja_windy - 4 };

			InvalidateRect(hWnd, &drawArea_animacja_wysiadania, TRUE);
			hdc = BeginPaint(hWnd, &ps);

			for (int j = 0; j < ilosc_osob_animacja; j++)rysowanie_ludzika(hdc, ((maksymalny_udzwig_windy / srednia_waga_czlowieka) - j) * 40 + maksymalna_ilosc_oczekujacych * 40 - 10 + pozycja_animacji_x, aktualna_pozycja_windy - 65, tablica_pasazerow[j]);

			Sleep(100);

			EndPaint(hWnd, &ps);

			if (pozycja_animacji_x < koncowa_pozycja_x_animacji)pozycja_animacji_x = pozycja_animacji_x + 10;
			else
			{
				wysiadanie = true;
				pozycja_animacji_x = 0;
				
				wysiadanie_pasazerow();

				repaint_winda(hWnd, hdc, ps);

				repaint_zamykanie_drzwi_windy(hWnd, hdc, ps, (maksymalny_udzwig_windy / srednia_waga_czlowieka) * 40 + maksymalna_ilosc_oczekujacych * 40 + 25);

				SetTimer(hWnd, TMR_1, 1, 0);
				KillTimer(hWnd, TMR_4);
			}

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
