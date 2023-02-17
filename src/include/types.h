#pragma once
#include <stdint.h>

/* Décris le contenue d'une cellule d'un labyrinthe */
typedef enum _content {
    OBSTABLE, VIDE, ENTREE, SORTIE
} Content;

/* Catégorise les différents types de labyrinthe */
typedef enum _categorie {
    SIMPLE, /* Un seul joueur résoud le labyrinthe */
    DOUBLE_NO_SYNCHRO, /* Deux joueurs résolvent simultanément et indépendament le labyrinthe */
    DOUBLE_SYNCHRO /* Deux joueurs résolvent simultanément le labyrinthe. Ils devront faire une synchronisation pour passer une porte en même temps */
} Categorie;

typedef int16_t taille_t;
typedef int16_t pos_t;
typedef int8_t  coord_t;

#define TAILLE_LABYRINTHE (1 << sizeof(taille_t))