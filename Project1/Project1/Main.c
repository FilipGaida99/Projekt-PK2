#include <stdio.h>
#include <stdlib.h>

#include "Funkcje.h"
//#include <mxml.h>
#include "mxml-3.0/mxml.h"

int main(int ileArg, char* arg[]) {
	ZmienKolor(BIALY);

	if (ileArg > 1) {
		if (!strcmp(arg[1], "-l")) {

		}
		if (!strcmp(arg[1], "-h")) {

		}
		if (!strcmp(arg[1], "-c")){
			if()

		}
	}


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
	
	

	Gracz gracz1;
	Gracz gracz2;
	
	
	
	IniciujGre(&gracz1, &gracz2,trybGry);
	mxml_node_t* xml;
	mxml_node_t* tura_n;
	mxml_node_t* czyja_n;
	int tura = 1;
	UtworzZapis(trybGry, 0, 1, gracz1, gracz2,&xml);
	tura_n = mxmlFindPath(xml, "Ustawienia/Tura");
	czyja_n = mxmlFindPath(xml, "Ustawienia/Czyja_Tura");

	Historia* ruchy=0;
	DodajdoListy(&ruchy, start, 0, 0);
	int wynik1, wynik2;
	if (trybGry == 1) {
		do {
			
			Oczysc();
			mxmlSetInteger(czyja_n, 1);
			printf("Tura gracza pierwszego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			printf("Tura: %d \n", tura);
			WypiszRuchy(ruchy);
			DodajdoListy(&ruchy, start, 0, 0);
			wynik1=Bitwa(&gracz1, &gracz2,&xml, &ruchy);
			Oczysc();
			mxmlSetInteger(czyja_n, 2);
			printf("Tura gracza drugiego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			printf("Tura: %d \n", tura);
			WypiszRuchy(ruchy);
			DodajdoListy(&ruchy, start, 0, 0);
			wynik2 = Bitwa(&gracz2, &gracz1,&xml, &ruchy);
			mxmlSetInteger(tura_n, ++tura);

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



		do {
			printf("Tura: %d \n", tura);
			WypiszRuchy(ruchy);
			DodajdoListy(&ruchy, start, 0, 0);
			wynik1 = Bitwa(&gracz1, &gracz2, &xml,&ruchy);
			DodajdoListy(&ruchy, start, 0, 0);
			wynik2 = BitwaAI(&gracz1,&AI, &ruchy);
			mxmlSetInteger(tura_n, ++tura);
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

	UsunListe(&ruchy);
	//usun¹æ
	while (1)
	{
		getchar();
	}
	
	return 0;
}