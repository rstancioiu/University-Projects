clear all;

xMin = [5 5 0 0 0 0];
MP1 = [1 2 1 5 0 2];
MP2 = [2 2 1 2 2 1];
MP3 = [1 0 3 2 2 0];
tps1 = [8 15 0 5 0 10];
tps2 = [7 12 2 15 7 12];
tps3 = [8 1 11 0 10 25];
tps4 = [2 10 5 4 13 7];
tps5 = [5 0 8 7 10 25];
tps6 = [5 5 3 12 8 6];
tps7 = [5 3 5 8 0 7];
b = [4800; 4800; 4800; 4800; 4800; 4800; 4800; 350; 620; 485];
A = [tps1; tps2; tps3; tps4; tps5; tps6; tps7; MP1; MP2; MP3];

%------- Calcul comptable ----------------------
[f_compta x_compta benefice] = Comptable(A,b,xMin);
%-----------------------------------------------

%------- Calcul responsable personnel ----------
[f_perso x_perso tempsM4] = ResponsablePersonnel(A,b,xMin,benefice,f_compta);
%-----------------------------------------------

%------- Calcul responsable atelier ------------
[f_atelier x_atelier nbProduits] = ResponsableAtelier(A,b,xMin);
%-----------------------------------------------

%------- Calcul responsable stocks -------------
[f_stock x_stock nbStock] = ResponsableStocks(A,b,xMin,benefice,f_compta);
%-----------------------------------------------


%------- Calcul responsable commercial ---------
[f_commerce x_commerce ecartEA] = ResponsableCommercial(A,b,xMin,benefice,f_compta);
%-----------------------------------------------

%------- Calcul de la matrice des gains --------
[MatriceGain, MatriceGainPourcent] = CalculMatriceGain(f_stock,f_perso,f_compta,f_atelier,f_commerce,x_compta, x_atelier, x_stock, x_commerce, x_perso, benefice, nbProduits, nbStock, ecartEA, tempsM4);
%-----------------------------------------------

