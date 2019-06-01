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




/**Funkcja czyszcz¹ca pozosta³oœci z bufora.*/
void WyczyscBufor();
/**Funkcja wyœwietlaj¹ca graczowi planszê jego lub przeciwnika.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param gracz Gracz, którego planszê nale¿y wyœwietliæ
@param dyskrecja Parametr okreœlaj¹cy czy nale¿y wyœwietliæ statki. Dla zmiennej równej 0, statki s¹ wyœwietlane, w przeciwnym wypadku s¹ pomijane.
*/
void RysujPlansze(Konfiguracja* konfiguracja, Gracz gracz, int dyskrecja);
/**Funkcja rozpoczyna przygotowanie do gry w przypadku, gdy nie rozpoczêto od wczytania zapisu. Funkcja komunikuje siê z u¿ytkwonikiem przez konsole.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param gracz Gracz, którego pole gry bêdzie ustalane na podstawie informacji otrzymywanych od u¿ytkownika
*/
void Rozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz);
/**Funkcja rozpoczyna przygotowanie do gry losowo umieszczaj¹c wszystkie statki.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param gracz Gracz, którego pole gry bêdzie ustalane poprzez losowe rozmieszczenie statków
*/
void AutoRozmieszczenie(Konfiguracja* konfiguracja, Gracz* gracz);

/**Funkcja czyszcz¹ca konsolê. Zabezpieczenie przed podgl¹daniem pól przeciwnika. W przyadku nieobs³ugiwanego systemu operacyjnego funkcja wypisze dostateczn¹ iloœæ nowych linii, aby gracze nie widzieli w swojej turze statków przeciwnika.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
*/
void Oczysc(Konfiguracja* konfiguracja);
/**Funkcja zmieniaj¹ca kolor tekstu. Ma zastosowanie estetyczne. Dla nieobs³ugiwanych systemów nie wykona ¿adnej instrukcji.
@param typ Kod koloru na który zostanie zamieniony kolor tekstu w konsoli
*/
void ZmienKolor(Kolor typ);

/**Funkcja do komunikacji z u¿ytkownikiem, pozwalaj¹ca na u¿ywanie komend w dowolnej chwili. Po wprowadzeniu komendy wypisuje informacje, które dzia³anie próbowano wykonaæ.
@param liczbaArgumentow Parametr okreœlaj¹cy jak du¿o danych chcemy otrzymaæ. Dla 0 wymaga naciœniêcia klawisza "Enter", dla 1 wymaga jednej wartoœci, dla 2 dwóch wartoœci. Inne wartoœci nie s¹ obs³ugiwane
@return  -2 (ZAD_FLOTA) Zwracana, gdy u¿yto komendy "flota"
@return -3 (ZAD_COFNIJ)	Zwracana, gdy u¿yto komendy "cofnij"
@return -4 (ZAD_ZAPISZ) Zwracana, gdy u¿yto komendy "zapisz"
@return -5 (ZAD_WCZYTAJ) Zwracana, gdy u¿yto komendy "wczytaj"
@return 0 (ZAD_BRAK) Zwracana, gdy liczbaArgumentow wynosi 0 i nie podano komedy
@return Liczba ca³kowita, której cyfra jednoœci ma wartoœæ drugiego argumentu a pozosta³a czêœæ liczby wartoœæ pierwszego. Zwracana tylko gdy nie u¿yto ¿adnej komendy
*/
int WprowadzZadanie(int liczbaArgumentow);

/**Funkcja wypisuj¹ca statki przeciwnika, które nie zosta³y zatopione.
@param przeciwnik Gracz, którego niezatopione statki nale¿y wypisaæ
*/
void WypiszFlote(Gracz* przeciwnik);
/**Funkcja wypisuj¹ca ruchy poprzedniego gracza. Nie modyfikuje listy. Wypisuje do pierwszego sppotkanego elementu z typem wyliczeniowym okreœlonym jako "start".
@param ruchy Lista wykonanych ruchów.
*/
void WypiszRuchy(Historia* ruchy);

/**Funkcja pobiera od u¿ytkownika informacje gdzie powinien zostaæ umieszczony pojedyczy statek.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param dlugosc D³ugoœæ statku, który nale¿y umieœciæ
@param gracz Gracz, który aktualnie ustawia statki
@param historia Lista ruchów gracza. Pozwala na cofanie statków
@param rodzaj identyfikator statku. Ta wartoœæ zostanie wpisana na pole w momencie ustawiania statku
*/
void PobierzKoordynaty(Konfiguracja* konfiguracja, int dlugosc, Gracz* gracz, Historia** historia, int rodzaj);

/**Funckja stanowi g³ówn¹ pêtle gry. Przekazuje graczom odpowiednie informacje porz¹dkuj¹ce grê. Tylko dla gry w trybie hotseat
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
*/
void GameLoop(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);
/**Funckja stanowi g³ówn¹ pêtle gry. Przekazuje graczowi odpowiednie informacje porz¹dkuj¹ce grê. Tylko dla gry z komputerem.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
*/
void GameLoopAI(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);

/**Funkcja koñczy grê og³aszaj¹c gracza i usuwaj¹c dynamicznie zaalokowan¹ pamiêæ.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
@param wynik Wynik na podstawie którego zaostaje wytypowany wygrywaj¹cy gracz
*/
void ZakonczGre(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka, int wynik);


/**Komunikacja z u¿ytkownikiem po zakoñczeniu przygotowañ. Pojedyncze wywo³anie jest pojedyncz¹ tur¹ jednego gracza
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
@return 0 (B_KONIEC) Informacja o tym, ¿e jeden z graczy straci³ wszystkie statki. Wartoœæ ta koñczy pêtle gry
@return 1 (B_NIEKONIEC) Informacja o tym, ¿e ka¿dy gracz ma jeszcze statki
@return 2 (B_WCZYTAJ) Wartoœæ zwracana w przypadku udanego wcytania gry
*/
int Bitwa(Konfiguracja *konfiguracja, Rozgrywka *rozgrywka);

#endif