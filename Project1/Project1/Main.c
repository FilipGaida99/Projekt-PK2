#include <stdio.h>
#include <stdlib.h>


#include "Funkcje.h"

int main() {
	printf("\033[0; 31m"); //Set the text to the color red
	printf("Hello\n"); //Display Hello in red




	int trybGry;
	printf("Witaj w grze w statki. Nacisnij dowolny klawisz aby rozpoczac.\n");
	getchar();
	printf("Wybierz tryb gry. 0-gra z komputerem lub 1-gra w dwie osoby(hotseat):");
	do {
		scanf("%d", &trybGry);
		switch (trybGry)
		{
		case 0: printf("Wybrano gre z komputerem.\n");
			break;
		case 1: printf("Wybrano gre hotseat.\n");
			break;
		default: printf("Wybierz miedzy 0 i 1!");
			break;
		}
	} while (trybGry!= 0 && trybGry != 1);
	system("cls");

	int poleGracza1[ROZMIAR_POLA][ROZMIAR_POLA];
	int poleGracza2[ROZMIAR_POLA][ROZMIAR_POLA];
	
	IniciujGre(poleGracza1, poleGracza2);



	while (0==0)
	{
		getchar();
	}
	
	return 0;
}