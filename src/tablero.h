#pragma once
/****************************************************************************************
 * inicializaTablero
 *
 * Inicializa los colores de las casillas del tablero. NO coloca ninguna pieza.
 ***************************************************************************************/
void inicializaTablero ();
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
/****************************************************************************************
 * muevePieza
 *
 * Mueve la pieza que hay en la casilla origen a la casilla destino
 ***************************************************************************************/
void muevePieza (AJD_idCasilla origen, AJD_idCasilla destino);
/****************************************************************************************
 * actualizaCursor
 *
 * Actualiza el cursor segun la accion indicada
 ***************************************************************************************/
void actualizaCursor (AJD_Accion accion);
/****************************************************************************************
 * obtenSeleccion
 *
 * Obtiene la casilla seleccionada (si es que se ha hecho una accion de seleccion)
 ***************************************************************************************/
AJD_Seleccion obtenSeleccion (AJD_EstadoPtr estadoJuego, AJD_Accion accion);
/****************************************************************************************
 * obtenPieza
 *
 * Obtiene la pieza que hay en la casilla (ptr) indicada o NO_PIEZA si está vacía
 ***************************************************************************************/
AJD_Pieza obtenPieza (AJD_CasillaPtr pcasilla);
/****************************************************************************************
 * idToPtr
 *
 * Obten puntero de una casilla del tablero a partir de su id (p.ej a3)
 ***************************************************************************************/
AJD_CasillaPtr idToPtr (AJD_idCasilla id);
/****************************************************************************************
 * ptrToId
 *
 * Obtiene el idCasilla correspondiente a una casilla.
 ***************************************************************************************/
AJD_idCasilla ptrToId (AJD_CasillaPtr casilla);
/****************************************************************************************
 * ptrToStr
 *
 * Obtiene el nombre de una casilla en formato texto (p.ej: "a8")
 ***************************************************************************************/
char *ptrToStr (AJD_CasillaPtr casilla);
/****************************************************************************************
 * hayPiezaJugador
 *
 * Devuelve TRUE si en la casilla indicada hay una pieza del jugador, FALSE en caso
 * contrario (no hay pieza o es del color del oponente)
 ***************************************************************************************/
AJD_Bool hayPiezaJugador (AJD_Bool jueganBlancas, AJD_CasillaPtr pcasilla);
/****************************************************************************************
 * hayPiezaOponente
 *
 * Devuelve TRUE si en la casilla indicada hay una pieza del jugador contrario, 
 * FALSE en caso contrario (no hay pieza o es del mismo color)
 ***************************************************************************************/
AJD_Bool hayPiezaOponente(AJD_Bool jueganBlancas, AJD_CasillaPtr pcasilla);
/****************************************************************************************
 * casillaVacia
 *
 * Devuelve TRUE si la casilla está vacía, FALSE en caso contrario
 ***************************************************************************************/
AJD_Bool casillaVacia (AJD_CasillaPtr pcasilla);
/****************************************************************************************
 * caminoLibre
 *
 * Devuelve TRUE si no hay ninguna pieza entre las casillas origen y destino indicadas, 
 *          FALSE en caso contrario.
 ***************************************************************************************/
AJD_Bool caminoLibre (AJD_CasillaPtr origen, AJD_CasillaPtr destino, int dx, int dy);
