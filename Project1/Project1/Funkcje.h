#ifndef FUNKCJE_H
#define FUNKCJE_H
/**@file*/
#include "mxml-3.0/mxml.h"

#define ROZMIAR_POLA 10

#define NO_SYS_NEWLINE 100


//Makra zwi�kszaj�ce czytelno�� kodu
//WPROWAZDZANIE KOMEND
#define ZAD_FLOTA -2
#define ZAD_COFNIJ -3
#define ZAD_ZAPISZ -4
#define ZAD_WCZYTAJ -5
#define ZAD_BRAK 0
//WYNIK BITWY
#define B_KONIEC 0
#define B_NIEKONIEC 1
#define B_WCZYTAJ 2
//WYNIK STRZA�U
#define ST_BLAD 0
#define ST_PUDLO 1
#define ST_CEL 2
#define ST_ZATOP 3

#define ZIELONY 10
#define CZERWONY 12
#define NIEBIESKI 11
#define BIALY 15

#if defined(unix) || defined(__unix__) || defined(__unix)
#define UBIALY "\x1B[37m"
#define UCZERWONY "\x1B[31m"
#define UZIELONY "\x1B[32m"
#define UNIEBIESKI "\x1B[36m"
#endif

/**typ wyliczeniowy okre�laj�cy jaka czynno�� zosta�a wykonana. */
typedef enum Dzialania{start,ustaw,strzal}Zadanie; 

/**struktura przechowuj�ca dane gracza. */
typedef struct {
	//Dynamicznie alokowana tablica dwuwymiarowa - plansza
	int** pole; 
	//Liczba reprezentuj�ca statki gracza
	int statki;
}Gracz;


/**struktura obs�uguj�ca sztuczn� inteligencj�. */
typedef struct {
	int stanPoprzedni;
	int(*stan[6])(int poprzedniePole);
	int aktualnePole;
}Wybor;

/**Lista dynamicza, b�d�ca histori� ruch�w, kt�re zosta�y wykonane. */
typedef struct Lista{
	Zadanie zadanie;
	int argument;
	int rodzaj;
	struct Lista* pPoprzednia;
}Historia;

/**Funkcja czyszcz�ca pozosta�o�ci z bufora.*/
void WyczyscBufor();
/**Funkcja wy�wietlaj�ca graczowi plansz� jego lub przeciwnika.
@param gracz Gracz, kt�rego plansz� nale�y wy�wietli�
@param dyskrecja Parametr okre�laj�cy czy nale�y wy�wietli� statki. Dla zmiennej r�wnej 0, statki s� wy�wietlane, w przeciwnym wypadku s� pomijane.
*/
void RysujPlansze(Gracz gracz, int dyskrecja);
/**Funkcja rozpoczyna przygotowanie do gry w przypadku, gdy nie rozpocz�to od wczytania zapisu. Funkcja komunikuje si� z u�ytkwonikiem przez konsole.
@param gracz Gracz, kt�rego pole gry b�dzie ustalane na podstawie informacji otrzymywanych od u�ytkownika
*/
void Rozmieszczenie(Gracz* gracz);
/**Funkcja rozpoczyna przygotowanie do gry losowo umieszczaj�c wszystkie statki.
@param gracz Gracz, kt�rego pole gry b�dzie ustalane poprzez losowe rozmieszczenie statk�w.
*/
void AutoRozmieszczenie(Gracz* gracz);
/**Procedura alokowania i wype�nienia tablic b�d�ch planszami dla graczy. Tablice alokowane s� dynamicznie korzystaj�c z rozmiaru okre�lonego w makro ROZMIAR_POLA. Gdy alokacja pami�ci b�dzie niemo�liwa, funkcja zwolni zaalokowan� przez ni� pami��.
@param gracz1 Gracz, kt�rego tablic� nale�y wype�ni�
@param gracz2 Gracz, kt�rego tablic� nale�y wype�ni�. Powinien by� r�zny od gracza pierwszego.
@return 1, gdy alokacja zako�czy�a si� sukcesem
@return 0, gdy wyst�pi�y b��dy alokacji. Pami�� zaalokowana do b��du zostanie w funkcji zwolniona.
*/
int WypelnijTablice(Gracz* gracz1, Gracz*gracz2);

