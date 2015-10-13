function [X,solutionOptimale] = SolutionOptimale(benefice,tempsM4,nbProduits,nbStock,ecartEA)

    [minBenefice minNbProduits maxNbStock maxTempsM4 maxEcartEA] = CalculExtremites();
    [A,b,Aeq,beq,xMin,xMax] = Initialiser();
    f_atelier = [1 1 1 1 1 1];
    f_commerce=[-1 0 0 0 1 0];
    f_perso = [2 10 5 4 13 7];
    f_stock = [5 5 6 10 5 4];
    f_compta = [35/6 697/60 73/6 13/10 633/20 1649/60];
    i = 50;
    beneficep  = i/100*(benefice-minBenefice)+minBenefice;
    nbProduitsp= i/100*(nbProduits-minNbProduits)+minNbProduits;
    nbStockp   = i/100*(maxNbStock - nbStock)+nbStock;
    tempsM4p   = i/100*(maxTempsM4 - tempsM4)+tempsM4;
    ecartEAp   = i/100*(maxEcartEA - ecartEA)+ecartEA;
    A1=[A;f_compta;-f_compta   ;-f_perso;f_perso   ;f_atelier ;-f_atelier   ;-f_stock;f_stock   ;-f_commerce;f_commerce];
    b1=[b;benefice;-beneficep;-tempsM4;tempsM4p;nbProduits;-nbProduitsp;-nbStock;nbStockp;-ecartEA   ;ecartEAp];
    
    solutionf=[-(f_compta*(100/(benefice-minBenefice)))
               (f_perso*(100/(maxTempsM4 - tempsM4)))
               -f_atelier*(100/(nbProduits-minNbProduits))
               (f_stock*(100/(maxNbStock - nbStock)))
               (f_commerce*(100/(maxEcartEA - ecartEA)))];
    avgf = mean(solutionf)
    
    [x,~] = linprog(avgf,A1,b1,[],[],xMin,[]);
    solution=[ComputeSatisfactionMax(minBenefice,benefice,f_compta*x)
                      ComputeSatisfactionMin(tempsM4,maxTempsM4,f_perso*x)
                      ComputeSatisfactionMax(minNbProduits,nbProduits,f_atelier*x) 
                      ComputeSatisfactionMin(nbStock,maxNbStock,f_stock*x)
                      ComputeSatisfactionMin(ecartEA,maxEcartEA,f_commerce*x)];
    avg=mean(solution)
    solutionOptimale=solution
    X=[f_compta*x;f_perso*x;f_atelier*x;f_stock*x;f_commerce*x];
end

function [res] = ComputeSatisfactionMax(minimum,maximum,val)
    res = 100.0*(val - minimum)/(maximum-minimum);
end

function [res] = ComputeSatisfactionMin(minimum,maximum,val)
    res = 100.0*(maximum - val)/(maximum-minimum);
end