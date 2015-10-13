MatriceJugement = [ 6 5 5 5
                    5 2 6 7 
                    3 7 5 4
                    5 4 3 9
                    3 5 7 4 ];                
Poids = [1 1 1 1];

MatriceConcordance = CalculMatriceConcordance( MatriceJugement, Poids )
MatriceDiscordance = CalculMatriceDiscordance( MatriceJugement )

SeuilCon = 0.75;
SeuilDis = 0.3;

MatriceClassement = zeros(size(MatriceJugement));

for i=1:size(MatriceJugement,1),
    for j=1:size(MatriceJugement,1), 
        if (MatriceConcordance(i,j)>=SeuilCon && MatriceDiscordance(i,j)<=SeuilDis)
            MatriceClassement(i,j) = 1;
        end
    end
end

MatriceClassement
