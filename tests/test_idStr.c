#include <common.h>

int main()
{
    AJD_TableroPtr ptablero = obtenTableroPtr(); 
    AJD_CasillaPtr pcasilla = ptablero->casillas;
    int ncasillas = 64;
    char *strCasilla = "";

    inicializa();
    nuevoJuego();    
    liberaRecursos();


    while (ncasillas) {
        strCasilla = ptrToStr (pcasilla);
        if (!(ncasillas&7)) putchar('\n');        
        printf ("%02d: %s ", ptrToId (pcasilla), strCasilla);
        pcasilla++;
        ncasillas--;
    }
            
    return 0;
}
