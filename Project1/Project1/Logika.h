#ifndef LOGIKA_H
#define LOGIKA_H


/**Fukcja zamienia pozycjami graczy by gracz drugi m�g� wykona� swoj� tur�.
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
*/
void ZamianaGraczy(Rozgrywka* rozgrywka);



/**Funkcja dodaje odpowiedni� ilo�� punkt�w �ycia do statk�w gracza, na odpowiedniej pozycji.
@param gracz Gracz do kt�rego dodane zostan� punkty �ycia
@param pozycja Pozycja w systemie usemkowym na kt�r� zostanie dodana liczba punkt�w �ycia
@param dlugosc Dodawana liczba punkt�w �ycia, zale�na od statku
*/
void DodajdoFloty(Gracz* gracz, int pozycja, int dlugosc);


/**Procedura alokowania i wype�nienia tablic b�d�ch planszami dla graczy. Tablice alokowane s� dynamicznie korzystaj�c z rozmiaru okre�lonego w makro ROZMIAR_POLA. Gdy alokacja pami�ci b�dzie niemo�liwa, funkcja zwolni zaalokowan� przez ni� pami��.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param gracz1 Gracz, kt�rego tablic� nale�y wype�ni�
@param gracz2 Gracz, kt�rego tablic� nale�y wype�ni�. Powinien by� r�zny od gracza pierwszego
@return 1, gdy alokacja zako�czy�a si� sukcesem
@return 0, gdy wyst�pi�y b��dy alokacji. Pami�� zaalokowana do b��du zostanie w funkcji zwolniona
*/
int WypelnijTablice(Konfiguracja* konfiguracja, Gracz* gracz1, Gracz*gracz2);

/**Funkcja ustawiaj�ca statki na planszy. Sprawdza czy statek mo�e zosta� ustawiony, a nast�pnie go ustawia zmieniaj�c warto�ci znajduj�ce si� na tablicy dwu wymiarowej na identyfikator statku.
@param gracz Gracz, kt�ry ustawia statek
@param dlugosc D�ugo�� ustawianego statku
@param pole Pole, w kt�rym zaczyna si� statek. Jest to liczba dwucyfrowa, kt�rej pierwsza cyfra to pole na osi X, a druga to pole na osi Y
@param kierunek Informacja, w kt�rym kierunku powinien by� ustawiony statek. 1- wertykalnie. 0- horyzontalnie
@param rodzajStatku Identyfikator statku. Ta liczba zostanie wpisana na pole podczas ustawiania. Powinna by� mniejsza od -3 oraz unikalna (wyj�tkiem s� statki zajmuj�ce jedno pole)
@return 0, gdy na podanych koordynatach nie mo�na ustwawi� statku w danym kierunku
@return 1, gdy statek zosta� ustawiony poprawnie
*/
int UstawStatek(Konfiguracja* konfiguracja, Gracz* gracz, int dlugosc, int pole, int kierunek, int rodzajStatku);

/**Funkcja usuwaj�ca statek z planszy. U�ywana tylko w trakcie przygotowania do gry na ��danie gracza. Nast�pnie prosi o podanie nowych koordynat�w tak jakby poprzedni ruch nie zosta� wykonany .Do poprawnego dzia�ania lista historia powinna by� poprawnie zainicjowana, a jej najstarszy element powinien by� utworzony z typem wyliczeniowym start.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param historia Wska�nik na liste ruch�w
@param gracz Gracz, kt�ry aktualnie wykonuje ruch
@return 1, gdy usuni�to i ponownie ustawiono statek
@return 0, gdy nie ma ju� ruch�w do cofni�cia na li�cie
@return -1, gdy lista zosta�a �le utworzona (jej pierwszy element nie ma okre�lonego odpowiedniego typu wyliczeniowego)
*/
int UsunStatek(Konfiguracja* konfiguracja, Historia** historia, Gracz* gracz);

/**Procedura usuwania dynamicznie zaalokownaych tablic.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param gracz1 Gracz, kt�rego tablic� nale�y wype�ni�
@param gracz2 Gracz, kt�rego tablic� nale�y wype�ni�. Powinien by� r�zny od gracza pierwszego
*/
void UsunTablice(Konfiguracja* konfiguracja, Gracz* gracz1, Gracz*gracz2);


/**Funckja sprawdzaj�ca czy strza� jest mo�liwy do wykonanania oraz zwracaj�ca informacja o tym jaki by� wynik strza�u.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param atakowanyGracz Gracz, w kt�rego pole zostanie wykonany strza�
@param pole Pole, w kt�re nale�y strzeli�. Koordynaty s� podane jako liczba dwucyfrowa. Pierwsza cyfra okre�la pole na osi X, a druga na osi Y
@return 0 (ST_BLAD) Informacja o tym, �e strza� nie mo�e sota� wykonany w podane pole
@return 1 (ST_PUDLO) Informacja o tym, �e strza� zosta� wykonany poprawnie w pole na kt�rym nie by�o statku
@return 2 (ST_CEL) Informacja o tym, �e strza� zosta� wykonany poprawnie w pole na kt�rym znajdowa� si� statek
@return 3 (ST_ZATOP) Informacja o tym, �e strza� zosta� wykonany poprawnie oraz statek, kt�ry trafiono zosta� zatopiony
*/
int Strzal(Konfiguracja* konfiguracja, Gracz* atakowanyGracz, int pole);


/**Funkcja do daj�ca element do listy na jej pocz�tek.
@param lista Wska�nik na list� do kt�rej nale�y doda� element
@param zadanie Warto�� okre�la jaki typ ruchy zosta� wykonany.
@param argument Informacja do zapsiania na li�cie. Mo�e by� dowolna, ale zosta�a przygotowana na koordynaty
@param rodzaj informacja do zapsiania na li�cie. Mo�e by� dowolna, ale zosta�a przygotowana na kierunek ustawienia statku lub wynik strza�u
@return Wskaznik na liste
@return NULL, gdy nie uda�o si� zaalokowa� pami�ci
*/
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj);

/**Usuwanie dynamicznie zaalokowanej listy.
@param lista Wska�nik na list�, kt�rej pami�c nale�y usun��
*/
void UsunListe(Historia**lista);






#endif // !LOGIKA_H
