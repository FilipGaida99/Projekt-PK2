#include <stdio.h>
#include "Struktury.h"
#include "Interface.h"
#include "Logika.h"
#include "AI.h"
#include "Zapis.h"


void WyczyscBufor() {
	char pozostalosc;
	do {
		pozostalosc = getchar();
	} while (pozostalosc != '\n');
}

void ZmienKolor(Kolor typ) {
#if defined(_WIN32) || defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
#if defined(_WIN32)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)typ);
#endif
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
	switch (typ)
	{
	case bialy:
		printf(UBIALY);
		break;
	case czerowny:
		printf(UCZERWONY);
		break;
	case zielony:
		printf(UZIELONY);
		break;
	case niebieski:
		printf(UNIEBIESKI);
		break;
	default:
		break;
	}
#endif
#else
	//dla innych systemów nie zmienia koloru tekstu
#endif
}

void Oczysc(Konfiguracja* konfiguracja) {
#if defined(_WIN32) || defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
#if defined(_WIN32)
	system("cls");
#endif
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
	printf("\n");
	system("clear");
	system("clear");
#endif
#else
	int i;
	for (i = 0; i < konfiguracja->no_system_new_line_number; i++) {
		printf("\n");
	}

#endif
}

void RysujPlansze(Konfiguracja* konfiguracja, Gracz player, int dyskrecja) {
	int i, j, c = 0;

	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		printf("|");
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			if (player.pole[i][j] == -1) {
				ZmienKolor(czerwony);
				printf("%3c", 'X');
				ZmienKolor(bialy);
				printf("|");
			}
			else if (player.pole[i][j] == -2) {
				ZmienKolor(niebieski);
				printf("%3c", 'U');
				ZmienKolor(bialy);
				printf("|");
			}
			else if (player.pole[i][j] >= -9 && player.pole[i][j] <= -3 && dyskrecja == 0) {

				ZmienKolor(zielony);
				printf("%3c", 'V');
				ZmienKolor(bialy);
				printf("|");
			}
			else {
				printf("%3d|", (i * 10 + j));
			}
		}
		printf("\n");
	}

}

void Rozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz) {
	Historia* historia = 0;
	DodajdoListy(&historia, start, 0, 0);
	gracz->statki = 0;
	printf("Twoje Pole: \n");
	RysujPlansze(konfiguracja, *gracz, 0);
	printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");
	int i, c, pole;
	c = -9;
	pole = konfiguracja->rozmiarPola_x * konfiguracja->rozmiarPola_y;
	if (pole / 60 > 0) {
		PobierzKoordynaty(konfiguracja, 4, gracz, &historia, c);
		DodajdoFloty(gracz, c, 4);
	}
	for (i = 0, c = -8; i < (pole / 40); i++, c++) {
		PobierzKoordynaty(konfiguracja, 3, gracz, &historia, c);
		DodajdoFloty(gracz, c, 3);
	}
	for (i = 0, c = -6; i < (pole / 35) + 1; i++, c++) {
		PobierzKoordynaty(konfiguracja, 2, gracz, &historia, c);
		DodajdoFloty(gracz, c, 2);
	}
	for (i = 0, c = -3; i < (pole / 26) + 1; i++) {
		PobierzKoordynaty(konfiguracja, 1, gracz, &historia, c);
		DodajdoFloty(gracz, c, 1);
	}


	int komenda;
	do {
		printf("Oto twoje ustawienie. Czy chcesz je zmienic? Nie bedzie mozna tego zrobic pozniej!\nJezeli chcesz cofnac wprowadz komende \"cofnij\"\nJezeli nie, wprowadz dowolny znak");

		komenda = WprowadzZadanie(0);
		if (komenda == konfiguracja->zad_flota || komenda == konfiguracja->zad_zapisz || komenda == konfiguracja->zad_wczytaj) {
			printf("Nie mozna teraz wykonac tej akcji.\n");
		}
		else if (komenda == konfiguracja->zad_cofnij) {
			UsunStatek(konfiguracja, &historia, gracz);
			RysujPlansze(konfiguracja, *gracz, 0);
		}
	} while (komenda != konfiguracja->zad_brak);
	UsunListe(&historia);
}
void AutoRozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz) {
	gracz->statki = 0;
	int i, c, pole;
	c = -9;
	pole = konfiguracja->rozmiarPola_x * konfiguracja->rozmiarPola_y;
	if (pole / 60 > 0) {
		while (!UstawStatek(konfiguracja, gracz, 4, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 4);
	}
	for (i = 0, c = -8; i < (pole / 40); i++, c++) {
		while (!UstawStatek(konfiguracja, gracz, 3, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 3);
	}
	for (i = 0, c = -6; i < (pole / 35) + 1; i++, c++) {
		while (!UstawStatek(konfiguracja, gracz, 2, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 2);
	}
	for (i = 0, c = -3; i < (pole / 26) + 1; i++) {
		while (!UstawStatek(konfiguracja, gracz, 1, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 1);
	}

	Oczysc(konfiguracja);
}

void WypiszRuchy(Historia* ruchy) {
	if (ruchy->pPoprzednia == 0 || ruchy->zadanie == start)
		return;

	printf("Twoj przeciwnik zakonczyl ture.\nWykonane przez niego ruchy to:\n");
	int i, iloscRuchow = 0;
	Historia* ruchy_w_turze = 0;
	Historia** enumerator;
	while (ruchy->zadanie != start) {
		iloscRuchow++;
		DodajdoListy(&ruchy_w_turze, strzal, ruchy->argument, ruchy->rodzaj);
		ruchy = ruchy->pPoprzednia;
	}
	enumerator = &ruchy_w_turze;
	for (i = 1; i <= iloscRuchow; i++) {
		printf("%d. Gracz strzelil w pole: %d. %s", i, (*enumerator)->argument, (*enumerator)->rodzaj ? "Trafil\n" : "Spudlowal\n");
		enumerator = &(*enumerator)->pPoprzednia;
	}
	UsunListe(&ruchy_w_turze);

}

void WypiszFlote(Gracz* przeciwnik) {
	int mnoznik = 1;
	printf("Jednomasztowce: %d\n", (przeciwnik->statki / mnoznik) % 8);
	mnoznik *= 8;
	//int wynik = ((przeciwnik->statki / mnoznik) % 8 != 0 ) + ((przeciwnik->statki / (mnoznik * 8)) % 8 != 0) + ((przeciwnik->statki / (mnoznik * 8 * 8)) % 8 != 0);
	printf("Dwumasztowce: %d\n", ((przeciwnik->statki / mnoznik) % 8 != 0) + ((przeciwnik->statki / (mnoznik * 8)) % 8 != 0) + ((przeciwnik->statki / (mnoznik * 8 * 8)) % 8 != 0));
	mnoznik *= 8 * 8 * 8;
	printf("Trzymasztowce: %d\n", ((przeciwnik->statki / mnoznik) % 8 != 0) + ((przeciwnik->statki / (mnoznik * 8)) % 8 != 0));
	mnoznik *= 8 * 8;
	printf("Czteromasztowiec: %d\n", (przeciwnik->statki / mnoznik) % 8 != 0);
}

void PobierzKoordynaty(Konfiguracja* konfiguracja, int dlugosc, Gracz* gracz, Historia** historia, int rodzaj) {
	switch (dlugosc)
	{
	case 4: printf("Ustaw czteromasztowiec:");
		break;
	case 3: printf("Ustaw trzymasztowiec:");
		break;
	case 2: printf("Ustaw dwumasztowiec:");
		break;
	case 1: printf("Ustaw jednomasztowce (podaj tylko numery pola):");
		break;
	default: printf("blad");
		break;
	}
	int dane;
	int wynik = 0;
	do {
		if (dlugosc == 1) {
			dane = WprowadzZadanie(1);
		}
		else {
			dane = WprowadzZadanie(2);
		}
		if (dane == -3) {
			UsunStatek(konfiguracja, historia, gracz);
			switch (dlugosc)
			{
			case 4: printf("Ustaw czteromasztowiec:");
				break;
			case 3: printf("Ustaw trzymasztowiec:");
				break;
			case 2: printf("Ustaw dwumasztowiec:");
				break;
			case 1: printf("Ustaw jednomasztowce (podaj tylko numery pola):");
				break;
			default: printf("blad");
				break;
			}
			continue;
		}
		if (dane == -2 || dane == -4 || dane == -5) {
			printf("Nie mozna teraz wykonac tej akcji.\n");
		}
		while (dane == -1) {
			printf("Kordynaty podany w blednym formacie. Odpowiedni format to <numer_pola> <kierunek>");
			dane = WprowadzZadanie(2);
		}
		wynik = UstawStatek(konfiguracja, gracz, dlugosc, dane / 100, dane % 100, rodzaj);
		if (wynik == 0) {
			printf("Nie mozna tu ustawic statku. Podaj ponownie: ");
		}
	} while (wynik == 0 /*&& printf("Nie mozna tu ustawic statku. Podaj ponownie: ")*/);
	DodajdoListy(historia, ustaw, dlugosc * 10000 + dane, rodzaj);
	printf("\n");
	RysujPlansze(konfiguracja, *gracz, 0);

}

int WprowadzZadanie(int liczbaArgumentow) {
	char buffor[15];
	int i, wynik, arg1, arg2;
	do {
		wynik = scanf("%14[^\n]s", buffor);
		if (wynik == 0 && liczbaArgumentow == 0) {
			return 0;
		}
		WyczyscBufor();
	} while (wynik != 1);
	//Zwiêkszenie zakresu poprawnych komend przez znormalizowanie tekstu
	for (i = 0; buffor[i] != '\0'; i++) {
		buffor[i] = tolower((unsigned char)buffor[i]);
	}

	if (!strcmp(buffor, "cofnij")) {
		printf("\nproba cofania\n");
		return -3;
	}
	if (!strcmp(buffor, "zapisz")) {
		printf("\nproba zapisu\n");
		return -4;
	}
	if (!strcmp(buffor, "wczytaj")) {
		printf("\nproba wczytywania\n");
		return -5;
	}
	if (!strcmp(buffor, "flota")) {
		printf("\nRaport:\n");
		return -2;
	}
	if (liczbaArgumentow == 0) {
		return 0;
	}
	if (sscanf(buffor, "%d%d", &arg1, &arg2) < liczbaArgumentow)
		return -1;
	if (liczbaArgumentow < 2) {
		arg2 = 0;
	}

	return arg1 * 100 + arg2;

}


void GameLoop(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka) {
	int tura = 1, wynik1, wynik2;
	int trigger = mxmlGetInteger(rozgrywka->czyja_n);//zmienna steruj¹ca, pozwala pomin¹æ turê gracza pierwszego, je¿eli dane z pliku tego wymagaj¹.
	WyczyscBufor();
	do {
		if (trigger != 2) {
			tura = mxmlGetInteger(rozgrywka->tura_n);
			Oczysc(konfiguracja);
			mxmlSetInteger(rozgrywka->czyja_n, 1);
			printf("Tura gracza pierwszego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			printf("Tura: %d \n", tura);
			wynik1 = Bitwa(konfiguracja, rozgrywka);
			if (wynik1 == konfiguracja->b_wczytaj) {
				if (mxmlGetInteger(rozgrywka->czyja_n) == 1) {
					wynik2 = 1;//instieje mo¿liwoœæ, ¿e zmienna zostanie u¿yta bez iniciacji dlatego odrazu iniciuje j¹ nie zmieniaj¹c¹ dzia³ania wartosci¹.
					continue;
				}
				tura = mxmlGetInteger(rozgrywka->tura_n);
				//w przypadku gdy powinna byæ tura gracza 2 nic nie zmienia kolejnoœci po wczytaniu, zmienia siê tylko licznik tur.
			}
			if (wynik1 == konfiguracja->b_koniec) {
				break;
			}
		}
		ZamianaGraczy(rozgrywka);

		trigger = 1;

		Oczysc(konfiguracja);
		mxmlSetInteger(rozgrywka->czyja_n, 2);
		printf("Tura gracza drugiego. Nacisnij dowolny klawisz.\n");
		WyczyscBufor();
		printf("Tura: %d \n", tura);
		wynik2 = Bitwa(konfiguracja, rozgrywka);
		if (wynik2 == konfiguracja->b_wczytaj) {
			if (mxmlGetInteger(rozgrywka->czyja_n) == 2) {
				wynik1 = 1;
				trigger = 2;
			}
		}
		ZamianaGraczy(rozgrywka);
		mxmlSetInteger(rozgrywka->tura_n, ++tura);

	} while (wynik1 && wynik2);
	ZakonczGre(konfiguracja, rozgrywka, wynik1);
}

void GameLoopAI(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka) {
	//inicializacja sztucznej inteligencji

	InicializujAI(&rozgrywka->AI, konfiguracja);
	int tura = 1, wynik1, wynik2;
	do {
		tura = mxmlGetInteger(rozgrywka->tura_n);
		printf("Tura: %d \n", tura);
		wynik1 = Bitwa(konfiguracja, rozgrywka);
		if (wynik1 == konfiguracja->b_wczytaj) {//powstrzymanie komputera przed wykonaniem ruchu.
			wynik2 = 1;
			continue;
		}
		DodajdoListy(&rozgrywka->ruchy, start, 0, 0);
		wynik2 = BitwaAI(konfiguracja, rozgrywka, &rozgrywka->AI);
		mxmlSetInteger(rozgrywka->tura_n, ++tura);
	} while (wynik1 && wynik2);
	ZakonczGre(konfiguracja, rozgrywka, wynik1);
}


void ZakonczGre(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka, int wynik) {
	
	printf("Gracz pierwszy:\n");
	RysujPlansze(konfiguracja, rozgrywka->gracz1, 0);
	printf("Gracz drugi:\n");
	RysujPlansze(konfiguracja, rozgrywka->gracz2, 0);

	if (wynik == konfiguracja->b_koniec) {
		printf("Wygral pierwszy gracz");
	}
	else if (konfiguracja->trybGry == 1) {
		printf("Wygral drugi gracz");
	}
	else
	{
		printf("Wygral Komputer");
	}
	free(konfiguracja->plikZapisu);
	mxmlDelete(rozgrywka->xml);
	UsunListe(&rozgrywka->ruchy);
	UsunTablice(konfiguracja, &rozgrywka->gracz1, &rozgrywka->gracz2);
	WyczyscBufor();
	getchar();
}

int Bitwa(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka) {

	WypiszRuchy(rozgrywka->ruchy);
	DodajdoListy(&rozgrywka->ruchy, start, 0, 0);

	printf("Oto twoje pole:\n");
	RysujPlansze(konfiguracja, rozgrywka->gracz1, 0);
	printf("Oto plansza przeciwnika:\n");
	RysujPlansze(konfiguracja, rozgrywka->gracz2, 1);

	//pêtla do pobierania informacji. Jest z zawsze prawdziwym warunkiem aby jedyne co mog³o j¹ zatrzymaæ to zwrócenie wartoœæi funkcji
	while (1) {
		int cel, wynik, wynik_wczytywania;
		printf("Gdzie strzelac: ");
		do {
			do {
				cel = WprowadzZadanie(1);
				if (cel == konfiguracja->zad_zapisz) {
					if (Zapisz(konfiguracja, rozgrywka)) {
						printf("Udany zapis\n");
						printf("Gdzie strzelac: ");

					}
					else {
						printf("Nieudane zapisywanie, sprawdz czy plik zapisu nie jest otwarty.\n");
					}
					cel = -1;
					continue;
				}
				if (cel == konfiguracja->zad_cofnij) {
					printf("Nie mozna teraz wykonac tej akcji.\nPodaj inna komende lub miejsce strzalu:");
					cel = -1;
				}
				if (cel == konfiguracja->zad_flota) {
					WypiszFlote(&rozgrywka->gracz2);
					cel = -1;
					printf("Gdzie strzelac: ");
				}
				if (cel == konfiguracja->zad_wczytaj) {
					if ((wynik_wczytywania = Wczytaj(konfiguracja, rozgrywka)) == 1) {
						printf("Udane wczytanie\n");
						return konfiguracja->b_wczytaj;

					}
					else if (wynik_wczytywania == -1) {
						printf("Blad konfiguracji. Zmien konfiguracje na zgodna z zapisem i uruchom ponownie.\nOczekiwane wartosci to:\nX:%d\nY:%d\n", konfiguracja->rozmiarPola_x, konfiguracja->rozmiarPola_y);
					}
					else {
						printf("Nieudane wczytywanie, sprawdz czy plik zapisu istnieje i nie jest otwarty. Plik moze byc uszkodzony.\n");
					}
					cel = -1;
					continue;
				}
			} while (cel == -1);
			wynik = Strzal(konfiguracja, &rozgrywka->gracz2, cel / 100);
			if (wynik == konfiguracja->st_blad) {
				printf("W te pole nie mozna wycelowac. Podaj ponownie:");
			}
		} while (wynik == konfiguracja->st_blad);

		if (wynik > 1) {
			if (wynik == konfiguracja->st_cel)
				printf("Trafiony!\n");
			else
				printf("Trafiony zatopiony!\n");
			DodajdoListy(&rozgrywka->ruchy, strzal, cel / 100, 1);
			RysujPlansze(konfiguracja, rozgrywka->gracz2, 1);
			if (rozgrywka->gracz2.statki == 0) {
				return konfiguracja->b_koniec;
			}
		}

		if (wynik == konfiguracja->st_pudlo) {
			printf("Pudlo :(\n");
			DodajdoListy(&rozgrywka->ruchy, strzal, cel / 100, 0);
			return konfiguracja->b_niekoniec;
		}

	}
}
