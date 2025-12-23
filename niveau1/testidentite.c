#include <stdio.h>
#include <stdlib.h>
#include "identite.h"

int main(void){
    //tests
    int id = 1234;
    char* nom = "LEE";
    char* prenom = "bruce";
    char sexe = 'M';
    char datn[] = "27/11/1940";
    tIdentite id1 = IdentiteCreer(id, nom, prenom, sexe, datn);
    int id_id1 = IdentiteIdentifiant(id1);
    char* nom_id1 = IdentiteNom(id1);
    char* prenom_id1 = IdentitePrenom(id1);
    char sexe_id1 = IdentiteSexe(id1);
    char* DateN_id1 = IdentiteDateNaissance(id1);
    printf("%d, %s, %s, %c, %s\n", id_id1, nom_id1, prenom_id1, sexe_id1, DateN_id1);
    IdentiteLiberer(id1);

    //tests pour les fichiers
    FILE* f_id;
    f_id = fopen("personne.ind", "rt");
    tIdentite id2 = IdentiteLiref(f_id);
    IdentiteAfficher(id2);
    IdentiteLiberer(id2);

}   