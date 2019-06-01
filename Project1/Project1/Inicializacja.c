#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Struktury.h"
#include "Inicializacja.h"
#include "Interface.h"
#include "Logika.h"
#include "AI.h"
#include "Zapis.h"

int PobierzParametry(int ileArg, char* arg[], Konfiguracja *konfiguracja) {

	konfiguracja->trybGry = -1;
	konfiguracja->automat = 0;
	konfiguracja->wczytaj = 0;
	konfiguracja->plikZapisu = strdup("Zapis.xml");
	if (ileArg > 1) {
		if (!strcmp(arg[1], "-l")) {
			konfiguracja->wczytaj = 1;
			if (ileArg > 2) {
				free(konfiguracja->plikZapisu);
				konfiguracja->plikZapisu = strdup(arg[2]);
			}

		}
		else if (!strcmp(arg[1], "-h")) {
			konfiguracja->trybGry = 1;
		}
		else if (!strcmp(arg[1], "-c")) {
			if (ileArg > 2 && !strcmp(arg[2], "-a")) {
				konfiguracja->automat = 1;
			}
			konfiguracja->trybGry = 0;

		}
	}
	if (konfiguracja->wczytaj == 0 && (konfiguracja->trybGry != 1 && konfiguracja->trybGry != 0)) {
		return 0;
	}

	return 1;
}

int Skonfiguruj(Konfiguracja *konfiguracja) {
	ZmienKolor(bialy);
	FILE* plikKonfiguracji = fopen("config.ini", "r");
	if (plikKonfiguracji == NULL) {
		return 0;
	}
	char buffor[30];
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->rozmiarPola_x);
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->rozmiarPola_y);
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->no_system_new_line_number);
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->trudnosc);
	if (konfiguracja->rozmiarPola_x < 5 || konfiguracja->rozmiarPola_x>10 || konfiguracja->rozmiarPola_y < 5 || konfiguracja->rozmiarPola_y>10) {
		konfiguracja->rozmiarPola_x = konfiguracja->rozmiarPola_y = 0;
		return -1;
	}
	return 1;
}

int PobierzKodowanie(Konfiguracja *konfiguracja) {
	FILE* plikKodowania = fopen("Kod.txt", "r");
	if (plikKodowania == NULL) {
		printf("Brak pliku kodowania");
		return 0;
	}
	char buffor[30];
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->zad_flota);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->zad_cofnij);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->zad_zapisz);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->zad_wczytaj);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->zad_brak);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->b_koniec);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->b_niekoniec);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->b_wczytaj);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->st_blad);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->st_pudlo);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->st_cel);
	fscanf(plikKodowania, "%s %d", &buffor, &konfiguracja->st_zatop);
	return 1;
}

void UstawParametry(Konfiguracja *konfiguracja) {
	printf("Witaj w grze w statki. Nacisnij dowolny klawisz aby rozpoczac. Uzyj komendy \"wczytaj\", jesli chcesz wczytac ostatnia gre.\n");
	int komenda;
	do {
		komenda = WprowadzZadanie(0);
		if (komenda == konfiguracja->zad_flota || komenda == konfiguracja->zad_zapisz || komenda == konfiguracja->zad_cofnij) {
			printf("Nie mozna teraz wykonac tej akcji.\n");
		}
		else if (komenda == konfiguracja->zad_wczytaj) {
			konfiguracja->wczytaj = 1;
			break;
		}
	} while (komenda != konfiguracja->zad_brak);
	if (konfiguracja->wczytaj == 0) {
		printf("Wybierz tryb gry. 0-gra z komputerem (wprowadz \"-1\" by umiescic statki automatycznie) lub 1-gra w dwie osoby(hotseat):");
		do {

			scanf("%d", &konfiguracja->trybGry);
			switch (konfiguracja->trybGry)
			{
			case -1:
				konfiguracja->automat = 1;
				konfiguracja->trybGry = 0;
			case 0: printf("Wybrano gre z komputerem.\n");
				break;
			case 1: printf("Wybrano gre hotseat.\n");
				break;
			default: printf("Wybierz miedzy 0 i 1!");
				break;
			}
		} while (konfiguracja->trybGry != 0 && konfiguracja->trybGry != 1);
	}

}

int Iniciuj(Konfiguracja*konfiguracja, Rozgrywka *rozgrywka) {
	rozgrywka->xml = 0;
	rozgrywka->ruchy = 0;
	int wynik_wczytywania;
	srand((unsigned int)time(NULL));
	if (WypelnijTablice(konfiguracja, &rozgrywka->gracz1, &rozgrywka->gracz2) == 0) {
		printf("Bledna alokacja pamieci. Sprobuj uruchomic ponownie.");
		return 0;
	}
	if (konfiguracja->wczytaj == 1) {//rozpoczêcie od wczytywania

		if ((wynik_wczytywania = Wczytaj(konfiguracja, rozgrywka)) != 1) {
			if (wynik_wczytywania == -1) {
				printf("Blad konfiguracji. Zmien konfiguracje na zgodna z zapisem i uruchom ponownie.\nOczekiwane wartosci to:\nX:%d\nY:%d\n", konfiguracja->rozmiarPola_x, konfiguracja->rozmiarPola_y);
			}
			else {
				printf("Nie znaleziono zapisu lub plik z zapisem jest uszkodzony. \nSprobuj uruchomic gre ponownie lub zaczac od poczatku");
			}
			UsunTablice(konfiguracja, &rozgrywka->gracz1, &rozgrywka->gracz2);
			return 0;
		}
		konfiguracja->trybGry = mxmlGetInteger(mxmlFindPath(rozgrywka->xml, "Informacje/Ustawienia/Tryb_Gry"));
	}
	else {//przygotowanie do gry
		Oczysc(konfiguracja);
		if (konfiguracja->automat == 0) {
			Rozmieszczenie(konfiguracja, &rozgrywka->gracz1);
		}
		else { //automat == 1-automatyczne rozmieszczenie.
			AutoRozmieszczenie(konfiguracja, &rozgrywka->gracz1);
		}
		if (konfiguracja->trybGry == 1) {
			Oczysc(konfiguracja);
			printf("Tura gracza drugiego. Nacisnij dowolny klawisz");
			getchar();
			Oczysc(konfiguracja);
			Rozmieszczenie(konfiguracja, &rozgrywka->gracz2);
		}
		else {
			AutoRozmieszczenie(konfiguracja, &rozgrywka->gracz2);
		}

		UtworzZapis(konfiguracja, rozgrywka);
	}
	return 1;
}