/**Funkcja ustawiaj�ca statki na planszy. Sprawdza czy statek mo�e zosta� ustawiony, a nast�pnie go ustawia zmieniaj�c warto�ci znajduj�ce si� na tablicy dwu wymiarowej na identyfikator statku.
@param gracz Gracz, kt�ry ustawia statek
@param dlugosc D�ugo�� ustawianego statku
@param pole Pole, w kt�rym zaczyna si� statek. Jest to liczba dwucyfrowa, kt�rej pierwsza cyfra to pole na osi X, a druga to pole na osi Y
@param kierunek Informacja, w kt�rym kierunku powinien by� ustawiony statek. 1- wertykalnie. 0- horyzontalnie
@param rodzajStatku Identyfikator statku. Ta liczba zostanie wpisana na pole podczas ustawiania. Powinna by� mniejsza od -3 oraz unikalna (wyj�tkiem s� statki zajmuj�ce jedno pole)
@return 0, gdy na podanych koordynatach nie mo�na ustwawi� statku w danym kierunku
@return 1, gdy statek zosta� ustawiony poprawnie
*/
int UstawStatek(Gracz* gracz, int dlugosc, int pole, int kierunek, int rodzajStatku);

/**Funkcja usuwaj�ca statek z planszy. U�ywana tylko w trakcie przygotowania do gry na ��danie gracza. Nast�pnie prosi o podanie nowych koordynat�w tak jakby poprzedni ruch nie zosta� wykonany .Do poprawnego dzia�ania lista historia powinna by� poprawnie zainicjowana, a jej najstarszy element powinien by� utworzony z typem wyliczeniowym start. 
@param historia Wska�nik na liste ruch�w
@param gracz Gracz, kt�ry aktualnie wykonuje ruch
@return 1, gdy usuni�to i ponownie ustawiono statek
@return 0, gdy nie ma ju� ruch�w do cofni�cia na li�cie
@return -1, gdy lista zosta�a �le utworzona (jej pierwszy element nie ma okre�lonego odpowiedniego typu wyliczeniowego)
*/
int UsunStatek(Historia** historia, Gracz* gracz);

/**Procedura usuwania dynamicznie zaalokownaych tablic.
@param gracz1 Gracz, kt�rego tablic� nale�y wype�ni�
@param gracz2 Gracz, kt�rego tablic� nale�y wype�ni�. Powinien by� r�zny od gracza pierwszego.
*/
void UsunTablice(Gracz* gracz1, Gracz*gracz2);


