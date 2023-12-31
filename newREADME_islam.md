
   Q1:  les structures de données à utiliser :
   

--> Matrices B, C, et A: Utilisez des tableaux bidimensionnels pour stocker les matrices B, C et A.

--> Tampon T[N]: Un tableau de taille N pour stocker les résultats intermédiaires.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

   Q2:  qoui faire pour protéger l'accès à ces données :
   

--> Utilisez des mécanismes de verrous (locks) pour protéger l'accès concurrent aux structures de données partagées, comme les matrices et le tampon.

--> Assurez-vous d'implémenter un mécanisme de synchronisation pour que les producteurs et les consommateurs accèdent aux données de manière ordonnée.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

   Q3:  les risques :

   
--> Conditions de concurrence: Risque de conditions de concurrence si l'accès aux données partagées n'est pas correctement synchronisé.

--> Dépassement de tampon: Risque de dépassement de tampon si les producteurs génèrent des résultats plus rapidement que les consommateurs ne peuvent les traiter.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
