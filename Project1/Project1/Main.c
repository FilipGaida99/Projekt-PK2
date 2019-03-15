#include <stdio.h>
#include <stdlib.h>

#include "Funkcje.h"

int main() {

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

	Gracz gracz1 = { poleGracza1,20 };
	Gracz gracz2 = { poleGracza2,20 };
	
	

	IniciujGre(&gracz1, &gracz2,trybGry);

	if (trybGry == 1) {
		int wynik1, wynik2;
		do {
			
			Oczysc();
			printf("Tura gracza pierwszego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			getchar();
			wynik1=Bitwa(&gracz1, &gracz2);
			Oczysc();
			printf("Tura gracza drugiego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			getchar();
			wynik2 = Bitwa(&gracz2, &gracz1);

		} while (wynik1 && wynik2);
	}
	if (trybGry == 0) {
		int wynik1,wynik2;
		do {
			wynik1 = Bitwa(&gracz1, &gracz2);
			wynik2 = BitwaAI(&gracz2, &gracz1);
		} while (wynik1 && wynik2);
	}



	while (1)
	{
		getchar();
	}
	
	return 0;
}