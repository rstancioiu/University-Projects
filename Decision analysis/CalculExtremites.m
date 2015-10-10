function [minBenefice minNbProduits maxNbProduits maxTempsM4 maxEcartEA] = CalculExtremites()
    [A,b,Aeq,beq,xMin,xMax] = Initialiser();
    xMin = [5 5 0 0 0 0];
    f_compta = [35/6 697/60 73/6 13/10 633/20 1649/60];
    f_atelier = [1 1 1 1 1 1];
    f_stock = [5 5 6 10 5 4];
    f_perso = [2 10 5 4 13 7];
    f_commerce = [1 0 0 0 -1 0];
    [x_compta,minBenefice] = linprog(f_compta, A, b, Aeq, beq, xMin, xMax);
    [x_atelier,minNbProduits] = linprog(f_atelier, A, b, Aeq, beq, xMin, xMax);
    [x_stock,maxNbProduits] = linprog(-f_stock, A, b, Aeq, beq, xMin, xMax);
    maxNbProduits=-maxNbProduits;
    [x_perso,maxTempsM4] = linprog(-f_perso, A, b, Aeq, beq, xMin, xMax);
    maxTempsM4=-maxTempsM4;
    [x_commerce,ecartEA1] = linprog(f_commerce, A, b, Aeq, beq, xMin, xMax);
    [x_commerce,ecartEA2] = linprog(-f_commerce, A, b, Aeq, beq, xMin, xMax);
    maxEcartEA = max(abs(ecartEA1),abs(ecartEA2));
    
end