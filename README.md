# projet
projet système d'exploitation

On veut effectuer en parallèle(En utilisant le modèle producteurs/consommateur) le produit de deux matrices: 
B (n1* m1)  et C (n2 * m2) ⇒ la matrice résultante A=B*C ;

Les matrices sont remplis par des valeurs aléatoires

Les résultats intermédiaires seront placés dans un tampon de taille “T[N]”.

Chaque threads producteurs calcule une ligne de la matrice résultante A et range les résultat dans le tampon T

Les threads consommateurs consomment l'élément T[y]  le place dans la matrice résultante A  au bon emplacement!

q1: Quelles sont les structures de données à utiliser ?
les matric B et C sont initialisées avec des valeurs aléatoires et la matrice A de resultat et le tableau T Tampon pour stocker les résultats intermédiaires.
q2: Comment allez-vous protéger l'accès à ces données?
les thread producer et consumer et pthread_mutex_t mutex 
q3- quels sont les risques?

1-Cloner le projet github : projet  ; et le modifier le selon les exigences ci-dessus

2- Pour chaque nouvelle idée créer une nouvelle branche; les autres étudiants peuvent améliorer l'idée en créant une nouvelle branche!

3-Les premières réponses sont mieux notées!

4-Bien gérer les éxceptions 

5-Bien gérer les messages d'erreurs!

