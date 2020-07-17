#pragma once

/****************************************************************************************
 * inicializa
 *
 * Inicializa todos los elementos del juego: tablero, librerías de pantalla, etc.
 *
 ***************************************************************************************/
void inicializa();
/****************************************************************************************
 * liberaRecursos
 *
 * Libera los recursos usados por el programa, incluída la UI
 ***************************************************************************************/
void liberaRecursos();
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
 *  actualizaJuego
 *
 *  Actualiza estado de juego
 ***************************************************************************************/
void actualizaEstadoJuego (AJD_TableroPtr tablero);
/****************************************************************************************
 *  efectuaJugada
 *
 *  Efectua movimiento de piezas blancas / negras 
 ***************************************************************************************/
void efectuaJugada (AJD_TableroPtr tablero);

/****************************************************************************************
 * obtenEstadoPtr
 *
 * Obten el puntero al estado de juego
 ***************************************************************************************/
AJD_EstadoPtr obtenEstadoPtr ();
