#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funkcje.h"
#include <time.h>

#if _WIN32
#include <Windows.h>
#endif
#include "mxml-3.0/mxml.h"



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
	ZmienKolor(BIALY);
	FILE* plikKonfiguracji = fopen("config.ini","r");
	if (plikKonfiguracji == NULL) {
		printf("Brak pliku konfiguracji");
		return 0;
	}
	char buffor[30];
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->rozmiarPola_x);
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->rozmiarPola_y);
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->no_system_new_line_number);
	fscanf(plikKonfiguracji, "%s %d", &buffor, &konfiguracja->trudnosc);
	if (konfiguracja->rozmiarPola_x < 5 || konfiguracja->rozmiarPola_x>10 || konfiguracja->rozmiarPola_y < 5 || konfiguracja->rozmiarPola_y>10) {
		printf("PLIK KONIFIGURACJI: Podano rozmiar poza zakresem. Dozwolony zakres to miêdzy 5, a 10");
		return 0;
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
	if (WypelnijTablice(konfiguracja,&rozgrywka->gracz1, &rozgrywka->gracz2) == 0) {
		printf("Bledna alokacja pamieci. Sprobuj uruchomic ponownie.");
		return 0;
	}
	if (konfiguracja->wczytaj == 1) {//rozpoczêcie od wczytywania

		if (Wczytaj(konfiguracja,rozgrywka) == 0) {

			printf("Nie znaleziono zapisu lub plik z zapisem jest uszkodzony. \nSprobuj uruchomic gre ponownie lub zaczac od poczatku");
			UsunTablice(konfiguracja,&rozgrywka->gracz1, &rozgrywka->gracz2);
			return 0;
		}
		konfiguracja->trybGry = mxmlGetInteger(mxmlFindPath(rozgrywka->xml, "Informacje/Ustawienia/Tryb_Gry"));
	}
	else {//przygotowanie do gry
		Oczysc();
		if (konfiguracja->automat == 0) {
			Rozmieszczenie(konfiguracja,&rozgrywka->gracz1);
		}
		else { //automat == 1-automatyczne rozmieszczenie.
			AutoRozmieszczenie(konfiguracja, &rozgrywka->gracz1);
		}
		if (konfiguracja->trybGry == 1) {
			Oczysc();
			printf("Tura gracza drugiego. Nacisnij dowolny klawisz");
			getchar();
			Oczysc();
			Rozmieszczenie(konfiguracja, &rozgrywka->gracz2);
		}
		else {
			AutoRozmieszczenie(konfiguracja, &rozgrywka->gracz2);
		}

		UtworzZapis(konfiguracja, rozgrywka);
	}
	
}

void ZamianaGraczy(Rozgrywka* rozgrywka) {
	Gracz tymczasowa = rozgrywka->gracz1;
	rozgrywka->gracz1 = rozgrywka->gracz2;
	rozgrywka->gracz2 = tymczasowa;
}

