# Projet.SE  
                     

# Q1/Structures de données utilisées :

Matrices (B, C, A) : Utilisées pour stocker les données des matrices B, C et la matrice résultante A.
Tampon (buffer) : Partagé entre les threads producteurs et consommateurs pour stocker les résultats intermédiaires de la multiplication de matrices.

# Q2/Protection de l'accès aux données :
Utilisation de sémaphores pour synchroniser l'accès aux données partagées entre les threads :
empty : Gère la quantité d'espace vide dans le tampon qui stocke les résultats intermédiaires.
full : Gère la quantité d'espace occupé dans le tampon.
mutex : Assure un accès exclusif au tampon partagé entre les threads.

# Q3/Risques potentiels :

Concurrence : Risque d'accès simultané aux données partagées par plusieurs threads, pouvant entraîner des incohérences ou des résultats erronés.
Interblocage : Risque qu'un ensemble de threads se bloque mutuellement en attendant des ressources ou en verrouillant des ressources de manière cyclique.



  # Multiplication de Matrices avec Threads
Ce programme C réalise la multiplication de matrices en utilisant des threads pour un calcul parallèle efficace. Il suit les étapes suivantes :

##Fonctionnalités du programme :
Initialisation :
L'utilisateur est invité à saisir :
La taille du tampon (BUFFER_SIZE).
Les dimensions des matrices C et B.
Les matrices C et B sont remplies avec des valeurs aléatoires.
Vérifications :
Vérification que BUFFER_SIZE est supérieur à zéro.
Vérification que les dimensions des matrices C et B sont comprises entre 1 et 100.
Vérification que la multiplication de matrices est possible (le nombre de colonnes de C est égal au nombre de lignes de B).
Allocation de mémoire :
Allocation d'un tampon (buffer) de taille BUFFER_SIZE pour stocker les résultats intermédiaires de la multiplication de matrices.
Initialisation des sémaphores et du mutex :
Initialisation de deux sémaphores (empty et full) et d'un mutex pour la synchronisation entre les threads.
Création des threads :
Des threads producteurs sont créés pour calculer les éléments de la matrice résultante (matrice A).
Des threads consommateurs sont également créés pour extraire les résultats intermédiaires du tampon et les placer dans la matrice A.
Fonctions de producteur et de consommateur :
Le producteur calcule les éléments de la matrice résultante en multipliant les lignes de C par les colonnes de B, puis stocke les résultats dans le tampon.
Le consommateur extrait les éléments du tampon et les place dans la matrice résultante A.
Attente des threads :
Le programme principal attend la fin de l'exécution de tous les threads producteurs et consommateurs.
Affichage de la matrice résultante :
Une fois la multiplication terminée, la matrice résultante A est affichée à l'écran.
Libération de la mémoire et destruction des sémaphores et du mutex :
Libération de l'espace mémoire alloué pour le tampon.
Destruction des sémaphores et du mutex utilisés pour la synchronisation des threads.
