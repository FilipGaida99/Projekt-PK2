#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Funkcje.h"

#include <Windows.h>

Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj) {
	Historia* temp = (Historia*)malloc(sizeof(Historia));
	temp->zadanie = zadanie;
	temp->argument = argument;
	temp->rodzaj = rodzaj;
	temp->pPoprzednia = *lista;
	*lista = temp;
	
	return *lista;
}

void WyczyscBufor() {
	char pozostalosc;
	do {
		pozostalosc = getchar();
	} while (pozostalosc != '\n');
}

void ZmienKolor(int typ) {
#if _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), typ);
#else

#endif
}

void Oczysc() {
#if _WIN32
	system("cls");
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
		//TODO przeniesc to do niezale¿nej fukcji bo sie powtarza
		printf("Teraz ustaw usuniety statek ponownie:");
		int dane;
		do {
			dane = WprowadzZadanie(2);
			if (dane == -3) {
				UsunStatek(&temp->pPoprzednia, gracz);
				RysujPlansze(*gracz, 0);
			}
			while (dane == -1) {
				printf("Kordynaty podany w blednym formacie. Odpowiedni format to <numer_pola> <kierunek>");
				dane = WprowadzZadanie(2);
			}
		} while (UstawStatek(gracz, dlugosc, dane / 100, dane % 100,temp->rodzaj) == 0 && printf("Nie mozna tu ustawic statku. Podaj ponownie: "));
		temp->argument = dlugosc * 10000 + dane;
		*historia = temp;

		return 1;
	}
	if (temp->zadanie == start) {
		return 0;
	}
	printf("Debug error");

}


void PobierzKoordynaty(int dlugosc, Gracz* gracz, Historia** historia) {
	int i, j, dane, rodzaj;
	i = 4 - dlugosc;
	rodzaj = -9;
	for (j=0;j<4;j++){
	for (; i >= 0 && dlugosc > 0; i--) {
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
		//dla jednomasztowców pomija siê kierunek
		if (dlugosc == 1) {
			for (i = 0; i < 4; i++) {
				//pêtla do pobierania danych
				do {
					
					dane = WprowadzZadanie(1);
					while (dane == -1) {
						printf("Kordynaty podany w blednym formacie. Odpowiedni format to <numer_pola>");
						dane = WprowadzZadanie(1);
					}
				} while (UstawStatek(gracz, dlugosc, dane / 100, 0,-3) == 0 && printf("Nie mozna tu ustawic statku. Podaj ponownie: "));
				printf("\n");
				RysujPlansze(*gracz, 0);
				DodajdoListy(historia, ustaw, dlugosc * 10000 + dane,-3);

			}
			return;
		
		}
		do {
			dane = WprowadzZadanie(2);
			if (dane == -3) {
				UsunStatek(historia, gracz);
				RysujPlansze(*gracz, 0);
			}
			while (dane==-1) {
				printf("Kordynaty podany w blednym formacie. Odpowiedni format to <numer_pola> <kierunek>");
				dane = WprowadzZadanie(2);
			}
		} while (UstawStatek(gracz, dlugosc, dane/100, dane%100,rodzaj) == 0 && printf("Nie mozna tu ustawic statku. Podaj ponownie: "));
		DodajdoListy(historia, ustaw, dlugosc * 10000 + dane, rodzaj);
		rodzaj++;
		printf("\n");
		RysujPlansze(*gracz, 0);
		}
			dlugosc--;
			i= 4-dlugosc;
	}
}

int WprowadzZadanie(int liczbaArgumentow) {
	char buffor[15];
	int wynik, arg1,arg2;
	do {
		wynik = scanf("%14[^\n]", buffor);
		WyczyscBufor();
	} while (wynik != 1);
	if (!strcmp(buffor, "cofnij")) {
		printf("\nproba cofania");
		return -3;
	}
	if (!strcmp(buffor, "restart")) {
		printf("\nproba restartu");
		return -2;
	}
	if (sscanf(buffor, "%d%d", &arg1, &arg2) < liczbaArgumentow)
		return -1;
	if (liczbaArgumentow < 2) {
		arg2 = 0;
	}
	return arg1*100+arg2;

}




