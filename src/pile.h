#pragma once
#include <stdbool.h>

typedef struct _pile Pile;

Pile* pile_vide();

void* pile_sommet(Pile const*);

void  pile_ajouter(Pile *, void *);

void  pile_supprimer(Pile *);

bool  pile_est_vide(Pile const*);

void  pile_liberer(Pile *);