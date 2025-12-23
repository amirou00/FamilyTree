#include <stddef.h>
#include <stdio.h>
#ifndef IDENTITE_H
#define IDENTITE_H

// Type permettant de manipuler l’identité d’une personne
typedef struct sIdentite *tIdentite;

extern tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]);
extern int IdentiteIdentifiant(tIdentite Identite);
extern char *IdentiteNom(tIdentite Identite);
extern char *IdentitePrenom(tIdentite Identite);
extern char IdentiteSexe(tIdentite Identite);
extern char *IdentiteDateNaissance(tIdentite Identite);
extern void IdentiteAfficher(tIdentite Identite);
extern void IdentiteLiberer(tIdentite Identite);
extern tIdentite IdentiteLiref(FILE *f);
   
#endif