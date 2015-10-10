function [f_atelier x_atelier nbProduits] = ResponsableAtelier()
    [A,b,Aeq,beq,xMin,xMax] = Initialiser();
    f_atelier = [1 1 1 1 1 1];
    [x_atelier,nbProduits] = linprog(-f_atelier, A, b,Aeq, beq, xMin, xMax);
    nbProduits=-nbProduits;
end