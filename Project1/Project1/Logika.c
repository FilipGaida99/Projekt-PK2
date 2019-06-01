#include "Struktury.h"
#include "Logika.h"
#include "Interface.h"
#include "mxml-3.0/mxml.h"


void ZamianaGraczy(Rozgrywka* rozgrywka) {
	Gracz tymczasowa = rozgrywka->gracz1;
	rozgrywka->gracz1 = rozgrywka->gracz2;
	rozgrywka->gracz2 = tymczasowa;
}


int WypelnijTablice(Konfiguracja* konfiguracja, Gracz* gracz1, Gracz*gracz2) {
	if ((gracz1->pole = malloc(konfiguracja->rozmiarPola_x * sizeof(int*))) == NULL) return 0;
	if ((gracz2->pole = malloc(konfiguracja->rozmiarPola_x * sizeof(int*))) == NULL) {
		free(gracz1->pole);
		return 0;
	}

	int i, j;
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		if ((gracz1->pole[i] = malloc(konfiguracja->rozmiarPola_y * sizeof(int))) == NULL) {
			break;
		}
		if ((gracz2->pole[i] = malloc(konfiguracja->rozmiarPola_y * sizeof(int))) == NULL) {
			free(gracz1->pole[i]);
			break;
		}
	}
	if (i < konfiguracja->rozmiarPola_x) {
		//wyst¹pi³y b³êdy alokacji nale¿y usun¹æ dotychczas zaalokowan¹ pamiêæ
		for (i--; i >= 0; i--) {
			free(gracz1->pole[i]);
			free(gracz2->pole[i]);
		}
		free(gracz1->pole);
		free(gracz2->pole);
		return 0;
	}
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			gracz1->pole[i][j] = gracz2->pole[i][j] = 10 * i + j;
		}

	}
	return 1;
}

void DodajdoFloty(Gracz* gracz, int pozycja, int dlugosc) {
	int mnoznik = 010;
	int i;
	for (i = 0; i < -4 - pozycja; i++) {
		mnoznik *= 010;
	}
	if (pozycja == -3) {
		mnoznik = 1;
	}

	gracz->statki += dlugosc * mnoznik;
}

int UstawStatek(Konfiguracja* konfiguracja, Gracz* gracz, int dlugosc, int pole, int kierunek, int rodzajStatku) {
	int x = pole / 10;
	int y = pole % 10;
	int i;
	if (x < 0 || x >= konfiguracja->rozmiarPola_x || y < 0 || y >= konfiguracja->rozmiarPola_y)
		return 0;
	if (kierunek) {
		if (x + dlugosc > konfiguracja->rozmiarPola_x)
			return 0;
		x -= 1;
		for (i = 0; i <= dlugosc + 1; i++) {
			if (x >= 0 && x < konfiguracja->rozmiarPola_x) {
				if (gracz->pole[x][y] < 0) {
					return 0;
				}
				if (y - 1 >= 0 && gracz->pole[x][y - 1] < 0) {
					return 0;
				}
				if (y + 1 < konfiguracja->rozmiarPola_y && gracz->pole[x][y + 1] < 0) {
					return 0;
				}
			}
			x++;
		}
		for (x--; i > 2; i--) {
			x--;
			gracz->pole[x][y] = rodzajStatku;
		}
		return 1;

	}
	else {
		if (y + dlugosc > konfiguracja->rozmiarPola_y)
			return 0;
		y -= 1;
		for (i = 0; i <= dlugosc + 1; i++) {
			if (y >= 0 && y < konfiguracja->rozmiarPola_y) {
				if (gracz->pole[x][y] < 0) {
					return 0;
				}
				if (x - 1 >= 0 && gracz->pole[x - 1][y] < 0) {
					return 0;
				}
				if (x + 1 < konfiguracja->rozmiarPola_x && gracz->pole[x + 1][y] < 0) {
					return 0;
				}
			}
			y++;
		}
		for (y--; i > 2; i--) {
			y--;
			gracz->pole[x][y] = rodzajStatku;
		}
		return 1;
	}
}

int UsunStatek(Konfiguracja* konfiguracja, Historia** historia, Gracz* gracz) {
	Historia* temp = *historia;
	if (temp->zadanie == ustaw) {

		int dlugosc, pole, kierunek, i, x, y;
		//odszyfrowanie danych z struktury
		dlugosc = temp->argument / 10000;
		pole = (temp->argument % 10000) / 100;
		kierunek = temp->argument % 100;
		x = pole / 10;
		y = pole % 10;

		for (i = 0; i < dlugosc; i++) {
			gracz->pole[x][y] = x * 10 + y;

			if (kierunek) {
				x++;
			}
			else {
				y++;
			}
		}

		RysujPlansze(konfiguracja, *gracz, 0);

		PobierzKoordynaty(konfiguracja, dlugosc, gracz, &(temp->pPoprzednia), temp->rodzaj);
		return 1;
	}
	if (temp->zadanie == start) {
		return 0;
	}
	return -1;

}

int Strzal(Konfiguracja* konfiguracja, Gracz* atakowanyGracz, int pole) {
	int x = pole / 10;
	int y = pole % 10;

	if (x < 0 || x >= konfiguracja->rozmiarPola_x || y < 0 || y >= konfiguracja->rozmiarPola_y) {
		return konfiguracja->st_blad;
	}

	if (atakowanyGracz->pole[x][y] <= -3 && atakowanyGracz->pole[x][y] >= -9) { //-3 1-masztowiec, (-6)do(-4) 2-maszztowiec, (-8)do(-7) 3-maszztowiec, -9 4-maszztowiec
		int pozycja = 01;
		switch (atakowanyGracz->pole[x][y])
		{
		case -3:
			atakowanyGracz->statki -= pozycja;
			break;
		case -6:
			pozycja *= 010;
		case -5:
			pozycja *= 010;
		case -4:
			pozycja *= 010;
			atakowanyGracz->statki -= pozycja;
			break;
		case -8:
			pozycja *= 010;
		case -7:
			pozycja *= 010000;
			atakowanyGracz->statki -= pozycja;
			break;
		case -9:
			pozycja *= 01000000;
			atakowanyGracz->statki -= pozycja;
			break;
		}
		atakowanyGracz->pole[x][y] = -1;

		if (((atakowanyGracz->statki / pozycja) % 8 == 0) || pozycja == 01) {
			return konfiguracja->st_zatop;
		}
		return konfiguracja->st_cel;
	}
	else if (atakowanyGracz->pole[x][y] < 0) {
		return konfiguracja->st_blad;
	}
	else {
		atakowanyGracz->pole[x][y] = -2; //pudlo
		return konfiguracja->st_pudlo;
	}
}

Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj) {
	Historia* temp = (Historia*)malloc(sizeof(Historia));
	if (temp == 0) {
		return 0;
	}
	temp->zadanie = zadanie;
	temp->argument = argument;
	temp->rodzaj = rodzaj;
	temp->pPoprzednia = *lista;
	*lista = temp;
	return *lista;
}

void UsunTablice(Konfiguracja* konfiguracja, Gracz* gracz1, Gracz*gracz2) {
	int i;
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		free(gracz1->pole[i]);
		free(gracz2->pole[i]);
	}
	free(gracz1->pole);
	free(gracz2->pole);
}

void UsunListe(Historia**lista) {
	while (*lista != 0) {
		Historia* nastepna = (*lista)->pPoprzednia;
		free(*lista);
		*lista = nastepna;
	}
}