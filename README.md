Halouane Yacine 

q1: Quelles sont les structures de données à utiliser ? 
R1: Pour concrétiser ce projet de système d'exploitation, nous pouvons adopter les structures de données suivantes : 1. Matrices B, C et A : Utilisation de tableaux bidimensionnels pour représenter les matrices B, C et A, par exemple, déclarant int B[n1][m1], int C[n2][m2] et int A[n1][m2]. 2. Tampon T : Utilisation d'un tableau unidimensionnel pour symboliser le tampon T. Sa taille est de N éléments, pouvant être déclarée avec int T[N].

q2: Comment allez-vous protéger l'accès à ces données?
R2: Afin de garantir un accès sécurisé aux données partagées entre les threads producteurs et consommateurs, nous pouvons recourir à des mécanismes de synchronisation tels que des sémaphores ou des mutex. Voici comment les appliquer : 1. Mutex pour chaque ligne de la matrice A : Chaque thread producteur verrouillera la ligne de la matrice A qu'il calcule à l'aide d'un mutex avant d'écrire dans le tampon T. 2. Sémaphore pour le tampon T : Utilisation d'un sémaphore pour réguler l'accès au tampon T. Les producteurs doivent acquérir le sémaphore avant d'écrire dans le tampon, et les consommateurs doivent le libérer après avoir consommé un élément.

q3: Quels sont les risques?
R3: 1. Conditions de concurrence : Des résultats incorrects peuvent survenir si plusieurs threads tentent de modifier les mêmes données simultanément. Les mutex et les sémaphores nous aident à éviter cela. 2. Dépassement de tampon : Si les producteurs génèrent des données plus rapidement que les consommateurs ne peuvent les traiter, le tampon T pourrait déborder. Cela peut être évité en ajustant les taux de production et de consommation ou en redimensionnant le tampon. 3. Blocage : Une utilisation incorrecte des mutex ou des sémaphores peut entraîner un blocage où les threads restent bloqués
