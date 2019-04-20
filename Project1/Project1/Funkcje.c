#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Funkcje.h"

#if _WIN32
#include <Windows.h>
#endif
//#include <mxml.h>
#include "mxml-3.0/mxml.h"

const char* callback(mxml_node_t* node, int gdzie) {
	const char* element;

	element = mxmlGetElement(node);
	if (!strcmp(element, "Wartosc")) {
		return ("\n");
	}


	if (gdzie == MXML_WS_BEFORE_OPEN || gdzie == MXML_WS_AFTER_CLOSE) {
		return ("\n");
	}
	return NULL;
}


int UtworzZapis(int trybGry,int tura, int czyja,Gracz gracz1, Gracz gracz2, mxml_node_t** drzewo) {
	mxml_node_t *xml;
	mxml_node_t *informacje;
	mxml_node_t *dane;
	mxml_node_t *wartosc;
	mxml_node_t *element;
	xml = mxmlNewXML("1.0");
	informacje = mxmlNewElement(xml, "Informacje");
		dane = mxmlNewElement(informacje, "Ustawienia");
			wartosc = mxmlNewElement(dane, "Tryb_Gry");
			mxmlNewInteger(wartosc, trybGry);
			wartosc = mxmlNewElement(dane, "Tura");
			mxmlNewInteger(wartosc, tura);
			wartosc = mxmlNewElement(dane, "Czyja_Tura");
			mxmlNewInteger(wartosc, czyja);
		dane = mxmlNewElement(informacje, "Gracz1");
			wartosc= mxmlNewElement(dane, "Statki");
			mxmlNewInteger(wartosc, gracz1.statki);
			wartosc = mxmlNewElement(dane, "Pole");
			int i,j;
			for (i = 0; i < ROZMIAR_POLA; i++) {
				for (j = 0; j < ROZMIAR_POLA; j++) {
					element = mxmlNewElement(wartosc, "Wartosc");
					mxmlNewInteger(element, gracz1.pole[i][j]);
				}
			}
		dane = mxmlNewElement(informacje, "Gracz2");
			wartosc = mxmlNewElement(dane, "Statki");
			mxmlNewInteger(wartosc, gracz2.statki);
			wartosc = mxmlNewElement(dane, "Pole");
			
			for (i = 0; i < ROZMIAR_POLA; i++) {
				for (j = 0; j < ROZMIAR_POLA; j++) {
					element = mxmlNewElement(wartosc, "Wartosc");
					mxmlNewInteger(element, gracz2.pole[i][j]);
				}
			}
		dane = mxmlNewElement(informacje, "Ruchy");
			wartosc = mxmlNewElement(dane, "Element");
			mxmlNewInteger(wartosc, -1);
	*drzewo = xml;
	FILE *fp;

	fp = fopen("Zapis.xml", "w");
	if (fp) {
		mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
		fclose(fp);
		return 1;
	}
	return 0;
}

