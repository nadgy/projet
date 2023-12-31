created by draifi abdelhak et lounes mansouri

q1: Quelles sont les structures de données à utiliser ?

Les structures de données comprennent des matrices  (`MatriceB`, `MatriceC`, `MatriceA`), un tampon circulaire (`Tampon`) avec une structure `ElementMatrice`, et des mécanismes de synchronisation tels que des sémaphores (`vide`, `plein`) et un mutex (`mutex`).

q2: Comment allez-vous protéger l'accès à ces données?

L'accès aux données sera protégé par un mutex (`mutex`) pour assurer une exclusion mutuelle lors de l'accès aux structures partagées telles que le tampon circulaire et les matrices. Les sémaphores (`vide`, `plein`) seront utilisés pour la synchronisation entre le producteur et le consommateur.

q3- quels sont les risques?

Les risques comprennent des conditions de concurrence entraînant des résultats imprévisibles. La mauvaise gestion de la synchronisation peut causer la famine ou les blocages des threads.



