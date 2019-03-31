#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <stdio.h>


#define ROZMIAR_POLA 10

#define ZIELONY 10
#define CZERWONY 12
#define NIEBIESKI 11
#define BIALY 15


typedef enum Dzialania{start,ustaw,strzal}Zadanie;

typedef struct {
	int pole[ROZMIAR_POLA][ROZMIAR_POLA];
	int statki;
}Gracz;


typedef struct {
	int stanPoprzedni;
	int(*stan[6])(int poprzedniePole);
	int aktualnePole;
}Wybor;

typedef struct Lista{
	Zadanie zadanie;
	int argument;
	int rodzaj;
	struct Lista* pPoprzednia;
}Historia;

void WyczyscBufor();
void RysujPlansze(Gracz gracz, int dyskrecja);
void IniciujGre(Gracz* gracz1, Gracz* gracz2, int trybGry);
void WypelnijTablice(int poleGracza1[ROZMIAR_POLA][ROZMIAR_POLA], int poleGracza2[ROZMIAR_POLA][ROZMIAR_POLA]);
int Bitwa(Gracz* gracz1, Gracz* gracz2);
int BitwaAI(Gracz* gracz1, Gracz* gracz2, Wybor* AI);
void Oczysc();
void ZmienKolor(int typ);
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj);
int WprowadzZadanie(int liczbaArgumentow);

int Losuj(int poprzedniePole);
int IdzN(int poprzedniePole);
int IdzS(int poprzedniePole);
int IdzE(int poprzedniePole);
int IdzW(int poprzedniePole);
int IdzSkos(int poprzedniePole);

int CelowanieAI(Gracz* atakowanyGracz, Wybor* AI);


#endif // !FUNKCJE_H

