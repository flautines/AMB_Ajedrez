#include <stdint.h>
#include <ajd_types.h>
#include <tablero.h>
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
      tablero.casillas[col].pieza       = piezasMayoresN[col];  /* fila 1: piezas mayores negras  */
      tablero.casillas[7*8 + col].pieza = piezasMayoresB[col];  /* fila 8: piezas mayores blancas */

      tablero.casillas[8 + col].pieza   = PEON_N;               /* fila 2: peones negros      */
      tablero.casillas[6*8 + col].pieza = PEON_B;               /* fils 7: peones blancos     */
   }
}
