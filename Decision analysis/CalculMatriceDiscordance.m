function [ MatriceDiscordance ] = CalculMatriceDiscordance( MatriceJugement )
    %--Init matrice concordance                
    MatriceDiscordance = zeros(size(MatriceJugement,1));
    %--

    %--Init echelle max
    echmax = 10;
    %--

    for i=1:size(MatriceJugement,1)
        for j=1:size(MatriceJugement,1)
            n = 0;
            nsuivant = 0;
            for k=1:size(MatriceJugement,2),
                nsuivant = MatriceJugement(i,k) - MatriceJugement(j,k);
                if nsuivant > n
                    n = nsuivant;
                end
            end
            valeur = n/echmax;
            MatriceDiscordance(i,j) = valeur;
        end
    end
end