#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Funkcje.h"

#include <Windows.h>

void WyczyscBufor() {
	char pozostalosc;
	do {
		pozostalosc = getchar();
	} while (pozostalosc != '\n');
}

void Oczysc() {
	system("cls");
}

void RysujPlansze(Gracz player, int dyskrecja) {
	int i, j, c=0;
	
	for (i = 0; i < ROZMIAR_POLA; i++) {
		printf("|");
		for (j = 0; j < ROZMIAR_POLA; j++) {
			if (player.pole[i][j] == -1) {
				printf("%3c|", 'X');
				
			}
			else if (player.pole[i][j] == -2) {
				printf("%3c|", 'U');
			}
			else if (player.pole[i][j] == -3 && dyskrecja==0) {
				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE)  , 10);
				printf("%3c", 'V');
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				printf("|");
			}
			else {
				printf("%3d|", (i*10+j));
			}
		}
		printf("\n");
	}
	
}

int UstawStatek(Gracz* gracz, int dlugosc, int pole, int kierunek) {
	int x = pole / 10;
	int y = pole % 10;
	int i;
	if (x<0 || x>ROZMIAR_POLA || y<0 || y>ROZMIAR_POLA)
		return 0;
	if (kierunek) {
		if (x<0 || x>ROZMIAR_POLA || x + dlugosc > ROZMIAR_POLA)
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
			gracz->pole[x][y] = -3;
		}
		return 1;

	}
	else {
		if (y<0 || y>ROZMIAR_POLA || y + dlugosc > ROZMIAR_POLA)
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
			gracz->pole[x][y] = -3;
		}
		return 1;
	}
}

void PobierzKoordynaty(int dlugosc, Gracz* gracz) {
	int pole, kierunek, i, j;
	i = 4 - dlugosc;
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
		if (dlugosc == 1) {
			for (i = 0; i < 4; i++) {
				while (scanf("%d", &pole) != 1) {
					printf("Kordynaty podany w blednym formacie. Odpowiedni format to <numer_pola>");
					WyczyscBufor();
				};
				if (UstawStatek(gracz,dlugosc,pole,0) == 0) {
					printf("Nie mozna tu ustawic statku. Podaj ponownie\n");
					WyczyscBufor();
					i--;
					continue;
				}
				system("cls");
				RysujPlansze(*gracz, 0);
			}
			return;
		
		}
		do {
			while (scanf("%d %d", &pole, &kierunek) != 2) {
				printf("Kordynaty podany w blednym formacie. Odpowiedni format to <numer_pola> <kierunek>");
				WyczyscBufor();
			}
			WyczyscBufor();
		} while (UstawStatek(gracz, dlugosc, pole, kierunek) == 0 && printf("Nie mozna tu ustawic statku. Podaj ponownie: "));
		system("cls");
		RysujPlansze(*gracz, 0);
	
		}
			dlugosc--;
			i= 4-dlugosc;
	}
}


void IniciujGre(Gracz* gracz1, Gracz* gracz2, int trybGry) {
	int pole, kierunek;
	gracz1->statki = gracz2->statki = 20;
	WypelnijTablice(gracz1->pole,gracz2->pole);
	printf("Twoje Pole: \n");
	RysujPlansze(*gracz1, 0);
	printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");
	PobierzKoordynaty(4, gracz1);
	printf("Oto twoje ustawienie. Czy chcesz je zmienic?");
	WyczyscBufor();
	getchar();//TODO cofanie ruchu.
	if (trybGry == 1) {
		Oczysc();
		printf("Tura gracza drugiego. Nacisnij dowolny klawisz");
		getchar();
		Oczysc();
		printf("Twoje Pole: \n");
		RysujPlansze(*gracz2, 0);
		printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");
		PobierzKoordynaty(4, gracz2);
		Oczysc();
	}
	else {
		srand(time(0));
		int pozycja =rand() % 100;
		int kierunek = rand() % 2;
		while(!UstawStatek(gracz2, 4, rand()%100, rand()%2));
		while (!UstawStatek(gracz2, 3, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 3, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 2, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 2, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 2, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2));
		while (!UstawStatek(gracz2, 1, rand() % 100, rand() % 2));

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
	if (atakowanyGracz->pole[x][y] == -3) {
		atakowanyGracz->pole[x][y] = -1;
		return 1;
	}
	else if (atakowanyGracz->pole[x][y] < 0) {
		return 0;
	}
	else {
		atakowanyGracz->pole[x][y] = -2;
		return 2;
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
		printf("Wydaj rozkaz: ");
		scanf("%6s", &rozkaz);
		if (!strcmp(rozkaz, "cofnij")) {

		}
		else if (!strcmp(rozkaz, "strzel")) {
			scanf("%d", &cel);
			int wynik;
			while ((wynik = Strzal(gracz2, cel)) == 0) {
				printf("Juz tam celowano. Podaj ponownie:");
				WyczyscBufor();
				scanf("%d", &cel);
			}
			if (wynik == 1) {
				printf("Trafiono!\n");
				gracz2->statki -= 1;
				if (gracz2->statki == 0) {
					return 0;
				}
			}
			if (wynik == 2) {
				printf("Pudlo :(\n");
				return 1;
			}
		}
	}
}

int BitwaAI(Gracz* graczAI, Gracz* gracz2) {
	int cel;
	do {
		if (gracz2->statki == 0) {
			return 0;
		}
		printf("Strzelil, nie pudlo");
	 cel = rand() % 100;
	} while (Strzal(gracz2, cel) != 2);
	return 1;
}


