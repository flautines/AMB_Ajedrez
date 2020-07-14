#include <common.h>
#include <log.h>

void printTurno (AJD_TurnoPtr turno)
{
    char* str_pieza = " PTCADR";
    printf ("origen: %c en %d\ndestino: %c en %d\nCOME:%d, JAQUE:%d, MATE:%d\n",
            str_pieza[turno->origen.pieza], turno->origen.id, 
            str_pieza[turno->destino.pieza], turno->destino.id, 
            turno->come_pieza, turno->jaque, turno->mate);
}

int main ()
{
    AJD_Casilla origen  = { NEGRO, CABALLO, BLANCO, b5 };
    AJD_Casilla destino = { NEGRO, CABALLO, BLANCO, d6 };
    AJD_Turno blancas = {
        0, 1, 1,
        origen, destino, 
        0, 0, 0, 0, 0
    };
    AJD_Casilla origenN  = { NEGRO, TORRE, BLANCO, f5 };
    AJD_Casilla destinoN = { NEGRO, TORRE, BLANCO, f6 };
    AJD_Turno negras = {
        1, 1, 0,
        origenN, destinoN, 
        0, 0, 0, 1, 0        
    };

    char buffB[10] = "";
    char buffN[10] = "";
    turnoToAlgebra (&blancas, buffB);
    turnoToAlgebra (&negras, buffN);

    //uint16_t turno = 25;

    //char buff[20] = "";

    //sprintf (buff, "\033[32;1m%d. %s\t%s\033[0m", turno, buffB, buffN);

    //puts (buff);

    puts (buffB);
    puts (buffN);

    algebraToTurno (buffN, &negras, NEGRO);
    printTurno (&negras);

    printf ("f5: %d, f6: %d\n", strToIdCasilla ("f5"), strToIdCasilla ("f6"));
    printf ("29: %s, 21: %s\n", idCasillaToString (29), idCasillaToString  (21));
}
