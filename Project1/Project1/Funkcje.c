#include <stdio.h>
#include "Funkcje.h"

void WyczyscBufor() {
	char pozostalosc;
	do {
		pozostalosc = getchar();
	} while (pozostalosc != '\n');
}

void RysujPlansze(int poleGry[ROZMIAR_POLA][ROZMIAR_POLA], int dyskrecja) {
	int i, j, c=0;
	
	for (i = 0; i < ROZMIAR_POLA; i++) {
		printf("|");
		for (j = 0; j < ROZMIAR_POLA; j++) {
			if (poleGry[i][j] == -1) {
				printf("%3c|", 'X');
				
			}
			else if (poleGry[i][j] == -2) {
				printf("%3c|", 'U');
			}
			else {
				printf("%3d|", poleGry[i][j]);
			}
		}
		printf("\n");
	}
	
}

int UstawStatek(int poleGracza[ROZMIAR_POLA][ROZMIAR_POLA], int dlugosc, int pole, int kierunek) {
	if (kierunek) {
		int x = pole % 10;
		int y = pole / 10;
		int i;
		for (i = 0; i < dlugosc; i++) {
			if (poleGracza[x][y] < 0) {
				return 0;
			}
			y++;
		}
		for (; i > 0; i--) {
			poleGracza[x][y] = -3;
			y--;
		}

	}
	else {

	}
}

void IniciujGre(int poleGracza1[ROZMIAR_POLA][ROZMIAR_POLA], int poleGracza2[ROZMIAR_POLA][ROZMIAR_POLA]) {
	int pole, kierunek;
	WypelnijTablice(poleGracza1, poleGracza2);
	printf("Twoje Pole: \n");
	RysujPlansze(poleGracza1, 0);
	printf("Teraz nalezy umiescic statki.\n Nalezy podac pole oraz kierunek:\n 0-od lewej do prawej.\n 1-od gory do dolu.\n");
	printf("Ustaw czteromasztowiec:");
	while (scanf("%d %d", &pole, &kierunek) != 2) {
		printf("Kordynaty podany w blednym formacie. Odpowiedni format to <numer_pola> <kierunek>");
		WyczyscBufor();
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
