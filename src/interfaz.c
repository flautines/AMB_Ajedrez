#include <common.h>
#include <utils.h>
#include <ncurses.h>
#include <string.h>

#define TABLERO_ROW_START       9
#define TABLERO_COL_START       10
#define MARCADOR_ROW_START      TABLERO_ROW_START
#define MARCADOR_COL_START      TABLERO_COL_START + 8 * 3 + 2
#define MARCADOR_ROW_END        TABLERO_ROW_START + 8 * 3 - 1
#define MARCADOR_ROW_SEGUNDOS_B MARCADOR_ROW_START + 5
#define MARCADOR_ROW_SEGUNDOS_N MARCADOR_ROW_SEGUNDOS_B + 2
#define MARCADOR_JUGADOR_ACTUAL MARCADOR_ROW_END - 1

/****************************************************************************************
 * Variables privadas
 ***************************************************************************************/
/** sprites para los cursores
 ***************************************************************************************/
AJD_Sprite sprCurFijo;
AJD_Sprite sprCurMovil;

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
 * liberaPantalla
 *
 * Libera los recursos de ncurses.
 ***************************************************************************************/
void liberaPantalla()
{
    endwin();
}
/****************************************************************************************
 * inicializaSprites
 *
 * Establece los sprites necesarios para los cursores de selección y pieza seleccionada.
 * Por ahora estos 'sprites' estan formados de caracteres especiales de recuadro. Se
 * utilizan constantes ACS_ de la librería ncurses para conseguir mayor portabilidad.
 ****************************************************************************************/
void inicializaSprites(AJD_TableroPtr tablero)
{
    /* Definicion de los cursores 
       c89 es una mi%$$## y no permite lista de inicialización
       con los caracteres ACS_ ya que son expresiones no constantes */
    int i;
    for (i=0; i<NCHARS_IN_SPRITE; i++) 
        sprCurMovil.ch[i] = sprCurFijo.ch[i] = '.';

    sprCurMovil.ch[0] = ACS_ULCORNER;
    sprCurMovil.ch[2] = ACS_URCORNER;
    sprCurMovil.ch[6] = ACS_LLCORNER;
    sprCurMovil.ch[8] = ACS_LRCORNER;

    sprCurFijo.ch[0] = ACS_ULCORNER;
    sprCurFijo.ch[1] = ACS_HLINE;
    sprCurFijo.ch[2] = ACS_URCORNER;
    sprCurFijo.ch[3] = ACS_VLINE;
    sprCurFijo.ch[5] = ACS_VLINE;
    sprCurFijo.ch[6] = ACS_LLCORNER;
    sprCurFijo.ch[7] = ACS_HLINE;
    sprCurFijo.ch[8] = ACS_LRCORNER;

    tablero->curMovil.sprite = &sprCurMovil;
    tablero->curFijo.sprite = &sprCurFijo;
}
/****************************************************************************************
 * dibujaJuego
 *
 * Dibuja todos los elementos del juego
 ***************************************************************************************/
void dibujaJuego(AJD_TableroPtr tablero, AJD_EstadoPtr estadoJuego)
{
    dibujaTablero (tablero);
    dibujaMarcadores (estadoJuego);

    /* Dibuja el cursor movil (seleccion de pieza) */
    if (tablero->curMovil.visible)
    {
        dibujaCursor(tablero, tablero->curMovil);
    }
    /* Dibuja el cursor fijo (pieza seleccionada) */
    if (tablero->curFijo.visible)
    {
        dibujaCursor(tablero, tablero->curFijo);
    }
}
/****************************************************************************************
 * dibujaTablero
 *
 * Dibuja el tablero en pantalla según su estado actual.
 ***************************************************************************************/
void dibujaTablero(const AJD_TableroPtr tablero)
{
    AJD_idCasilla idCasilla = h1;

    /* Dibuja la primera casilla */
    dibujaCasilla (a8);
    /* Dibuja el resto de casillas */
    while (idCasilla)
        dibujaCasilla (idCasilla--);

    dibujaEncabezados ();   
}
/****************************************************************************************
 * dibujaCursor
 *
 * Dibuja el cursor indicado
 ***************************************************************************************/
