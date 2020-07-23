#include <common.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>

/****************************************************************************************
 * Variables PRIVADAS
 ***************************************************************************************/
AJD_LogPartida logPartida;  /* Almacena todos los movimientos en notacion algebraica */
/****************************************************************************************
 * movToStr
 *  Convierte la información de un movimiento a string (notacion reducida)
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
    char       strOrigen[2] = "";

    /* Si hay algún enroque no se anota nada más */
    if (enroque == ER_LARGO) {
        strncpy (buff, "O-O-O", 5);
        buff += 5;
    }
    else if (enroque == ER_CORTO) {
        strncpy (buff, "O-O", 3);
        buff += 3;
    }
    else {
        /* 1. Letra de la pieza, excepto si es un peon */
        if ( chPieza != 'P')
            *buff++ = chPieza;
    
        /* Nombre de la casilla origen */
        strncpy (strOrigen, ptrToStr (porigen), 2);
    
        /* 2. 'x' si captura pieza */
        if (movimiento->captura) {
            if (chPieza == 'P') {
                /* Cuando un peon captura, se indica la columna de origen */
                *buff++ = strOrigen[0];
            }
            *buff++ = 'x';
        }
    
        /* 3. Nombre de la casilla destino */
        strncpy (buff, ptrToStr (pdestino), 2);        
        buff += 2;
    
        if (movimiento->mate)       *buff++ = '#';
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
    num = sprintf (buff, "%d.", jugada->nturno);
    buff += num;

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
/****************************************************************************************
 * nuevoNodo
 *  Crea un nuevo nodo de movimiento en notación algebraica.
 ***************************************************************************************/
AJD_NodoMovPtr nuevoNodo (const char *strMovimiento, int len)
{
    /*int len = strlen (strMovimiento);*/
    AJD_NodoMovPtr pnodo = calloc (1, sizeof (AJD_NodoMov));

    pnodo->data = (char*) malloc (len+1);
    strncpy (pnodo->data, strMovimiento, len+1);
    
    return pnodo; 
}
/****************************************************************************************
 * liberaNodo
 *  Libera la memoria usada por un nodo de notación algebraica.
 ***************************************************************************************/
void liberaNodo (AJD_NodoMovPtr pnodo)
{
    if (pnodo) {
        if (pnodo->data)
            free (pnodo->data);
        free (pnodo);
    }
}

/****************************************************************************************
 * initLogPartida
 *  Inicializa el log de movimientos de la partida.
 ***************************************************************************************/
AJD_Bool initLogPartida()
{
    logPartida.begin = logPartida.end = NULL;
    logPartida.len = 0;
    return TRUE;
}
/****************************************************************************************
 * appendMovimiento
 *  Añade un movimiento en formato interno al log de partida. El movimiento se guarda
 * en el log en formato algebraico (string).
 ***************************************************************************************/
AJD_Bool appendMovimiento (AJD_MovimientoPtr movimiento)
{
    char            buff[40] = "";
    int             len      = 0;
    AJD_NodoMovPtr  pnodo    = NULL;

    len = movToStr (movimiento, buff) + 1;
    pnodo = nuevoNodo (buff, len);

    /* Lista no vacía, añadimos el nodo a continuación del último */
    if (logPartida.len) {
        logPartida.end->next = pnodo;
        logPartida.end = pnodo;
    } 
    /* Lista vacía, primer nodo. Tanto begin como end apuntan a este nodo */
    else {
        logPartida.begin = pnodo;
        logPartida.end   = pnodo;
    }
    logPartida.len++;

    return TRUE;
}

void sprintPartida(char *buff)
{
    AJD_NodoMovPtr pnodo = logPartida.begin;
    int len = logPartida.len+1;
    int nturno = 0;
    char strTurno[25];
    int nuevoTurno = 1;

    while (--len) {
        int num = 0;
        if (nuevoTurno) {
            nturno++;
            sprintf (strTurno, "%d. ", nturno);
        }
        num = sprintf (buff, "%s%s ",  nuevoTurno ? strTurno : "", pnodo->data);
        pnodo = pnodo->next;
        buff += num;
        nuevoTurno ^=1;
        
    }
}

void liberaLogPartida()
{
    AJD_NodoMovPtr pnodo = logPartida.begin;
    AJD_NodoMovPtr pnext;
    int len = logPartida.len;

    while (len--) {
        pnext = pnodo->next;
        if (pnodo)
            liberaNodo (pnodo);
        pnodo = pnext;
    }
}