int Zapisz(Gracz gracz1, Gracz gracz2, mxml_node_t**xml, Historia* ruchy) {
	mxml_node_t* informacje;
	mxml_node_t *dane;
	mxml_node_t *wartosc;
	int czyja;
	informacje = mxmlGetFirstChild(*xml);
	dane = mxmlFindPath(informacje, "Ustawienia/Czyja_Tura");
	czyja = mxmlGetInteger(dane);
	if (czyja == 2) { //zamaiana kolejnosci graczy, aby zachowaæ sposób kodowania informacji
		Gracz tymczasowa = gracz1;
		gracz1 = gracz2;
		gracz2 = tymczasowa;
	}
	dane = mxmlFindPath(informacje, "Gracz1");
	wartosc = mxmlGetFirstChild(dane);
	mxmlSetInteger(wartosc, gracz1.statki);
	dane = mxmlFindPath(dane, "Pole");
	wartosc = mxmlGetFirstChild(dane);
	int i,j;
	for (i = 0; i < ROZMIAR_POLA; i++) {
		for (j = 0; j < ROZMIAR_POLA; j++) {
			int wynik = gracz1.pole[i][j];
			mxmlSetInteger(wartosc, gracz1.pole[i][j]);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}


	dane = mxmlFindPath(informacje, "Gracz2");
	wartosc = mxmlGetFirstChild(dane);
	mxmlSetInteger(wartosc, gracz2.statki);
	dane = mxmlFindPath(dane, "Pole");
	wartosc = mxmlGetFirstChild(dane);
	for (i = 0; i < ROZMIAR_POLA; i++) {
		for (j = 0; j < ROZMIAR_POLA; j++) {
			int wynik = gracz2.pole[i][j];
			mxmlSetInteger(wartosc, gracz2.pole[i][j]);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}
	dane = mxmlFindPath(informacje, "Ruchy");
	mxmlDelete(dane);
	dane = mxmlNewElement(informacje, "Ruchy");
	while (ruchy != 0 && ruchy->zadanie!=start) {
		wartosc = mxmlNewElement(dane, "Element");
		mxmlNewInteger(wartosc, ruchy->argument * 10 + ruchy->rodzaj);
		ruchy = ruchy->pPoprzednia;
	}
	FILE *fp;

	fp = fopen("Zapis.xml", "w");
	if (fp) {
		mxmlSaveFile(*xml, fp, MXML_NO_CALLBACK);
		fclose(fp);
		return 1;
	}
	return 0;

}

int Wczytaj(Gracz* gracz1,Gracz* gracz2, mxml_node_t**xml, Historia** ruchy) {
	FILE *plik;
	mxml_node_t *drzewo;
	mxml_node_t *informacje;
	mxml_node_t *dane;
	mxml_node_t *wartosc;

	plik = fopen("Zapis.xml", "r");
	if (plik == 0) {
		return 0;
	}
	drzewo = mxmlLoadFile(NULL, plik, MXML_INTEGER_CALLBACK);
	if (drzewo == 0) {
		return 0;
	}
	if (*xml != 0) {
		mxmlDelete(*xml);
	}
	*xml = drzewo;
	informacje = mxmlGetFirstChild(drzewo);
	dane = mxmlFindPath(informacje, "Gracz1/Statki");
	gracz1->statki = mxmlGetInteger(dane);
	dane = mxmlFindPath(informacje, "Gracz1/Pole");
	wartosc = mxmlGetFirstChild(dane);
	int i, j;
	for (i = 0; i < ROZMIAR_POLA; i++) {
		for (j = 0; j < ROZMIAR_POLA; j++) {
			gracz1->pole[i][j] = mxmlGetInteger(wartosc);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}
	dane = mxmlFindPath(informacje, "Gracz2/Statki");
	gracz2->statki = mxmlGetInteger(dane);
	dane = mxmlFindPath(informacje, "Gracz2/Pole");
	wartosc = mxmlGetFirstChild(dane);
	for (i = 0; i < ROZMIAR_POLA; i++) {
		for (j = 0; j < ROZMIAR_POLA; j++) {
			gracz2->pole[i][j] = mxmlGetInteger(wartosc);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}

	dane = mxmlFindPath(informacje, "Ruchy");
	DodajdoListy(ruchy, start, 0, 0);
	wartosc = mxmlGetLastChild(dane);
	while (wartosc != 0) {
		int dane = mxmlGetInteger(wartosc);
		DodajdoListy(ruchy, strzal, dane / 10, dane % 10);
		wartosc = mxmlGetPrevSibling(wartosc);
	}
	return 1;
}


Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj) {
	Historia* temp = (Historia*)malloc(sizeof(Historia));
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
	Historia* temp = 0;
	while (ruchy->zadanie != start) {
		iloscRuchow++;
		DodajdoListy(&temp, strzal, ruchy->argument, ruchy->rodzaj);
		ruchy = ruchy->pPoprzednia;
	}
	for (i = 1; i <= iloscRuchow; i++) {
		printf("%d. Gracz strzelil w pole: %d. %s", i, temp->argument, temp->rodzaj ? "Trafil\n" : "Spudlowal\n");
		temp = temp->pPoprzednia;
	}
	UsunListe(&temp);

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
#if defined(_WIN32) || defined(unix) || defined(__unix__) || defined(__unix)
#if defined(_WIN32)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), typ);
#endif
#if defined(unix) || defined(__unix__) || defined(__unix)
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

#endif
}

void Oczysc() {
#if defined(_WIN32) || defined(unix) || defined(__unix__) || defined(__unix)
#if defined(_WIN32)
	system("cls");
#endif
#if defined(unix) || defined(__unix__) || defined(__unix)
	printf("\033[2J");
	//system("clear");
#endif
#else
	int i;
	for (i = 0; i < 100; i++) {
		printf("\n");
	}

#endif
}



void RysujPlansze(Gracz player, int dyskrecja) {
	int i, j, c=0;
	
	for (i = 0; i < ROZMIAR_POLA; i++) {
		printf("|");
		for (j = 0; j < ROZMIAR_POLA; j++) {
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

int UstawStatek(Gracz* gracz, int dlugosc, int pole, int kierunek, int rodzajStatku) {
	int x = pole / 10;
	int y = pole % 10;
	int i;
	if (x<0 || x>=ROZMIAR_POLA || y<0 || y>=ROZMIAR_POLA)
		return 0;
	if (kierunek) {
		if (x + dlugosc > ROZMIAR_POLA)
			return 0;
		x -= 1;
		for (i = 0; i <= dlugosc +1 ; i++) {
			if (x>=0 && x<ROZMIAR_POLA) {
				if (gracz->pole[x][y] < 0) {
					return 0;
				}
				if (y - 1 >= 0 && gracz->pole[x][y - 1] < 0) {
					return 0;
				}
				if (y + 1 < ROZMIAR_POLA && gracz->pole[x][y + 1] < 0) {
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
		if (y + dlugosc > ROZMIAR_POLA)
			return 0;
		y -= 1;
		for (i = 0; i <= dlugosc +1; i++) {
			if (y >= 0 && y < ROZMIAR_POLA) {
				if (gracz->pole[x][y] < 0) {
					return 0;
				}
				if (x - 1 >= 0 && gracz->pole[x - 1][y] < 0) {
					return 0;
				}
				if (x + 1 < ROZMIAR_POLA && gracz->pole[x + 1][y] < 0) {
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

int UsunStatek(Historia** historia, Gracz* gracz) {
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
		
		RysujPlansze(*gracz, 0);

		PobierzKoordynaty(dlugosc, gracz, &(temp->pPoprzednia), temp->rodzaj);
		return 1;
	}
	if (temp->zadanie == start) {
		return 0;
	}
	printf("Debug error");
	return -1;

}


void PobierzKoordynaty(int dlugosc, Gracz* gracz, Historia** historia,int rodzaj) {
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
			UsunStatek(historia, gracz);
			//RysujPlansze(*gracz, 0);
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
		wynik = UstawStatek(gracz, dlugosc, dane / 100, dane % 100, rodzaj);
		if (wynik == 0) {
			printf("Nie mozna tu ustawic statku. Podaj ponownie: ");
		}
	} while (wynik == 0 /*&& printf("Nie mozna tu ustawic statku. Podaj ponownie: ")*/);
	DodajdoListy(historia, ustaw, dlugosc * 10000 + dane, rodzaj);
	printf("\n");
	RysujPlansze(*gracz, 0);

}


int WprowadzZadanie(int liczbaArgumentow) {
	char buffor[15];
	int wynik, arg1,arg2;
	do {
		wynik = scanf("%14[^\n]", buffor);
		if (liczbaArgumentow == 0 && getchar() == '\n') {
			return 0;
		}
		WyczyscBufor();
	} while (wynik != 1);
		
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


void Rozmieszczenie(Gracz* gracz) {
	Historia* historia = 0;
	DodajdoListy(&historia, start, 0, 0);
	gracz->statki = 04332224;
	printf("Twoje Pole: \n");
	RysujPlansze(*gracz, 0);
	printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");


	PobierzKoordynaty(4, gracz, &historia, -9);
	PobierzKoordynaty(3, gracz, &historia, -8);
	PobierzKoordynaty(3, gracz, &historia, -7);
	PobierzKoordynaty(2, gracz, &historia, -6);
	PobierzKoordynaty(2, gracz, &historia, -5);
	PobierzKoordynaty(2, gracz, &historia, -4);
	PobierzKoordynaty(1, gracz, &historia, -3);
	PobierzKoordynaty(1, gracz, &historia, -3);
	PobierzKoordynaty(1, gracz, &historia, -3);
	PobierzKoordynaty(1, gracz, &historia, -3);


	int komenda;
	do {
		printf("Oto twoje ustawienie. Czy chcesz je zmienic? Nie bedzie mozna tego zrobic pozniej!\nJezeli chcesz cofnac wprowadz komende \"cofnij\"\nJezeli nie, wprowadz dowolny znak");

		komenda = WprowadzZadanie(0);
		if (komenda == -2 || komenda == -4 || komenda == -5) {
			printf("Nie mozna teraz wykonac tej akcji.\n");
		}
		else if (komenda == -3) {
			UsunStatek(&historia, gracz);
			RysujPlansze(*gracz, 0);
		}
	} while (komenda != 0);
	UsunListe(&historia);
}
void AutoRozmieszczenie(Gracz* gracz) {
	gracz->statki = 04332224;
	srand((int)(time(0)));
	while (!UstawStatek(gracz, 4, rand() % 100, rand() % 2, -9));
	while (!UstawStatek(gracz, 3, rand() % 100, rand() % 2, -8));
	while (!UstawStatek(gracz, 3, rand() % 100, rand() % 2, -7));
	while (!UstawStatek(gracz, 2, rand() % 100, rand() % 2, -6));
	while (!UstawStatek(gracz, 2, rand() % 100, rand() % 2, -5));
	while (!UstawStatek(gracz, 2, rand() % 100, rand() % 2, -4));
	while (!UstawStatek(gracz, 1, rand() % 100, rand() % 2, -3));
	while (!UstawStatek(gracz, 1, rand() % 100, rand() % 2, -3));
	while (!UstawStatek(gracz, 1, rand() % 100, rand() % 2, -3));
	while (!UstawStatek(gracz, 1, rand() % 100, rand() % 2, -3));

	Oczysc();
}




void WypelnijTablice(int poleGracza1[ROZMIAR_POLA][ROZMIAR_POLA], int poleGracza2[ROZMIAR_POLA][ROZMIAR_POLA]) {
	int i, j, c = 0;
	for (i = 0; i < ROZMIAR_POLA; i++) {
		for (j = 0; j < ROZMIAR_POLA; j++) {

			poleGracza1[i][j] = poleGracza2[i][j] = c++;

		}

	}
}

int Strzal(Gracz* atakowanyGracz, int pole) {
	int x = pole / 10;
	int y = pole % 10;

	if (x<0 || x>=ROZMIAR_POLA || y<0 || y>=ROZMIAR_POLA) {
		return 0;
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
			return 3;
		}
		return 2;
	}
	else if (atakowanyGracz->pole[x][y] < 0) {
		return 0; 
	}
	else {
		atakowanyGracz->pole[x][y] = -2; //pudlo
		return 1;
	}
}

int Bitwa(Gracz* gracz1, Gracz* gracz2, mxml_node_t** xml, Historia** ruchy) {

	WypiszRuchy(*ruchy);
	DodajdoListy(ruchy, start, 0, 0);

	printf("Oto twoje pole:\n");
	RysujPlansze(*gracz1, 0);
	printf("Oto plansza przeciwnika:\n");
	RysujPlansze(*gracz2, 1);
	
	

	while (1) {
		int cel,wynik;
		printf("Gdzie strzelac: ");
		do {
			do {
				cel = WprowadzZadanie(1);
				if (cel == -4) {
					printf("Proba zapisu \n");
					if (Zapisz(*gracz1, *gracz2, xml,*ruchy)) {
						printf("Udany zapis\n");
						printf("Gdzie strzelac: ");
						
					}
					else {
						printf("Nieudane wczytywanie, sprawdz czy plik zapisu istnieje i nie jest otwarty.\n");
					}
					cel = -1;
					continue;
				}
				if (cel == -3) {
					printf("Nie mozna teraz wykonac tej akcji.\nPodaj inna komende lub miejsce strzalu:");
					cel = -1;
				}
				if (cel == -2) {
					WypiszFlote(gracz2);
					cel = -1;
					printf("Gdzie strzelac: ");
				}
				if (cel == -5) {
					printf("Proba wczytywania \n");
					if (Wczytaj(gracz1, gracz2, xml,ruchy)){
						printf("Udane wczytanie\n");
						//RysujPlansze(*gracz1, 0);
						//printf("Gdzie strzelac: ");
						return 2;
						
					}
					else {
						printf("Nieudane wczytywanie, sprawdz czy plik zapisu istnieje i nie jest otwarty. Plik moze byc uszkodzony.\n");
					}
					cel = -1;
					continue;
				}
			} while (cel == -1);
			wynik = Strzal(gracz2, cel / 100);
			if (wynik == 0) {
				printf("W te pole nie mozna wycelowac. Podaj ponownie:");
			}
		}while (wynik == 0);

		if (wynik > 1) {
			if (wynik == 2)
				printf("Trafiony!\n");
			else
				printf("Trafiony zatopiony!\n");
			DodajdoListy(ruchy, strzal, cel / 100, 1);
			RysujPlansze(*gracz2,1);
			if (gracz2->statki == 0) {
				return 0;
			}
		}	

		if (wynik == 1) {
			printf("Pudlo :(\n");
			DodajdoListy(ruchy, strzal, cel / 100, 0);
			return 1;
		}
		
	}
}


int Losuj(int poprzedniePole) {
	return rand() % 100;
}

int IdzN(int poprzedniePole) {
	int tymczasowa = poprzedniePole - 10;
	if (tymczasowa / 10 < 0) {
		return -1;
	}
	return tymczasowa;

}
int IdzS(int poprzedniePole) {
	int tymczasowa = poprzedniePole + 10;
	if (tymczasowa / 10 >= ROZMIAR_POLA) {
		return -1;
	}
	return tymczasowa;
}
int IdzSkos(int poprzedniePole) {
	int tymczasowa = poprzedniePole + 11;
	if (tymczasowa / 10 >= ROZMIAR_POLA || tymczasowa % 10 >= ROZMIAR_POLA) {
		return -1;
	}
	return tymczasowa;
}
int IdzE(int poprzedniePole) {
	int tymczasowa = poprzedniePole + 1;
	if (tymczasowa % 10 >= ROZMIAR_POLA) {
		return -1;
	}
	return tymczasowa;

}
int IdzW(int poprzedniePole) {
	int tymczasowa = poprzedniePole - 1;
	if (tymczasowa % 10 < 0) {
		return -1;
	}
	return tymczasowa;
}

int BitwaAI(Gracz* atakowanyGracz, Wybor* AI, Historia** ruchy) {
	int wynik, cel;
	do {
		do{
			if (AI->stanPoprzedni > 5) { //przechwytywanie osobliwych b³êdów w celowaniu
				AI->stanPoprzedni = 0;
			}
			cel = AI->stan[AI->stanPoprzedni](AI->aktualnePole);
			AI->aktualnePole = cel;

		} while (cel == -1 && AI->stanPoprzedni++);
		wynik = Strzal(atakowanyGracz, cel);
		if (wynik != 0) {
			DodajdoListy(ruchy, strzal, cel, wynik - 1);
		}
		if (atakowanyGracz->statki == 0) {
			return 0;
		}
		
		if (wynik == 3) {//statek zosta³ zatopiony po strzle
			AI->stanPoprzedni = 0;
			continue;
		}
		if (wynik == 2 && AI->stanPoprzedni==3 ) { // gdy celowano na skos i trafiono odrazu przechodzi siê do nastêpnego
			AI->stanPoprzedni++;
			continue;
		}
		
		if (AI->stanPoprzedni == 0) {
			if (wynik == 2) {//trafiono, spróbuj znaleŸæ resztê statku
				AI->stanPoprzedni++;
			}

		}
		else {

			if (wynik == 1) { //podczas szukania zmienia siê kierunek tylko podczas nietrafienia
				AI->stanPoprzedni++;
			}
		}
		
		
	} while (wynik != 1);
	
	return 1;
}

