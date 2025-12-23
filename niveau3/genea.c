#include "genea.h"
#include "identite.h"
#include <stdio.h>
#include <stdlib.h>

// Arbre généalogique
struct sArbre
{
    struct sFiche *pPremiere; // Adresse de la première fiche
    struct sFiche *pDerniere; // Adresse de la dernière fiche
};

// Fiche associée à chaque individu présent dans l’arbre
struct sFiche
{
    tIdentite Identite; // Accès aux informations de l’identité de la personne
    struct sFiche *pPere; // Adresse de la fiche du père
    struct sFiche *pMere; // Adresse de la fiche de la mère
    struct sFiche *pSuivante; // Adresse de la fiche suivante
};


static void AfficherAscRecursive(tArbre arbre, struct sFiche* fiche, int niveau){
    //cette fonction va aider la fonction ArbreAfficherAscendants a afficher les ascendants de la personne de
    //maniere recursive, avec les bonnes indentations/tabulations

    // Afficher la personne
    IdentiteAfficher(fiche->Identite);

    // Afficher le pere de la personne (s'il est present dans l'arbre) recursivement
    if (fiche->pPere != NULL) {
        for(int i = 0; i < niveau + 1; i++){
            printf("\t");
            fflush(stdout);
        } // indentation avant "pere"
        printf("Pere : ");
        fflush(stdout);
        AfficherAscRecursive(arbre, fiche->pPere, niveau + 1);
    }
    // Afficher la mere de la personne (si elle est presente dans l'arbre) recursivement
    if (fiche->pMere != NULL) {
        for(int i = 0; i < niveau + 1; i++){
            printf("\t");
            fflush(stdout);
        } // indentation avant "mere"
        printf("Mere : ");
        fflush(stdout);
        AfficherAscRecursive(arbre, fiche->pMere, niveau + 1);
    }
}


static void AfficherAscRecursiveGV(tArbre arbre, struct sFiche* fichePreced, struct sFiche* fiche, FILE* f){
    //cette fonction recursive va aider ArbreEcrireAscendantsGV a faire les bonnes affichage
    
    //choisir la couleur de case
    if(IdentiteSexe(fiche->Identite) == 'M'){
        fputs("node [color=blue];\n\t", f);
    }
    else if(IdentiteSexe(fiche->Identite) == 'F'){
        fputs("node [color=green];\n\t", f);
    }
    //informations personnelles
    fprintf(f, "%d [label=\"%s\\n%s\\n%s\"];\n\t", 
            IdentiteIdentifiant(fiche->Identite),
            IdentiteNom(fiche->Identite),
            IdentitePrenom(fiche->Identite),
            IdentiteDateNaissance(fiche->Identite));
    // id -> id
    fprintf(f, "%d -> %d\n\n\t", IdentiteIdentifiant(fichePreced->Identite), IdentiteIdentifiant(fiche->Identite));

    //partie recursive
    if(fiche->pPere != NULL){
        AfficherAscRecursiveGV(arbre, fiche, fiche->pPere, f);
    }

    if(fiche->pMere != NULL){
        AfficherAscRecursiveGV(arbre, fiche, fiche->pMere, f);
    }

}


tArbre ArbreCreer(void){
    tArbre arbre = malloc(sizeof(struct sArbre));
    if(arbre == NULL){
        return NULL;
    }
    arbre->pPremiere = NULL;
    arbre->pDerniere = NULL;
    return arbre;
}


void ArbreAfficher(tArbre Arbre){
    if(Arbre == NULL || Arbre->pPremiere == NULL){
        printf("l'arbre genealogique est vide\n");
        return;
    }

    //on prend le permier pointeur vers sFiche et on fait  un parcours de l'arbre
    struct sFiche *actuel = Arbre->pPremiere;

    while(actuel != NULL){
        IdentiteAfficher(actuel->Identite);

        printf("\tpere : ");
        fflush(stdout);
        if(actuel->pPere != NULL){
            IdentiteAfficher(actuel->pPere->Identite);
        }
        else{
            printf("inconnu\n");
        }

        printf("\tmere : ");
        fflush(stdout);
        if(actuel->pMere != NULL){
            IdentiteAfficher(actuel->pMere->Identite);
        }
        else{
            printf("inconnue\n");
        }

        actuel = actuel->pSuivante;
    }
}


