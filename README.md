q1: Quelles sont les structures de données à utiliser ?

Les structures de données utilisées dans le code sont :

- Les pointeurs doubles (**B, **C, **A) pour représenter les matrices B, C, et A. Ce sont des tableaux 2D alloués dynamiquement avec malloc. La taille de ces tableaux dépend des dimensions des matrices.
- Un tableau double (T[N]) pour représenter le tampon où sont stockés les résultats intermédiaires.
- Un mutex pthread_mutex_t pour assurer l'exclusion mutuelle lors de l'accès au tampon.
- Deux sémaphores sem_t (empty, full) pour gérer le tampon.
- Les tableaux pthread_t (producers[N], consumers[N]) pour stocker les threads producteurs et consommateurs.

q2: Comment allez-vous protéger l'accès à ces données?

L'accès aux données partagées (le tampon T) est protégé à l'aide d'un mutex et de sémaphores :

- Le mutex assure qu'un seul thread peut accéder au tampon à la fois. Ceci est réalisé en verrouillant le mutex avant qu'un thread n'accède au tampon et en le déverrouillant après que le thread ait fini avec le tampon.
- Les sémaphores sont utilisés pour gérer le tampon. Le sémaphore 'empty' indique le nombre de places vides dans le tampon et le sémaphore 'full' indique le nombre de places dans le tampon qui sont remplies avec des données. Avant qu'un thread producteur puisse insérer un élément dans le tampon, il décrémente le sémaphore 'empty'. Après avoir inséré l'élément, il incrémente le sémaphore 'full'. De même, avant qu'un thread consommateur puisse retirer un élément du tampon, il décrémente le sémaphore 'full'. Après avoir retiré l'élément, il incrémente le sémaphore 'empty'.

q3: Quels sont les risques?

Les risques associés à ce programme sont principalement liés aux problèmes de concurrence :

- Les courses de données : Si le mutex n'est pas correctement utilisé, plusieurs threads pourraient accéder et modifier le tampon simultanément, menant à des résultats incohérents et erronés.
- Les interblocages : Si le mutex et les sémaphores ne sont pas correctement utilisés, le programme pourrait atteindre un état où chaque thread attend que l'autre libère une ressource, provoquant un blocage indéfini du programme.
- La famine : Si un thread est continuellement refusé l'accès à une ressource (dans ce cas, le tampon), il pourrait être affamé et incapable de progresser.
- Les débordements / sous-débordements dans le tampon : Si les sémaphores ne sont pas correctement utilisés, le tampon pourrait déborder (plus d'articles produits que le tampon peut contenir) ou sous-déborder (une tentative de consommer un article lorsque le tampon est vide).
