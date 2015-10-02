function [f_commerce x_commerce ecartEA] = ResponsableCommercial(A,b,xMin,beneficeMax,f_compta)
    p1 = [1 0 0 0 -1 0];
    eps=1e-3;
    f_commerce=-p1;
    X = zeros(6,100);
    Y = zeros(1,100);
    pos=1;
    for i=1:100
        b1 = [b; 0; -beneficeMax*i/100];
        A1 = [A; p1; -f_compta];
        [X(:,i),Y(1,i)] = linprog(f_commerce, A1, b1, [] ,[] , xMin, []);
        if(Y(1,i)<eps)
            pos=i;
        end
    end
    figure(1);
    plot(1:100,Y);
    title('Responsable Commercial');
    xlabel('Pourcentage du benefice maximal');
    ylabel('Ecart');
    x_commerce=X(:,pos);
    ecartEA=Y(1,pos);
end