#include <common.h>
#include <utils.h>

#include <string.h>

#define TABLERO_ROW_START   10
#define TABLERO_COL_START   10
#define MARCADOR_ROW_START  TABLERO_ROW_START
#define MARCADOR_COL_START  TABLERO_COL_START + 8 * 3 + 2
#define MARCADOR_LAST_ROW   TABLERO_ROW_START + 8 * 3 - 1
/////////////////////////////////////////////////////////////////////////////////////////
// VARIABLES GLOBALES
AJD_Sprite sprCursorPiezaSeleccionada;
AJD_Sprite sprCursorMovil;
/////////////////////////////////////////////////////////////////////////////////////////
// FUNCIONES PRIVADAS (forward declarations)
void actualizaTiempoGUI (AJD_Color juegan_blancas, int itime);
/////////////////////////////////////////////////////////////////////////////////////////
// INTERFAZ PÚBLICA (Implementación)
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// inicializaPantalla
//
// Inicializa libreria ncurses y la interfaz de usuario en general.
//
void inicializaPantalla()
{
    // Inicializa ncurses
    initscr();

    // Habilita teclas especiales (F1, cursores, ESC, etc.)
    keypad (stdscr, 1);

    // Envío de teclas inmediato (sin pulsar ENTER)
    cbreak();

    // No mostrar caracteres pulsados
    noecho();

    // No espera en lectura de caracter
    nodelay(stdscr, 1);

    // Ocultar cursor de pantalla
    curs_set(0);

    // Inicializa colores 
    if ( has_colors() )
    {
        start_color();

        // Negro sobre fondo blanco para dibujar casilla blanca
        init_pair(1, COLOR_BLACK, COLOR_WHITE);

        // Blanco sobre negro para dibujar casilla negra y textos
        init_pair(2, COLOR_WHITE, COLOR_BLACK);

        // Amarillo sobre negro para los menus
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);

        attron(COLOR_PAIR(1));
    }

    clear();
}
/////////////////////////////////////////////////////////////////////////////////////////
// inicializaSprites
//
// Establece los sprites necesarios para los cursores de selección y pieza seleccionada.
// Por ahora estos 'sprites' estan formados de caracteres especiales de recuadro. Se
// utilizan constantes ACS_ de la librería ncurses para conseguir mayor portabilidad.
//
void inicializaSprites(AJD_TableroPtr tablero)
{
    // Definicion de los cursores
    chtype charsCursorMovil[] = {
        ACS_ULCORNER, '.', ACS_URCORNER, 
        '.','.','.', 
        ACS_LLCORNER, '.', ACS_LRCORNER,
    };
    chtype charsCursorPiezaSeleccionada[] = {
        ACS_ULCORNER, ACS_HLINE  , ACS_URCORNER ,
        ACS_VLINE   , '.'        , ACS_VLINE    ,
        ACS_LLCORNER, ACS_HLINE  , ACS_LRCORNER 
    };
    
    memcpy (sprCursorMovil.ch, charsCursorMovil, 
            NCHARS_IN_SPRITE*sizeof(chtype));

    memcpy (sprCursorPiezaSeleccionada.ch, charsCursorPiezaSeleccionada, 
            NCHARS_IN_SPRITE*sizeof(chtype));

    tablero->cursorMovil.sprite = &sprCursorMovil;
    tablero->cursorPiezaSeleccionada.sprite = &sprCursorPiezaSeleccionada;
}
/////////////////////////////////////////////////////////////////////////////////////////
// liberaPantalla
//
// Libera los recursos de ncurses.
//
void liberaPantalla()
{
    endwin();
}
/////////////////////////////////////////////////////////////////////////////////////////
// dibujaJuego
//
// Dibuja todos los elementos del juego
//
void dibujaJuego(AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    dibujaTablero (tablero);
    dibujaMarcadores (estado_juego);
}
/////////////////////////////////////////////////////////////////////////////////////////
// dibujaTablero
//
// Dibuja el tablero en pantalla según su estado actual.
//
void dibujaTablero(AJD_TableroPtr tablero)
{
    int row, col, y, x;

    int idCasilla = 0;
    for (row=0; row < 8; row++)
    for (col=0; col < 8; col++)
    {
        dibujaCasilla (tablero, idCasilla, 
            TABLERO_COL_START + col * ANCHO_CASILLA,
            TABLERO_ROW_START + row * ALTO_CASILLA);

        // Siguiente casilla
        idCasilla++;
    }

    // Dibuja los numeros de las filas
    attron ( COLOR_PAIR (2) );
    row = 8;
    y = TABLERO_ROW_START + 1;
    x = TABLERO_COL_START-2;
    while (row)
    {
        mvprintw (y, x, "%d", row);
        y += 3;
        row--;
    }

    // Dibuja letras de columnas
    col = 8;
    x = TABLERO_COL_START + 8 * 3 - 2;
    y = TABLERO_ROW_START - 2;
    while (col)
    {
        mvprintw (y, x, "%c", col+'a'-1);
        x -= 3;
        col--;
    }

    // Dibuja el cursor de seleccion
    if (tablero->cursorMovil.visible)
    {
        dibujaCursor(tablero->cursorMovil);
    }
    if (tablero->cursorPiezaSeleccionada.visible)
    {
        dibujaCursor(tablero->cursorPiezaSeleccionada);
    }

}
/////////////////////////////////////////////////////////////////////////////////////////
// dibujaCasilla
//
// Dibuja una casilla del tablero con su pieza si la tuviera.
//
// ...###
// .p.###
// ...###
//
void dibujaCasilla(AJD_TableroPtr tablero, int ncasilla, int posx, int posy)
{
    // columna y fila correspondientes a esta casilla en el tablero
    int nrow = ncasilla / 8;
    int ncol = ncasilla & 7;

    // Puntero a la casilla a dibujar
    AJD_CasillaPtr casilla = &(tablero->casilla[nrow * 8 + ncol]);

    // Color principal/fondo a usar dependiendo de la casilla
    int color = casilla->color == BLANCO ? 1 : 2;   
    attron(COLOR_PAIR (color));

    for (nrow = 0; nrow < ALTO_CASILLA; nrow++)
    {       
        move (posy+nrow, posx);
        printw("   ");  // 3 espacios

        // Dibuja pieza si es que hay alguna en la casilla actual
        if (casilla->pieza)
        {           
            dibujaPieza (posy+1, posx+1, casilla->pieza, casilla->color_pieza);
        }

        attron(COLOR_PAIR (color));
    }
}
////////////////////////////////////////////////////////////////////////////
// dibujaPieza Dibuja una pieza del ajedrez en la posición indicada
//
void dibujaPieza (int posy, int posx, AJD_Pieza pieza, AJD_Color color)
{
    // Array de caracteres representado la letra a dibujar de cada pieza
    char *sprPiezas=" PTCADR";
    char sprite = sprPiezas[pieza];

    // Piezas negras las mostraremos en minusculas
    if (color == NEGRO) sprite += 32;
    if (color == BLANCO) color = 1;

    // Dibuja la letra de la pieza en color blanco sobre fondo negro
    attron (COLOR_PAIR (1));
    mvprintw (posy, posx, "%c", sprite);
    attroff (COLOR_PAIR (1));
}
////////////////////////////////////////////////////////////////////////////
// dibujaMarcadores Dibuja los marcadores de tiempo, turno, etc.
//
void dibujaMarcadores(AJD_EstadoPtr estado_juego)
{
    char buff[6];
    int x,y;
    AJD_TurnoPtr turno_actual = &(estado_juego->turno_actual);

    x = MARCADOR_COL_START;
    y = MARCADOR_ROW_START + 1;
    attron (COLOR_PAIR (2));
    mvprintw (y,x, "AJEDREZ 1.0");

    y += 1;
    mvprintw (y,x, "(c) Jun 2020 Andres Mata");

    y += 4;
    mvprintw (y,x, "Blancas %s", strSegundos (buff, estado_juego->segundos_blancas));

    y += 2;
    mvprintw (y,x, "Negras %s", strSegundos (buff, estado_juego->segundos_negras));

    y += 2;
    if (turno_actual->enroque)
    {
        mvprintw (y,x, turno_actual->enroque == ENROQUE_LARGO 
                        ? "O-O-O" : "O-O");
    }   

    y = MARCADOR_LAST_ROW - 1;
    mvprintw (y,x, "Turno %02d (Jugador %c)", 
                    estado_juego->nturno, turno_actual->njugador+'1');

    y += 1;
    mvprintw (y,x, "Juegan %s", turno_actual->juegan_blancas ? "blancas" : "negras ");

    // Debug info
    mvprintw (MARCADOR_LAST_ROW+2, 0, "O-O-O (BLANCAS) %s", 
              estado_juego->enroque_permitido &ERLARGO_PROHIBIDO_B 
              ? "PROH" : "PERM");

    mvprintw (MARCADOR_LAST_ROW+3, 0, "O-O (BLANCAS) %s", 
              estado_juego->enroque_permitido &ERCORTO_PROHIBIDO_B 
              ? "PROH" : "PERM");

    mvprintw (MARCADOR_LAST_ROW+4, 0, "O-O-O (NEGRAS) %s",
              estado_juego->enroque_permitido &ERLARGO_PROHIBIDO_N 
              ? "PROH" : "PERM");

    mvprintw (MARCADOR_LAST_ROW+5, 0, "O-O (NEGRAS) %s", 
              estado_juego->enroque_permitido &ERCORTO_PROHIBIDO_N
              ? "PROH" : "PERM");


}
////////////////////////////////////////////////////////////////////////////
// dibujaCursor Dibuja el cursor de seleccion
//
void dibujaCursor (AJD_Cursor cursor)
{
    int y, x;
    AJD_Bool flash = FALSE;
    AJD_CasillaPtr casillaCursor = cursor.casilla;
    
    y = TABLERO_ROW_START + (casillaCursor->id/8) * ALTO_CASILLA;
    x = TABLERO_COL_START + (casillaCursor->id & 7) * ANCHO_CASILLA;

    flash = cursor.flash;

    if (flash)
        attron (A_BLINK);
    else
        attroff (A_BLINK);

    if (casillaCursor->color == BLANCO)
        attron (A_REVERSE);
    else
        attroff (A_REVERSE);

    chtype* ch = &(cursor.sprite->ch[0]);

    for (int dy=0; dy<ALTO_CASILLA; dy++)
    {    
        for (int dx=0; dx<ANCHO_CASILLA; dx++)
        {
            if (*ch != '.')
                mvaddch (y + dy, x + dx, *ch);
            ch++;
        }
        
    }
    attroff (A_BLINK);
    attroff (A_REVERSE);
}
////////////////////////////////////////////////////////////////////////////
// dibujaMenu Dibuja un menu con su titulo y sus elementos.
//
void dibujaMenu(int y, int x, menu_t* menu)
{
    int nitems = menu->nitems;
    int item_id = 1;

    // Menu Title
    attron ( COLOR_PAIR (3) );  
    mvprintw (y, x, menu->items[0].menuString);

    while (nitems--)
    {
        y += 2;
        menuItem_t* menu_item = &(menu->items[item_id]);        
        attron (COLOR_PAIR (2) );

        // Resaltar la opcion actual del menu
        if (item_id == menu->selected)
            attron (COLOR_PAIR (1) );

        mvprintw (y, x, menu_item->menuString);
        item_id++;
    }

    y += 3;
    attron (COLOR_PAIR (2));
    mvprintw (y,x, "Cursores selecciona opcion, ENTER confirmar");
}
////////////////////////////////////////////////////////////////////////////
// muestraMenu Muestra un menu y espera selección.
//             Devuelve true cuando se ha seleccionado una opción del menu
//
AJD_Bool muestraMenu (int x, int y, menu_t* menu)
{   
    int ch;

    clear();
    do
    {
        dibujaMenu (x, y, menu);
        ch = getch();
        if (ch == KEY_UP)   menu->selected--;
        if (ch == KEY_DOWN) menu->selected++;

        if (menu->selected < 1) menu->selected = menu->nitems;
        if (menu->selected > menu->nitems) menu->selected = 1;
    } while (ch != '\n');

    clear();
    return true;
}
////////////////////////////////////////////////////////////////////////////
// procesaTeclado Lectura del teclado y actualizacion de cursor
//             Devuelve celda origen y destino de la pieza a mover.
//
void procesaTeclado (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego)
{
    int ch;    
    AJD_Cursor*   cursorMovil;
    AJD_idCasilla idCasilla;
    AJD_TurnoPtr  turno_actual;

    cursorMovil  = &tablero->cursorMovil;
    idCasilla    = cursorMovil->casilla->id;
    turno_actual = &(estado_juego->turno_actual);
    
 
    ch = getch();
    switch (ch)
    {
        case KEY_UP:    
            idCasilla -= 8;
            // Aseguramos que el cursor movil se mantiene en los límites del tablero
            idCasilla &= 63;
            cursorMovil->casilla = &tablero->casilla[idCasilla];
            break;

        case KEY_DOWN:  
            idCasilla += 8;
            // Aseguramos que el cursor movil se mantiene en los límites del tablero
            idCasilla &= 63;
            cursorMovil->casilla = &tablero->casilla[idCasilla];
            break;

        case KEY_LEFT:  
            idCasilla -= 1;
            // Aseguramos que el cursor movil se mantiene en los límites del tablero
            idCasilla &= 63;
            cursorMovil->casilla = &tablero->casilla[idCasilla];
            break;

        case KEY_RIGHT: 
            idCasilla += 1;
            // Aseguramos que el cursor movil se mantiene en los límites del tablero
            idCasilla &= 63;            
            cursorMovil->casilla = &tablero->casilla[idCasilla];
            break;

        case '\n':
            // Si no hay ninguna seleccion previa, la selección actual corresponde al origen
            if (estado_juego->casilla_seleccionada == NONE)
            {                
                estado_juego->casilla_seleccionada = ORIGEN_SELECCIONADO;
                
                // Rellenar la información de casilla_origen con los datos de la
                // casilla seleccionada.
                memcpy (&(turno_actual->origen), 
                        cursorMovil->casilla, 
                        sizeof (AJD_Casilla));

                tablero->cursorPiezaSeleccionada.casilla = cursorMovil->casilla;
            }
            // Si había seleccion previa, la selección actual corresponde al destino
            else
            {
                // Rellenar la información de casilla_destino con los datos de la
                // casilla seleccionada.
                memcpy (&(turno_actual->destino), 
                        cursorMovil->casilla, 
                        sizeof (AJD_Casilla));

                // Si se selecciona como casilla destino la misma casilla
                // origen se cancela la selección.
                if (turno_actual->destino.id == turno_actual->origen.id)
                {
                    estado_juego->casilla_seleccionada = NO_SELECCION;
                    tablero->cursorPiezaSeleccionada.visible = 0;
                }
                // Si no, hemos acabado con la seleccion de origen y destino
                else
                    estado_juego->casilla_seleccionada = DESTINO_SELECCIONADO;                
            }
            break;

        case '\033':  // ESC
            estado_juego->fin_juego = TRUE;    // Salir del juego
    }    
    
    mvprintw (0,0, "cursorMovil.id: %2d", cursorMovil->casilla->id);
    mvprintw (0,25, "Pieza de color: %s", cursorMovil->casilla->color_pieza
                                            ? "BLANCO" : "NEGRO ");
    mvprintw (0,50, "Casilla de color: %s", cursorMovil->casilla->color
                                            ? "BLANCO" : "NEGRO ");
    mvprintw (1,0, "casilla_seleccionada: %d", estado_juego->casilla_seleccionada);
}
////////////////////////////////////////////////////////////////////////////
// FUNCIONES VISUALIZACION PARA DEPURACION
////////////////////////////////////////////////////////////////////////////
// muestraMovInfo
//
void muestraMovInfo (AJD_MovInfo* movInfo)
{
    dibujaPieza (1, 25, movInfo->origen->pieza, movInfo->origen->color_pieza);
    mvprintw (1, 27, "src:%2d dst:%2d dy:%2d dx:%2d distY:%2d distX:%2d",        
        movInfo->origen->id,
        movInfo->destino->id,
        movInfo->dy,
        movInfo->dx,
        movInfo->distY,
        movInfo->distX);
}
/*
void actualizaTiempoGUI (AJD_Color juegan_blancas, int itime)
{
    if (juegan_blancas)
        mvprintw (MARCADOR_ROW_START+5, MARCADOR_COL_START, 
            "Blancas 00:%d", itime);
    else
        mvprintw (MARCADOR_ROW_START+7, MARCADOR_COL_START,
            "Negras 00:%d", itime);
}
*/
