function [] = MatriceContrainte() 
    xMin = [5 5 0 0 0 0];
    xMax = [5000 5000 5000 5000 5000 5000];
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

    [f_stock x_stock nbStock] = ResponsableStocks(A,b,xMin,xMax);
    [f_perso x_perso tempsM4] = ResponsablePersonnel(A,b,xMin,xMax);
    [f_compta x_compta benefice] = Comptable(A,b,xMin,xMax);
    [f_atelier x_atelier nbProduits] = ResponsableAtelier(A,b,xMin,xMax);
    [f_commerce x_commerce ecartEA] = ResponsableCommercial(A,b,xMin,xMax);

    %------- Calcul de la matrice des gains --------
    [MatriceGain, MatriceGainPourcent] = CalculMatriceGain(f_stock,f_perso,f_compta,f_atelier,f_commerce,x_compta, x_atelier, x_stock, x_commerce, x_perso, benefice, nbProduits, nbStock, ecartEA, tempsM4);
    %-----------------------------------------------

end


% ----------- Responsable des stocks -----------
function [f_stock x_stock nbStock] = ResponsableStocks(A,b,xMin,xMax)
    f_stock = [5 5 6 10 5 4];
    x_stock = linprog(f_stock, A, b, zeros(1, 6), 0, xMin, xMax);
    nbStock = f_stock*x_stock;
end
% ----------------------------------------------


% ---------- Responsable du personnel ----------
function [f_perso x_perso tempsM4] = ResponsablePersonnel(A,b,xMin,xMax)
    f_perso = [2 10 5 4 13 7];
    x_perso = linprog(f_perso, A, b, zeros(1, 6), 0, xMin, xMax);
    tempsM4 = f_perso*x_perso;
end
% ----------------------------------------------


% ----------------- Comptable ------------------
function [f_compta x_compta benefice] = Comptable(A,b,xMin,xMax)
    f_compta = [35/6 697/60 73/6 13/10 633/20 1649/60];
    x_compta = linprog(-f_compta, A, b, zeros(1, 6), 0, xMin, xMax);
    %benefice = 35/6*x_compta(1) + 697/60*x_compta(2) + 73/6*x_compta(3) + 13/10*x_compta(4) + 633/20*x_compta(5) + 1649/60*x_compta(6);
    benefice = f_compta*x_compta;
end
% ----------------------------------------------


%------------ Responsable d'atelier ------------
function [f_atelier x_atelier nbProduits] = ResponsableAtelier(A,b,xMin,xMax)
    f_atelier = [1 1 1 1 1 1];
    x_atelier = linprog(-f_atelier, A, b, zeros(1, 6), 0, xMin, xMax);
    nbProduits = f_atelier*x_atelier;
end
% ----------------------------------------------


%----------- Responsable commercial ------------
function [f_commerce x_commerce ecartEA] = ResponsableCommercial(A,b,xMin,xMax)
    eps=0.000025;
    p1 = [1 0 0 0 -1 0];
    p2 = [-1 0 0 0 1 0];
    f_commerce = [1 0 0 0 -1 0];
    b = [b; eps;eps];
    A = [A; p1 ;p2];
    x_commerce = linprog(f_commerce, A, b, zeros(1, 6) , 0, xMin, xMax);
    ecartEA = abs(f_commerce*x_commerce)
end
% ----------------------------------------------