void GameLoop(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka) {
	int tura = 1, wynik1, wynik2;
	int trigger = mxmlGetInteger(rozgrywka->czyja_n);//zmienna steruj¹ca, pozwala pomin¹æ turê gracza pierwszego, je¿eli dane z pliku tego wymagaj¹.
	do {
		if (trigger != 2) {
			tura = mxmlGetInteger(rozgrywka->tura_n);
			Oczysc();
			mxmlSetInteger(rozgrywka->czyja_n, 1);
			printf("Tura gracza pierwszego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			printf("Tura: %d \n", tura);
			wynik1 = Bitwa(konfiguracja,rozgrywka);
			if (wynik1 == konfiguracja->b_wczytaj) {
				if (mxmlGetInteger(rozgrywka->czyja_n) == 1) {
					wynik2 = 1;//instieje mo¿liwoœæ, ¿e zmienna zostanie u¿yta bez iniciacji dlatego odrazu iniciuje j¹ nie zmieniaj¹c¹ dzia³ania wartosci¹.
					continue;
				}
				tura = mxmlGetInteger(rozgrywka->tura_n);
				//w przypadku gdy powinna byæ tura gracza 2 nic nie zmienia kolejnoœci po wczytaniu, zmienia siê tylko licznik tur.
			}
		}
		ZamianaGraczy(konfiguracja);

		trigger = 1;

		Oczysc();
		mxmlSetInteger(rozgrywka->czyja_n, 2);
		printf("Tura gracza drugiego. Nacisnij dowolny klawisz.\n");
		WyczyscBufor();
		printf("Tura: %d \n", tura);
		wynik2 = Bitwa(konfiguracja,rozgrywka);
		if (wynik2 == konfiguracja->b_wczytaj) {
			if (mxmlGetInteger(rozgrywka->czyja_n) == 2) {
				wynik1 = 1;
				trigger = 2;
			}
		}
		ZamianaGraczy(konfiguracja);
		mxmlSetInteger(rozgrywka->tura_n, ++tura);

	} while (wynik1 && wynik2);
}

void GameLoopAI(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka) {
	//inicializacja sztucznej inteligencji
	Wybor AI;
	InicializujAI(&AI, konfiguracja);
	int tura = 1, wynik1, wynik2;
	do {
		tura = mxmlGetInteger(rozgrywka->tura_n);
		printf("Tura: %d \n", tura);
		wynik1 = Bitwa(konfiguracja,rozgrywka);
		if (wynik1 == konfiguracja->b_wczytaj) {//powstrzymanie komputera przed wykonaniem ruchu.
			wynik2 = 1;
			continue;
		}
		DodajdoListy(&rozgrywka->ruchy, start, 0, 0);
		wynik2 = BitwaAI(konfiguracja,rozgrywka, &AI);
		mxmlSetInteger(rozgrywka->tura_n, ++tura);
	} while (wynik1 && wynik2);
	ZakonczGre(konfiguracja, rozgrywka, wynik1);
}

void InicializujAI(Wybor* AI, Konfiguracja* konfiguracja) {
	srand(time(NULL));
	AI->aktualnePole = 0;
	AI->stanPoprzedni = 0;
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

void ZakonczGre(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka, int wynik) {
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
	UsunTablice(konfiguracja,&rozgrywka->gracz1, &rozgrywka->gracz2);
	WyczyscBufor();
	getchar();
}


int UtworzZapis(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka) {
	mxml_node_t *xml;
	mxml_node_t *informacje;
	mxml_node_t *dane;
	mxml_node_t *wartosc;
	mxml_node_t *element;
	xml = mxmlNewXML("1.0");
	informacje = mxmlNewElement(xml, "Informacje");
		dane = mxmlNewElement(informacje, "Ustawienia");
			wartosc = mxmlNewElement(dane, "Tryb_Gry");
			mxmlNewInteger(wartosc, konfiguracja->trybGry);
			wartosc = mxmlNewElement(dane, "Tura");
			mxmlNewInteger(wartosc, 1);
			wartosc = mxmlNewElement(dane, "Czyja_Tura");
			mxmlNewInteger(wartosc, 1);
		dane = mxmlNewElement(informacje, "Gracz1");
			wartosc= mxmlNewElement(dane, "Statki");
			mxmlNewInteger(wartosc, rozgrywka->gracz1.statki);
			wartosc = mxmlNewElement(dane, "Pole");
			int i,j;
			for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
				for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
					element = mxmlNewElement(wartosc, "Wartosc");
					mxmlNewInteger(element, rozgrywka->gracz1.pole[i][j]);
				}
			}
		dane = mxmlNewElement(informacje, "Gracz2");
			wartosc = mxmlNewElement(dane, "Statki");
			mxmlNewInteger(wartosc, rozgrywka->gracz2.statki);
			wartosc = mxmlNewElement(dane, "Pole");
			
			for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
				for (j = 0; j < konfiguracja->rozmiarPola_x; j++) {
					element = mxmlNewElement(wartosc, "Wartosc");
					mxmlNewInteger(element, rozgrywka->gracz2.pole[i][j]);
				}
			}
		dane = mxmlNewElement(informacje, "Ruchy");
			wartosc = mxmlNewElement(dane, "Element");
			mxmlNewInteger(wartosc, -1);
		if (rozgrywka->xml) {
			mxmlDelete(rozgrywka->xml);
		}
	rozgrywka->xml = xml;
	rozgrywka->tura_n = mxmlFindPath(rozgrywka->xml, "Informacje/Ustawienia/Tura");
	rozgrywka->czyja_n = mxmlFindPath(rozgrywka->xml, "Informacje/Ustawienia/Czyja_Tura");
	FILE *fp;

	fp = fopen(konfiguracja->plikZapisu, "w");
	if (fp) {
		mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
		fclose(fp);
		return 1;
	}
	return 0;
}

