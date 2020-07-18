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
 * liberaPantalla
 *
 * Libera los recursos de ncurses.
 ***************************************************************************************/
void liberaPantalla();
/****************************************************************************************
 * inicializaSprites
 *
 * Establece los sprites necesarios para los cursores de selección y pieza seleccionada.
 * Por ahora estos 'sprites' estan formados de caracteres especiales de recuadro. Se
 * utilizan constantes ACS_ de la librería ncurses para conseguir mayor portabilidad.
 ****************************************************************************************/
void inicializaSprites(AJD_TableroPtr tablero);
/****************************************************************************************
 * dibujaJuego
 *
 * Dibuja todos los elementos del juego
 ***************************************************************************************/
 void dibujaJuego(AJD_TableroPtr tablero, AJD_EstadoPtr estadoJuego);
/****************************************************************************************
 * dibujaTablero
 *
 * Dibuja el tablero en pantalla según su estado actual.
 ***************************************************************************************/
void dibujaTablero(const AJD_TableroPtr tablero);
/****************************************************************************************
 * dibujaCursor
 *
 * Dibuja el cursor indicado
 ***************************************************************************************/
void dibujaCursor (const AJD_TableroPtr tablero, AJD_Cursor cursor);
/****************************************************************************************
 * dibujaMarcadores 
 *
 * Dibuja los marcadores de tiempo, turno, etc.
 ***************************************************************************************/
void dibujaMarcadores(const AJD_EstadoPtr estadoJuego);
/****************************************************************************************
 * dibujaPieza
 *
 * Dibuja la pieza indicada en las coordenadas y,x
 ***************************************************************************************/
void dibujaPieza (int y, int x, AJD_Pieza pieza);
/****************************************************************************************
 * dibujaFlags
 *
 * Muestra diferentes flags provocados por el movimiento realizado
 ***************************************************************************************/
void dibujaFlags (const AJD_EstadoPtr estadoJuego);
/****************************************************************************************
 * procesaTeclado
 *
 * Lectura del teclado. Devuelve la accion de juego a realizar correspondiente.
 ***************************************************************************************/
AJD_Accion procesaTeclado ();
