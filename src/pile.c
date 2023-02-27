#include "pile.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct pile_element {
    struct pile_element *suivant;
    void                *element;
};

static struct pile_element* nouvel_element(void *valeur) {
    struct pile_element *element = malloc(sizeof(struct pile_element));
    element -> element = valeur;
    return element;
}

static void detruire_element(struct pile_element *elt) {
    free(elt);
}

struct _pile {
    struct pile_element *tete;
};

Pile* pile_vide() {
    Pile* nouvelle_pile   = malloc(sizeof(Pile));
    nouvelle_pile -> tete = NULL;
    return nouvelle_pile;
}

void* pile_sommet(Pile const* pile) {
    return pile -> tete -> element;
}

void  pile_ajouter(Pile *pile, void *elt) {
    struct pile_element* nouveau = nouvel_element(elt);
    nouveau -> suivant = pile -> tete;
    pile -> tete = nouveau;
} 

void  pile_supprimer(Pile *pile) {
    if (pile_est_vide(pile)) {
        fprintf(stderr, "Pile vide\n");
        exit(EXIT_FAILURE);
    }
    struct pile_element *a_supprimer = pile -> tete;
    pile -> tete = a_supprimer -> suivant;
    detruire_element(a_supprimer);
}

bool  pile_est_vide(Pile const* pile) {
    return pile -> tete == NULL;
}

void  pile_liberer(Pile *pile) {
    while (!pile_est_vide(pile)) {
        pile_supprimer(pile);
    }
    free(pile);
}