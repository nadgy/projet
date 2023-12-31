project se2   groupe 3

amir bouferkas
ben arouche saber

Q1: les principales structures de données sont :

- Matrices A, B, et C . Ces matrices représentent les données d'entrée (B et C) et de sortie (A) du produit matriciel.

- Tableau T comme buffer pour le transfert de données entre les threads. 
les threads producteurs calcule les line de la matrice A a partir de les matrice B et C en suit les stocke dans le buffer T pour que les threads consommateurs peux retirer les valuers et les places au bon emplacement dans la matrice A.

-Sémaphores pour la synchronisation entre les threads producteurs et consommateurs. 
par utilise un tableu de semaphore pour synchronise les pairs producteur/consommateur , et un semaphore mut pour sécuriser l'accès a T


Q2: sécuriser l'accès a le buffer T en utilisant :

-le semaphore mutex (mut) :

le semaphore mutex (mut) est utilisé pour protéger l'accès à le buffer T.
Un thread producteur doit acquérir ce mutex avant de remplir le buffer (T), et le thread consommateur doit également acquérir ce mutex apres de vider le tampon.

-Tableau de Sémaphores (mut_pro_con[N]) :

chaque sémaphore du tableau correspondant à un pair producteur/consommateur spécifique. Ces sémaphores sont initialisées à 0, ce qui signifie que les threads consommateurs doivent attendre une permission avant d'accéder au tampon. le thread producteur donne cette permission en postant la sémaphore correspondante après avoir rempli le tampon.



Q3 : les reisque sont :

-Risque que deux threads modifient les mêmes données simultanément. Le mutex (mut) est utilisé pour éviter cela.
-Risque de blocage si les threads ne libèrent pas correctement les semaphores . Il est crucial de gérer les semaphores de bon manière.
-Risque de performance , une synchronisation excessive peut affecter les performances. Il faut minimiser les sections critiques pour éviter le blocage excessif des threads .
-Resque d'accès concurrent aux données partagées . Si les données partagées ne sont pas synchronisées correctement, cela peut conduire à des résultats incorrects.
