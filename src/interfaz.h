#pragma once

/*#include <tablero.h>
#include <juego.h>
#include <menu.h>
#include <movimiento.h>

#include <ncurses.h>
*/
#define ANCHO_CASILLA 3
#define ALTO_CASILLA  3
/////////////////////////////////////////////////////////////////////////////////////////
// VARIABLES GLOBALES
extern AJD_Sprite sprCursorPiezaSeleccionada;
extern AJD_Sprite sprCursorMovil;
/////////////////////////////////////////////////////////////////////////////////////////
// inicializaPantalla
//
// Inicializa libreria ncurses y la interfaz de usuario en general.
//
void inicializaPantalla();
/////////////////////////////////////////////////////////////////////////////////////////
// inicializaSprites
//
// Establece los sprites necesarios para los cursores de selección y pieza seleccionada.
//
void inicializaSprites(AJD_TableroPtr tablero);
/////////////////////////////////////////////////////////////////////////////////////////
// liberaPantalla
//
// Libera los recursos de ncurses.
//
void liberaPantalla();
/////////////////////////////////////////////////////////////////////////////////////////
// dibujaJuego
//
// Dibuja todos los elementos del juego
//
void dibujaJuego (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego);
/////////////////////////////////////////////////////////////////////////////////////////
// dibujaTablero
//
// Dibuja el tablero en pantalla según su estado actual.
//
void dibujaTablero(AJD_TableroPtr tablero);
////////////////////////////////////////////////////////////////////////////
// dibujaPieza Dibuja una pieza del ajedrez en la posición indicada
//
void dibujaPieza (int posy, int posx, AJD_Pieza pieza, AJD_Color color);
/////////////////////////////////////////////////////////////////////////////////////////
// dibujaCasilla
//
// Dibuja una casilla del tablero con su pieza si la tuviera.
//
// ...###
// .p.###
// ...###
//
void dibujaCasilla(AJD_TableroPtr tablero, int ncasilla, int posx, int posy);
////////////////////////////////////////////////////////////////////////////
// dibujaMarcadores Dibuja los marcadores de tiempo, turno, etc.
//
void dibujaMarcadores(AJD_EstadoPtr estado);
////////////////////////////////////////////////////////////////////////////
// dibujaCursor Dibuja el cursor de seleccion
//
void dibujaCursor (AJD_Cursor cursor);
////////////////////////////////////////////////////////////////////////////
// dibujaMenu Dibuja un menu con su titulo y sus elementos.
//
void dibujaMenu (int y, int x, menu_t* menu);
////////////////////////////////////////////////////////////////////////////
// muestraMenu Muestra un menu y espera selección.
//             Devuelve true cuando se ha seleccionado una opción del menu
//
AJD_Bool muestraMenu (int x, int y, menu_t* menu);
////////////////////////////////////////////////////////////////////////////
// procesaTeclado Lectura del teclado y actualizacion de cursor
//             Devuelve celda origen y destino de la pieza a mover.
//
void procesaTeclado (AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego);

// Visualizaciones para depuracion 
void muestraMovInfo (AJD_MovInfoPtr movInfo);
