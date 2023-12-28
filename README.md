# projet
projet système d'exploitation

**reponse des questions :**

--> Q1 : Structures de données
  + Matrices B et C :Peuvent être représentées comme des tableaux bidimensionnels en mémoire, où chaque thread producteur peut accéder à des lignes spécifiques.

  + Matrice résultante A :Comme les matrices B et C, elle peut être représentée comme un tableau bidimensionnel en mémoire.

  + Tampon T (Buffer) :Un tampon de type Queue ou Buffer pour stocker les résultats intermédiaires. Assurez-vous que la taille du tampon est suffisamment grande pour éviter le blocage 
                  des producteurs ou des consommateurs.


--> Q2 : Protection de l'accès
  + Synchronisation :Utilisez des mécanismes de synchronisation tels que Mutexes ou Sémaphores pour assurer un accès exclusif aux zones de mémoire partagées, comme les tampons T et la 
                                            matrice résultante A.
    
  + Verrouillage de secteur critique :Divisez la matrice résultante A en secteurs critiques et verrouillez chaque secteur lorsqu'un thread y accède pour éviter les conflits d'écriture.


--> Q3 : Risques
  + Concurrence d'accès :Si l'accès aux données partagées n'est pas correctement synchronisé, cela peut entraîner des résultats incorrects ou incohérents dans la matrice résultante A.
  + 
  + Débordement du tampon :Si la taille du tampon T n'est pas suffisamment grande, cela peut entraîner un blocage des producteurs ou des pertes de données si le tampon est rempli avant 
                                             que le consommateur ne puisse le traiter.
    
  + Deadlocks :Une mauvaise gestion des mécanismes de synchronisation peut conduire à des deadlocks, où les threads se bloquent mutuellement.
  + 
  + Inefficacité de la gestion mémoire :Une mauvaise gestion de la mémoire peut entraîner des fuites de mémoire ou une utilisation excessive de la mémoire, ce qui peut affecter les 
                                          performances du programme.
    
  + Optimisation de la performance :Il peut être nécessaire d'optimiser les algorithmes utilisés pour le calcul des produits matriciels afin de garantir une utilisation efficace des 
                                     ressources matérielles et minimiser les temps d'attente.
    
  + Erreur dans les calculs :Les résultats intermédiaires stockés dans le tampon T doivent être corrects. Des erreurs de calcul peuvent survenir si les threads producteurs ne sont pas 
                                                         correctement mis en œuvre

