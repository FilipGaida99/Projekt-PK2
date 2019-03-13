#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <stdio.h>


#define ROZMIAR_POLA 10

typedef struct {
	int pole[ROZMIAR_POLA][ROZMIAR_POLA];
	int statki;
}Gracz;

void WyczyscBufor();
void RysujPlansze(Gracz gracz, int dyskrecja);
void IniciujGre(Gracz gracz1, Gracz gracz2);
void WypelnijTablice(int poleGracza1[ROZMIAR_POLA][ROZMIAR_POLA], int poleGracza2[ROZMIAR_POLA][ROZMIAR_POLA]);


#endif // !FUNKCJE_H