/**Funckja sprawdzaj�ca czy strza� jest mo�liwy do wykonanania oraz zwracaj�ca informacja o tym jaki by� wynik strza�u.
@param atakowanyGracz Gracz, w kt�rego pole zostanie wykonany strza�.
@param pole Pole, w kt�re nale�y strzeli�. Koordynaty s� podane jako liczba dwucyfrowa. Pierwsza cyfra okre�la pole na osi X, a druga na osi Y
@return 0 (ST_BLAD) Informacja o tym, �e strza� nie mo�e sota� wykonany w podane pole
@return 1 (ST_PUDLO) Informacja o tym, �e strza� zosta� wykonany poprawnie w pole na kt�rym nie by�o statku
@return 2 (ST_CEL) Informacja o tym, �e strza� zosta� wykonany poprawnie w pole na kt�rym znajdowa� si� statek
#define 3 (ST_ZATOP) Informacja o tym, �e strza� zosta� wykonany poprawnie oraz statek, kt�ry trafiono zosta� zatopiony
*/
int Strzal(Gracz* atakowanyGracz, int pole);
/**Komunikacja z u�ytkownikiem po zako�czeniu przygotowa�. Pojedyncze wywo�anie jest pojedyncz� tur� jednego gracza
@param gracz1 Gracz, kt�rego tura powinna by� wykonana
@param gracz2 Gracz, kt�ry b�dzie przeciwnikiem dla gracz1. Powinien by� r�zny od gracza pierwszego
@param xml Drzewo obs�ugiwane przez bibilotek� Mini-XML. Parametr potrzebny do zapisywania i wczytywania gry w trakcie rozgrywki
@param ruchy Historia ruch�w gracza. Wykonane ruchy zostan� wy�wielone na pocz�tku tury przeciwnika
@return 0 (B_KONIEC) Informacja o tym, �e jeden z graczy straci� wszystkie statki. Warto�� ta ko�czy p�tle gry
@return 1 (B_NIEKONIEC) Informacja o tym, �e ka�dy gracz ma jeszcze statki.
@return 2 (B_WCZYTAJ) Warto�� zwracana w przypadku udanego wcytania gry. 
*/
int Bitwa(Gracz* gracz1, Gracz* gracz2, mxml_node_t** xml, Historia** ruchy);
/**Funkcja czyszcz�ca konsol�. Zabezpieczenie przed podgl�daniem p�l przeciwnika. W przyadku nieobs�ugiwanego systemu operacyjnego funkcja wypisze dostateczn� ilo�� nowych linii, aby gracze nie widzieli w swojej turze statk�w przeciwnika.
*/
void Oczysc();
/**Funkcja zmieniaj�ca kolor tekstu. Ma zastosowanie estetyczne. Dla nieobs�ugiwanych system�w nie wykona �adnej instrukcji.
*/ 
void ZmienKolor(int typ);
/**Funkcja do daj�ca element do listy na jej pocz�tek.
@param lista Wska�nik na list� do kt�rej nale�y doda� element
@param zadanie Warto�� okre�la jaki typ ruchy zosta� wykonany.
@param argument Informacja do zapsiania na li�cie. Mo�e by� dowolna, ale zosta�a przygotowana na koordynaty
@param rodzaj informacja do zapsiania na li�cie. Mo�e by� dowolna, ale zosta�a przygotowana na kierunek ustawienia statku lub wynik strza�u
@return Wskaznik na liste
@return NULL, gdy nie uda�o si� zaalokowa� pami�ci.
*/
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj);
/**Funkcja do komunikacji z u�ytkownikiem, pozwalaj�ca na u�ywanie komend w dowolnej chwili. Po wprowadzeniu komendy wypisuje informacje, kt�re dzia�anie pr�bowano wykona�.
@param liczbaAgumentow Parametr okre�laj�cy jak du�o danych chcemy otrzyma�. Dla 0 wymaga naci�ni�cia klawisza "Enter", dla 1 wymaga jednej warto�ci, dla 2 dw�ch warto�ci. Inne warto�ci nie s� obs�ugiwane
@return  -2 (ZAD_FLOTA) Zwracana, gdy u�yto komendy "flota"
@return -3 (ZAD_COFNIJ)	Zwracana, gdy u�yto komendy "cofnij"
@return -4 (ZAD_ZAPISZ) Zwracana, gdy u�yto komendy "zapisz"
@return -5 (ZAD_WCZYTAJ) Zwracana, gdy u�yto komendy "wczytaj"
@return 0 (ZAD_BRAK) Zwracana, gdy liczbaArgumentow wynosi 0 i nie podano komedy
@return Liczba ca�kowita, kt�rej cyfra jedno�ci ma warto�� drugiego argumentu a pozosta�a cz�� liczby warto�� pierwszego. Zwracana tylko gdy nie u�yto �adnej komendy
*/
int WprowadzZadanie(int liczbaArgumentow);
/**Usuwanie dynamicznie zaalokowanej listy.
@param lista Wska�nik na list�, kt�rej pami�c nale�y usun��
*/
void UsunListe(Historia**lista);

/**Funkcja wypisuj�ca statki przeciwnika, kt�re nie zosta�y zatopione.
@param przeciwnik Gracz, kt�rego niezatopione statki nale�y wypisa�
*/
void WypiszFlote(Gracz* przeciwnik);
/**Funkcja wypisuj�ca ruchy poprzedniego gracza. Nie modyfikuje listy. Wypisuje do pierwszego sppotkanego elementu z typem wyliczeniowym okre�lonym jako "start".
@param ruchy Lista wykonanych ruch�w.
*/
void WypiszRuchy(Historia* ruchy);

