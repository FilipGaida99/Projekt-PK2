#ifndef STRUKTURY_H
#define STRUKTURY_H

#include "mxml-3.0/mxml.h"


/**typ wyliczeniowy okre�laj�cy jaka czynno�� zosta�a wykonana. */
typedef enum Dzialania { start, ustaw, strzal }Zadanie;

typedef enum { zielony = 10, niebieski, czerwony, bialy = 15 }Kolor;

//Struktura przechowuj�ca konfiguracje gry oraz kodowanie warto�ci zwracanej przez funkcje
typedef struct  {
	//Ustawienia gry
	int trybGry;
	int automat;
	int wczytaj;
	//Konfiguracja gry
	int rozmiarPola_x;
	int rozmiarPola_y;
	int trudnosc;
	int no_system_new_line_number;
	char* plikZapisu;

	//kody funkcji
	int zad_brak;
	int zad_wczytaj;
	int zad_zapisz;
	int zad_cofnij;
	int zad_flota;
	int b_koniec;
	int b_niekoniec;
	int b_wczytaj;
	int st_blad;
	int st_pudlo;
	int st_cel;
	int st_zatop;
}Konfiguracja;



/**struktura obs�uguj�ca sztuczn� inteligencj�. */
typedef struct {
	//Indeks informuj�cy, kt�ry z element�w tablicy  wska�nik�w na funkcje b�dzie u�ywany w trakcie decyzji
	int stanPoprzedni;
	//Tablica wska�nik�w na funkcje steruj�ce ostrza�em
	int(*stan[6])(Konfiguracja *konfiguracja, int poprzedniePole);
	//Koordynaty pola na kt�re ostatnio celowano
	int aktualnePole;
}Wybor;

/**struktura przechowuj�ca dane gracza. */
typedef struct {
	//Dynamicznie alokowana tablica dwuwymiarowa - plansza
	int** pole;
	//Liczba reprezentuj�ca statki gracza
	int statki;
}Gracz;


/**Lista dynamicza, b�d�ca histori� ruch�w, kt�re zosta�y wykonane. */
typedef struct Lista {
	//Zmienna s�u��ca jako etykieta dla danego ruchu
	Zadanie zadanie;
	//Argument jakiego u�yto podczas wykonywania ruchu.
	int argument;
	//Pomocniczy argument dla niekt�rych typ�w ruchu
	int rodzaj;
	//Wska�nik na poprzedni element w li�cie
	struct Lista* pPoprzednia;
}Historia;
//Struktura przechowuj�c informacje o obecnej rozgrywce
typedef struct {
	Gracz gracz1;
	Gracz gracz2;
	mxml_node_t* xml;
	mxml_node_t* tura_n;
	mxml_node_t* czyja_n;
	Historia* ruchy;
	Wybor AI;
}Rozgrywka;

#endif // !STRUKTURY_H
