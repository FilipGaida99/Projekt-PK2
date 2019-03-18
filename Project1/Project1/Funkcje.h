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

typedef struct Lista{
	Zadanie zadanie;
	int argument;
	struct Lista* pPoprzednia;
}Historia;

void WyczyscBufor();
void RysujPlansze(Gracz gracz, int dyskrecja);
void IniciujGre(Gracz* gracz1, Gracz* gracz2, int trybGry);
void WypelnijTablice(int poleGracza1[ROZMIAR_POLA][ROZMIAR_POLA], int poleGracza2[ROZMIAR_POLA][ROZMIAR_POLA]);
int Bitwa(Gracz* gracz1, Gracz* gracz2);
int BitwaAI(Gracz* gracz1, Gracz* gracz2);
void Oczysc();
void ZmienKolor(int typ);
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument);
int WprowadzZadanie(int liczbaArgumentow);

#endif // !FUNKCJE_H

