#ifndef INIT_H
#define INIT_H


/**Funkcja pobiera argumenty wiersza poleceñ i ustawia wed³ug nich konfiguracje.
@param ileArg iloœæ argrumentów wiersza poleceñ
@param arg argumenty wiersza poleceñ
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@return 0, gdy pobranie parametrów by³o nieudane
@return 1, gdy pobranie prametrów powiod³o siê.
*/
int PobierzParametry(int ileArg, char* arg[], Konfiguracja *konfiguracja);
/**Funkcja pobiera konfiguracje z pliku config.ini
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@return 1, gdy znaleziono plik i odczytano konfiguracje
@return 0, gdy nie znaleziono pliku
@return -1, gdy podany zakres by³ niedozwolony
*/
int Skonfiguruj(Konfiguracja *konfiguracja);
/**Funckcja ustawia brakuj¹ce parametry, przekazywane zwykle przez wiersz poleceñ, komunikuj¹c siê z u¿ytkownikiem.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
*/
void UstawParametry(Konfiguracja *konfiguracja);
/**Funkcja przeprowadza ca³¹ pocz¹tkow¹ fazê gry.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
@return 0, gdy nie uda³o siê poprawnie rozpocz¹æ gry
@return 1, gdy ca³y proces przebieg³ pomyœlnie
*/
int Iniciuj(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);

/**Funkcja pobiera kodowanie z pliku kod.txt.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@return 0, gdy nie znaleziono pliku z kodowaniem
@return 1, gdy odczytano kodowanie z pliku. Nie gwarantuje to poprawnoœci kodowania w przypadku zmiany kodu Ÿród³owego programu.
*/
int PobierzKodowanie(Konfiguracja *konfiguracja);


#endif
