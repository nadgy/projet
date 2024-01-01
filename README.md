Q1: Quelles sont les structures de données à utiliser ?

Matrices (B, C, A) : Utilisées pour stocker les données des matrices B, C et la matrice résultante A.

Tampon (T) : tampon partagé entre les threads producteurs et consommateurs pour stocker les résultats intermédiaires.


Q2: Comment allez-vous protéger l'accès à ces données?


empty :  Gère la quantité d'espace vide dans le tampon qui stocke les résultats intermédiaires
full :  Gère la quantité d'espace occupé dans le tampon
mutex : Assure un accès exclusif au tampon partagé entre les threads

3: Quels sont les risques?

Concurrence : Risque d'accès simultané aux données partagées par plusieurs threads    
