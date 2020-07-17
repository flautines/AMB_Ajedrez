#include <common.h>
#include <movimiento.h>

#include <stdio.h>
/****************************************************************************************
 * Variables PRIVADAS
 ***************************************************************************************/
AJD_Estado estadoJuego;
AJD_Estado estadoJuegoInicio = {
    {1,                                 /* Jugada 1         */
        { e2, e2, 0, 0, 0, 0 },         /* Peon rey Blancas */
        { e7, e7, 0, 0, 0, 0 } },       /* Peon rey Negras  */
    TRUE,                               /* Juegan Blancas   */
    0,                                  /* Tiempo Blancas   */
    0,                                  /* Tiempo Negras    */
    FALSE                               /* Fin de juego     */
};
/****************************************************************************************
 ****************************************************************************************
 * Funciones PUBLICAS (Implementacion)
 ****************************************************************************************
 ***************************************************************************************/
/****************************************************************************************
 * inicializa
 *
 * Inicializa todos los elementos del juego: tablero, librerías de pantalla, etc.
 *
 ***************************************************************************************/
void inicializa()
{
    inicializaTablero();
    inicializaPantalla();
}
/****************************************************************************************
 * liberaRecursos
 *
 * Libera los recursos usados por el programa, incluída la UI
 ***************************************************************************************/
void liberaRecursos()
{   
   liberaPantalla();
   puts ("ncurses finalizado");
}
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
void nuevoJuego()
{
    tableroDisposicionInicial();

    estadoJuego = estadoJuegoInicio;
}
/****************************************************************************************
 *  ejecutaPartida
 *
 * Bucle principal, se ejecuta hasta que termina la partida.
 * Devuelve TRUE si el juego ha terminado.
 ***************************************************************************************/
AJD_Bool ejecutaPartida()
{    

    AJD_TableroPtr ptablero = obtenTableroPtr();
    while (!estadoJuego.finJuego)
    {        
        dibujaJuego(ptablero, &estadoJuego);        

        procesaTeclado (ptablero, &estadoJuego);

        efectuaJugada (ptablero);

        dibujaFlags (&estadoJuego);

        actualizaEstadoJuego(ptablero);
    }
    
    return estadoJuego.finJuego;
}
/****************************************************************************************
 *  actualizaEstadoJuego
 *
 *  Actualiza estado de juego
 ***************************************************************************************/
void actualizaEstadoJuego (AJD_TableroPtr tablero)
{       
    estadoJuego.jueganBlancas ^= 1;
    estadoJuego.jugadorActual ^= 1;
}
/****************************************************************************************
 *  efectuaJugada
 *
 *  Efectua movimiento de piezas blancas o negras
 ***************************************************************************************/
void efectuaJugada (AJD_TableroPtr tablero)
{
    AJD_MovimientoPtr movimiento;

    if (estadoJuego.jueganBlancas)
    {
        movimiento = &estadoJuego.jugada.movBlancas;
    }
    else
    {
        movimiento = &estadoJuego.jugada.movNegras;
    }

    efectuaMovimiento (tablero, movimiento);   
}
/****************************************************************************************
 * obtenEstadoPtr
 *
 * Obten el puntero al estado de juego
 ***************************************************************************************/
AJD_EstadoPtr obtenEstadoPtr ()
{
    return &estadoJuego;
}