/**Funckcja losuj�ca koordynaty dla algorytmu celowania komputera.
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int Losuj(int poprzedniePole);
/**Funkcja przenosz�ca celowanie o jedno pole do g�ry.
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzN(int poprzedniePole);
/**Funkcja przenosz�ca celowanie o jedno pole w d�.
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzS(int poprzedniePole);
/**Funckja przenosz�ca celowanie o jedno pole w prawo.
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzE(int poprzedniePole);
/**Funckja przenosz�ca celowanie o jedno pole w lewo.
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzW(int poprzedniePole);
/**Funkcja przenosz�ca celowanie o jedno pole w prawo i jedno w d�.
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzSkos(int poprzedniePole);
/**Funkcja obs�uguj�ca tur� dla gry z komputerem. 
@param atakowanyGracz Gracz, kt�ry jest przeciwnikiem komputera
@param AI Struktura posiadaj�ca zainicjowan� tablice wska�nik�w na funckje celuj�ce oraz indeks nast�pnej funkcji do wykonania
@param ruchy Historia ruch�w gracza. Wykonane ruchy zostan� wy�wielone na pocz�tku tury przeciwnika
@return 0 (B_KONIEC) Informacja o tym, �e jeden z graczy straci� wszystkie statki. Warto�� ta ko�czy p�tle gry
@return 1 (B_NIEKONIEC) Informacja o tym, �e ka�dy gracz ma jeszcze statki.
*/
int BitwaAI(Gracz* atakowanyGracz, Wybor* AI, Historia** ruchy);

/**Funkcja pobiera od u�ytkownika informacje gdzie powinien zosta� umieszczony pojedyczy statek.
@param dlugosc D�ugo�� statku, kt�ry nale�y umie�ci�
@param gracz Gracz, kt�ry aktualnie ustawia statki
@param historia Lista ruch�w gracza. Pozwala na cofanie statk�w
@param rodzaj identyfikator statku. Ta warto�� zostanie wpisana na pole w momencie ustawiania statku
*/
void PobierzKoordynaty(int dlugosc, Gracz* gracz, Historia** historia, int rodzaj);

//Funkcje do obs�ugi zapis�w

/**Funkcja tworzy zapis na podstawie ustawie� i zako�czonego, przez obu graczy, przygotowania do gry. Tworzy drzewo xml, kt�re pozwala na �atwe zapisywanie w p�niejszej cz�sci gry.
@param trybGry Informacja o tym w jakim trybie toczy si� rozgrywka
@param gracz1 Struktura przechowuj�ca dane gracza pierwszego
@param gracz2 Struktura przechowuj�ca dane gracza drugiego
@param xml Wska�nik pod kt�rym zostanie utworzone drzewo xml 
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo�na by�o zapisa� do pliku. W tym przypadku drzewo zosta�o utworzone tylko w pami�ci aplikacji
*/
int UtworzZapis(int trybGry, Gracz gracz1, Gracz gracz2, mxml_node_t** xml);
/**Funkcja wykonuj�ca zapis w tracie rozgrywki. Pocz�tkowo funkcja aktualizuje drzewo danymi, kt�rych ci�g�y zapis do drzewa by�by nieoptymalny. Dane aktualizowane znajduj� sie w strukturach Gracz.
@param gracz1 Struktura przechowuj�ca dane gracza pierwszego
@param gracz2 Struktura przechowuj�ca dane gracza drugiego
@param xml Wskaznik na drzewo porzechowuj�ce informacje, kt�re nale�y zapisa� do pliku.
@param ruchy Lista ruch�w
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo�na by�o zapisa� do pliku. W tym przypadku drzewo zosta�o utworzone tylko w pami�ci aplikacji
*/
int Zapisz(Gracz gracz1, Gracz gracz2, mxml_node_t**xml, Historia* ruchy);
/**Funkcja wczytuje dane z pliku z zapisem nadpisuj�c obecne informacje. Nadpisane zostan� dane graczy, drzewo xml oraz lista wykonanych ruch�w.
@param gracz1 Wska�nik na strukture do kt�rej zostan� wpisane dane gracza pierwszego
@param gracz2 Wska�nik na strukture do kt�rej zostan� wpisane dane gracza drugiego
@param xml Wska�nik pod kt�rym zostanie utworzone drzewo xml
@param ruchy Wska�nik na liste ruch�w
@return 0, gdy nie mo�na by�o otworzy� pliku lub za�adowanie drzewa z tego pliku by�o niemo�liwe
@return 1, gdy procedura odczytywania przebieg�a pomy�lnie
*/
int Wczytaj(Gracz* gracz1, Gracz* gracz2, mxml_node_t**xml, Historia** ruchy);

#endif // !FUNKCJE_H