int Zapisz(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka) {
	
	mxml_node_t* informacje;
	mxml_node_t *dane;
	mxml_node_t *wartosc;
	int czyja;
	informacje = mxmlGetFirstChild(rozgrywka->xml);
	dane = mxmlFindPath(informacje, "Ustawienia/Czyja_Tura");
	czyja = mxmlGetInteger(dane);
	if (czyja == 2) { //zamaiana kolejnosci graczy, aby zachowaæ sposób kodowania informacji
		ZamianaGraczy(rozgrywka);
	}
	dane = mxmlFindPath(informacje, "Gracz1");
	wartosc = mxmlGetFirstChild(dane);
	mxmlSetInteger(wartosc, rozgrywka->gracz1.statki);
	dane = mxmlFindPath(dane, "Pole");
	wartosc = mxmlGetFirstChild(dane);
	int i,j;
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			mxmlSetInteger(wartosc, rozgrywka->gracz1.pole[i][j]);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}

	dane = mxmlFindPath(informacje, "Gracz2");
	wartosc = mxmlGetFirstChild(dane);
	mxmlSetInteger(wartosc, rozgrywka->gracz2.statki);
	dane = mxmlFindPath(dane, "Pole");
	wartosc = mxmlGetFirstChild(dane);
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			mxmlSetInteger(wartosc, rozgrywka->gracz2.pole[i][j]);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}
	dane = mxmlFindPath(informacje, "Ruchy");
	mxmlDelete(dane);
	dane = mxmlNewElement(informacje, "Ruchy");
	Historia* ruchy= rozgrywka->ruchy;
	while (ruchy != 0 && ruchy->zadanie!=start) {
		wartosc = mxmlNewElement(dane, "Element");
		mxmlNewInteger(wartosc, ruchy->argument * 10 + ruchy->rodzaj);
		ruchy = ruchy->pPoprzednia;
	}
	FILE *fp;

	fp = fopen(konfiguracja->plikZapisu, "w");
	if (fp) {
		mxmlSaveFile(rozgrywka->xml, fp, MXML_NO_CALLBACK);
		fclose(fp);
		return 1;
	}
	return 0;

}

