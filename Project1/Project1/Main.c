#include <stdio.h>
#include <stdlib.h>

#include "Struktury.h"
#include "Zapis.h"
#include "Logika.h"
#include "AI.h"
#include "Interface.h"
#include "Inicializacja.h"
#include "mxml-3.0/mxml.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main(int ileArg, char* arg[]) {
	Konfiguracja konfiguracja;
	Rozgrywka rozgrywka;
	if (Skonfiguruj(&konfiguracja) == 0 || PobierzKodowanie(&konfiguracja)==0) {
		printf("Blad pliku konfiguracji lub pliku kodowania");
		if (konfiguracja.rozmiarPola_x == 0) {
			printf("PLIK KONIFIGURACJI : Podano rozmiar poza zakresem.Dozwolony zakres to miêdzy 5, a 10");
		}
		return 0;
	}
	
	if (PobierzParametry(ileArg, arg, &konfiguracja) == 0) {
		UstawParametry(&konfiguracja);
	}
	if (Iniciuj(&konfiguracja, &rozgrywka) == 0) {
		getchar();
		return 0;
	}
	DodajdoListy(&rozgrywka.ruchy, start, 0, 0);
	if (konfiguracja.trybGry) {
		GameLoop(&konfiguracja, &rozgrywka);
	}
	else {
		GameLoopAI(&konfiguracja, &rozgrywka);
	}
	
	_CrtDumpMemoryLeaks();
	return 0;
}