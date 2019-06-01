#ifndef STRUKTURY_H
#define STRUKTURY_H

#include "mxml-3.0/mxml.h"


/**typ wyliczeniowy okreœlaj¹cy jaka czynnoœæ zosta³a wykonana. */
typedef enum Dzialania { start, ustaw, strzal }Zadanie;

typedef enum { zielony = 10, niebieski, czerwony, bialy = 15 }Kolor;

//Struktura przechowuj¹ca konfiguracje gry oraz kodowanie wartoœci zwracanej przez funkcje
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



/**struktura obs³uguj¹ca sztuczn¹ inteligencjê. */
typedef struct {
	//Indeks informuj¹cy, który z elementów tablicy  wskaŸników na funkcje bêdzie u¿ywany w trakcie decyzji
	int stanPoprzedni;
	//Tablica wskaŸników na funkcje steruj¹ce ostrza³em
	int(*stan[6])(Konfiguracja *konfiguracja, int poprzedniePole);
	//Koordynaty pola na które ostatnio celowano
	int aktualnePole;
}Wybor;

/**struktura przechowuj¹ca dane gracza. */
typedef struct {
	//Dynamicznie alokowana tablica dwuwymiarowa - plansza
	int** pole;
	//Liczba reprezentuj¹ca statki gracza
	int statki;
}Gracz;


/**Lista dynamicza, bêd¹ca histori¹ ruchów, które zosta³y wykonane. */
typedef struct Lista {
	//Zmienna s³u¿¹ca jako etykieta dla danego ruchu
	Zadanie zadanie;
	//Argument jakiego u¿yto podczas wykonywania ruchu.
	int argument;
	//Pomocniczy argument dla niektórych typów ruchu
	int rodzaj;
	//WskaŸnik na poprzedni element w liœcie
	struct Lista* pPoprzednia;
}Historia;
//Struktura przechowuj¹c informacje o obecnej rozgrywce
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
