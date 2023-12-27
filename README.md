# Multiplication des Matrices

Ce programme en langage C fait la multiplication des matrices à l'aide du paradigme producteur-consommateur, en utilisant des threads et des sémaphores pour la synchronisation  et la consommation .

## Utilisation

- Pour lancer ce programme, executez la commande suivante:

```
gcc -o matrice matrice.c && ./matrice
```

- Saisez les dimensions des matrices B et C, ainsi que la taille du tampon

Les matrices B et C sont initialisées avec des valeurs aléatoires, Le programme alloue dynamiquement de la mémoire pour les matrices et initialise les sémaphores.

Les threads producteur et consommateur sont créés pour effectuer la multiplication de matrices et copier les résultats qui sont stockés dans un tampon, Les sémaphores sont utilisées pour gérer la synchronisation entre les threads. Une fois les threads terminés, le programme affiche la matrice résultante A et libère la mémoire allouée dynamiquement. 



## Réponses aux questions du projet

**q1): Quelles sont les structures de données à utiliser ?**

r1):les structures de données principales à utiliser sont :

- B, C, A : des tableaux dynamiques (alloués dynamiquement à l'aide de malloc) représentent les matrices B, C, et la matrice résultante A. 
B : Tableau pour stocker les éléments de la matrice B.
C : Tableau pour stocker les éléments de la matrice C.
A : Tableau pour stocker les résultats de la multiplication.

- T : Tableau temporaire utilisé comme tampon pour stocker les indices des lignes produites.

- les semaphores : sem_empty, sem_full, mutex ,ils sont utilisés pour la synchronisation entre les threads producteur et consommateur afin d'éviter les problèmes de concurrence.

- Variables d'indices : ils sont utilisées pour contrôler l'accès concurrent aux lignes des matrices B, C, et A.
nextRowToProduce : Variable indiquant la prochaine ligne à produire.
nextRowToConsume : Variable indiquant la prochaine ligne à consommer.


**q2): Comment allez-vous protéger l'accès à ces données?**

r2):On peux protégé l'accès aux données en utilisant des sémaphores et un mutex.

- Mutex (mutex) : Le mutex est utilisé pour assurer un accès exclusif aux sections critiques du code, Dans notre cas, il est utilisé pour protéger les variables nextRowToProduce et nextRowToConsume.(sem_t mutex)


- Sémaphores (sem_empty, sem_full) : Les sémaphores sont utilisés pour coordonner la production et la consommation.

- sem_empty: est utilisé pour suivre le nombre de places vides dans le tampon T (capacité restante pour les lignes à produire).

- sem_full: est utilisé pour suivre le nombre de places occupées dans le tampon T (lignes prêtes à être consommées).c

On a utilisé sem_wait et sem_post pour gérer l'accès au tampon T de manière synchronisée entre les threads producteur et consommateur.Ces mécanismes de synchronisation assurent que les opérations sur les compteurs nextRowToProduce et nextRowToConsume, ainsi que l'accès aux données partagées, sont réalisées de manière cohérente et évitent les conditions de concurrence.


**q3): quels sont les risques?**

r3): Certains des risques potentiels dans le code sont incluent :

- Conditions de concurrence :
On a utilisé des sémaphores et un mutex pour éviter les conditions de concurrence lors de l'accès aux données partagées. Cependant, si ces mécanismes de synchronisation ne sont pas correctement mis en œuvre, cela peut entraîner des conditions de concurrence, telles que la lecture ou la modification simultanée de données par plusieurs threads.

- interblocage :
Un interblocage se produit lorsqu'un ensemble de threads est bloqué parce que chaque thread attend qu'une ressource soit libérée par un autre thread.L'utilisation incorrecte des sémaphores et des mutex peut conduire à des interblocage.

- Boucle infinie :
On a utilisé la boucle while (1) dans les fonctions produceRow et consumeRow et cela peuvent conduire à des boucles infinies si les conditions de terminaison ne sont pas correctement gérées. Il est important de s'assurer que toutes les conditions de sortie de la boucle sont correctement définies pour éviter un blocage.

- Allocation dynamique de mémoire : 
Il est essentiel de libérer correctement la mémoire avec free pour éviter les fuites de mémoire.

Pour minimiser ces risques, il est essentiel de concevoir et de tester soigneusement le programme, en s'assurant que les mécanismes de synchronisation (mutex, sémaphores) sont correctement utilisés, et en évitant les situations propices aux conditions de course, aux deadlocks et à la starvation. Les tests approfondis et l'analyse statique du code peuvent contribuer à identifier et à résoudre ces problèmes.

README.md
Affichage de README.md en cours...