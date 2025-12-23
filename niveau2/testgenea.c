#include <stdio.h>
#include <stdlib.h>
#include "genea.h"
#include "identite.h"

int main(int argc, char* argv[]){
    if(argc != 3){ // 2 arguments + le fichier
        printf("vous devez saisir deux arguments!!!\n");
        return 1;
    }
    // j'ai utilisé le minimum de fonction possible, cra certains fonctions utilisent d'autres fonctions, et si 
    // elles marchent, normalement ceux utilisées marchent aussi 
    tArbre arbre = ArbreLirePersonnesFichier(argv[1]); // argv[1] : arbre4.ind
    ArbreAfficher(arbre);
    printf("\n"); // pour faire joli(dans l'affichage globale)
    ArbreLireLienParenteFichier(arbre, argv[2]); // argv[2] : arbre4.par
    ArbreAfficher(arbre);
    ArbreLiberer(arbre);
    return 0;
}