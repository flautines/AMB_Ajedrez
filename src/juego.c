#include <stdint.h>
#include <ajd_types.h>
#include <tablero.h>
#include <interfaz.h>
#include <juego.h>
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
