#ifndef FUNKCJE_H
#define FUNKCJE_H
/**@file*/
#include "mxml-3.0/mxml.h"

#define ROZMIAR_POLA 10

#define NO_SYS_NEWLINE 100


//Makra zwiêkszaj¹ce czytelnoœæ kodu
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
//WYNIK STRZA£U
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

/**typ wyliczeniowy okreœlaj¹cy jaka czynnoœæ zosta³a wykonana. */
typedef enum Dzialania{start,ustaw,strzal}Zadanie; 

/**struktura przechowuj¹ca dane gracza. */
typedef struct {
	//Dynamicznie alokowana tablica dwuwymiarowa - plansza
	int** pole; 
	//Liczba reprezentuj¹ca statki gracza
	int statki;
}Gracz;


/**struktura obs³uguj¹ca sztuczn¹ inteligencjê. */
typedef struct {
	int stanPoprzedni;
	int(*stan[6])(int poprzedniePole);
	int aktualnePole;
}Wybor;

/**Lista dynamicza, bêd¹ca histori¹ ruchów, które zosta³y wykonane. */
typedef struct Lista{
	Zadanie zadanie;
	int argument;
	int rodzaj;
	struct Lista* pPoprzednia;
}Historia;

/**Funkcja czyszcz¹ca pozosta³oœci z bufora.*/
void WyczyscBufor();
/**Funkcja wyœwietlaj¹ca graczowi planszê jego lub przeciwnika.
@param gracz Gracz, którego planszê nale¿y wyœwietliæ
@param dyskrecja Parametr okreœlaj¹cy czy nale¿y wyœwietliæ statki. Dla zmiennej równej 0, statki s¹ wyœwietlane, w przeciwnym wypadku s¹ pomijane.
*/
void RysujPlansze(Gracz gracz, int dyskrecja);
/**Funkcja rozpoczyna przygotowanie do gry w przypadku, gdy nie rozpoczêto od wczytania zapisu. Funkcja komunikuje siê z u¿ytkwonikiem przez konsole.
@param gracz Gracz, którego pole gry bêdzie ustalane na podstawie informacji otrzymywanych od u¿ytkownika
*/
void Rozmieszczenie(Gracz* gracz);
/**Funkcja rozpoczyna przygotowanie do gry losowo umieszczaj¹c wszystkie statki.
@param gracz Gracz, którego pole gry bêdzie ustalane poprzez losowe rozmieszczenie statków.
*/
void AutoRozmieszczenie(Gracz* gracz);
/**Procedura alokowania i wype³nienia tablic bêd¹ch planszami dla graczy. Tablice alokowane s¹ dynamicznie korzystaj¹c z rozmiaru okreœlonego w makro ROZMIAR_POLA. Gdy alokacja pamiêci bêdzie niemo¿liwa, funkcja zwolni zaalokowan¹ przez ni¹ pamiêæ.
@param gracz1 Gracz, którego tablicê nale¿y wype³niæ
@param gracz2 Gracz, którego tablicê nale¿y wype³niæ. Powinien byæ rózny od gracza pierwszego.
@return 1, gdy alokacja zakoñczy³a siê sukcesem
@return 0, gdy wyst¹pi³y b³êdy alokacji. Pamiêæ zaalokowana do b³êdu zostanie w funkcji zwolniona.
*/
int WypelnijTablice(Gracz* gracz1, Gracz*gracz2);

/**Funkcja ustawiaj¹ca statki na planszy. Sprawdza czy statek mo¿e zostaæ ustawiony, a nastêpnie go ustawia zmieniaj¹c wartoœci znajduj¹ce siê na tablicy dwu wymiarowej na identyfikator statku.
@param gracz Gracz, który ustawia statek
@param dlugosc D³ugoœæ ustawianego statku
@param pole Pole, w którym zaczyna siê statek. Jest to liczba dwucyfrowa, której pierwsza cyfra to pole na osi X, a druga to pole na osi Y
@param kierunek Informacja, w którym kierunku powinien byæ ustawiony statek. 1- wertykalnie. 0- horyzontalnie
@param rodzajStatku Identyfikator statku. Ta liczba zostanie wpisana na pole podczas ustawiania. Powinna byæ mniejsza od -3 oraz unikalna (wyj¹tkiem s¹ statki zajmuj¹ce jedno pole)
@return 0, gdy na podanych koordynatach nie mo¿na ustwawiæ statku w danym kierunku
@return 1, gdy statek zosta³ ustawiony poprawnie
*/
int UstawStatek(Gracz* gracz, int dlugosc, int pole, int kierunek, int rodzajStatku);

