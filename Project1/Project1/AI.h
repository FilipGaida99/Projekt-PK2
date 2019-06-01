#ifndef AI_H
#define AI_H

/**Funkcja ustawia sztuczn¹ inteligencjê zgdonie z konfiguracj¹.
@param AI, struktura przechowuj¹ca dane sztucznej inteligencji
@param konfiguracja struktura przechowuj¹ca ustawienia gry
*/
void InicializujAI(Wybor* AI, Konfiguracja* konfiguracja);


/**Funckcja losuj¹ca koordynaty dla algorytmu celowania komputera.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int Losuj(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja przenosz¹ca celowanie o jedno pole do góry.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzN(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja przenosz¹ca celowanie o jedno pole w dó³.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzS(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funckja przenosz¹ca celowanie o jedno pole w prawo.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzE(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funckja przenosz¹ca celowanie o jedno pole w lewo.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzW(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja przenosz¹ca celowanie o jedno pole w prawo i jedno w dó³.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param poprzedniePole Poprzednie pole w które celowano
@return Nowe pole, w które nale¿y wycelowaæ
*/
int IdzSkos(Konfiguracja* konfiguracja, int poprzedniePole);
/**Funkcja obs³uguj¹ca turê dla gry z komputerem.
@param konfiguracja struktura przechowuj¹ca ustawienia gry
@param rozgrywka struktura przechowuj¹ca dane obecnej rozgrywki
@param AI Struktura posiadaj¹ca zainicjowan¹ tablice wskaŸników na funckje celuj¹ce oraz indeks nastêpnej funkcji do wykonania
@return 0 (B_KONIEC) Informacja o tym, ¿e jeden z graczy straci³ wszystkie statki. Wartoœæ ta koñczy pêtle gry
@return 1 (B_NIEKONIEC) Informacja o tym, ¿e ka¿dy gracz ma jeszcze statki.
*/
int BitwaAI(Konfiguracja* konfiguracja, Rozgrywka* rozgrywka, Wybor* AI);


#endif // !AI_H

