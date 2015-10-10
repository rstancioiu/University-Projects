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
[MatriceGain, MatriceGainPourcent] = CalculMatriceGain(f_stock,f_perso,f_compta,f_atelier,f_commerce,x_compta, x_atelier, x_stock, x_commerce, x_perso, benefice, nbProduits, nbStock, ecartEA, tempsM4);
%-----------------------------------------------

