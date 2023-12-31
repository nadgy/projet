# projet
projet système d'exploitation

On veut effectuer en parallèle(En utilisant le modèle producteurs/consommateur) le produit de deux matrices: 
B (n1* m1)  et C (n2 * m2) ⇒ la matrice résultante A=B*C ;

Les matrices sont remplis par des valeurs aléatoires

Les résultats intermédiaires seront placés dans un tampon de taille “T[N]”.

Chaque threads producteurs calcule une ligne de la matrice résultante A et range les résultat dans le tampon T

Les threads consommateurs consomment l'élément T[y]  le place dans la matrice résultante A  au bon emplacement!

q1: Quelles sont les structures de données à utiliser ?
-Le programme utilise des matrices pour représenter les données, notamment les matrices B, C, et A pour effectuer la multiplication matricielle. Il utilise également un tampon circulaire (tableau de structures Element T) pour la communication entre les threads.



q2: Comment allez-vous protéger l'accès à ces données?
- L'accès aux données est protégé à l'aide de verrous (mutex). Les sections critiques, où les threads accèdent et modifient les données partagées (comme le tampon circulaire T), sont entourées de verrous pour assurer une exécution atomique. De plus, des variables conditionnelles (empty et full) sont utilisées pour synchroniser la production et la consommation de données.
- 
q3- quels sont les risques?
- Les principaux risques incluent les conditions de concurrence qui pourraient survenir lors de l'accès concurrentiel au tampon circulaire (T). Ces conditions peuvent provoquer des résultats incorrects lors de la lecture ou de l'écriture des données. De plus, une mauvaise synchronisation entre les threads producteurs et le thread consommateur peut entraîner des blocages (deadlocks) ou des situations où le tampon est soit trop plein (full) soit trop vide (empty), affectant les performances globales du programme.
  

1-Cloner le projet github : projet  ; et le modifier le selon les exigences ci-dessus

2- Pour chaque nouvelle idée créer une nouvelle branche; les autres étudiants peuvent améliorer l'idée en créant une nouvelle branche!

3-Les premières réponses sont mieux notées!

4-Bien gérer les éxceptions 

5-Bien gérer les messages d'erreurs!

