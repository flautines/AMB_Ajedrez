#include <common.h>
#include <movimiento.h>
#include <pgnreader.h>
#include <string.h>

char *strMovs[] = { "e4", "c6", "d4", "Nf6", "Nc3", "d6", "Nf3", "d5", "Bd3", "e5", "Be3",
    "Ng4", "O-O", "Nd7", "h3", "Ngf6" ,"b3" ,"exd4" ,"Nxd4" ,"Ne5" ,"Qe2" ,"b6" ,"Rfe1" ,
    "Nxd3" ,"cxd3" ,"Qd6" ,"exd5" ,"Qe7" ,"dxc6" ,"Bf5" ,"Qf3" ,"Bg4" ,"hxg4" ,"Qe6" ,"Nxe6" ,
    "Nxg4" ,"Nc7+" ,"Kd8" ,"Nxa8" ,"b5", "Qxg4", "f6", "Qd7#",0
};


int main()
{
    char **it = strMovs;
    uint8_t nturno = 1;
    char strTurno[] = "1.";
    AJD_Movimiento mov = { 0,0, {0,0,0,0}};

    inicializa();
    nuevoJuego();
    liberaRecursos();

    while (*it) {
        mov = strToMov (*it++, BLANCO);
        if (nturno&1) sprintf (strTurno, "%d.", (nturno>>1)+1);
        else strcpy (strTurno, "");
        printf ("%s%s%s%s ", 
            strTurno, 
            mov.flags.captura ? "x":"", 
            ptrToStr (idToPtr (mov.idDestino)),
            mov.flags.jaque ? "+" :
                mov.flags.mate ? "#" : "");
        nturno++;
    }
    return 0;
}
