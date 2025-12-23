#include "identite.h"
#include <stdio.h>
#include <stdlib.h>
#define LG_DATE 10 // Nombre de caractères d’une date sous la forme jj/mm/aaaa
#define LG_LIGNE 80 // taille maximal d'une ligne

// Identité d’une personne
struct sIdentite
{
    int Identifiant; // Identifiant unique
    char *Nom; // Nom (chaîne de caractères allouée dynamiquement)
    char *Prenom; // Prénom (chaîne de caractères allouée dynamiquement)
    char Sexe; // ’F’ ou ’M’
    char DateNaissance[LG_DATE+1]; // Date de naissance sous la forme jj/mm/aaaa
};

//pour calculer la taille d'un string. On n'inclut pas le \0
static int len(char* chaine){
    int taille = 0;
    while(chaine[taille] != '\0'){
        taille++;
    }
    return taille;
}


tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]){
    tIdentite identite = malloc(sizeof(struct sIdentite));
    if(identite == NULL){
        return NULL;
    }   

    //on va écrire, lettre par lettre, les élements de Nom dans identite->Nom
    int lenN = len(Nom);
    identite->Nom = malloc(lenN+1 * sizeof(char));
    if(identite->Nom == NULL){
        free(identite);
        return NULL;
    }
    for(int i = 0; i < lenN; i++){
        identite->Nom[i] = Nom[i];
    }
    identite->Nom[lenN] = '\0';

    //on va écrire, lettre par lettre, les élements de Prenom dans identite->Prenom
    int lenP = len(Prenom);
    identite->Prenom = malloc(lenP+1 * sizeof(char));
    if(identite->Prenom == NULL){
        free(identite->Nom);
        free(identite);
        return NULL;
    }
    for(int i = 0; i < lenP; i++){
        identite->Prenom[i] = Prenom[i];
    }
    identite->Prenom[lenP] = '\0';

    identite->Identifiant = Id;
    identite->Sexe = Sexe;

    //on va écrire, lettre par lettre, les élements de DateNais dans DateNaissance
    for(int i = 0; i < LG_DATE; i++){
        identite->DateNaissance[i] = DateNais[i];
    }
    identite->DateNaissance[LG_DATE] = '\0';

    return identite;

}


int IdentiteIdentifiant(tIdentite Identite){
    return Identite->Identifiant;
}


char *IdentiteNom(tIdentite Identite){
    return Identite->Nom;
}


char *IdentitePrenom(tIdentite Identite){
    return Identite->Prenom;
}


char IdentiteSexe(tIdentite Identite){
    return Identite->Sexe;
}


char *IdentiteDateNaissance(tIdentite Identite){
    return Identite->DateNaissance;
}


void IdentiteAfficher(tIdentite Identite){
    int id = Identite->Identifiant;
    char sexe = Identite->Sexe;
    printf("[%d] %s %s, %c, %s\n", id, Identite->Nom, Identite->Prenom, sexe, Identite->DateNaissance);
    // ici je prefere mettre les donnees de type char* directement dans le print
}


void IdentiteLiberer(tIdentite Identite){
    free(Identite->Nom);
    free(Identite->Prenom);
    free(Identite);
}


tIdentite IdentiteLiref(FILE *f) {
    char fin;
    int id;
    char nom[LG_LIGNE + 1];
    char prenom[LG_LIGNE + 1];
    char sexe;
    char dateNaissance[LG_DATE + 1];

    // Lire l'identifiant
    fscanf(f, "%d", &id);
    fgetc(f); // pour enlever le '\n'

    // Lire le nom
    fscanf(f, "%s", nom);
    fgetc(f);

    // Lire le prénom
    fscanf(f, "%s", prenom);
    fgetc(f);

    // Lire le sexe
    sexe = fgetc(f);
    fgetc(f);

    // Lire la date de naissance
    fscanf(f, "%s", dateNaissance);
    fin = fgetc(f);
    if(fin == EOF){ 
        // soit c'est EOF, soit \n. Si c'est \n, on peut techniquement continuer
        // car cette fonction va être utiliser dans une boucle
        return NULL;
    }

    return IdentiteCreer(id, nom, prenom, sexe, dateNaissance);
}