/**Funkcja usuwaj¹ca statek z planszy. U¿ywana tylko w trakcie przygotowania do gry na ¿¹danie gracza. Nastêpnie prosi o podanie nowych koordynatów tak jakby poprzedni ruch nie zosta³ wykonany .Do poprawnego dzia³ania lista historia powinna byæ poprawnie zainicjowana, a jej najstarszy element powinien byæ utworzony z typem wyliczeniowym start. 
@param historia WskaŸnik na liste ruchów
@param gracz Gracz, który aktualnie wykonuje ruch
@return 1, gdy usuniêto i ponownie ustawiono statek
@return 0, gdy nie ma ju¿ ruchów do cofniêcia na liœcie
@return -1, gdy lista zosta³a Ÿle utworzona (jej pierwszy element nie ma okreœlonego odpowiedniego typu wyliczeniowego)
*/
int UsunStatek(Historia** historia, Gracz* gracz);

/**Procedura usuwania dynamicznie zaalokownaych tablic.
@param gracz1 Gracz, którego tablicê nale¿y wype³niæ
@param gracz2 Gracz, którego tablicê nale¿y wype³niæ. Powinien byæ rózny od gracza pierwszego.
*/
void UsunTablice(Gracz* gracz1, Gracz*gracz2);


/**Funckja sprawdzaj¹ca czy strza³ jest mo¿liwy do wykonanania oraz zwracaj¹ca informacja o tym jaki by³ wynik strza³u.
@param atakowanyGracz Gracz, w którego pole zostanie wykonany strza³.
@param pole Pole, w które nale¿y strzeliæ. Koordynaty s¹ podane jako liczba dwucyfrowa. Pierwsza cyfra okreœla pole na osi X, a druga na osi Y
@return 0 (ST_BLAD) Informacja o tym, ¿e strza³ nie mo¿e sotaæ wykonany w podane pole
@return 1 (ST_PUDLO) Informacja o tym, ¿e strza³ zosta³ wykonany poprawnie w pole na którym nie by³o statku
@return 2 (ST_CEL) Informacja o tym, ¿e strza³ zosta³ wykonany poprawnie w pole na którym znajdowa³ siê statek
#define 3 (ST_ZATOP) Informacja o tym, ¿e strza³ zosta³ wykonany poprawnie oraz statek, który trafiono zosta³ zatopiony
*/
int Strzal(Gracz* atakowanyGracz, int pole);
/**Komunikacja z u¿ytkownikiem po zakoñczeniu przygotowañ. Pojedyncze wywo³anie jest pojedyncz¹ tur¹ jednego gracza
@param gracz1 Gracz, którego tura powinna byæ wykonana
@param gracz2 Gracz, który bêdzie przeciwnikiem dla gracz1. Powinien byæ rózny od gracza pierwszego
@param xml Drzewo obs³ugiwane przez bibilotekê Mini-XML. Parametr potrzebny do zapisywania i wczytywania gry w trakcie rozgrywki
@param ruchy Historia ruchów gracza. Wykonane ruchy zostan¹ wyœwielone na pocz¹tku tury przeciwnika
@return 0 (B_KONIEC) Informacja o tym, ¿e jeden z graczy straci³ wszystkie statki. Wartoœæ ta koñczy pêtle gry
@return 1 (B_NIEKONIEC) Informacja o tym, ¿e ka¿dy gracz ma jeszcze statki.
@return 2 (B_WCZYTAJ) Wartoœæ zwracana w przypadku udanego wcytania gry. 
*/
int Bitwa(Gracz* gracz1, Gracz* gracz2, mxml_node_t** xml, Historia** ruchy);
/**Funkcja czyszcz¹ca konsolê. Zabezpieczenie przed podgl¹daniem pól przeciwnika. W przyadku nieobs³ugiwanego systemu operacyjnego funkcja wypisze dostateczn¹ iloœæ nowych linii, aby gracze nie widzieli w swojej turze statków przeciwnika.
*/
void Oczysc();
/**Funkcja zmieniaj¹ca kolor tekstu. Ma zastosowanie estetyczne. Dla nieobs³ugiwanych systemów nie wykona ¿adnej instrukcji.
*/ 
void ZmienKolor(int typ);
/**Funkcja do daj¹ca element do listy na jej pocz¹tek.
@param lista WskaŸnik na listê do której nale¿y dodaæ element
@param zadanie Wartoœæ okreœla jaki typ ruchy zosta³ wykonany.
@param argument Informacja do zapsiania na liœcie. Mo¿e byæ dowolna, ale zosta³a przygotowana na koordynaty
@param rodzaj informacja do zapsiania na liœcie. Mo¿e byæ dowolna, ale zosta³a przygotowana na kierunek ustawienia statku lub wynik strza³u
@return Wskaznik na liste
@return NULL, gdy nie uda³o siê zaalokowaæ pamiêci.
*/
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj);
/**Funkcja do komunikacji z u¿ytkownikiem, pozwalaj¹ca na u¿ywanie komend w dowolnej chwili. Po wprowadzeniu komendy wypisuje informacje, które dzia³anie próbowano wykonaæ.
@param liczbaAgumentow Parametr okreœlaj¹cy jak du¿o danych chcemy otrzymaæ. Dla 0 wymaga naciœniêcia klawisza "Enter", dla 1 wymaga jednej wartoœci, dla 2 dwóch wartoœci. Inne wartoœci nie s¹ obs³ugiwane
@return  -2 (ZAD_FLOTA) Zwracana, gdy u¿yto komendy "flota"
@return -3 (ZAD_COFNIJ)	Zwracana, gdy u¿yto komendy "cofnij"
@return -4 (ZAD_ZAPISZ) Zwracana, gdy u¿yto komendy "zapisz"
@return -5 (ZAD_WCZYTAJ) Zwracana, gdy u¿yto komendy "wczytaj"
@return 0 (ZAD_BRAK) Zwracana, gdy liczbaArgumentow wynosi 0 i nie podano komedy
@return Liczba ca³kowita, której cyfra jednoœci ma wartoœæ drugiego argumentu a pozosta³a czêœæ liczby wartoœæ pierwszego. Zwracana tylko gdy nie u¿yto ¿adnej komendy
*/
int WprowadzZadanie(int liczbaArgumentow);
/**Usuwanie dynamicznie zaalokowanej listy.
@param lista WskaŸnik na listê, której pamiêc nale¿y usun¹æ
*/
void UsunListe(Historia**lista);

