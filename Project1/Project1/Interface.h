#ifndef INTERFACE_H
#define INTERFACE_H



#if _WIN32
#include <Windows.h>
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define UBIALY "\x1B[37m"
#define UCZERWONY "\x1B[31m"
#define UZIELONY "\x1B[32m"
#define UNIEBIESKI "\x1B[36m"
#define UBIALY "\x1B[37m"
#endif




/**Funkcja czyszcz�ca pozosta�o�ci z bufora.*/
void WyczyscBufor();
/**Funkcja wy�wietlaj�ca graczowi plansz� jego lub przeciwnika.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param gracz Gracz, kt�rego plansz� nale�y wy�wietli�
@param dyskrecja Parametr okre�laj�cy czy nale�y wy�wietli� statki. Dla zmiennej r�wnej 0, statki s� wy�wietlane, w przeciwnym wypadku s� pomijane.
*/
void RysujPlansze(Konfiguracja* konfiguracja, Gracz gracz, int dyskrecja);
/**Funkcja rozpoczyna przygotowanie do gry w przypadku, gdy nie rozpocz�to od wczytania zapisu. Funkcja komunikuje si� z u�ytkwonikiem przez konsole.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param gracz Gracz, kt�rego pole gry b�dzie ustalane na podstawie informacji otrzymywanych od u�ytkownika
*/
void Rozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz);
/**Funkcja rozpoczyna przygotowanie do gry losowo umieszczaj�c wszystkie statki.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param gracz Gracz, kt�rego pole gry b�dzie ustalane poprzez losowe rozmieszczenie statk�w
*/
void AutoRozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz);

/**Funkcja czyszcz�ca konsol�. Zabezpieczenie przed podgl�daniem p�l przeciwnika. W przyadku nieobs�ugiwanego systemu operacyjnego funkcja wypisze dostateczn� ilo�� nowych linii, aby gracze nie widzieli w swojej turze statk�w przeciwnika.
@param konfiguracja struktura przechowuj�ca ustawienia gry
*/
void Oczysc(Konfiguracja* konfiguracja);
/**Funkcja zmieniaj�ca kolor tekstu. Ma zastosowanie estetyczne. Dla nieobs�ugiwanych system�w nie wykona �adnej instrukcji.
@param typ Kod koloru na kt�ry zostanie zamieniony kolor tekstu w konsoli
*/
void ZmienKolor(Kolor typ);

/**Funkcja do komunikacji z u�ytkownikiem, pozwalaj�ca na u�ywanie komend w dowolnej chwili. Po wprowadzeniu komendy wypisuje informacje, kt�re dzia�anie pr�bowano wykona�.
@param liczbaArgumentow Parametr okre�laj�cy jak du�o danych chcemy otrzyma�. Dla 0 wymaga naci�ni�cia klawisza "Enter", dla 1 wymaga jednej warto�ci, dla 2 dw�ch warto�ci. Inne warto�ci nie s� obs�ugiwane
@return  -2 (ZAD_FLOTA) Zwracana, gdy u�yto komendy "flota"
@return -3 (ZAD_COFNIJ)	Zwracana, gdy u�yto komendy "cofnij"
@return -4 (ZAD_ZAPISZ) Zwracana, gdy u�yto komendy "zapisz"
@return -5 (ZAD_WCZYTAJ) Zwracana, gdy u�yto komendy "wczytaj"
@return 0 (ZAD_BRAK) Zwracana, gdy liczbaArgumentow wynosi 0 i nie podano komedy
@return Liczba ca�kowita, kt�rej cyfra jedno�ci ma warto�� drugiego argumentu a pozosta�a cz�� liczby warto�� pierwszego. Zwracana tylko gdy nie u�yto �adnej komendy
*/
int WprowadzZadanie(int liczbaArgumentow);

/**Funkcja wypisuj�ca statki przeciwnika, kt�re nie zosta�y zatopione.
@param przeciwnik Gracz, kt�rego niezatopione statki nale�y wypisa�
*/
void WypiszFlote(Gracz* przeciwnik);
/**Funkcja wypisuj�ca ruchy poprzedniego gracza. Nie modyfikuje listy. Wypisuje do pierwszego sppotkanego elementu z typem wyliczeniowym okre�lonym jako "start".
@param ruchy Lista wykonanych ruch�w.
*/
void WypiszRuchy(Historia* ruchy);

/**Funkcja pobiera od u�ytkownika informacje gdzie powinien zosta� umieszczony pojedyczy statek.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param dlugosc D�ugo�� statku, kt�ry nale�y umie�ci�
@param gracz Gracz, kt�ry aktualnie ustawia statki
@param historia Lista ruch�w gracza. Pozwala na cofanie statk�w
@param rodzaj identyfikator statku. Ta warto�� zostanie wpisana na pole w momencie ustawiania statku
*/
void PobierzKoordynaty(Konfiguracja* konfiguracja, int dlugosc, Gracz* gracz, Historia** historia, int rodzaj);

/**Funckja stanowi g��wn� p�tle gry. Przekazuje graczom odpowiednie informacje porz�dkuj�ce gr�. Tylko dla gry w trybie hotseat
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
*/
void GameLoop(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);
/**Funckja stanowi g��wn� p�tle gry. Przekazuje graczowi odpowiednie informacje porz�dkuj�ce gr�. Tylko dla gry z komputerem.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
*/
void GameLoopAI(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);

/**Funkcja ko�czy gr� og�aszaj�c gracza i usuwaj�c dynamicznie zaalokowan� pami��.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
@param wynik Wynik na podstawie kt�rego zaostaje wytypowany wygrywaj�cy gracz
*/
void ZakonczGre(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka, int wynik);


/**Komunikacja z u�ytkownikiem po zako�czeniu przygotowa�. Pojedyncze wywo�anie jest pojedyncz� tur� jednego gracza
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
@return 0 (B_KONIEC) Informacja o tym, �e jeden z graczy straci� wszystkie statki. Warto�� ta ko�czy p�tle gry
@return 1 (B_NIEKONIEC) Informacja o tym, �e ka�dy gracz ma jeszcze statki
@return 2 (B_WCZYTAJ) Warto�� zwracana w przypadku udanego wcytania gry
*/
int Bitwa(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);

#endif