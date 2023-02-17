#pragma once
#include "types.h"
#include <stdbool.h>

typedef struct labyrinthe Labyrinthe;

/* Création du labyrinthe */
Labyrinthe* creer_labyrinthe(Categorie const);

/* Accesseurs */
Content labyrinthe_obtenir(Labyrinthe const*, pos_t const);
pos_t*  labyrinthe_joueurs(Labyrinthe const*, int*);
void    labyrinthe_nombre_joueurs(Labyrinthe const*, int*);
void    labyrinthe_modif_joueurs(Labyrinthe const*, pos_t*, int const*);

bool    labyrinthe_est_entree(Labyrinthe const*, pos_t const);
void    labyrinthe_obtenir_entree(Labyrinthe const*, pos_t*);

bool    labyrinthe_est_sortie(Labyrinthe const*, pos_t const);
void    labyrinthe_obtenir_sortie(Labyrinthe const*, pos_t*);

/* Destruction du labyrinthe */
void detruire_labyrinthe(Labyrinthe*);