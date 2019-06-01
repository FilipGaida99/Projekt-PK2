#ifndef INIT_H
#define INIT_H


/**Funkcja pobiera argumenty wiersza polece� i ustawia wed�ug nich konfiguracje.
@param ileArg ilo�� argrument�w wiersza polece�
@param arg argumenty wiersza polece�
@param konfiguracja struktura przechowuj�ca ustawienia gry
@return 0, gdy pobranie parametr�w by�o nieudane
@return 1, gdy pobranie prametr�w powiod�o si�.
*/
int PobierzParametry(int ileArg, char* arg[], Konfiguracja *konfiguracja);
/**Funkcja pobiera konfiguracje z pliku config.ini
@param konfiguracja struktura przechowuj�ca ustawienia gry
@return 1, gdy znaleziono plik i odczytano konfiguracje
@return 0, gdy nie znaleziono pliku
@return -1, gdy podany zakres by� niedozwolony
*/
int Skonfiguruj(Konfiguracja *konfiguracja);
/**Funckcja ustawia brakuj�ce parametry, przekazywane zwykle przez wiersz polece�, komunikuj�c si� z u�ytkownikiem.
@param konfiguracja struktura przechowuj�ca ustawienia gry
*/
void UstawParametry(Konfiguracja *konfiguracja);
/**Funkcja przeprowadza ca�� pocz�tkow� faz� gry.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
@return 0, gdy nie uda�o si� poprawnie rozpocz�� gry
@return 1, gdy ca�y proces przebieg� pomy�lnie
*/
int Iniciuj(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);

/**Funkcja pobiera kodowanie z pliku kod.txt.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@return 0, gdy nie znaleziono pliku z kodowaniem
@return 1, gdy odczytano kodowanie z pliku. Nie gwarantuje to poprawno�ci kodowania w przypadku zmiany kodu �r�d�owego programu.
*/
int PobierzKodowanie(Konfiguracja *konfiguracja);


#endif
