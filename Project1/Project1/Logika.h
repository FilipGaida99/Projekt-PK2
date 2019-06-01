#ifndef LOGIKA_H
#define LOGIKA_H


/**Fukcja zamienia pozycjami graczy by gracz drugi móg³ wykonaæ swoj¹ turê.
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
*/
void ZamianaGraczy(Rozgrywka* rozgrywka);



/**Funkcja dodaje odpowiedni¹ iloœæ punktów ¿ycia do statków gracza, na odpowiedniej pozycji.
@param gracz Gracz do którego dodane zostan¹ punkty ¿ycia
@param pozycja Pozycja w systemie usemkowym na któr¹ zostanie dodana liczba punktów ¿ycia
@param dlugosc Dodawana liczba punktów ¿ycia, zale¿na od statku
*/
void DodajdoFloty(Gracz* gracz, int pozycja, int dlugosc);


/**Procedura alokowania i wype³nienia tablic bêd¹ch planszami dla graczy. Tablice alokowane s¹ dynamicznie korzystaj¹c z rozmiaru okreœlonego w makro ROZMIAR_POLA. Gdy alokacja pamiêci bêdzie niemo¿liwa, funkcja zwolni zaalokowan¹ przez ni¹ pamiêæ.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param gracz1 Gracz, którego tablicê nale¿y wype³niæ
@param gracz2 Gracz, którego tablicê nale¿y wype³niæ. Powinien byæ rózny od gracza pierwszego
@return 1, gdy alokacja zakoñczy³a siê sukcesem
@return 0, gdy wyst¹pi³y b³êdy alokacji. Pamiêæ zaalokowana do b³êdu zostanie w funkcji zwolniona
*/
int WypelnijTablice(Konfiguracja* konfiguracja, Gracz* gracz1, Gracz*gracz2);

/**Funkcja ustawiaj¹ca statki na planszy. Sprawdza czy statek mo¿e zostaæ ustawiony, a nastêpnie go ustawia zmieniaj¹c wartoœci znajduj¹ce siê na tablicy dwu wymiarowej na identyfikator statku.
@param gracz Gracz, który ustawia statek
@param dlugosc D³ugoœæ ustawianego statku
@param pole Pole, w którym zaczyna siê statek. Jest to liczba dwucyfrowa, której pierwsza cyfra to pole na osi X, a druga to pole na osi Y
@param kierunek Informacja, w którym kierunku powinien byæ ustawiony statek. 1- wertykalnie. 0- horyzontalnie
@param rodzajStatku Identyfikator statku. Ta liczba zostanie wpisana na pole podczas ustawiania. Powinna byæ mniejsza od -3 oraz unikalna (wyj¹tkiem s¹ statki zajmuj¹ce jedno pole)
@return 0, gdy na podanych koordynatach nie mo¿na ustwawiæ statku w danym kierunku
@return 1, gdy statek zosta³ ustawiony poprawnie
*/
int UstawStatek(Konfiguracja* konfiguracja, Gracz* gracz, int dlugosc, int pole, int kierunek, int rodzajStatku);

/**Funkcja usuwaj¹ca statek z planszy. U¿ywana tylko w trakcie przygotowania do gry na ¿¹danie gracza. Nastêpnie prosi o podanie nowych koordynatów tak jakby poprzedni ruch nie zosta³ wykonany .Do poprawnego dzia³ania lista historia powinna byæ poprawnie zainicjowana, a jej najstarszy element powinien byæ utworzony z typem wyliczeniowym start.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param historia WskaŸnik na liste ruchów
@param gracz Gracz, który aktualnie wykonuje ruch
@return 1, gdy usuniêto i ponownie ustawiono statek
@return 0, gdy nie ma ju¿ ruchów do cofniêcia na liœcie
@return -1, gdy lista zosta³a Ÿle utworzona (jej pierwszy element nie ma okreœlonego odpowiedniego typu wyliczeniowego)
*/
int UsunStatek(Konfiguracja* konfiguracja, Historia** historia, Gracz* gracz);

/**Procedura usuwania dynamicznie zaalokownaych tablic.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param gracz1 Gracz, którego tablicê nale¿y wype³niæ
@param gracz2 Gracz, którego tablicê nale¿y wype³niæ. Powinien byæ rózny od gracza pierwszego
*/
void UsunTablice(Konfiguracja* konfiguracja, Gracz* gracz1, Gracz*gracz2);


/**Funckja sprawdzaj¹ca czy strza³ jest mo¿liwy do wykonanania oraz zwracaj¹ca informacja o tym jaki by³ wynik strza³u.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param atakowanyGracz Gracz, w którego pole zostanie wykonany strza³
@param pole Pole, w które nale¿y strzeliæ. Koordynaty s¹ podane jako liczba dwucyfrowa. Pierwsza cyfra okreœla pole na osi X, a druga na osi Y
@return 0 (ST_BLAD) Informacja o tym, ¿e strza³ nie mo¿e sotaæ wykonany w podane pole
@return 1 (ST_PUDLO) Informacja o tym, ¿e strza³ zosta³ wykonany poprawnie w pole na którym nie by³o statku
@return 2 (ST_CEL) Informacja o tym, ¿e strza³ zosta³ wykonany poprawnie w pole na którym znajdowa³ siê statek
@return 3 (ST_ZATOP) Informacja o tym, ¿e strza³ zosta³ wykonany poprawnie oraz statek, który trafiono zosta³ zatopiony
*/
int Strzal(Konfiguracja* konfiguracja, Gracz* atakowanyGracz, int pole);


/**Funkcja do daj¹ca element do listy na jej pocz¹tek.
@param lista WskaŸnik na listê do której nale¿y dodaæ element
@param zadanie Wartoœæ okreœla jaki typ ruchy zosta³ wykonany.
@param argument Informacja do zapsiania na liœcie. Mo¿e byæ dowolna, ale zosta³a przygotowana na koordynaty
@param rodzaj informacja do zapsiania na liœcie. Mo¿e byæ dowolna, ale zosta³a przygotowana na kierunek ustawienia statku lub wynik strza³u
@return Wskaznik na liste
@return NULL, gdy nie uda³o siê zaalokowaæ pamiêci
*/
Historia* DodajdoListy(Historia** lista, Zadanie zadanie, int argument, int rodzaj);

/**Usuwanie dynamicznie zaalokowanej listy.
@param lista WskaŸnik na listê, której pamiêc nale¿y usun¹æ
*/
void UsunListe(Historia**lista);






#endif // !LOGIKA_H