/**Funkcja wypisuj¹ca statki przeciwnika, które nie zosta³y zatopione.
@param przeciwnik Gracz, którego niezatopione statki nale¿y wypisaæ
*/
void WypiszFlote(Gracz* przeciwnik);
/**Funkcja wypisuj¹ca ruchy poprzedniego gracza. Nie modyfikuje listy. Wypisuje do pierwszego sppotkanego elementu z typem wyliczeniowym okreœlonym jako "start".
@param ruchy Lista wykonanych ruchów.
*/
void WypiszRuchy(Historia* ruchy);

/**Funckcja losuj¹ca koordynaty dla algorytmu celowania komputera.
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int Losuj(int poprzedniePole);
/**Funkcja przenosz¹ca celowanie o jedno pole do góry.
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzN(int poprzedniePole);
/**Funkcja przenosz¹ca celowanie o jedno pole w dó³.
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzS(int poprzedniePole);
/**Funckja przenosz¹ca celowanie o jedno pole w prawo.
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzE(int poprzedniePole);
/**Funckja przenosz¹ca celowanie o jedno pole w lewo.
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzW(int poprzedniePole);
/**Funkcja przenosz¹ca celowanie o jedno pole w prawo i jedno w dó³.
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzSkos(int poprzedniePole);
/**Funkcja obs³uguj¹ca turê dla gry z komputerem. 
@param atakowanyGracz Gracz, który jest przeciwnikiem komputera
@param AI Struktura posiadaj¹ca zainicjowan¹ tablice wskaŸników na funckje celuj¹ce oraz indeks nastêpnej funkcji do wykonania
@param ruchy Historia ruchów gracza. Wykonane ruchy zostan¹ wyœwielone na pocz¹tku tury przeciwnika
@return 0 (B_KONIEC) Informacja o tym, ¿e jeden z graczy straci³ wszystkie statki. Wartoœæ ta koñczy pêtle gry
@return 1 (B_NIEKONIEC) Informacja o tym, ¿e ka¿dy gracz ma jeszcze statki.
*/
int BitwaAI(Gracz* atakowanyGracz, Wybor* AI, Historia** ruchy);

