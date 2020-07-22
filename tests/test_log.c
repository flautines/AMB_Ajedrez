#include <common.h>
#include <log.h>

void printMov(AJD_MovimientoPtr mov)
{
    printf("{origen, destino, captura, jaque, enroque, mate} = { %s, %s, %d, %d, %d, %d}\t",
           ptrToStr (idToPtr(mov->idOrigen)),
           ptrToStr (idToPtr(mov->idDestino)),
           mov->captura,
           mov->jaque,
           mov->enroque,
           mov->mate);

}
int main()
{
    char buff[40] = "";
    AJD_EstadoPtr pestado;
    AJD_idCasilla idCasilla = a2;

    inicializa();
    nuevoJuego();

    pestado = obtenEstadoPtr();
    pestado->jugada.movBlancas.idDestino = e4;
    pestado->jugada.movBlancas.jaque = TRUE;
    pestado->jugada.movNegras.idOrigen = e7;
    pestado->jugada.movNegras.idDestino = e5;  

    liberaRecursos();
    
    puts ("BLANCAS");
    for (idCasilla = a2; idCasilla <= h1; idCasilla++)
    {
        pestado->jugada.movBlancas.idOrigen = idCasilla;
        pestado->jugada.movBlancas.captura = idCasilla&3 ? FALSE : TRUE;
        pestado->jugada.movBlancas.enroque = idCasilla==e1 ? ER_LARGO : ER_NONE;
        pestado->jugada.movBlancas.mate = idCasilla&15 ? FALSE : TRUE;
        jugadaToStr (&pestado->jugada, buff);
        puts (buff);
    }

    puts ("NEGRAS");
    for (idCasilla = a8; idCasilla <= h7; idCasilla++)
    {
        pestado->jugada.movNegras.idOrigen = idCasilla;
        pestado->jugada.movNegras.captura = idCasilla&7 ? FALSE :TRUE;
        pestado->jugada.movNegras.enroque = idCasilla==e8 ? ER_CORTO : ER_NONE;
        pestado->jugada.movNegras.mate = idCasilla&15 ? FALSE : TRUE;

        jugadaToStr (&pestado->jugada, buff);
        puts (buff);
    }
    
    return 0;
}
