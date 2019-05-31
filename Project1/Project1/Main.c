#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funkcje.h"

#include "mxml-3.0/mxml.h"

int main(int ileArg, char* arg[]) {
	Konfiguracja konfiguracja;
	Rozgrywka rozgrywka;
	if (Skonfiguruj(&konfiguracja) == 0 || PobierzKodowanie(&konfiguracja)==0) {
		return 0;
	}
	
	if (PobierzParametry(ileArg, arg, &konfiguracja) == 0) {
		UstawParametry(&konfiguracja);
	}
	Iniciuj(&konfiguracja, &rozgrywka);
	DodajdoListy(&rozgrywka.ruchy, start, 0, 0);
	if (konfiguracja.trybGry) {
		GameLoop(&konfiguracja, &rozgrywka);
	}
	else {
		GameLoopAI(&konfiguracja, &rozgrywka);
	}
	
	return 0;
}