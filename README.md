Zeghmiche Dalila
Ouslimane Hanane
Réponses aux questions:

q1: Quelles sont les structures de données à utiliser ?
- Le programme utilise des matrices pour représenter les données, notamment les matrices B, C, et A pour effectuer la multiplication matricielle. Il utilise également un tampon circulaire (tableau de structures Element T) pour la communication entre les threads.

q2: Comment allez-vous protéger l'accès à ces données?
- L'accès aux données est protégé à l'aide de verrous (mutex). Les sections critiques, où les threads accèdent et modifient les données partagées (comme le tampon circulaire T), sont entourées de verrous pour assurer une exécution atomique. De plus, des variables conditionnelles (empty et full) sont utilisées pour synchroniser la production et la consommation de données.

q3: Quels sont les risques?
- Les principaux risques incluent les conditions de concurrence qui pourraient survenir lors de l'accès concurrentiel au tampon circulaire (T). Ces conditions peuvent provoquer des résultats incorrects lors de la lecture ou de l'écriture des données. De plus, une mauvaise synchronisation entre les threads producteurs et le thread consommateur peut entraîner des blocages (deadlocks) ou des situations où le tampon est soit trop plein (full) soit trop vide (empty), affectant les performances globales du programme.
