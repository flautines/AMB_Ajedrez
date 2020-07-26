#include <common.h>
#include <pgnreader.h>
#include <movimiento.h>
#include <string.h>
/*
 * FUNCIONES PRIVADAS (fwd decl.)
 */
void strEnroqueToMov (char *buff, AJD_Color colorPieza, AJD_MovimientoPtr movimiento);
void strPiezaMayorToMov (char *buff, AJD_Color colorPieza, AJD_MovimientoPtr movimiento);
void strPeonToMov (char *buff, AJD_Color colorPieza, AJD_MovimientoPtr movimiento);
void strJaqueToMov (char *buff, AJD_MovimientoPtr movimiento);
AJD_Pieza strToPieza (char c, AJD_Color colorPieza);

/****************************************************************************************
 * strToMov
 * Interpreta un string con información de un movimiento en notación algebraica 
 * y devuelve los datos correspondientes en formato AJD_Movimiento.
 ***************************************************************************************/
AJD_Movimiento strToMov (char *buff, AJD_Color colorPieza)
{
    AJD_Movimiento movimiento = { 0, 0, {0,0,0,0}};
    char c;

    c = *buff;

    /* Si se detecta un enroque no hay que interpretar nada más del texto. 
     * Rellenamos la información del movimiento según el tipo de enroque
     * y ya hemos acabado.                                               */
    if (c == 'O') {
        strEnroqueToMov (buff, colorPieza, &movimiento);
    }
    else {
        /* Si no es un enroque, el primer caracter determina la pieza. */
        if (c < 'a')
            strPiezaMayorToMov (buff, colorPieza, &movimiento);
        else
            strPeonToMov (buff, colorPieza, &movimiento);
        
        /* Detectar '+' (jaque), "#" (mate) */
        strJaqueToMov (buff, &movimiento);
    }

    return movimiento;
}

/*
 * PRIVADAS
 */
void strEnroqueToMov (char *buff, AJD_Color colorPieza, AJD_MovimientoPtr movimiento)
{
    movimiento->idOrigen = colorPieza ? INICIO_REY_BLANCO : INICIO_REY_NEGRO;
    if (strlen(buff) == 3) {
        movimiento->idDestino = colorPieza ? EC_DESTINO_REY_BLANCO : EC_DESTINO_REY_NEGRO;
        movimiento->flags.enroque = ER_CORTO;
    }
    else {
        movimiento->idDestino = colorPieza ? EL_DESTINO_REY_BLANCO : EL_DESTINO_REY_NEGRO;
        movimiento->flags.enroque = ER_LARGO;
    }
}

void strPiezaMayorToMov (char *buff, AJD_Color colorPieza, AJD_MovimientoPtr movimiento)
{
    char c = *buff;
    /*AJD_Pieza pieza = strToPieza (c, colorPieza);*/
    AJD_Bool hayFila, hayColumna;

    hayFila = hayColumna = FALSE;

    buff++;
    c=*buff;
    if ( c >= 'a' && c<= 'h' && buff[1] >= 'a' && buff[1] <= 'h' ) {
        hayColumna = TRUE;
        buff++;
    }
    else if ( c >= '1' && c <= '8') {
        hayFila = TRUE;
        buff++;
    }
    if (c == 'x') {
        movimiento->flags.captura = TRUE;
        buff++;
    }
    movimiento->idDestino = strToId (buff);
}

void strPeonToMov (char *buff, AJD_Color colorPieza, AJD_MovimientoPtr movimiento)
{
    /*AJD_Pieza pieza = colorPieza ? PEON_B : PEON_N;*/
    if (buff[1] == 'x') {
        movimiento->flags.captura = TRUE;
        buff+=2;
    }
    movimiento->idDestino = strToId (buff);
}

AJD_Pieza strToPieza (char c, AJD_Color colorPieza)
{
    AJD_Pieza pieza = NO_PIEZA;
    switch (c) {
        case 'T': pieza = (colorPieza ? TORRE_B   : TORRE_N);
        case 'C': pieza = (colorPieza ? CABALLO_B : CABALLO_N);
        case 'A': pieza = (colorPieza ? ALFIL_B   : ALFIL_N);
        case 'D': pieza = (colorPieza ? DAMA_B    : DAMA_N);
        case 'R': pieza = (colorPieza ? REY_B     : REY_N);
    }

    return pieza;
}

void strJaqueToMov (char *buff, AJD_MovimientoPtr movimiento)
{
    char *pch;    
    if ( (pch = strchr(buff, '+')) )
        movimiento->flags.jaque = TRUE;

    if ( (pch = strchr(buff, '#')) )
        movimiento->flags.mate = TRUE;
}
