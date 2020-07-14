#pragma once
/****************************************************************************************
 * inicializaTablero
 *
 * Inicializa los colores de las casillas del tablero. NO coloca ninguna pieza.
 ***************************************************************************************/
void inicializaTablero ();
/****************************************************************************************
 * obtenCasilla
 *
 * Obten la casilla del tablero a partir de su id (p.ej a3)
 ***************************************************************************************/
AJD_Casilla obtenCasilla (AJD_idCasilla id);
/****************************************************************************************
 * obtenTableroPtr
 *
 * Obten el puntero al tablero
 ***************************************************************************************/
AJD_TableroPtr obtenTableroPtr ();
/****************************************************************************************
 * tableroDisposicionInicial
 *
 * Coloca las piezas del ajedrez en su disposición de partida nueva
 ***************************************************************************************/
void tableroDisposicionInicial ();
