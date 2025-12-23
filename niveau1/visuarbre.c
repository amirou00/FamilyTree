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
    ArbreAfficher(arbre);
    printf("\n"); // pour faire joli, comme avec niveau2 (dans l'affichage globale)
    ArbreLireLienParenteFichier(arbre, argv[2]); // argv[2] : arbre10.par
    ArbreAfficher(arbre);
    ArbreEcrireGV(arbre, argv[3]); // pas d'affichage, ça va juste remplir le fichier .dot (deja cree)
    ArbreLiberer(arbre);
    return 0;
}