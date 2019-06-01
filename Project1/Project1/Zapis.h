#ifndef ZAPIS_H
#define ZAPIS_H
/**@file*/
#include "mxml-3.0/mxml.h"

/**Funkcja zapisuje dane gracza do drzewa xml.
@param parent Cz�� drzewa pod kt�re zapisane zostan� dane gracza
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param gracz Struktura z danymi gracza zapisywanymi do drzewa
*/
void ZapiszGracza(mxml_node_t* parent, Konfiguracja* konfiguracja, Gracz gracz);
/**Funkcja odczytuje dane gracza z drzewa xml.
@param parent Cz�� drzewa pod kt�re zapisane zosta�y dane gracza
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param gracz Struktura z danymi gracza czytanymi z drzewa
*/
void WczytajGracza(mxml_node_t* parent, Konfiguracja* konfiguracja, Gracz *gracz);
/**Funkcja sprawdza zgodno�� pliku zapisu z obecn� konfiguracj� gry.
@param xml Drzewo xml odczytane z pliku zapisu
@param konfiguracja struktura przechowuj�ca ustawienia gry
@return 0, gdy dane nie s� zgodne
@return 1, gdy dane s� zgodne
*/
int SprawdzZgodnosc(mxml_node_t* xml, Konfiguracja* konfiguracja);


//Funkcje do obs�ugi zapis�w

/**Funkcja tworzy zapis na podstawie ustawie� i zako�czonego, przez obu graczy, przygotowania do gry. Tworzy drzewo xml, kt�re pozwala na �atwe zapisywanie w p�niejszej cz�sci gry.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo�na by�o zapisa� do pliku. W tym przypadku drzewo zosta�o utworzone tylko w pami�ci aplikacji
*/
int UtworzZapis(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka);
/**Funkcja wykonuj�ca zapis w tracie rozgrywki. Pocz�tkowo funkcja aktualizuje drzewo danymi, kt�rych ci�g�y zapis do drzewa by�by nieoptymalny. Dane aktualizowane znajduj� sie w strukturach Gracz.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo�na by�o zapisa� do pliku. W tym przypadku drzewo zosta�o utworzone tylko w pami�ci aplikacji
*/
int Zapisz(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka);
/**Funkcja wczytuje dane z pliku z zapisem nadpisuj�c obecne informacje. Nadpisane zostan� dane graczy, drzewo xml oraz lista wykonanych ruch�w.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
@return 0, gdy nie mo�na by�o otworzy� pliku lub za�adowanie drzewa z tego pliku by�o niemo�liwe
@return 1, gdy procedura odczytywania przebieg�a pomy�lnie
*/
int Wczytaj(Konfiguracja* konfiguracja, Rozgrywka *rozgrywka);

#endif // !ZAPIS_H

