#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <stdio.h>

#include "mxml-3.0/mxml.h"



#define ROZMIAR_POLA 10


//Makra zwiêkszaj¹ce czytelnoœæ kodu
//WPROWAZDZANIE KOMEND
#define ZAD_FLOTA -2
#define ZAD_COFNIJ -3
#define ZAD_ZAPISZ -4
#define ZAD_WCZYTAJ -5
#define ZAD_BRAK 0
//WYNIK BITWY
#define B_KONIEC 0
#define B_NIEKONIEC 1
#define B_WCZYTAJ 2
//WYNIK STRZA£U
#define ST_BLAD 0
#define ST_PUDLO 1
#define ST_CEL 2
#define ST_ZATOP 3

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

//typ wyliczeniowy okreœlaj¹cy jaka czynnoœæ zosta³a wykonana.
typedef enum Dzialania{start,ustaw,strzal}Zadanie; 

//struktura przechowuj¹ca dane gracza.
typedef struct {
	int** pole;
	int statki;
}Gracz;


//struktura obs³uguj¹ca sztuczn¹ inteligencjê.
typedef struct {
	int stanPoprzedni;
	int(*stan[6])(int poprzedniePole);
	int aktualnePole;
}Wybor;

//Lista, bêd¹ca histori¹ czynnosci, które zosta³y wykonane.
typedef struct Lista{
	Zadanie zadanie;
	int argument;
	int rodzaj;
	struct Lista* pPoprzednia;
}Historia;

//Funkcja czyszcz¹ca pozosta³oœci z bufora.
void WyczyscBufor();
//Funkcja wyœwietlaj¹ca graczowi planszê jego lub przeciwnika. Dla zmiennej dyskrecja równej 0, statki s¹ wyœwietlane, dla 1 s¹ pomijane.
void RysujPlansze(Gracz gracz, int dyskrecja);
//Funkcja rozpoczyna przygotowanie do gry w przypadku, gdy nie rozpoczêto od wczytania zapisu.
void Rozmieszczenie(Gracz* gracz);
//Funkcja rozpoczyna przygotowanie do gry oraz losowo umieszcza wszystkie statki.
void AutoRozmieszczenie(Gracz* gracz);
//Procedura alokowania wype³nienia tablic bêd¹ch plansz¹ dla graczy w odpowiedni sposób.
int WypelnijTablice(Gracz* gracz1, Gracz*gracz2);
//Procedura usuwania tablic
void UsunTablice(Gracz* gracz1, Gracz*gracz2);
//Komunikacja z u¿ytkownikiem po zakoñczeniu przygotowañ.
int Bitwa(Gracz* gracz1, Gracz* gracz2, mxml_node_t** xml, Historia** ruchy);
//Funkcja czyszcz¹ca konsolê. Zabezpieczenie przed podgl¹daniem pól przeciwnika.
void Oczysc();
//Funkcja zmieniaj¹ca kolor tekstu.
void ZmienKolor(int typ);
//Funkcja do daj¹ca element do listy na jej pocz¹tek.
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj);
//Funkcja do komunikacji z u¿ytkownikiem, pozwalaj¹ca na u¿ywanie komend w dowolnej chwili
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
int Wczytaj(Gracz* gracz1, Gracz* gracz2, mxml_node_t**xml, Historia** ruchy);

#endif // !FUNKCJE_H

