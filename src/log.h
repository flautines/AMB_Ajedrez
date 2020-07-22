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
