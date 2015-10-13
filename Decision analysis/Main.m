clear all;


%------- Calcul comptable ----------------------
[f_compta x_compta benefice] = Comptable();
%-----------------------------------------------

%------- Calcul responsable personnel ----------
[f_perso x_perso tempsM4] = ResponsablePersonnel(benefice,f_compta);
%-----------------------------------------------

%------- Calcul responsable atelier ------------
[f_atelier x_atelier nbProduits] = ResponsableAtelier();
%-----------------------------------------------

%------- Calcul responsable stocks -------------
[f_stock x_stock nbStock] = ResponsableStocks(benefice,f_compta);
%-----------------------------------------------

%------- Calcul responsable commercial ---------
[f_commerce x_commerce ecartEA] = ResponsableCommercial(benefice,f_compta);
%-----------------------------------------------

%------- Calcul de la matrice des gains --------
fonctions = [f_compta;f_atelier;f_stock;f_commerce;f_perso];
solutions = [x_compta x_atelier x_stock x_commerce x_perso];
pointDeMire = [benefice nbProduits nbStock ecartEA tempsM4 ];
[MatriceGain, MatriceGainPourcent] = CalculMatriceGain(fonctions,solutions,pointDeMire);
%-----------------------------------------------

%------- Calcul de la solution optimale --------
[X,solutionOptimale] = SolutionOptimale(benefice,tempsM4,nbProduits,nbStock,ecartEA)
%-----------------------------------------------
