#include <stdio.h>
#include <stdlib.h>

#include "Funkcje.h"
#include <mxml.h>


int main() {
	ZmienKolor(BIALY);

	
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

	Gracz gracz1;
	Gracz gracz2;
	
	
	
	IniciujGre(&gracz1, &gracz2,trybGry);
	mxml_node_t* xml;
	Zapisz(trybGry, 0, 1, gracz1, gracz2,&xml);
	Wczytaj();
	int wynik1, wynik2;
	if (trybGry == 1) {
		int tura = 1;
		do {
			
			Oczysc();
			printf("Tura gracza pierwszego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			getchar();
			wynik1=Bitwa(&gracz1, &gracz2,xml,tura,1,trybGry);
			Oczysc();
			printf("Tura gracza drugiego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			getchar();
			wynik2 = Bitwa(&gracz2, &gracz1,xml, tura++,2, trybGry);

		} while (wynik1 && wynik2);
	}
	if (trybGry == 0) {
		Wybor AI;
		AI.aktualnePole = 0;
		AI.stan[0] = Losuj;
		AI.stan[1] = IdzS;
		AI.stan[2] = IdzN;
		AI.stan[3] = IdzSkos;
		AI.stan[4] = IdzW;
		AI.stan[5] = IdzE;
		AI.stanPoprzedni = 0;

		int tura = 1;
		do {
			wynik1 = Bitwa(&gracz1, &gracz2, xml, tura, 1, trybGry);
			wynik2 = BitwaAI(&gracz1,&AI);
			tura++;
		} while (wynik1 && wynik2);
	}
	if (wynik1 == 0) {
		printf("Wygral pierwszy gracz");
	}
	else if(trybGry==1){
		printf("Wygral drugi gracz");
	}
	else
	{
		printf("Wygral Komputer");
	}



	while (1)
	{
		getchar();
	}
	
	return 0;
}