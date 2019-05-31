#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funkcje.h"

#include "mxml-3.0/mxml.h"

int main(int ileArg, char* arg[]) {
	ZmienKolor(BIALY);
	Konfiguracja konfiguracja;
	Rozgrywka rozgrywka;
	if (Skonfiguruj(&konfiguracja) == 0) {
		return 0;
	}
	if (PobierzParametry(ileArg, arg, &konfiguracja) == 0) {
		UstawParametry(&konfiguracja);
	}
	//DodajdoListy(rozgrywka.ruchy, start, 0, 0);
	if (konfiguracja.trybGry) {
		GameLoop(&konfiguracja, &rozgrywka);
	}
	else {
		GameLoopAI(&konfiguracja, &rozgrywka);
	}/*
	if (wynik1 == konfiguracja.b_koniec) {
		printf("Wygral pierwszy gracz");
	}
	else if(konfiguracja.trybGry==1){
		printf("Wygral drugi gracz");
	}
	else
	{
		printf("Wygral Komputer");
	}
	
	mxmlDelete(xml);
	UsunListe(&ruchy);
	UsunTablice(&gracz1, &gracz2);
	*/
	return 0;
}