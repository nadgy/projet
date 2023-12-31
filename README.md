
nom : boukabous
prenom : farid
group : 02
# projet

projet système d'exploitation

On veut effectuer en parallèle(En utilisant le modèle producteurs/consommateur) le produit de deux matrices: 
B (n1* m1)  et C (n2 * m2) ⇒ la matrice résultante A=B*C ;

Les matrices sont remplis par des valeurs aléatoires

Les résultats intermédiaires seront placés dans un tampon de taille “T[N]”.

Chaque threads producteurs calcule une ligne de la matrice résultante A et range les résultat dans le tampon T

Les threads consommateurs consomment l'élément T[y]  le place dans la matrice résultante A  au bon emplacement!

q1: Quelles sont les structures de données à utiliser ?
= Structures de Données
Matrices B, C, et A : Utilisez des tableaux bidimensionnels pour représenter les matrices.
Tampon T : Utilisez un tableau (T) pour stocker les résultats .
q2: Comment allez-vous protéger l'accès à ces données?
 = Protection de l'Accès aux Données
Mutex pour le Tampon T : Implémentez un mutex (pthread_mutex_t) pour protéger l'accès concurrentiel au tampon T.
Mutex pour la Matrice A : Si nécessaire, utilisez un autre mutex pour protéger l'accès à la matrice résultante A.
q3- quels sont les risques?
 =  Risques
Problèmes de Concurrence : Assurez une synchronisation adéquate de l'accès aux données partagées (matrices et tampon) pour éviter les conditions de concurrence.
Problèmes de Performance : Une gestion inadéquate des threads peut entraîner des performances dégradées.
1-Cloner le projet github : projet  ; et le modifier le selon les exigences ci-dessus

2- Pour chaque nouvelle idée créer une nouvelle branche; les autres étudiants peuvent améliorer l'idée en créant une nouvelle branche!

3-Les premières réponses sont mieux notées!

4-Bien gérer les éxceptions 

5-Bien gérer les messages d'erreurs!

