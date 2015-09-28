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
    ecartEACompta = f_commerce*x_compta;
    ecartEAAtelier = f_commerce*x_atelier;
    ecartEAStock = f_commerce*x_stock;
    ecartEAPerso = f_commerce*x_perso;
    
    %--Resp personnel
    tempsM4Compta = f_perso*x_compta;
    tempsM4Atelier = f_perso*x_atelier;
    tempsM4Stock = f_perso*x_stock;
    tempsM4Commerce = f_perso*x_commerce;
    
    MatriceGain = [ benefice            beneficeAtelier     beneficeStock       beneficeCommerce    beneficePerso
                    nbProduitsCompta    nbProduits          nbProduitsStock     nbProduitsCommerce  nbProduitsPerso
                    nbStockCompta       nbStockAtelier      nbStock             nbStockCommerce     nbStockPerso
                    ecartEACompta       ecartEAAtelier      ecartEAStock        ecartEA             ecartEAPerso
                    tempsM4Compta       tempsM4Atelier      tempsM4Stock        ecartEAStock        tempsM4         ]
                 
    MatriceGainPourcent = 100*[ benefice/benefice               beneficeAtelier/benefice    beneficeStock/benefice          beneficeCommerce/benefice       beneficePerso/benefice
                                nbProduitsCompta/nbProduits     nbProduits/nbProduits       nbProduitsStock/nbProduits      nbProduitsCommerce/nbProduits   nbProduitsPerso/nbProduits
                                nbStockCompta/nbStock           nbStockAtelier/nbStock      nbStock/nbStock                 nbStockCommerce/nbStock         nbStockPerso/nbStock
                                ecartEACompta/ecartEA           ecartEAAtelier/ecartEA      ecartEAStock/ecartEA            ecartEA/ecartEA                 ecartEAPerso/ecartEA
                                tempsM4Compta/tempsM4           tempsM4Atelier/tempsM4      tempsM4Stock/tempsM4            tempsM4Commerce/tempsM4         tempsM4/tempsM4             ]
                                
end
