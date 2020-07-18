#include <common.h>
#include <stdio.h>
/****************************************************************************************
* Variables PRIVADAS
 ***************************************************************************************/
AJD_Tablero tablero;
 
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
 * obtenCasilla
 *
 * Obten la casilla del tablero a partir de su id (p.ej a3)
 ***************************************************************************************/
AJD_Casilla obtenCasilla (AJD_idCasilla id)
{
	return tablero.casillas[id];
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


AJD_Seleccion obtenSeleccion (AJD_EstadoPtr estadoJuego, AJD_Accion accion)
{  
    /* 
     * Si no hay ninguna seleccion previa, la selección actual corresponde al origen,
     * Si había una selección anterior, la selección actual corresponde al destino.
     * Si se selecciona como destino la misma casilla de origen, se cancela la selección
     */
    if (accion == CUR_SELEC)
    {
        if (estadoJuego->casillaSeleccionada == NO_SELECCION)
        {
            estadoJuego->casillaSeleccionada = ORIGEN_SELECCIONADO;
            /* Con el cursor fijo damos indicación visual de la casilla origen */
            tablero.curFijo.casilla = tablero.curMovil.casilla;
            tablero.curFijo.visible = TRUE;
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
    return estadoJuego->casillaSeleccionada;
}
