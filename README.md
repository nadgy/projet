#projet

q1: Quelles sont les structures de données à utiliser ?
    =B qui represente la matrice B.
    =C qui represente la matrice C.
    {Les matrices B et C sont initialisees avec des valeurs aleatoires}
    =A qui represente le resultat de la multiplication de B*C.
    =BUFFER qui est un tableau temporaire utilise comme un tompon
    -L'utilisation des tableaux dynamiques pour les matrices.
    -L'utilisation d'un tableau dynamique qui represente le Tampon.

q2: Comment allez-vous protéger l'accès à ces données?
    -Il n'est pas vraiment important de proteger les matrices B et C puisque les deux ne serront pas modifier ou qu'une errur se produit quand on les access au meme temps.
    -L'utilisations de PTHREAD_MATRIX_LOCK and UNLOCK pour proteger l'access au Tampon (avec le semaphores matrix)
    -L'utilisations des semaphores empty,full pour gerer la synchronisation entre producteurs et consommateurs.

q3- quels sont les risques?
    -Pour l'instant, le code peut gerer les matrices carrees
    et des fois non-carrees. Si on essaie d'utiliser des matrices non carrees, cela peut provoquer une erreur de segmentation.
    -avec le risque d'avoir le code vole :)

