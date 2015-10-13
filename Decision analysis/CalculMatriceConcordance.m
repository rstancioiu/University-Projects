function [ MatriceConcordance ] = CalculMatriceConcordance( MatriceJugement, Poids )      
    nPoids = sum(Poids);
    
    %--Init matrice concordance
    MatriceConcordance = zeros(size(MatriceJugement,1));
    %-- 

    for i=1:size(MatriceJugement,1),
        for j=1:size(MatriceJugement,1)
            n = 0;
            for k=1:size(Poids,2),
                if MatriceJugement(i,k) >= MatriceJugement(j,k)
                    n = n + Poids(k);
                end
            end
            MatriceConcordance(i,j) = n/nPoids;
        end
    end
end