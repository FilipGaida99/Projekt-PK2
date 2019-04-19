#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <stdio.h>
//#include <mxml.h>

#include "mxml-3.0/mxml.h"



#define ROZMIAR_POLA 10


#define ZIELONY 10
#define CZERWONY 12
#define NIEBIESKI 11
#define BIALY 15

#if defined(unix) || defined(__unix__) || defined(__unix)
#define UBIALY "\x1B[37m"
#define UCZERWONY "\x1B[31m"
#define UZIELONY "\x1B[32m"
#define UNIEBIESKI "\x1B[34m"
#endif

//typ wyliczeniowy okre�laj�cy jaka czynno�� zosta�a wykonana.
typedef enum Dzialania{start,ustaw,strzal}Zadanie; 

//struktura przechowuj�ca dane gracza.
typedef struct {
	int pole[ROZMIAR_POLA][ROZMIAR_POLA];
	int statki;
}Gracz;


//struktura obs�uguj�ca sztuczn� inteligencj�.
typedef struct {
	int stanPoprzedni;
	int(*stan[6])(int poprzedniePole);
	int aktualnePole;
}Wybor;

//Lista, b�d�ca histori� czynnosci, kt�re zosta�y wykonane.
typedef struct Lista{
	Zadanie zadanie;
	int argument;
	int rodzaj;
	struct Lista* pPoprzednia;
}Historia;

//Funkcja czyszcz�ca pozosta�o�ci z bufora.
void WyczyscBufor();
//Funkcja wy�wietlaj�ca graczowi plansz� jego lub przeciwnika. Dla zmiennej dyskrecja r�wnej 0, statki s� wy�wietlane, dla 1 s� pomijane.
void RysujPlansze(Gracz gracz, int dyskrecja);
//Funkcja rozpoczyna przygotowanie do gry w przyadku, gdy nie rozpocz�to od wczytania zapisu.
void IniciujGre(Gracz* gracz1, Gracz* gracz2, int trybGry);
//Procedura wype�nienia tablic b�d�ch plansz� dla graczy w odpowiedni spos�b.
void WypelnijTablice(int poleGracza1[ROZMIAR_POLA][ROZMIAR_POLA], int poleGracza2[ROZMIAR_POLA][ROZMIAR_POLA]);
//Komunikacja z u�ytkownikiem po zako�czeniu przygotowa�.
int Bitwa(Gracz* gracz1, Gracz* gracz2, mxml_node_t** xml, Historia** ruchy);
//Funkcja czyszcz�ca konsol�. Zabezpieczenie przed podgl�daniem p�l przeciwnika.
void Oczysc();
//Funkcja zmieniaj�ca kolor tekstu.
void ZmienKolor(int typ);
//Funkcja do daj�ca element do listy na jej pocz�tek.
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj);
//Funkcja do komunikacji z u�ytkownikiem, pozwalaj�ca na u�ywanie komend w dowolnej chwili
int WprowadzZadanie(int liczbaArgumentow);
//Usuwanie listy
void UsunListe(Historia**lista);
//
void WypiszRuchy(Historia* ruchy);

int Losuj(int poprzedniePole);
int IdzN(int poprzedniePole);
int IdzS(int poprzedniePole);
int IdzE(int poprzedniePole);
int IdzW(int poprzedniePole);
int IdzSkos(int poprzedniePole);

int BitwaAI(Gracz* atakowanyGracz, Wybor* AI, Historia** ruchy);

void PobierzKoordynaty(int dlugosc, Gracz* gracz, Historia** historia, int rodzaj);
int UtworzZapis(int trybGry, int tura, int czyja, Gracz gracz1, Gracz gracz2, mxml_node_t** xml);
int Wczytaj(Gracz* gracz1, Gracz* gracz2, mxml_node_t**xml);

#endif // !FUNKCJE_H