void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite){
    struct sFiche* fich = malloc(sizeof(struct sFiche));
    if(fich == NULL){
        perror("erreur d'allocation");
    }

    fich->Identite = Identite;
    fich->pPere = NULL;
    fich->pMere = NULL;
    fich->pSuivante = NULL;

    if(Arbre->pPremiere == NULL){
        Arbre->pPremiere = fich;
        Arbre->pDerniere = fich;
    }
    else{
        Arbre->pDerniere->pSuivante = fich;
        Arbre->pDerniere =  fich;
    }
}


void ArbreLiberer(tArbre Arbre){
    if(Arbre == NULL){
        // l'arbre est libre...
        return;
    }

    struct sFiche* actuel = Arbre->pPremiere;
    struct sFiche* suivant = NULL; // pour l'instant il pointe vers rien (vers NULL), on l'initialise dans la boucle
    while(actuel != NULL){
        suivant = actuel->pSuivante;
        IdentiteLiberer(actuel->Identite);
        free(actuel);
        actuel = suivant;
    }
    
    free(Arbre);
}


tArbre ArbreLirePersonnesFichier(char Fichier[]){
    FILE* f;
    f = fopen(Fichier, "rt");
    if(f == NULL){
        printf("problem d'ouverture!!!\n");
        return NULL;
    }

    tArbre arbre = ArbreCreer();
    if(arbre == NULL){
        printf("problem dans la creation de l'arbre!!!\n");
        fclose(f);
        return NULL;
    }

    tIdentite identite = IdentiteLiref(f);
    while(identite != NULL){
        ArbreAjouterPersonne(arbre, identite);
        identite = IdentiteLiref(f);
    }

    fclose(f);
    return arbre;

}


void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente){
    int idEnfant_touve = 0; // pour savoir si la fonction devrait afficher l'erreur a la fin ou pas
    int idParent_touve = 0; // idem
    struct sFiche* address_enfant; // pour ne pas refaire une boucle while apres les verifications
    struct sFiche* address_parent; // idem
    // je l'appel address car c'est une address dans le memoire

    struct sFiche* actuel = Arbre->pPremiere;
    while(actuel != NULL){
        int id_actuel = IdentiteIdentifiant(actuel->Identite);
        if(id_actuel == IdEnfant){
            idEnfant_touve = 1;
            address_enfant = actuel; 
        }
        if(id_actuel == IdParent){
            idParent_touve = 1;
            address_parent = actuel;
        }
        actuel = actuel->pSuivante;
    }

    if(idEnfant_touve == 0){
        perror("l'identifiant de l'enfant n'a pas ete trouve");
        return;
    }
    if(idParent_touve == 0){
        perror("l'identifiant du parent n'a pas ete trouve");
        return; 
    }
    if(address_enfant == address_parent){
        perror("l'enfant ne peut pas etre son propre parent :/");
        return;
    }

    if(Parente == 'M'){
        address_enfant->pMere = address_parent;
    }
    else if(Parente == 'P'){
        address_enfant->pPere = address_parent;
    }
    
}


int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente){
    int n = fscanf(f, "%d %d %c", pIdEnfant, pIdParent, pParente);
    if(n < 3){ // la fonction doit lire 3 informations
        return 0;
    }
    return 1;

}


tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]){
    FILE* f;
    f = fopen(Fichier, "rt");
    if(f == NULL){
        printf("problem dans l'ouverture du fichier\n");
        return NULL;
    }

    int idEnfant;
    int idParent;
    char parente;

    int n = ArbreLireLienParentef(f, &idEnfant, &idParent, &parente);
    while(n == 1){
        ArbreAjouterLienParente(Arbre, idEnfant, idParent, parente);
        n = ArbreLireLienParentef(f, &idEnfant, &idParent, &parente);
    }

    fclose(f);
    return Arbre;

}


