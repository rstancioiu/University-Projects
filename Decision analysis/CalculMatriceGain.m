function [MatriceGain, MatriceGainPourcent] = CalculMatriceGain(f_stock,f_perso,f_compta,f_atelier,f_commerce,x_compta, x_atelier, x_stock, x_commerce, x_perso, benefice, nbProduits, nbStock, ecartEA, tempsM4)
    %--Comptable
    beneficeAtelier  = f_compta*x_atelier;
    beneficeStock    = f_compta*x_stock;
    beneficeCommerce = f_compta*x_commerce;
    beneficePerso    = f_compta*x_perso;
    
    %--Resp Atelier
    nbProduitsCompta   = f_atelier*x_compta;
    nbProduitsStock    = f_atelier*x_stock;
    nbProduitsCommerce = f_atelier*x_commerce ;
    nbProduitsPerso    = f_atelier*x_perso;
    
    %--Resp stocks
    nbStockCompta   = f_stock*x_compta;
    nbStockAtelier  = f_stock*x_atelier;
    nbStockCommerce = f_stock*x_commerce;
    nbStockPerso    = f_stock*x_perso;

    %--Resp commercial
    ecartEACompta = abs(f_commerce*x_compta);
    ecartEAAtelier = abs(f_commerce*x_atelier);
    ecartEAStock = abs(f_commerce*x_stock);
    ecartEAPerso = abs(f_commerce*x_perso);
    
    %--Resp personnel
    tempsM4Compta = f_perso*x_compta;
    tempsM4Atelier = f_perso*x_atelier;
    tempsM4Stock = f_perso*x_stock;
    tempsM4Commerce = f_perso*x_commerce;
    
    MatriceGain = [ benefice            beneficeAtelier     beneficeStock       beneficeCommerce    beneficePerso
                    nbProduitsCompta    nbProduits          nbProduitsStock     nbProduitsCommerce  nbProduitsPerso
                    nbStockCompta       nbStockAtelier      nbStock             nbStockCommerce     nbStockPerso
                    ecartEACompta       ecartEAAtelier      ecartEAStock        ecartEA             ecartEAPerso
                    tempsM4Compta       tempsM4Atelier      tempsM4Stock        tempsM4Commerce        tempsM4         ];
    MatriceGain = MatriceGain'
                 
    
    [minBenefice minNbProduits maxNbStock maxTempsM4 maxEcartEA] = CalculExtremites();
    
    MatriceGainPourcent =  [    ComputeSatisfactionMax(minBenefice,benefice,benefice)                 ComputeSatisfactionMax(minBenefice,benefice,beneficeAtelier)    ComputeSatisfactionMax(minBenefice,benefice,beneficeStock)            ComputeSatisfactionMax(minBenefice,benefice,beneficeCommerce)          ComputeSatisfactionMax(minBenefice,benefice,beneficePerso)
                                ComputeSatisfactionMax(minNbProduits,nbProduits,nbProduitsCompta)     ComputeSatisfactionMax(minNbProduits,nbProduits,nbProduits)     ComputeSatisfactionMax(minNbProduits,nbProduits,nbProduitsStock)      ComputeSatisfactionMax(minNbProduits,nbProduits,nbProduitsCommerce)    ComputeSatisfactionMax(minNbProduits,nbProduits,nbProduitsPerso)
                                ComputeSatisfactionMin(nbStock,maxNbStock,nbStockCompta)              ComputeSatisfactionMin(nbStock,maxNbStock,nbStockAtelier)       ComputeSatisfactionMin(nbStock,maxNbStock,nbStock)                    ComputeSatisfactionMin(nbStock,maxNbStock,nbStockCommerce)             ComputeSatisfactionMin(nbStock,maxNbStock,nbStockPerso)
                                ComputeSatisfactionMin(ecartEA,maxEcartEA,ecartEACompta)              ComputeSatisfactionMin(ecartEA,maxEcartEA,ecartEAAtelier)       ComputeSatisfactionMin(ecartEA,maxEcartEA,ecartEAStock)               ComputeSatisfactionMin(ecartEA,maxEcartEA,ecartEA)                     ComputeSatisfactionMin(ecartEA,maxEcartEA,ecartEAPerso)
                                ComputeSatisfactionMin(tempsM4,maxTempsM4,tempsM4Compta)              ComputeSatisfactionMin(tempsM4,maxTempsM4,tempsM4Atelier)       ComputeSatisfactionMin(tempsM4,maxTempsM4,tempsM4Stock)               ComputeSatisfactionMin(tempsM4,maxTempsM4,tempsM4Commerce)             ComputeSatisfactionMin(tempsM4,maxTempsM4,tempsM4)             ];
    MatriceGainPourcent = MatriceGainPourcent'
    
    dlmwrite('MatriceGainPourcent.csv', MatriceGainPourcent);
    
end

function [res] = ComputeSatisfactionMax(minimum,maximum,val)
    res = 100.0*(val - minimum)/(maximum-minimum);
end

function [res] = ComputeSatisfactionMin(minimum,maximum,val)
    res = 100.0*(maximum - val)/(maximum-minimum);
end
