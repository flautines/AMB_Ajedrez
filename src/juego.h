#pragma once

/****************************************************************************************
 * inicializa
 *
 * Inicializa todos los elementos del juego: tablero, librerías de pantalla, etc.
 *
 ***************************************************************************************/
void inicializa();
/****************************************************************************************
 * nuevoJuego
 *
 * Prepara el estado del juego y el tablero para una partida nueva:
 *
 *    - Estado del juego: ninguna pieza movida, ningún rey en jaque,
 *      turno del jugador 1, juegan blancas, contadores a cero
 *
 *    - Coloca las piezas en el tablero para una partida nueva
 *
 *    - Comienza el turno 1
 *
 ***************************************************************************************/
void nuevoJuego();
/****************************************************************************************
 *  ejecutaPartida
 *
 * Bucle principal, se ejecuta hasta que termina la partida.
 * Devuelve TRUE si el juego ha terminado.
 ***************************************************************************************/
AJD_Bool ejecutaPartida();
/****************************************************************************************
 * obtenEstadoPtr
 *
 * Obten el puntero al estado de juego
 ***************************************************************************************/
/*AJD_EstadoPtr obtenEstadoPtr ();*/
