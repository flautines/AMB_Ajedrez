#include <tablero.h>
// variable global con datos para una casilla 'NULL' (sin nada)
AJD_Casilla casilla_nul = { BLANCO, NONE, BLANCO, 0 };
// variable global para facilitar conversion de idCasilla a String
char* strCasillas[] = {
   "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
   "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
   "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
   "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
   "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
   "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
   "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
   "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};
////////////////////////////////////////////////////////////////////////////
// tableroDisposicionInicial
//
// Dispone las piezas en el tablero para una partida nueva
//
void tableroDisposicionInicial (AJD_TableroPtr tablero)
{   
   AJD_Pieza piezasMayores[8] = { TORRE, CABALLO, ALFIL, DAMA, REY, ALFIL, CABALLO, TORRE };
   for (int col=0; col < 8; col++)
   {
      tablero->casilla[col].pieza               = piezasMayores[col]; // fila 1: piezas mayores negras
      tablero->casilla[col].color_pieza         = NEGRO;

      tablero->casilla[7*8 + col].pieza         = piezasMayores[col]; // fila 8: piezas mayores blancas

      tablero->casilla[8 + col].pieza           = PEON;               // fila 2: peones negros
      tablero->casilla[8 + col].color_pieza     = NEGRO;
      
      tablero->casilla[6*8 + col].pieza         = PEON;               // fils 7: peones blancos
   }
}
///////////////////////////////////////////////////////////////////////////
// strToIdCasilla
//
// Convierte una casilla en formato string (p.ej: "a8") a idCasilla
//
AJD_idCasilla strToIdCasilla (char *strCasilla)
{
   AJD_idCasilla idCasilla = 0;
   char c = *strCasilla++;

   idCasilla = c - 'a'; // columna
   c = *strCasilla;
   idCasilla += ('8'-c)*8; // fila

   return idCasilla;
}
///////////////////////////////////////////////////////////////////////////
// idCasillaToString
//
// Devuelve idCasilla en formato string (p.ej: 0 -> "a8")
//
char *idCasillaToString (AJD_idCasilla idCasilla)
{
   return strCasillas[idCasilla];
}