/**Funkcja pobiera od u¿ytkownika informacje gdzie powinien zostaæ umieszczony pojedyczy statek.
@param dlugosc D³ugoœæ statku, który nale¿y umieœciæ
@param gracz Gracz, który aktualnie ustawia statki
@param historia Lista ruchów gracza. Pozwala na cofanie statków
@param rodzaj identyfikator statku. Ta wartoœæ zostanie wpisana na pole w momencie ustawiania statku
*/
void PobierzKoordynaty(int dlugosc, Gracz* gracz, Historia** historia, int rodzaj);

//Funkcje do obs³ugi zapisów

/**Funkcja tworzy zapis na podstawie ustawieñ i zakoñczonego, przez obu graczy, przygotowania do gry. Tworzy drzewo xml, które pozwala na ³atwe zapisywanie w póŸniejszej czêsci gry.
@param trybGry Informacja o tym w jakim trybie toczy siê rozgrywka
@param gracz1 Struktura przechowuj¹ca dane gracza pierwszego
@param gracz2 Struktura przechowuj¹ca dane gracza drugiego
@param xml WskaŸnik pod którym zostanie utworzone drzewo xml 
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo¿na by³o zapisaæ do pliku. W tym przypadku drzewo zosta³o utworzone tylko w pamiêci aplikacji
*/
int UtworzZapis(int trybGry, Gracz gracz1, Gracz gracz2, mxml_node_t** xml);
/**Funkcja wykonuj¹ca zapis w tracie rozgrywki. Pocz¹tkowo funkcja aktualizuje drzewo danymi, których ci¹g³y zapis do drzewa by³by nieoptymalny. Dane aktualizowane znajduj¹ sie w strukturach Gracz.
@param gracz1 Struktura przechowuj¹ca dane gracza pierwszego
@param gracz2 Struktura przechowuj¹ca dane gracza drugiego
@param xml Wskaznik na drzewo porzechowuj¹ce informacje, które nale¿y zapisaæ do pliku.
@param ruchy Lista ruchów
@return 1, gdy poprawnie wykonano zapis do pliku
@return 0, gdy nie mo¿na by³o zapisaæ do pliku. W tym przypadku drzewo zosta³o utworzone tylko w pamiêci aplikacji
*/
int Zapisz(Gracz gracz1, Gracz gracz2, mxml_node_t**xml, Historia* ruchy);
/**Funkcja wczytuje dane z pliku z zapisem nadpisuj¹c obecne informacje. Nadpisane zostan¹ dane graczy, drzewo xml oraz lista wykonanych ruchów.
@param gracz1 Wska¿nik na strukture do której zostan¹ wpisane dane gracza pierwszego
@param gracz2 Wska¿nik na strukture do której zostan¹ wpisane dane gracza drugiego
@param xml WskaŸnik pod którym zostanie utworzone drzewo xml
@param ruchy WskaŸnik na liste ruchów
@return 0, gdy nie mo¿na by³o otworzyæ pliku lub za³adowanie drzewa z tego pliku by³o niemo¿liwe
@return 1, gdy procedura odczytywania przebieg³a pomyœlnie
*/
int Wczytaj(Gracz* gracz1, Gracz* gracz2, mxml_node_t**xml, Historia** ruchy);

#endif // !FUNKCJE_H

