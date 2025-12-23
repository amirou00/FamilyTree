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
    if(Arbre == NULL){
        perror("arbre non alloue");
        return; // ce n'est pas une erreur d'allocation, l'arbre n'existe meme pas, donc le programme se termine...
    }

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
    struct sFiche* suivant = NULL; // pour l'instant il pointe vers rien, on l'initialise dans la boucle
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


