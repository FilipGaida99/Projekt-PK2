#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funkcje.h"
#include <time.h>

#include "mxml-3.0/mxml.h"



int main(int ileArg, char* arg[]) {
	ZmienKolor(BIALY);
	srand(time(NULL));
	int trybGry = -1;
	int automat = 0;
	int wczytaj = 0;
	if (ileArg > 1) {
		if (!strcmp(arg[1], "-l")) {
			wczytaj = 1;
		}
		else if (!strcmp(arg[1], "-h")) {
			trybGry = 1;
		}
		else if (!strcmp(arg[1], "-c")) {
			if (ileArg > 2 && !strcmp(arg[2], "-a")) {
				automat = 1;
			}
			trybGry = 0;

		}
	}

	if (wczytaj==0 && (trybGry != 1 && trybGry != 0)) {

		printf("Witaj w grze w statki. Nacisnij dowolny klawisz aby rozpoczac. Uzyj komendy \"wczytaj\", jesli chcesz wczytac ostatnia gre.\n");
		int komenda;
		do {
			komenda = WprowadzZadanie(0);
			if (komenda == ZAD_FLOTA || komenda == ZAD_ZAPISZ || komenda == ZAD_COFNIJ) {
				printf("Nie mozna teraz wykonac tej akcji.\n");
			}
			else if (komenda == ZAD_WCZYTAJ) {
				wczytaj = 1;
				break;
			}
		} while (komenda != ZAD_BRAK);
		if (wczytaj == 0) {
			printf("Wybierz tryb gry. 0-gra z komputerem (wprowadz \"-1\" by umiescic statki automatycznie) lub 1-gra w dwie osoby(hotseat):");
			do {
				
				scanf("%d", &trybGry);
				switch (trybGry)
				{
				case -1:
					automat = 1;
					trybGry = 0;
				case 0: printf("Wybrano gre z komputerem.\n");
					break;
				case 1: printf("Wybrano gre hotseat.\n");
					break;
				default: printf("Wybierz miedzy 0 i 1!");
					break;
				}
			} while (trybGry != 0 && trybGry != 1);
		}
	}
	Gracz gracz1;
	Gracz gracz2;
	mxml_node_t* xml=0;
	mxml_node_t* tura_n;
	mxml_node_t* czyja_n;
	Historia* ruchy = 0;
	if (WypelnijTablice(&gracz1, &gracz2) == 0) {
		printf("Bledna alokacja pamieci. Sprobuj uruchomic ponownie.");
		return 0;
	}
	if (wczytaj == 1) {//rozpoczêcie od wczytywania
		
		if (Wczytaj(&gracz1, &gracz2, &xml, &ruchy) == 0) {
			
			printf("Nie znaleziono zapisu lub plik z zapisem jest uszkodzony. \nSprobuj uruchomic gre ponownie lub zaczac od poczatku");
			UsunTablice(&gracz1, &gracz2);
			return 0;
		}

		trybGry = mxmlGetInteger(mxmlFindPath(xml, "Informacje/Ustawienia/Tryb_Gry"));
	}
	else {//przygotowanie do gry
		Oczysc();
		if (automat == 0) {
			Rozmieszczenie(&gracz1);
		}
		else { //automat == 1-automatyczne rozmieszczenie.
			AutoRozmieszczenie(&gracz1);
		}
		if (trybGry == 1) {
			Oczysc();
			printf("Tura gracza drugiego. Nacisnij dowolny klawisz");
			getchar();
			Oczysc();
			Rozmieszczenie(&gracz2);
		}
		else {
			AutoRozmieszczenie(&gracz2);
		}
		
		UtworzZapis(trybGry, 1, 1, gracz1, gracz2, &xml);
	}
	
	
	int tura = 1;
	tura_n = mxmlFindPath(xml, "Informacje/Ustawienia/Tura");
	czyja_n = mxmlFindPath(xml, "Informacje/Ustawienia/Czyja_Tura");
	
	DodajdoListy(&ruchy, start, 0, 0);
	int wynik1, wynik2;
	if (trybGry == 1) {
		int trigger= mxmlGetInteger(czyja_n);//zmienna steruj¹ca, pozwala pomin¹æ turê gracza pierwszego, je¿eli dane z pliku tego wymagaj¹.
		do {
			if (trigger != 2) {
				tura = mxmlGetInteger(tura_n);
				Oczysc();
				mxmlSetInteger(czyja_n, 1);
				printf("Tura gracza pierwszego. Nacisnij dowolny klawisz.\n");
				WyczyscBufor();
				printf("Tura: %d \n", tura);
				wynik1 = Bitwa(&gracz1, &gracz2, &xml, &ruchy);
				if (wynik1 == B_WCZYTAJ) {
					if (mxmlGetInteger(czyja_n) == 1) {
						wynik2 = 1;//instieje mo¿liwoœæ, ¿e zmienna zostanie u¿yta bez iniciacji dlatego odrazu iniciuje j¹ nie zmieniaj¹c¹ dzia³ania wartosci¹.
						continue;
					}
					tura = mxmlGetInteger(tura_n);
					//w przypadku gdy powinna byæ tura gracza 2 nic nie zmienia kolejnoœci po wczytaniu, zmienia siê tylko licznik tur.
				}
			}
			
			trigger = 1;
			
			Oczysc();
			mxmlSetInteger(czyja_n, 2);
			printf("Tura gracza drugiego. Nacisnij dowolny klawisz.\n");
			WyczyscBufor();
			printf("Tura: %d \n", tura);
			wynik2 = Bitwa(&gracz2, &gracz1,&xml, &ruchy);
			if (wynik2 == B_WCZYTAJ) {
				if (mxmlGetInteger(czyja_n) == 2) {
					wynik1 = 1;
					trigger = 2;
				}
			}
			mxmlSetInteger(tura_n, ++tura);

		} while (wynik1 && wynik2);
	}
	if (trybGry == 0) {
		//inicializacja sztucznej inteligencji
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
			tura = mxmlGetInteger(tura_n);
			printf("Tura: %d \n", tura);
			wynik1 = Bitwa(&gracz1, &gracz2, &xml,&ruchy);
			if (wynik1 == B_WCZYTAJ) {//powstrzymanie komputera przed wykonaniem ruchu.
				wynik2 = 1;
				continue;
			}
			DodajdoListy(&ruchy, start, 0, 0);
			wynik2 = BitwaAI(&gracz1,&AI, &ruchy);
			mxmlSetInteger(tura_n, ++tura);
		} while (wynik1 && wynik2);
	}
	if (wynik1 == B_KONIEC) {
		printf("Wygral pierwszy gracz");
	}
	else if(trybGry==1){
		printf("Wygral drugi gracz");
	}
	else
	{
		printf("Wygral Komputer");
	}
	mxmlDelete(xml);
	UsunListe(&ruchy);
	//usun¹æ
	system("pause");

	UsunTablice(&gracz1, &gracz2);
	_CrtDumpMemoryLeaks();
	return 0;
}