#ifndef ZAPIS_H
#define ZAPIS_H
/**@file*/
#include "mxml-3.0/mxml.h"

/**Funkcja zapisuje dane gracza do drzewa xml.
@param parent Czêœæ drzewa pod które zapisane zostan¹ dane gracza
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param gracz Struktura z danymi gracza zapisywanymi do drzewa
*/
void ZapiszGracza(mxml_node_t* parent, Konfiguracja* konfiguracja, Gracz gracz);
/**Funkcja odczytuje dane gracza z drzewa xml.
@param parent Czêœæ drzewa pod które zapisane zosta³y dane gracza
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param gracz Struktura z danymi gracza czytanymi z drzewa
*/
void WczytajGracza(mxml_node_t* parent, Konfiguracja* konfiguracja, Gracz *gracz);
/**Funkcja sprawdza zgodnoœæ pliku zapisu z obecn¹ konfiguracj¹ gry.
@param xml Drzewo xml odczytane z pliku zapisu
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@return 0, gdy dane nie s¹ zgodne
@return 1, gdy dane s¹ zgodne
*/
int SprawdzZgodnosc(mxml_node_t* xml, Konfiguracja* konfiguracja);


//Funkcje do obs³ugi zapisów

/**Funkcja tworzy zapis na podstawie ustawieñ i zakoñczonego, przez obu graczy, przygotowania do gry. Tworzy drzewo xml, które pozwala na ³atwe zapisywanie w póŸniejszej czêsci gry.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo¿na by³o zapisaæ do pliku. W tym przypadku drzewo zosta³o utworzone tylko w pamiêci aplikacji
*/
int UtworzZapis(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka);
/**Funkcja wykonuj¹ca zapis w tracie rozgrywki. Pocz¹tkowo funkcja aktualizuje drzewo danymi, których ci¹g³y zapis do drzewa by³by nieoptymalny. Dane aktualizowane znajduj¹ sie w strukturach Gracz.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo¿na by³o zapisaæ do pliku. W tym przypadku drzewo zosta³o utworzone tylko w pamiêci aplikacji
*/
int Zapisz(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka);
/**Funkcja wczytuje dane z pliku z zapisem nadpisuj¹c obecne informacje. Nadpisane zostan¹ dane graczy, drzewo xml oraz lista wykonanych ruchów.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
@return 0, gdy nie mo¿na by³o otworzyæ pliku lub za³adowanie drzewa z tego pliku by³o niemo¿liwe
@return 1, gdy procedura odczytywania przebieg³a pomyœlnie
*/
int Wczytaj(Konfiguracja* konfiguracja, Rozgrywka *rozgrywka);

#endif // !ZAPIS_H

