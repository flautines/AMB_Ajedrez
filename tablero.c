#include <stdint.h>
#include <ajd_types.h>
#include <tablero.h>
#include <stdio.h>

AJD_Tablero tablero;

void inicializaTablero ()
{
	int ncasillas = 64;
	int ncolumnas = 0;
	AJD_CasillaPtr pcasilla = tablero.casillas;
	AJD_Color color_casilla = BLANCO;

	while (ncasillas--)
	{
		if (ncolumnas&7)
			color_casilla ^= 1;
		pcasilla->color = color_casilla;
		ncolumnas++;
		pcasilla++;
	}
}

void dibujaTablero() 
{
	int ncasillas = 64;
	AJD_CasillaPtr pcasilla = tablero.casillas;

	while (ncasillas--)
	{
		putchar (pcasilla->color ? 'B': 'N');
		if (!(ncasillas&7)) putchar ('\n');
		pcasilla++;		
	}
}
