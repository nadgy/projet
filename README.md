# projet
chekroune amina , groupe 2

q1: Quelles sont les structures de données à utiliser ?

Un tableau T de taille N * N, qui sert de tampon circulaire pour stocker les données produites par les producteurs et consommées par les consommateurs.
Trois variables de type sem_t, qui sont des sémaphores utilisés pour contrôler le nombre de cases vides et pleines dans le tampon, ainsi que pour synchroniser les producteurs et les consommateurs.

    .empty, qui indique le nombre de cases vides dans le tampon. Il est initialisé à N * N, la taille maximale du tampon, et il est décrémenté par les producteurs avant d’insérer un élément, et incrémenté par les consommateurs après avoir retiré un élément.

    .full, qui indique le nombre de cases pleines dans le tampon. Il est initialisé à 0, car le tampon est vide au début, et il est incrémenté par les producteurs après avoir inséré un élément, et décrémenté par les consommateurs avant de retirer un élément.

    .mutex, qui est un sémaphore binaire (qui ne peut prendre que les valeurs 0 ou 1) utilisé comme un verrou pour protéger la section critique où les éléments sont insérés ou retirés du tampon. Il est initialisé à 1, ce qui signifie qu’il est disponible, et il est acquis par les threads en utilisant la fonction pthread_mutex_lock, et relâché par les threads en utilisant la fonction pthread_mutex_unlock.

Deux tableaux threads_prod et threads_cons de taille N, qui contiennent les identifiants des threads producteurs et consommateurs. Ces tableaux sont utilisés pour créer et joindre les threads en utilisant les fonctions pthread_create et pthread_join.

Trois matrices A, B et C de taille N * N, qui sont utilisées pour stocker les données aléatoires générées par les producteurs, et pour afficher les résultats à la fin du programme.

q2: Comment allez-vous protéger l’accès à ces données ?

Les sémaphores empty et full sont utilisés pour réguler le nombre d’éléments dans le tampon, et pour empêcher que les producteurs n’insèrent des éléments dans un tampon plein, ou que les consommateurs ne retirent des éléments d’un tampon vide.
 Les fonctions sem_wait et sem_post sont utilisées pour décrémenter ou incrémenter les sémaphores, et pour bloquer ou réveiller les threads selon la valeur des sémaphores.

Le sémaphore mutex est utilisé comme un verrou pour protéger la section critique où les éléments sont insérés ou retirés du tampon. 
Seul un thread à la fois peut acquérir le verrou en utilisant la fonction pthread_mutex_lock, et le relâcher en utilisant la fonction pthread_mutex_unlock

q3: Quels sont les risques ?
Le risque de surproduction ou de sous-consommation, qui se produit lorsque les producteurs produisent plus d’éléments que les consommateurs ne peuvent en consommer, ou inversement. Cela peut entraîner un gaspillage de ressources

Le risque d’interblocage, qui se produit lorsque deux ou plusieurs threads attendent indéfiniment une ressource détenue par un autre thread, sans pouvoir progresser. Cela peut arriver si les threads ne respectent pas un ordre d’acquisition des sémaphores, ou si un thread oublie de relâcher un sémaphore après avoir terminé son travail.



