#include "Struktury.h"
#include "AI.h"
#include "Logika.h"

void InicializujAI(Wybor* AI, Konfiguracja* konfiguracja) {
	if (AI->stanPoprzedni < 0 || AI->stanPoprzedni>5) {
		AI->aktualnePole = 0;
		AI->stanPoprzedni = 0;
	}
	if (konfiguracja->trudnosc == 0) {// obs³uga typu ³atwego
		int i;
		for (i = 0; i < 6; i++) {
			AI->stan[i] = Losuj;
		}
		return;
	}
	AI->stan[0] = Losuj;
	AI->stan[1] = IdzS;
	AI->stan[2] = IdzN;
	AI->stan[3] = IdzSkos;
	AI->stan[4] = IdzW;
	AI->stan[5] = IdzE;
}

int Losuj(Konfiguracja *konfiguracja, int poprzedniePole) {
	int x = rand() % (konfiguracja->rozmiarPola_x);
	int y = rand() % (konfiguracja->rozmiarPola_y);
	return 10 * x + y;
}

int IdzN(Konfiguracja *konfiguracja, int poprzedniePole) {
	int tymczasowa = poprzedniePole - 10;
	if (tymczasowa / 10 < 0) {
		return -1;
	}
	return tymczasowa;

}
int IdzS(Konfiguracja *konfiguracja, int poprzedniePole) {
	int tymczasowa = poprzedniePole + 10;
	if (tymczasowa / 10 >= konfiguracja->rozmiarPola_x) {
		return -1;
	}
	return tymczasowa;
}
int IdzSkos(Konfiguracja *konfiguracja, int poprzedniePole) {
	int tymczasowa = poprzedniePole + 11;
	if (tymczasowa / 10 >= konfiguracja->rozmiarPola_x || tymczasowa % 10 >= konfiguracja->rozmiarPola_y) {
		return -1;
	}
	return tymczasowa;
}
int IdzE(Konfiguracja *konfiguracja, int poprzedniePole) {
	int tymczasowa = poprzedniePole + 1;
	if (tymczasowa % 10 >= konfiguracja->rozmiarPola_y) {
		return -1;
	}
	return tymczasowa;

}
int IdzW(Konfiguracja *konfiguracja, int poprzedniePole) {
	int tymczasowa = poprzedniePole - 1;
	if (tymczasowa % 10 < 0) {
		return -1;
	}
	return tymczasowa;
}

int BitwaAI(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka, Wybor* AI) {
	int wynik, cel;
	do {
		do {
			if (AI->stanPoprzedni > 5) { //przechwytywanie osobliwych b³êdów w celowaniu
				AI->stanPoprzedni = 0;
			}
			cel = AI->stan[AI->stanPoprzedni](konfiguracja, AI->aktualnePole);
			AI->aktualnePole = cel;

		} while (cel == -1 && AI->stanPoprzedni++);
		wynik = Strzal(konfiguracja, &rozgrywka->gracz1, cel);
		if (wynik != konfiguracja->st_blad) {
			DodajdoListy(&rozgrywka->ruchy, strzal, cel, wynik - 1);
		}
		if (rozgrywka->gracz1.statki == 0) {
			return konfiguracja->b_koniec;
		}

		if (wynik == konfiguracja->st_zatop) {//statek zosta³ zatopiony po strzle
			AI->stanPoprzedni = 0;
			continue;
		}
		if (wynik == konfiguracja->st_cel && AI->stanPoprzedni == 3) { // gdy celowano na skos i trafiono odrazu przechodzi siê do nastêpnego
			AI->stanPoprzedni++;
			continue;
		}

		if (AI->stanPoprzedni == 0) {
			if (wynik == konfiguracja->st_cel) {//trafiono, spróbuj znaleŸæ resztê statku
				AI->stanPoprzedni++;
			}

		}
		else {

			if (wynik == konfiguracja->st_pudlo) { //podczas szukania zmienia siê kierunek tylko podczas nietrafienia
				AI->stanPoprzedni++;
			}
		}


	} while (wynik != konfiguracja->st_pudlo);

	return konfiguracja->b_niekoniec;
}

