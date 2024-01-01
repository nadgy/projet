
                  " Ce projet a été réalisé en collaboration avec ZIOUI Lounes  "
Ce qui concerne les questions voici les réponse :

Q1: Quelles sont les structures de données à utiliser ?

       Matrices (B, C, A) : Utilisées pour stocker les données des matrices B, C et la matrice résultante A.

       Tampon (T) : tampon partagé entre les threads producteurs et consommateurs pour stocker les résultats intermédiaires.
Q2: Comment allez-vous protéger l'accès à ces données?

    on utulise des sémaphores pour synchroniser l'accès aux données partagées entre les threads ( empty , full ,mutex)
    empty :  Gère la quantité d'espace vide dans le tampon qui stocke les résultats intermédiaires
    full :  Gère la quantité d'espace occupé dans le tampon
    mutex : Assure un accès exclusif au tampon partagé entre les threads
Q3: Quels sont les risques?

   Concurrence : Risque d'accès simultané aux données partagées par plusieurs threads    
   interblocage : Risque qu'un ensemble de threads se bloque mutuellement