void ArbreEcrireGV(tArbre Arbre, char Fichier[]) {
    FILE* f = fopen(Fichier, "wt");
    if (f == NULL) {
        printf("erreur d'ouverture!!!");
        return;
    }

    fputs("digraph {\n\trankdir=\"BT\";\n\n\tnode [shape=box,color=blue,fontname=\"Arial\",fontsize=10];\n", f);

    //on prend le permier pointeur et on fait un parcours de l'arbre
    struct sFiche* actuel = Arbre->pPremiere;
    while (actuel != NULL) {
        if (IdentiteSexe(actuel->Identite) == 'M') {// pour les hommes
            fprintf(f, "\t%d [label=\"%s\\n%s\\n%s\"];\n",
                IdentiteIdentifiant(actuel->Identite),
                IdentiteNom(actuel->Identite),
                IdentitePrenom(actuel->Identite),
                IdentiteDateNaissance(actuel->Identite));
        }
        actuel = actuel->pSuivante;
    }

    fputc('\n', f);
    fputs("\tnode [color=green];\n", f);

    actuel = Arbre->pPremiere;
    while (actuel != NULL) {
        if (IdentiteSexe(actuel->Identite) == 'F') {// pour les femmes
            fprintf(f, "\t%d [label=\"%s\\n%s\\n%s\"];\n",
                IdentiteIdentifiant(actuel->Identite),
                IdentiteNom(actuel->Identite),
                IdentitePrenom(actuel->Identite),
                IdentiteDateNaissance(actuel->Identite));
        }
        actuel = actuel->pSuivante;
    }

    fputc('\n', f);
    fputs("\tedge [dir=none];\n", f);

    actuel = Arbre->pPremiere;
    while (actuel != NULL) {
        if (actuel->pPere != NULL) {// pour les id -> id
            fprintf(f, "\t%d -> %d;\n",
                IdentiteIdentifiant(actuel->Identite),
                IdentiteIdentifiant(actuel->pPere->Identite));
        }
        if (actuel->pMere != NULL) {
            fprintf(f, "\t%d -> %d;\n",
                IdentiteIdentifiant(actuel->Identite),
                IdentiteIdentifiant(actuel->pMere->Identite));
        }
        actuel = actuel->pSuivante;
    }

    fputs("}\n", f);
    fclose(f);

}


void ArbreAfficherAscendants(tArbre Arbre, int Identifiant){
    struct sFiche* personne = Arbre->pPremiere;
    int personne_trouve = 0;
    while(personne != NULL){
        if(IdentiteIdentifiant(personne->Identite) == Identifiant){
            personne_trouve = 1;
            break;
        }
        personne = personne->pSuivante;
    }

    if(personne_trouve == 0){// pour voir si la personne a ete trouve ou pas
        perror("la personne n'a pas ete trouve!!!");
    }

    AfficherAscRecursive(Arbre, personne, 0);
    // on utilise une autre fonction recurive (mentionnee tout en haut)

}


void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]){
    FILE* f;
    f = fopen(Fichier, "wt");
    if (f == NULL) {
        printf("erreur d'ouverture!!!\n");
        return;
    }

    struct sFiche* personne = Arbre->pPremiere;
    int personne_trouve = 0;
    while(personne != NULL){
        if(IdentiteIdentifiant(personne->Identite) == Identifiant){
            personne_trouve = 1;
            break;
        }
        personne = personne->pSuivante;
    }

    if(personne_trouve == 0){
        perror("la personne n'a pas ete trouve!!!\n");
        fclose(f);
        return;
    }

    fputs("digraph {\n\trankdir=\"BT\";\n\n\tnode [shape=box,fontname=\"Arial\",fontsize=10];\n\n\tedge [dir=none];\n\n\t", f);

    //affichage des informations pour la premiere personne
    if(IdentiteSexe(personne->Identite) == 'M'){
        fputs("node [color=blue];\n\t", f);
    }
    else if(IdentiteSexe(personne->Identite) == 'F'){
        fputs("node [color=green];\n\t", f);
    }
    fprintf(f, "%d [label=\"%s\\n%s\\n%s\"];\n\n\t", 
            IdentiteIdentifiant(personne->Identite),
            IdentiteNom(personne->Identite),
            IdentitePrenom(personne->Identite),
            IdentiteDateNaissance(personne->Identite));
    //affichage des informations de son pere
    if(personne->pPere != NULL){
        AfficherAscRecursiveGV(Arbre, personne, personne->pPere, f);
    }
    //affichage des informations de sa mere
    if(personne->pMere != NULL){
        AfficherAscRecursiveGV(Arbre, personne, personne->pMere, f);
    }

    fputs("}\n", f);
    fclose(f);

}