int Wczytaj(Konfiguracja* konfiguracja, Rozgrywka *rozgrywka) {
	FILE *plik;
	mxml_node_t *drzewo;
	mxml_node_t *informacje;
	mxml_node_t *dane;
	mxml_node_t *wartosc;

	plik = fopen(konfiguracja->plikZapisu, "r");
	if (plik == 0) {
		return 0;
	}
	drzewo = mxmlLoadFile(NULL, plik, MXML_INTEGER_CALLBACK);
	if (drzewo == 0) {
		return 0;
	}
	if (rozgrywka->xml != 0) {
		mxmlDelete(rozgrywka->xml);
	}
	rozgrywka->xml = drzewo;
	//Odczytwyanie danych graczy
	informacje = mxmlGetFirstChild(drzewo);
	dane = mxmlFindPath(informacje, "Gracz1/Statki");
	rozgrywka->gracz1.statki = mxmlGetInteger(dane);
	dane = mxmlFindPath(informacje, "Gracz1/Pole");
	wartosc = mxmlGetFirstChild(dane);
	int i, j;
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			rozgrywka->gracz1.pole[i][j] = mxmlGetInteger(wartosc);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}
	dane = mxmlFindPath(informacje, "Gracz2/Statki");
	rozgrywka->gracz2.statki = mxmlGetInteger(dane);
	dane = mxmlFindPath(informacje, "Gracz2/Pole");
	wartosc = mxmlGetFirstChild(dane);
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			rozgrywka->gracz2.pole[i][j] = mxmlGetInteger(wartosc);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}
	//Wczytywanie listy ruchów
	dane = mxmlFindPath(informacje, "Ruchy");
	DodajdoListy(&rozgrywka->ruchy, start, 0, 0);
	wartosc = mxmlGetLastChild(dane);
	while (wartosc != 0) {
		int dane = mxmlGetInteger(wartosc);
		DodajdoListy(&rozgrywka->ruchy, strzal, dane / 10, dane % 10);
		wartosc = mxmlGetPrevSibling(wartosc);
	}
	return 1;
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

void UsunListe(Historia**lista) {
	while (*lista != 0) {
		Historia* nastepna = (*lista)->pPoprzednia;
		free(*lista);
		*lista = nastepna;
	}
}

void WypiszRuchy(Historia* ruchy) {
	if (ruchy->pPoprzednia == 0 || ruchy->zadanie==start)
		return;

	printf("Twoj przeciwnik zakonczyl ture.\nWykonane przez niego ruchy to:\n");
	int i, iloscRuchow=0;
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
	printf("Dwumasztowce: %d\n", ((przeciwnik->statki / mnoznik) % 8!=0 )+ ((przeciwnik->statki / (mnoznik * 8)) % 8!=0) +((przeciwnik->statki / (mnoznik * 8 * 8)) % 8 != 0));
	mnoznik *= 8 * 8 * 8;
	printf("Trzymasztowce: %d\n",( (przeciwnik->statki / mnoznik) % 8 != 0) + ((przeciwnik->statki / (mnoznik * 8)) % 8 != 0));
	mnoznik *= 8 * 8;
	printf("Czteromasztowiec: %d\n", (przeciwnik->statki / mnoznik) % 8 != 0);
}


void WyczyscBufor() {
	char pozostalosc;
	do {
		pozostalosc = getchar();
	} while (pozostalosc != '\n');
}

void ZmienKolor(int typ) {
#if defined(_WIN32) || defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
#if defined(_WIN32)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), typ);
#endif
#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
	switch (typ)
	{case BIALY:
		printf(UBIALY);
		break;
	case CZERWONY:
		printf(UCZERWONY);
		break;
	case ZIELONY:
		printf(UZIELONY);
		break;
	case NIEBIESKI:
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

void Oczysc() {
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
	for (i = 0; i < NO_SYS_NEWLINE; i++) {
		printf("\n");
	}

#endif
}

void RysujPlansze(Konfiguracja* konfiguracja, Gracz player, int dyskrecja) {
	int i, j, c=0;
	
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		printf("|");
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			if (player.pole[i][j] == -1) {
				ZmienKolor(CZERWONY);
				printf("%3c", 'X');
				ZmienKolor(BIALY);
				printf("|");
			}
			else if (player.pole[i][j] == -2) {
				ZmienKolor(NIEBIESKI);
				printf("%3c", 'U');
				ZmienKolor(BIALY);
				printf("|");
			}
			else if (player.pole[i][j] >=-9 && player.pole[i][j] <= -3 && dyskrecja==0) {
				
				ZmienKolor(ZIELONY);
				printf("%3c", 'V');
				ZmienKolor(BIALY);
				printf("|");
			}
			else {
				printf("%3d|", (i*10+j));
			}
		}
		printf("\n");
	}
	
}

