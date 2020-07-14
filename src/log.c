#include <common.h>
#include <log.h>

#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////
// toAlgebrString
//  Convierte la información de un turno a formato Algebraico
//
void turnoToAlgebra (AJD_TurnoPtr turno, char* buff)
{
	AJD_CasillaPtr origen = &turno->origen;
	AJD_CasillaPtr destino = &turno->destino;
	char* str_pieza = "  TCADR";
	AJD_Pieza pieza = origen->pieza;
	
	if (origen->pieza != PEON)
		*buff++ = str_pieza[pieza];

	strncpy (buff, strCasillas[origen->id], 2);

	buff += 2;

	*buff++ = turno->come_pieza ? 'x' : '-';

	strncpy (buff, strCasillas[destino->id], 2);
	
	buff += 2;

	if (turno->jaque || turno->mate) *buff++ = '!';

	if (turno->mate) *buff++ = '!';

	*buff = '\0';
}
/////////////////////////////////////////////////////////////////////////////////////////
// algebraToTurno
//  Convierte información algebraica en format AJD_Turno
//
void algebraToTurno (char* buff, AJD_TurnoPtr turno, AJD_Color color_jugador)
{
  	char c;
  	char strCasilla[2];
  	turno->origen.id = strToIdCasilla (strCasilla);

  	c = *buff;
	if (c < 'a')
	{
	  	switch (c)
		{
		case 'T': turno->origen.pieza = TORRE;break;
		case 'C': turno->origen.pieza = CABALLO;break;
		case 'A': turno->origen.pieza = ALFIL;break;
		case 'D': turno->origen.pieza = DAMA;break;
		case 'R': turno->origen.pieza = REY;break;
		}
		buff++;
	}
	else
		turno->origen.pieza = PEON;

	turno->origen.color_pieza = color_jugador;
	turno->destino.color_pieza = color_jugador;

	strncpy (strCasilla, buff, 2);
	turno->origen.id = strToIdCasilla (strCasilla);

	buff += 2;

	if (*buff++ == 'x')
	{
		turno->come_pieza = TRUE;
		turno->destino.color_pieza ^= 1;
	}

	strncpy (strCasilla, buff, 2);
	turno->destino.id = strToIdCasilla (strCasilla);

	buff += 2;

	if (*buff++ == '!')
		turno->jaque = TRUE;

	if (*buff++ == '!')
		turno->mate = TRUE;
}
