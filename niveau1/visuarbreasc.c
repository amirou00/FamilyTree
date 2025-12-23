#include <stdio.h>
#include <stdlib.h>
#include "identite.h"
#include "genea.h"

int main(int argc, char* argv[]){
    
    if(argc != 4){ // 3 arguments + le fichier
        printf("vous devez saisir deux arguments!!!\n");
        return 1;
    }

    tArbre arbre = ArbreLirePersonnesFichier(argv[1]); // argv[1] : arbre10.ind
    printf("\n");
    ArbreLireLienParenteFichier(arbre, argv[2]); // argv[2] : arbre10.par
    ArbreAfficher(arbre);
    printf("\n");
    ArbreAfficherAscendants(arbre, 4); // exemple avec CHARLOT Jean-Louis, comme dans les consignes
    ArbreEcrireAscendantsGV(arbre, 4, argv[3]); // argv[3] : arbreAsc.dot
    ArbreLiberer(arbre);
    return 0;
}