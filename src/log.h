extern char partida[];

typedef struct _AJD_NodoMov {
    char *data;
    struct _AJD_NodoMov *next;
} AJD_NodoMov, *AJD_NodoMovPtr;

typedef struct  {
    AJD_NodoMovPtr begin;
    AJD_NodoMovPtr end;
    int      len;
} AJD_LogPartida, *AJD_LogPartidaPtr;

/****************************************************************************************
 * movToStr
 *  Convierte la información de un movimiento a string
 *  Devuelve tamaño del string formado (sin contar el '\0').
 ***************************************************************************************/
size_t movToStr (AJD_MovimientoPtr movimiento, char* buff);
/****************************************************************************************
 * jugadaToStr
 *  Convierte la información de una jugada a string
 *  Devuelve tamaño del string formado (sin contar el '\0').
 ***************************************************************************************/
void jugadaToStr (AJD_JugadaPtr jugada, char *buff);
/****************************************************************************************
 * piezaToStr
 *  Devuelve un char con la representación de la pieza (P, T, C, A, D, R).
 *  Si la pieza es NO_PIEZA devuelve el caracter ' '(ESPACIO).
 ***************************************************************************************/
char piezaToStr (AJD_Pieza pieza);
/****************************************************************************************
 * nuevoNodo
 *  Crea un nuevo nodo de movimiento en notación algebraica.
 ***************************************************************************************/
AJD_NodoMovPtr nuevoNodo (const char *strMovimiento, int len);
/****************************************************************************************
 * liberaNodo
 *  Libera la memoria usada por un nodo de notación algebraica.
 ***************************************************************************************/
void liberaNodo (AJD_NodoMovPtr pnodo);
/****************************************************************************************
 * initLogPartida
 *  Inicializa el log de movimientos de la partida.
 ***************************************************************************************/
AJD_Bool initLogPartida();
/****************************************************************************************
 * appendMovimiento
 *  Añade un movimiento en formato interno al log de partida. El movimiento se guarda
 * en el log en formato algebraico (string).
 ***************************************************************************************/
AJD_Bool appendMovimiento (AJD_MovimientoPtr movimiento);

void sprintPartida(char *buff);
void liberaLogPartida();