void dibujaCursor (const AJD_TableroPtr tablero, AJD_Cursor cursor)
{
    int y, x, dx, dy;
    chtype* ch;
    AJD_Bool flash = FALSE;

    /* Las casillas son un array, por tanto están seguidas en memoria.
     * Podemos obtener el idCasilla del cursor con aritmética de punteros
     * (siempre que sean punteros del mismo array) 
     */
    AJD_CasillaPtr casillaCursor = cursor.casilla;
    AJD_idCasilla  idCur         = casillaCursor - tablero->casillas;
    
    y = TABLERO_ROW_START + (idCur /8) * ALTO_CASILLA+1;
    x = TABLERO_COL_START + (idCur &7) * ANCHO_CASILLA;

    flash = cursor.flash;

    if (flash)
        attron (A_BLINK);
    else
        attroff (A_BLINK);

    if (casillaCursor->color == BLANCO)
        attron (A_REVERSE);
    else
        attroff (A_REVERSE);

    ch = cursor.sprite->ch;
    mvaddch (2,35, *ch);

    for (dy=0; dy<ALTO_CASILLA; dy++)
    {    
        for (dx=0; dx<ANCHO_CASILLA; dx++)
        {
            if (*ch != '.')
                mvaddch (y + dy, x + dx, *ch);
            ch++;
        }
        
    }
    attroff (A_BLINK);
    attroff (A_REVERSE);
    
}
/****************************************************************************************
 * dibujaMarcadores 
 *
 * Dibuja los marcadores de tiempo, turno, etc.
 ***************************************************************************************/
void dibujaMarcadores(const AJD_EstadoPtr estadoJuego)
{
    char buff[6];
    int x,y;

    x = MARCADOR_COL_START;
    y = MARCADOR_ROW_START + 1;
    attron (COLOR_PAIR (COLOR_TEXTO_NORMAL));
    mvprintw (y,x, "AJEDREZ 1.0");

    y += 1;
    mvprintw (y,x, "(c) Jul 2020 Andres Mata");

    y = MARCADOR_ROW_SEGUNDOS_B;
    mvprintw (y,x, "Blancas %s", strSegundos (buff, estadoJuego->segundosBlancas));

    y = MARCADOR_ROW_SEGUNDOS_N;
    mvprintw (y,x, "Negras %s", strSegundos (buff, estadoJuego->segundosNegras));
/*
    y += 2;
    if (turno_actual->enroque)
    {
        mvprintw (y,x, turno_actual->enroque == ENROQUE_LARGO 
                        ? "O-O-O" : "O-O");
    }   
*/
    y = MARCADOR_JUGADOR_ACTUAL;
    mvprintw (y,x, "Turno %02d (Jugador %c)", 
                    estadoJuego->jugada.nturno, estadoJuego->jugadorActual+'1');

    y += 1;
    mvprintw (y,x, "Juegan %s", estadoJuego->jueganBlancas ? "blancas" : "negras ");
/*
    // Debug info
    mvprintw (MARCADOR_LAST_ROW+2, 0, "O-O-O (BLANCAS) %s", 
              estadoJuego->enroque_permitido &ERLARGO_PROHIBIDO_B 
              ? "PROH" : "PERM");

    mvprintw (MARCADOR_LAST_ROW+3, 0, "O-O (BLANCAS) %s", 
              estadoJuego->enroque_permitido &ERCORTO_PROHIBIDO_B 
              ? "PROH" : "PERM");

    mvprintw (MARCADOR_LAST_ROW+4, 0, "O-O-O (NEGRAS) %s",
              estadoJuego->enroque_permitido &ERLARGO_PROHIBIDO_N 
              ? "PROH" : "PERM");

    mvprintw (MARCADOR_LAST_ROW+5, 0, "O-O (NEGRAS) %s", 
              estadoJuego->enroque_permitido &ERCORTO_PROHIBIDO_N
              ? "PROH" : "PERM");
*/
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
 * dibujaFlags
 *
 * Muestra diferentes flags provocados por el movimiento realizado
 ***************************************************************************************/
void dibujaFlags (const AJD_EstadoPtr estadoJuego)
{
    AJD_Movimiento movCausante = estadoJuego->jueganBlancas ? 
                                    estadoJuego->jugada.movBlancas :
                                    estadoJuego->jugada.movNegras;
    mvprintw (0,0, 
        "captura:%d   enroque:%d   jaque:%d   mate:%d",
        movCausante.captura,
        movCausante.enroque,
        movCausante.jaque,
        movCausante.mate);
}
/****************************************************************************************
 * procesaTeclado
 *
 * Lectura del teclado. Devuelve la accion de juego a realizar correspondiente.
 ***************************************************************************************/
AJD_Accion procesaTeclado ()
{
    int ch;

    ch = getch();

    switch (ch)
    {
        /* [ESC] salir  */
        case '\033'     : return SALIR;

        /* [ENTER] selecciona casilla debajo del cursor */
        case '\n'       : return CUR_SELEC;

        /* Teclas para mover el cursor de seleccion */
        case KEY_UP     : return CUR_ARR;
        case KEY_DOWN   : return CUR_ABJ;
        case KEY_LEFT   : return CUR_IZQ;
        case KEY_RIGHT  : return CUR_DER;

        default         : return NO_ACCION;
    }
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
    AJD_Casilla casilla = *(idToPtr (id));

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

