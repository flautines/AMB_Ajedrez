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