int UstawStatek(Konfiguracja* konfiguracja, Gracz* gracz, int dlugosc, int pole, int kierunek, int rodzajStatku) {
	int x = pole / 10;
	int y = pole % 10;
	int i;
	if (x<0 || x>= konfiguracja->rozmiarPola_x || y<0 || y>= konfiguracja->rozmiarPola_y)
		return 0;
	if (kierunek) {
		if (x + dlugosc > konfiguracja->rozmiarPola_x)
			return 0;
		x -= 1;
		for (i = 0; i <= dlugosc +1 ; i++) {
			if (x>=0 && x< konfiguracja->rozmiarPola_x) {
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
		for (i = 0; i <= dlugosc +1; i++) {
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

int UsunStatek(Konfiguracja* konfiguracja ,Historia** historia, Gracz* gracz) {
	Historia* temp = *historia;
	if (temp->zadanie == ustaw){
		
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
		
		RysujPlansze(konfiguracja,*gracz, 0);

		PobierzKoordynaty(konfiguracja, dlugosc, gracz, &(temp->pPoprzednia), temp->rodzaj);
		return 1;
	}
	if (temp->zadanie == start) {
		return 0;
	}
	return -1;

}


void PobierzKoordynaty(Konfiguracja* konfiguracja,int dlugosc, Gracz* gracz, Historia** historia,int rodzaj) {
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
		if (dlugosc == 1){
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
	RysujPlansze(konfiguracja,*gracz, 0);

}


int WprowadzZadanie(int liczbaArgumentow) {
	char buffor[15];
	int i, wynik, arg1,arg2;
	do {
		wynik = scanf("%14[^\n]s", buffor);
		if (liczbaArgumentow == 0 && getchar() == '\n') {
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

	return arg1*100+arg2;

}


void Rozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz) {
	Historia* historia = 0;
	DodajdoListy(&historia, start, 0, 0);
	gracz->statki = 04332224;
	printf("Twoje Pole: \n");
	RysujPlansze(konfiguracja, *gracz, 0);
	printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");
	int i, c, pole;
	c = -9;
	pole = konfiguracja->rozmiarPola_x * konfiguracja->rozmiarPola_y;
	
		PobierzKoordynaty(konfiguracja, 4, gracz, &historia, c++);
	for (i = 0; i < (pole / 60)+1; i++, c++) {
		PobierzKoordynaty(konfiguracja, 3, gracz, &historia, c);
	}
	for (i = 0; i < (pole / 30) + 1; i++, c++) {
		PobierzKoordynaty(konfiguracja, 2, gracz, &historia, c);
	}
	for (i = 0; i < (pole / 26) + 1; i++) {
		PobierzKoordynaty(konfiguracja, 1, gracz, &historia, c);
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
			RysujPlansze(konfiguracja,*gracz, 0);
		}
	} while (komenda != konfiguracja->zad_brak);
	UsunListe(&historia);
}
void AutoRozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz) {
	gracz->statki = 0;
	int i, c, pole;
	c = -9;
	pole = konfiguracja->rozmiarPola_x * konfiguracja->rozmiarPola_y;

		while (!UstawStatek(konfiguracja, gracz, 4, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 4);
	for (i = 0, c=-8; i < (pole / 60) + 1; i++, c++) {
		while (!UstawStatek(konfiguracja, gracz, 3, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 3);
	}
	for (i = 0, c=-6; i < (pole / 30) + 1; i++, c++) {
		while (!UstawStatek(konfiguracja, gracz, 2, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 2);
	}
	for (i = 0, c=-3; i < (pole / 26) + 1; i++) {
		while (!UstawStatek(konfiguracja, gracz, 1, rand() % 100, rand() % 2, c));
		DodajdoFloty(gracz, c, 1);
	}

	Oczysc();
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




int WypelnijTablice(Konfiguracja* konfiguracja, Gracz* gracz1, Gracz*gracz2) {
	if((gracz1->pole = malloc(konfiguracja->rozmiarPola_x * sizeof(int*)))==NULL) return 0;
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
			gracz1->pole[i][j] = gracz2->pole[i][j] = 10*i+j;
		}

	}
	return 1;
}

void UsunTablice(Konfiguracja* konfiguracja ,Gracz* gracz1, Gracz*gracz2) {
	int i;
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		free(gracz1->pole[i]);
		free(gracz2->pole[i]);
	}
	free(gracz1->pole);
	free(gracz2->pole);
}

int Strzal(Konfiguracja* konfiguracja, Gracz* atakowanyGracz, int pole) {
	int x = pole / 10;
	int y = pole % 10;

	if (x<0 || x>=konfiguracja->rozmiarPola_x || y<0 || y>=konfiguracja->rozmiarPola_y) {
		return konfiguracja->st_blad;
	}

	if (atakowanyGracz->pole[x][y] <=-3 && atakowanyGracz->pole[x][y]>=-9) { //-3 1-masztowiec, (-6)do(-4) 2-maszztowiec, (-8)do(-7) 3-maszztowiec, -9 4-maszztowiec
		int pozycja = 01;
		switch (atakowanyGracz->pole[x][y])
		{case -3:
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
		
		if (((atakowanyGracz->statki / pozycja) % 8 == 0) || pozycja== 01) {
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

int Bitwa(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka) {

	WypiszRuchy(rozgrywka->ruchy);
	DodajdoListy(&rozgrywka->ruchy, start, 0, 0);

	printf("Oto twoje pole:\n");
	RysujPlansze(konfiguracja,rozgrywka->gracz1, 0);
	printf("Oto plansza przeciwnika:\n");
	RysujPlansze(konfiguracja ,rozgrywka->gracz2, 1);

	//pêtla do pobierania informacji. Jest z zawsze prawdziwym warunkiem aby jedyne co mog³o j¹ zatrzymaæ to zwrócenie wartoœæi funkcji
	while (1) {
		int cel,wynik;
		printf("Gdzie strzelac: ");
		do {
			do {
				cel = WprowadzZadanie(1);
				if (cel == konfiguracja->zad_zapisz) {
					if (Zapisz(konfiguracja,rozgrywka)) {
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
					if (Wczytaj(konfiguracja,rozgrywka)){
						printf("Udane wczytanie\n");
						return konfiguracja->b_wczytaj;
						
					}
					else {
						printf("Nieudane wczytywanie, sprawdz czy plik zapisu istnieje i nie jest otwarty. Plik moze byc uszkodzony.\n");
					}
					cel = -1;
					continue;
				}
			} while (cel == -1);
			wynik = Strzal(konfiguracja,&rozgrywka->gracz2, cel / 100);
			if (wynik == konfiguracja->st_blad) {
				printf("W te pole nie mozna wycelowac. Podaj ponownie:");
			}
		}while (wynik == konfiguracja->st_blad);

		if (wynik > 1) {
			if (wynik == konfiguracja->st_cel)
				printf("Trafiony!\n");
			else
				printf("Trafiony zatopiony!\n");
			DodajdoListy(&rozgrywka->ruchy, strzal, cel / 100, 1);
			RysujPlansze(konfiguracja, rozgrywka->gracz2,1);
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


int Losuj(Konfiguracja *konfiguracja, int poprzedniePole) {
	int x = rand()% (konfiguracja->rozmiarPola_x);
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
		do{
			if (AI->stanPoprzedni > 5) { //przechwytywanie osobliwych b³êdów w celowaniu
				AI->stanPoprzedni = 0;
			}
			cel = AI->stan[AI->stanPoprzedni](konfiguracja,AI->aktualnePole);
			AI->aktualnePole = cel;

		} while (cel == -1 && AI->stanPoprzedni++);
		wynik = Strzal(konfiguracja,&rozgrywka->gracz1, cel);
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
		if (wynik == konfiguracja->st_cel && AI->stanPoprzedni==3 ) { // gdy celowano na skos i trafiono odrazu przechodzi siê do nastêpnego
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

