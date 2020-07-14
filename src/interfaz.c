#include <ncurses.h>
#include <ajd_types.h>
#include <tablero.h>
#include <interfaz.h>
#include <string.h>

#define TABLERO_ROW_START   9
#define TABLERO_COL_START   10
#define MARCADOR_ROW_START  TABLERO_ROW_START
#define MARCADOR_COL_START  TABLERO_COL_START + 8 * 3 + 2
#define MARCADOR_LAST_ROW   TABLERO_ROW_START + 8 * 3 - 1
/*
AJD_Sprite sprCursorPiezaSeleccionada;
AJD_Sprite sprCursorMovil;
*/
/****************************************************************************************
 * Funciones PRIVADAS (forward declaration)
***************************************************************************************/
void dibujaEncabezados ();
void dibujaCasilla (AJD_idCasilla);

/****************************************************************************************
 ****************************************************************************************
 * Funciones PUBLICAS (Implementacion)
 ****************************************************************************************
 ***************************************************************************************/
/****************************************************************************************
 * inicializaPantalla
 *
 * Inicializa libreria ncurses y la interfaz de usuario en general.
 ***************************************************************************************/
void inicializaPantalla()
{
    /* Inicializa ncurses */
    initscr();

    /* Habilita teclas especiales (F1, cursores, ESC, etc.) */
    keypad (stdscr, 1);

    /* Envío de teclas inmediato (sin pulsar ENTER) */
    cbreak();

    /* No mostrar caracteres pulsados */
    noecho();

    /* No espera en lectura de caracter */
    nodelay(stdscr, 1);

    /* Ocultar cursor de pantalla */
    curs_set(0);

    /* Inicializa colores */
    if ( has_colors() )
    {
        start_color();

        /* Negro sobre fondo blanco para dibujar casilla blanca */
        init_pair(BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

        /* Blanco sobre negro para dibujar casilla negra y textos */
        init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);

        /* Amarillo sobre negro para los menus */
        init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);

    }

    clear();
}
/****************************************************************************************
 * dibujaTablero
 *
 * Dibuja el tablero en pantalla según su estado actual.
 ***************************************************************************************/
void dibujaTablero(AJD_TableroPtr tablero)
{
    AJD_idCasilla idCasilla = h1;
    int ch;

    /* Dibuja la primera casilla */
    dibujaCasilla (a8);
    /* Dibuja el resto de casillas */
    while (idCasilla)
        dibujaCasilla (idCasilla--);

    dibujaEncabezados ();
/*
    Dibuja el cursor de seleccion 
    if (tablero->cursorMovil.visible)
    {
        dibujaCursor(tablero->cursorMovil);
    }
    if (tablero->cursorPiezaSeleccionada.visible)
    {
        dibujaCursor(tablero->cursorPiezaSeleccionada);
    }
*/
    
    while ( (ch = getch()) != '\033');
    endwin();
}
/****************************************************************************************
 * dibujaPieza
 *
 * Dibuja la pieza indicada en las coordenadas y,x
 ***************************************************************************************/
void dibujaPieza (int y, int x, AJD_Pieza pieza)
{
    /* Array de caracteres representado la letra a dibujar de cada pieza */
    char *sprPiezas=" PTCADRptcadr";
    char sprite = sprPiezas[pieza];

    /* Dibuja la letra de la pieza en color negro sobre blanco */
    attron (COLOR_PAIR (COLOR_TEXTO_RESALTADO));
    mvprintw (y, x, "%c", sprite);
    attroff (COLOR_PAIR (COLOR_TEXTO_RESALTADO));
}


/****************************************************************************************
 ****************************************************************************************
 * Funciones PRIVADAS (Implementacion)
 ****************************************************************************************
 ***************************************************************************************/
/****************************************************************************************
 * dibujaCasilla
 *
 * Dibuja la casilla 'id' y la pieza que contiene (si la hubiese)
 ***************************************************************************************/
void dibujaCasilla (AJD_idCasilla id)
{
    AJD_Casilla casilla = obtenCasilla (id);

    /* columna y fila correspondientes a esta casilla en el tablero */
    int y = (id / 8) * ANCHO_CASILLA + TABLERO_ROW_START;
    int x = (id & 7) * ALTO_CASILLA + TABLERO_COL_START;
    int nrows = ALTO_CASILLA;

    /* Color principal/fondo a usar dependiendo de la casilla */
    int color = (casilla.color) ? COLOR_CASILLA_BLANCA : COLOR_CASILLA_NEGRA;       

    while (nrows)
    {
        /* Activa el color para pintar esta casilla */
        attron(COLOR_PAIR (color));
        mvprintw (y + nrows, x, "   ");  /* 3 espacios */

        /* Dibuja pieza si es que hay alguna en la casilla actual */
        if (casilla.pieza)
        {           
            dibujaPieza (y+2, x+1, casilla.pieza);
        }

        /*attron(COLOR_PAIR (color));*/

        nrows--;
    }
}
/****************************************************************************************
 * dibujaPieza
 *
 * Dibuja la pieza indicada en las coordenadas y,x
 ***************************************************************************************/
void dibujaEncabezados ()
{
    int n = 8;
    int x,y;

    /*  Dibuja los numeros de las filas */
    attron ( COLOR_PAIR (COLOR_TEXTO_NORMAL) );
    y = TABLERO_ROW_START + 2;
    x = TABLERO_COL_START-2;
    while (n)
    {
        mvprintw (y, x, "%c", n+'0');
        y += 3;
        n--;
    }

    /* Dibuja letras de columnas */
    n = 8;
    x = TABLERO_COL_START + 8 * ANCHO_CASILLA - 2;
    y = TABLERO_ROW_START - 1;
    while (n)
    {
        mvprintw (y, x, "%c", n+'a'-1);
        x -= 3;
        n--;
    }
}
