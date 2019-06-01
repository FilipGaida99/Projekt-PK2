#ifndef AI_H
#define AI_H

/**Funkcja ustawia sztuczn� inteligencj� zgdonie z konfiguracj�.
@param AI, struktura przechowuj�ca dane sztucznej inteligencji
@param konfiguracja struktura przechowuj�ca ustawienia gry
*/
void InicializujAI(Wybor* AI, Konfiguracja* konfiguracja);


/**Funckcja losuj�ca koordynaty dla algorytmu celowania komputera.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int Losuj(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja przenosz�ca celowanie o jedno pole do g�ry.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzN(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja przenosz�ca celowanie o jedno pole w d�.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzS(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funckja przenosz�ca celowanie o jedno pole w prawo.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzE(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funckja przenosz�ca celowanie o jedno pole w lewo.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzW(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja przenosz�ca celowanie o jedno pole w prawo i jedno w d�.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param poprzedniePole Poprzednie pole w kt�re celowano
@return Nowe pole, w kt�re nale�y wycelowa�
*/
int IdzSkos(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja obs�uguj�ca tur� dla gry z komputerem.
@param konfiguracja struktura przechowuj�ca ustawienia gry
@param rozgrywka struktura przechowuj�ca dane obecnej rozgrywki
@param AI Struktura posiadaj�ca zainicjowan� tablice wska�nik�w na funckje celuj�ce oraz indeks nast�pnej funkcji do wykonania
@return 0 (B_KONIEC) Informacja o tym, �e jeden z graczy straci� wszystkie statki. Warto�� ta ko�czy p�tle gry
@return 1 (B_NIEKONIEC) Informacja o tym, �e ka�dy gracz ma jeszcze statki.
*/
int BitwaAI(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka, Wybor* AI);


#endif // !AI_H

