# Projet de programmation système

## Contexte

Le but de ce projet est d'utiliser vos compétences en C et vos connaissances des threads et des processus pour résoudre efficacement un problème de résolution de labyrinthe.
L'idée est d'écrire une première version séquentielle d'un algorithme de résolution de labyrinthe.
Puis, l'objectif sera de paralléliser la résolution de l'algorithme en utilisant des threads.
Enfin, un dernier axe bonus sera de résoudre une instance de labyrinthe à deux entrées et deux sorties.
Dans ce dernier cas, deux joueurs devront résoudre en simultané un labyrinthe.
La différence avec la résolution à une entrée est que pour arriver à résoudre une instance à deux entrées, il faudra synchroniser les joueurs pour qu'ils puissent passer une porte.

## Consignes

L'objectif est de résoudre une instance de labyrinthe.

0. écrire un programme qui génère une instance de labyrinthe simple (à une entrée et une sortie).
1. écrire un premier programme qui résolve une instance de labyrinthe simple.
2. écrire une deuxième méthode de résolution de labyrinthe utilisant des threads pour la résolution de labyrinthe.
    * écrire une première version sans limite du nombre de threads
    * écrire une deuxième version qui devra limiter le nombre de threads à utiliser.
3. Bonus : écrire une troisième méthode qui résoud des instances de labyrinthe à deux joueurs.
    * écrire un nouveau générateur de labyrinthe double (2 entrées et 2 sorties).
    * écrire un programme qui devra créer deux joueurs (deux processus) et qui devra résoudre une instance de labyrinthe à deux joueurs. Pour cette première version il n'y a pas de synchronisation à faire.
    * écrire une deuxième version qui devra synchroniser les deux joueurs pour qu'ils passent une porte en même temps. On pensera à modifier le générateur pour qu'il ajoute une porte sur le chemin menant à la sortie.
