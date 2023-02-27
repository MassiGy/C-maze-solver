#include "labyrinthe.h"
#include <stdlib.h>
#include <time.h>
#include "pile.h"

struct labyrinthe {
    Content *cells;
    pos_t    entree, sortie;
    pos_t    joueur;
};

#define ACCESS(x, y) ((x) + ((y) << sizeof(coord_t)))

static void _get_arround(Content const* cells, Content *content, pos_t pos) {
    int cpt = 0;
    for (coord_t i = -1; i <= 1; i+=2) {
        for (coord_t j = -1; j <= 1; j+=2) {
            content[cpt ++] = cells[pos + ACCESS(j, i)];
        }
    }
}

static void fill_labyrinthe(Labyrinthe *l) {
    srand(time(NULL));
    coord_t _entry    = (rand() % (TAILLE_LABYRINTHE - 4)) + 2;
    pos_t   pos_entry = ACCESS(1, _entry);
    l -> cells[pos_entry] = VIDE;

    Pile *stack = pile_vide();

    

    pile_liberer(stack);
}

/* Création du labyrinthe */
Labyrinthe* creer_labyrinthe(Categorie const categorie) {
    Labyrinthe *labyrinthe = malloc(sizeof(Labyrinthe));
    labyrinthe -> cells    = calloc(sizeof(Content), TAILLE_LABYRINTHE);
}

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
void detruire_labyrinthe(Labyrinthe* l) {
    free(l -> cells);
    free(l);
}