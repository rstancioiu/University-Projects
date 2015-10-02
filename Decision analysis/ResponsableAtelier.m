function [f_atelier x_atelier nbProduits] = ResponsableAtelier(A,b,xMin)
    f_atelier = [1 1 1 1 1 1];
    [x_atelier,nbProduits] = linprog(-f_atelier, A, b, [], [], xMin, []);
    nbProduits=-nbProduits;
end