function [MatriceGain, MatriceGainPourcent] = CalculMatriceGain(fonctions, solutions, pointDeMire)

    MatriceGain = zeros(5,5);
    for i=1:5
        for j=1:5
            MatriceGain(i,j) = abs(fonctions(j,:)*solutions(:,i));
        end
    end
    MatriceGain 
    
    [minBenefice, minNbProduits, maxNbStock, maxTempsM4, maxEcartEA] = CalculExtremites();
    extremites = [minBenefice minNbProduits maxNbStock maxEcartEA maxTempsM4 ];
    MatriceGainPourcent = zeros(5,5);
    for i=1:5
        for j=1:5
            if j<=2
                MatriceGainPourcent(i,j) = ComputeSatisfactionMax(extremites(j), pointDeMire(j), MatriceGain(i,j));
            else
                MatriceGainPourcent(i,j) = ComputeSatisfactionMin(pointDeMire(j), extremites(j), MatriceGain(i,j));
            end
        end
    end
    MatriceGainPourcent
    
    dlmwrite('MatriceGainPourcent.csv', MatriceGainPourcent);
    
end

function [res] = ComputeSatisfactionMax(minimum,maximum,val)
    res = 100.0*(val - minimum)/(maximum-minimum);
end

function [res] = ComputeSatisfactionMin(minimum,maximum,val)
    res = 100.0*(maximum - val)/(maximum-minimum);
end
