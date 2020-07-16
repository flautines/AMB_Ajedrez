#pragma once

/****************************************************************************************
 * sign (a)
 *   Devuelve -1 si a < 0
 *             1 si a > 0
 *             0 si a == 0
 ***************************************************************************************/
int sign (int a);
/****************************************************************************************
 * strSegundos
 *   Convierte segundos a string con formato MM:SS (MM: minutos, SS: segundos).
 ***************************************************************************************/
char* strSegundos (char* buff, uint16_t segundos);