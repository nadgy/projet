Q1: Quelles structures de données devraient être utilisées ?

-Matrices B, C et A :
Celles-ci peuvent être représentées sous forme de tableaux 2D ou de matrices.

-Buffer T :
Un tampon circulaire ou une structure de file d'attente peut être utilisé pour implémenter le tampon T. Ce tampon contiendra les résultats intermédiaires produits par les threads producteurs.

Q2: Comment protégerez-vous l'accès à ces données ?

-Mutex (Exclusion Mutuelle) :
Utilisez des verrous mutex pour garantir qu'un seul thread peut accéder aux données partagées (par exemple, aux matrices, au tampon) à la fois. Cela empêche les conditions de concurrence et la corruption des données.

-Sémaphore :
Des sémaphores peuvent être utilisés pour contrôler l'accès au tampon T. Les sémaphores peuvent signaler quand le tampon est disponible pour l'écriture ou la lecture.

Q3: Quels sont les risques ?

-Conditions de Course :
Sans une synchronisation adéquate, des conditions de course peuvent survenir, entraînant des résultats incorrects ou une corruption des données.

-Interblocages :
Une utilisation inefficace des mécanismes de synchronisation peut conduire à des interblocages, où les threads restent bloqués indéfiniment en attendant les uns les autres.

-Incohérence des Données :
Si la synchronisation n'est pas correcte, un accès concurrent aux données partagées peut entraîner des données incorrectes ou incohérentes.
