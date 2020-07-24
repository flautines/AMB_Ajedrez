#include <common.h>
#include <utils.h>
#include <stdio.h>
/****************************************************************************************
* Variables PRIVADAS
 ***************************************************************************************/
AJD_Tablero tablero;
/* String con los nombres de cada casilla */
char *strCasillas[] = {
   "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
   "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
   "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
   "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
   "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
   "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
   "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
   "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};
/****************************************************************************************
 * inicializaTablero
 *
 * Inicializa los colores de las casillas del tablero. NO coloca ninguna pieza.
 ***************************************************************************************/
void inicializaTablero ()
{
	int ncasillas = 64;
	int ncolumnas = 0;
	AJD_CasillaPtr pcasilla = tablero.casillas;
	AJD_Color color_casilla = BLANCO;

	while (ncasillas--)
	{
        /* 
        Cada casilla de una misma fila alterna de color,
        excepto la primera que mantiene el color de la fila anterior 
        */
		if (ncolumnas&7)
			color_casilla ^= 1;
		pcasilla->color = color_casilla;
		ncolumnas++;
		pcasilla++;
	}
}
/****************************************************************************************
 * obtenTableroPtr
 *
 * Obten el puntero al tablero
 ***************************************************************************************/
AJD_TableroPtr obtenTableroPtr ()
{
	return &tablero;
}
/****************************************************************************************
 * tableroDisposicionInicial
 *
 * Coloca las piezas del ajedrez en su disposición de partida nueva
 ***************************************************************************************/
void tableroDisposicionInicial ()
{
	int col;
    AJD_Pieza piezasMayoresB[8] = { TORRE_B, CABALLO_B, ALFIL_B, DAMA_B, REY_B, ALFIL_B, CABALLO_B, TORRE_B };
    AJD_Pieza piezasMayoresN[8] = { TORRE_N, CABALLO_N, ALFIL_N, DAMA_N, REY_N, ALFIL_N, CABALLO_N, TORRE_N };
    for (col=0; col < 8; col++)
   { 
        tablero.casillas[a8 + col].pieza = piezasMayoresN[col];  /* piezas mayores negras  */
        tablero.casillas[a1 + col].pieza = piezasMayoresB[col];  /* piezas mayores blancas */

        tablero.casillas[a7 + col].pieza   = PEON_N;             /* peones negros      */
        tablero.casillas[a2 + col].pieza = PEON_B;               /* peones blancos     */
   }
}
/****************************************************************************************
 * muevePieza
 *
 * Mueve la pieza que hay en la casilla origen a la casilla destino
 ***************************************************************************************/
void muevePieza (AJD_idCasilla origen, AJD_idCasilla destino)
{
    tablero.casillas[destino].pieza = tablero.casillas[origen].pieza;
    tablero.casillas[origen].pieza  = NO_PIEZA;
}
/****************************************************************************************
 * actualizaCursor
 *
 * Actualiza el cursor segun la accion indicada
 ***************************************************************************************/
void actualizaCursor (AJD_Accion accion)
{
    AJD_CursorPtr pcurMovil = &tablero.curMovil;
    AJD_CasillaPtr casillasBegin = tablero.casillas;
    AJD_CasillaPtr casillasEnd = &tablero.casillas[63];
    long offset = casillasEnd - casillasBegin +1;

    switch (accion)
    {
        case CUR_ARR: pcurMovil->casilla -= 8; break;
        case CUR_ABJ: pcurMovil->casilla += 8; break;
        case CUR_IZQ: pcurMovil->casilla -= 1; break;
        case CUR_DER: pcurMovil->casilla += 1; break;

        default: break;
    }
    
    /* Impedir que el cursor apunte a una casilla fuera del array de casillas */
    if (pcurMovil->casilla < casillasBegin)
        pcurMovil->casilla += offset;

    if (pcurMovil->casilla > casillasEnd)
        pcurMovil->casilla -= offset;
}
/****************************************************************************************
 * obtenSeleccion
 *
 * Obtiene la casilla seleccionada (si es que se ha hecho una accion de seleccion)
 ***************************************************************************************/
AJD_Seleccion obtenSeleccion (AJD_EstadoPtr estadoJuego, AJD_Accion accion)
{
    AJD_Color colorPieza = estadoJuego->jueganBlancas ? BLANCO : NEGRO;
    /* 
     * Si no hay ninguna seleccion previa, la selección actual corresponde al origen,
     * Si había una selección anterior, la selección actual corresponde al destino.
     * Si se selecciona como destino la misma casilla de origen, se cancela la selección
     */
    if (accion == CUR_SELEC)
    {
        if (estadoJuego->casillaSeleccionada == NO_SELECCION)
        {
            if (hayPiezaJugador(colorPieza, tablero.curMovil.casilla))
            {
                estadoJuego->casillaSeleccionada = ORIGEN_SELECCIONADO;
                /* Con el cursor fijo damos indicación visual de la casilla origen */
                tablero.curFijo.casilla = tablero.curMovil.casilla;
                tablero.curFijo.visible = TRUE;
            }
        }
        else
        {
            if (tablero.curFijo.casilla != tablero.curMovil.casilla)
            {
                estadoJuego->casillaSeleccionada = DESTINO_SELECCIONADO;
            }
            else
            {
                estadoJuego->casillaSeleccionada = NO_SELECCION;
                tablero.curFijo.visible = FALSE;
            }            
        }
    }

    /* Si se ha seleccionado destino, rellena los datos del movimiento */
    if (estadoJuego->casillaSeleccionada == DESTINO_SELECCIONADO)
    {
        AJD_MovimientoPtr pmov;
        pmov = estadoJuego->jueganBlancas ? 
                &estadoJuego->jugada.movBlancas : &estadoJuego->jugada.movNegras;

        pmov->idOrigen = ptrToId (tablero.curFijo.casilla);
        pmov->idDestino = ptrToId (tablero.curMovil.casilla);
    }

    return estadoJuego->casillaSeleccionada;
}
/****************************************************************************************
 * obtenPieza
 *
 * Obtiene la pieza que hay en la casilla (ptr) indicada o NO_PIEZA si está vacía
 ***************************************************************************************/
AJD_Pieza obtenPieza (AJD_CasillaPtr pcasilla)
{
    return pcasilla->pieza;
}
/****************************************************************************************
 * idToPtr
 *
 * Obten puntero de una casilla del tablero a partir de su id (p.ej a3)
 ***************************************************************************************/
AJD_CasillaPtr idToPtr (AJD_idCasilla id)
{
    return &tablero.casillas[id];
}
/****************************************************************************************
 * ptrToId
 *
 * Obtiene el idCasilla correspondiente a una casilla.
 ***************************************************************************************/
AJD_idCasilla ptrToId (AJD_CasillaPtr casilla)
{
    AJD_CasillaPtr casillasBegin = tablero.casillas;

    return (casilla - casillasBegin);
}
/****************************************************************************************
 * ptrToStr
 *
 * Obtiene el nombre de una casilla en formato texto (p.ej: "a8")
 ***************************************************************************************/
char *ptrToStr (AJD_CasillaPtr casilla)
{
    AJD_idCasilla idCasilla = ptrToId (casilla);
    return strCasillas[idCasilla];
}
/****************************************************************************************
 * hayPiezaJugador
 *
 * Devuelve TRUE si en la casilla indicada hay una pieza del jugador, FALSE en caso
 * contrario (no hay pieza o es del color del oponente)
 ***************************************************************************************/
AJD_Bool hayPiezaJugador(AJD_Color colorPieza, AJD_CasillaPtr pcasilla)
{
    AJD_Pieza minValido = colorPieza ? PEON_B : PEON_N;
    AJD_Pieza maxValido = colorPieza ? REY_B  : REY_N;
    
    return (pcasilla->pieza >= minValido && pcasilla->pieza <= maxValido);
}
/****************************************************************************************
 * hayPiezaOponente
 *
 * Devuelve TRUE si en la casilla indicada hay una pieza del jugador contrario, 
 * FALSE en caso contrario (no hay pieza o es del mismo color)
 ***************************************************************************************/
AJD_Bool hayPiezaOponente(AJD_Color colorPieza, AJD_CasillaPtr pcasilla)
{
    AJD_Pieza minValido = colorPieza ? PEON_N : PEON_B;
    AJD_Pieza maxValido = colorPieza ? REY_N  : REY_B;
    
    return (pcasilla->pieza >= minValido && pcasilla->pieza <= maxValido);
}
/****************************************************************************************
 * casillaVacia
 *
 * Devuelve TRUE si la casilla está vacía, FALSE en caso contrario
 ***************************************************************************************/
AJD_Bool casillaVacia (AJD_CasillaPtr pcasilla)
{
    return obtenPieza (pcasilla) == NO_PIEZA;
}
/****************************************************************************************
 * caminoLibre
 *
 * Devuelve TRUE si no hay ninguna pieza entre las casillas origen y destino indicadas, 
 *          FALSE en caso contrario.
 ***************************************************************************************/
AJD_Bool caminoLibre (AJD_CasillaPtr origen, AJD_CasillaPtr destino, int dx, int dy)
{
    AJD_CasillaPtr pcasilla;

    int offsetx = sign (dx);
    int offsety = sign (dy) * 8;
    int offset  = offsetx + offsety;

    for (pcasilla = origen+offset; pcasilla != destino; pcasilla += offset)
    {        
        if (!casillaVacia (pcasilla)) 
            return FALSE;
    }
    return TRUE;
}
