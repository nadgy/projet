

### q1: Quelles sont les structures de données à utiliser ? 
Les structures de données utilisées dans ce programme sont principalement des tableaux à deux dimensions (B, C, A) pour représenter les matrices et le tampon T est implémenté comme un tableau unidimensionnel simulant une structure à deux dimensions.


### q2: Comment allez-vous protéger l'accès à ces données?
L'accès aux données est protégé par un mutex (pthread_mutex_t mutex) qui assure l'exclusion mutuelle lors de l'accès aux structures de données partagées (T, A) Et deux sémaphores ( full et empty) sont utilisés pour la synchronisation entre les producteurs et les consommateurs, contrôlant l'accès aux emplacements vides et pleins du tampon.


### q3: quels sont les risques? 
Les risques potentiels comprennent les conditions de concurrence si l'accès aux données n'est pas correctement synchronisé. 
Il y a le risque de blocage si le tampon est plein et aucun consommateur n'est disponible pour libérer de l'espace ou vice versa.
