#pragma once

#define ANCHO_CASILLA 3
#define ALTO_CASILLA  3
/****************************************************************************************
 * enum para las parejas de colores ncurses                                            
 ***************************************************************************************/
enum { BLACK_WHITE = 1, WHITE_BLACK, YELLOW_BLACK };
/****************************************************************************************
 * aliases para diferentes colores usados en el juego
 ***************************************************************************************/
#define COLOR_CASILLA_BLANCA 	BLACK_WHITE
#define COLOR_CASILLA_NEGRA  	WHITE_BLACK
#define COLOR_MENU_NO_SEL    	YELLOW_BLACK
#define COLOR_MENU_SEL       	BLACK_WHITE
#define COLOR_TEXTO_NORMAL	 	WHITE_BLACK
#define COLOR_TEXTO_RESALTADO	BLACK_WHITE
/****************************************************************************************
 * VARIABLES GLOBALES
 ***************************************************************************************/
/*
extern AJD_Sprite sprCursorPiezaSeleccionada;
extern AJD_Sprite sprCursorMovil;
*/
/****************************************************************************************
 * inicializaPantalla
 *
 * Inicializa libreria ncurses y la interfaz de usuario en general.
 ***************************************************************************************/
void inicializaPantalla();
/****************************************************************************************
 * dibujaJuego
 *
 * Dibuja todos los elementos del juego
 ***************************************************************************************/
 void dibujaJuego(AJD_TableroPtr tablero, AJD_EstadoPtr estado_juego);
/****************************************************************************************
 * dibujaTablero
 *
 * Dibuja el tablero en pantalla según su estado actual.
 ***************************************************************************************/
void dibujaTablero(AJD_TableroPtr tablero);
/****************************************************************************************
 * dibujaMarcadores 
 *
 * Dibuja los marcadores de tiempo, turno, etc.
 ***************************************************************************************/
void dibujaMarcadores(AJD_EstadoPtr estado_juego);
/****************************************************************************************
 * dibujaPieza
 *
 * Dibuja la pieza indicada en las coordenadas y,x
 ***************************************************************************************/
void dibujaPieza (int y, int x, AJD_Pieza pieza);
