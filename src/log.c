#include <common.h>
#include <log.h>
#include <string.h>

/****************************************************************************************
 * movToStr
 *  Convierte la información de un movimiento a string
 *  Devuelve tamaño del string formado (sin contar el '\0').
 ***************************************************************************************/
size_t movToStr (AJD_MovimientoPtr movimiento, char* buff)
{
    AJD_CasillaPtr porigen  = idToPtr (movimiento->idOrigen);
    AJD_CasillaPtr pdestino = idToPtr (movimiento->idDestino);
    AJD_Pieza      pieza    = obtenPieza (porigen);
    char           chPieza  = piezaToStr (pieza);
    AJD_Enroque    enroque  = movimiento->enroque;
    char           *begin   = buff;    

    /* Si hay algún enroque no se anota nada más */
    if (enroque == ER_LARGO)
    {
        strncpy (buff, "O-O-O", 5);
        buff += 5;
    }
    else if (enroque == ER_CORTO) 
    {
        strncpy (buff, "O-O", 3);
        buff += 3;
    }
    else 
    {
        /* 1. Letra de la pieza, excepto si es un peon */
        if ( chPieza != 'P')
            *buff++ = chPieza;
    
        /* 2. Nombre de la casilla origen */
        strncpy (buff, ptrToStr (porigen), 2);
        buff += 2;
    
        /* 3. 'x' si captura pieza, o '-' en caso contrario */
        *buff++ = (movimiento->captura==TRUE) ? 'x' : '-';
    
        /* 4. Nombre de la casilla destino */
        strncpy (buff, ptrToStr (pdestino), 2);
        
        buff += 2;
    
        if (movimiento->mate) *buff++ = '#';
        else if (movimiento->jaque) *buff++ = '+';
    
    }
    *buff = '\0';

    return buff-begin;
}

/****************************************************************************************
 * jugadaToStr
 *  Convierte la información de una jugada a string
 *  Devuelve tamaño del string formado (sin contar el '\0').
 ***************************************************************************************/
void jugadaToStr (AJD_JugadaPtr jugada, char *buff)
{
    int num = 0;

    /* Numero de jugada */
    sprintf (buff, "%3d. ", jugada->nturno);
    buff += 5;

    /* Movimiento BLANCAS */
    num = movToStr (&jugada->movBlancas, buff);
    buff += num;

    /* Separación */
    *buff++ = ' ';

    /* Movimiento NEGRAS */
    num = movToStr (&jugada->movNegras, buff);
    buff += num;
}
/****************************************************************************************
 * piezaToStr
 *  Devuelve un char con la representación de la pieza (P, T, C, A, D, R).
 *  Si la pieza es NO_PIEZA devuelve el caracter ' '(ESPACIO).
 ***************************************************************************************/
char piezaToStr (AJD_Pieza pieza)
{
    /* Recordemos que AJD_Pieza es 
     *      (NO_PIEZA, PEON_B, CABALLO_B, ... , PEON_N, CABALLO_N, ...)
     * Por tanto cada representación de pieza se vuelve a repetir
     */
    char *strPiezas = " PTCADRPTCADR";
    return strPiezas[pieza];
}