void IniciujGre(Gracz* gracz1, Gracz* gracz2, int trybGry) {
	Historia* historia = 0;
	DodajdoListy(&historia, start, 0,0);
	int pole, kierunek;
	gracz1->statki = gracz2->statki = 04332224;
	printf("%d", gracz2->statki);
	WypelnijTablice(gracz1->pole,gracz2->pole);
	printf("Twoje Pole: \n");
	RysujPlansze(*gracz1, 0);
	printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");
	PobierzKoordynaty(4, gracz1,&historia);
	printf("Oto twoje ustawienie. Czy chcesz je zmienic? Nie bedzie mozna tego zrobic pozniej!\nJezeli chcesz cofnac wprowadz komende \"restart\"\nJezeli nie, wprowadz dowolny znak");
	WyczyscBufor();
	int komenda;
	do {
		komenda = WprowadzZadanie(1);
	} while (komenda == -1);
	
	

	if (trybGry == 1) {
		Oczysc();
		printf("Tura gracza drugiego. Nacisnij dowolny klawisz");
		getchar();
		Oczysc();
		printf("Twoje Pole: \n");
		RysujPlansze(*gracz2, 0);
		printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");
		PobierzKoordynaty(4, gracz2,&historia);
		Oczysc();
	}
	else {
		srand(time(0));
		while(!UstawStatek(gracz2, 4, rand()%100, rand()%2,-9));
		while (!UstawStatek(gracz2, 3, rand() % 100, rand() % 2,-8));
		while (!UstawStatek(gracz2, 3, rand() % 100, rand() % 2,-7));
		while (!UstawStatek(gracz2, 2, rand() % 100, rand() % 2, -6));
		while (!UstawStatek(gracz2, 2, rand() % 100, rand() % 2, -5));
		while (!UstawStatek(gracz2, 2, rand() % 100, rand() % 2, -4));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2, -3));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2, -3));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2, -3));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2, -3));

		Oczysc();

	}
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

int Bitwa(Gracz* gracz1, Gracz* gracz2) {
	char rozkaz[7];
	int cel;
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
			} while (cel == -1);
			wynik = Strzal(gracz2, cel / 100);
		}while (wynik == 0 && printf("W te pole nie mozna wycelowac. Podaj ponownie:") );
		printf("\n%o\n", gracz2->statki);
		if (wynik > 1) {
			if (wynik == 2)
				printf("Trafiony!\n");
			else
				printf("Trafiony zatopiony!\n");
			
			RysujPlansze(*gracz2,1);
			if (gracz2->statki == 0) {
				return 0;
			}
		}	

		if (wynik == 1) {
			printf("Pudlo :(\n");
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



int CelowanieAI(Gracz* atakowanyGracz, Wybor* AI) {
	int wynik, cel;
	do {
		do{
			cel = AI->stan[AI->stanPoprzedni](AI->aktualnePole);
			AI->aktualnePole = cel;

		} while (cel == -1 && AI->stanPoprzedni++);
		wynik = Strzal(atakowanyGracz, cel);
		
		if (atakowanyGracz->statki == 0) {
			return 0;
		}
		
		if (wynik == 3) {//statek zosta³ zatopiony po strzle
			AI->stanPoprzedni = 0;
		}
		if (wynik == 1 && AI->stanPoprzedni==4 ) {
			AI->stanPoprzedni++;
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
/*
int BitwaAI(Gracz* graczAI, Gracz* gracz2, Wybor* AI) {
	int cel;
	do {

		if (gracz2->statki == 0) {
			return 0;
		}
		

	} while (Strzal(gracz2, cel) != 1);
	return 1;
}
*/


