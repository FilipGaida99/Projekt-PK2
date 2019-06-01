#include <stdio.h>
#include <stdlib.h>
#include "Struktury.h"
#include "Zapis.h"
#include "Logika.h"

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
			wartosc = mxmlNewElement(dane, "Rozmiar");
				element = mxmlNewElement(wartosc, "X");
					mxmlNewInteger(element, konfiguracja->rozmiarPola_x);
				element = mxmlNewElement(wartosc, "Y");
					mxmlNewInteger(element, konfiguracja->rozmiarPola_y);
			wartosc = mxmlNewElement(dane, "AI");
				mxmlNewInteger(wartosc, 0);
			wartosc = mxmlNewElement(dane, "AI_Pole");
				mxmlNewInteger(wartosc, 0);
		dane = mxmlNewElement(informacje, "Gracz1");
			ZapiszGracza(dane, konfiguracja, rozgrywka->gracz1);
		dane = mxmlNewElement(informacje, "Gracz2");
			ZapiszGracza(dane, konfiguracja, rozgrywka->gracz2);
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
	czyja = mxmlGetInteger(rozgrywka->czyja_n);
	if (czyja == 2) { //zamaiana kolejnosci graczy, aby zachowaæ sposób kodowania informacji
		ZamianaGraczy(rozgrywka);
	}
	dane = mxmlFindPath(informacje, "Ustawienia/AI");
		mxmlSetInteger(dane, rozgrywka->AI.stanPoprzedni);
	dane = mxmlFindPath(informacje, "Ustawienia/AI_Pole");
		mxmlSetInteger(dane, rozgrywka->AI.aktualnePole);
	dane = mxmlFindPath(informacje, "Gracz1");
	mxmlDelete(dane); //Usuniêcie starych informacji
		dane = mxmlNewElement(informacje, "Gracz1");
		ZapiszGracza(dane,konfiguracja,rozgrywka->gracz1);
	dane = mxmlFindPath(informacje, "Gracz2");
	mxmlDelete(dane);
		dane = mxmlNewElement(informacje, "Gracz2");
		ZapiszGracza(dane, konfiguracja, rozgrywka->gracz2);
	
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
	if (SprawdzZgodnosc(drzewo, konfiguracja) == 0) {
		return -1;
	}
	if (rozgrywka->xml != 0) {
		mxmlDelete(rozgrywka->xml);
	}
	rozgrywka->xml = drzewo;
	//Odczytwyanie danych graczy
	informacje = mxmlGetFirstChild(drzewo);
	dane = mxmlFindPath(informacje, "Ustawienia/AI");
		rozgrywka->AI.stanPoprzedni = mxmlGetInteger(dane);
	dane = mxmlFindPath(informacje, "Ustawienia/AI_Pole");
		rozgrywka->AI.aktualnePole = mxmlGetInteger(dane);
	dane = mxmlFindPath(informacje, "Gracz1");
		WczytajGracza(dane, konfiguracja, &rozgrywka->gracz1);
	dane = mxmlFindPath(informacje, "Gracz2");
		WczytajGracza(dane, konfiguracja, &rozgrywka->gracz2);
	//Wczytywanie listy ruchów
	dane = mxmlFindPath(informacje, "Ruchy");
	DodajdoListy(&rozgrywka->ruchy, start, 0, 0);
	wartosc = mxmlGetLastChild(dane);
	while (wartosc != 0) {
		int dane = mxmlGetInteger(wartosc);
		DodajdoListy(&rozgrywka->ruchy, strzal, dane / 10, dane % 10);
		wartosc = mxmlGetPrevSibling(wartosc);
	}
	rozgrywka->tura_n = mxmlFindPath(rozgrywka->xml, "Informacje/Ustawienia/Tura");
	rozgrywka->czyja_n = mxmlFindPath(rozgrywka->xml, "Informacje/Ustawienia/Czyja_Tura");
	return 1;
}

void ZapiszGracza(mxml_node_t* parent, Konfiguracja* konfiguracja, Gracz gracz) {
	mxml_node_t* wartosc;
	mxml_node_t* element;
	wartosc = mxmlNewElement(parent, "Statki");
	mxmlNewInteger(wartosc, gracz.statki);
	wartosc = mxmlNewElement(parent, "Pole");
	int i, j;
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			element = mxmlNewElement(wartosc, "Wartosc");
			mxmlNewInteger(element, gracz.pole[i][j]);
		}
	}
}
void WczytajGracza(mxml_node_t* parent, Konfiguracja* konfiguracja, Gracz* gracz) {
	mxml_node_t* dane;
	mxml_node_t* wartosc;
	dane = mxmlFindPath(parent, "Statki");
	gracz->statki = mxmlGetInteger(dane);
	dane = mxmlFindPath(parent, "Pole");
	wartosc = mxmlGetFirstChild(dane);
	int i, j;
	for (i = 0; i < konfiguracja->rozmiarPola_x; i++) {
		for (j = 0; j < konfiguracja->rozmiarPola_y; j++) {
			gracz->pole[i][j] = mxmlGetInteger(wartosc);
			wartosc = mxmlWalkNext(wartosc, dane, MXML_NO_DESCEND);
		}
	}
}

int SprawdzZgodnosc(mxml_node_t* xml, Konfiguracja* konfiguracja) {
	mxml_node_t* dane;
	mxml_node_t* dane2;
	dane = mxmlFindPath(xml, "Informacje/Ustawienia/Rozmiar/X");
	dane2 = mxmlFindPath(xml, "Informacje/Ustawienia/Rozmiar/Y");
	if (mxmlGetInteger(dane) != konfiguracja->rozmiarPola_x || mxmlGetInteger(dane2) != konfiguracja->rozmiarPola_y) {
		konfiguracja->rozmiarPola_x = mxmlGetInteger(dane);
		konfiguracja->rozmiarPola_y = mxmlGetInteger(dane2);
		return 0;
	}
	return 1;
}
