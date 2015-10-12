function [f_stock x_stock nbStock] = ResponsableStocks(beneficeMax,f_compta)
    [A,b,Aeq,beq,xMin,xMax] = Initialiser();
    f_stock = [5 5 6 10 5 4];
    X=zeros(6,100);
    Y=zeros(1,100);
    for i=1:100
        b1 = [b; -beneficeMax*i/100];
        A1 = [A; -f_compta];
        [X(:,i),Y(1,i)] = linprog(f_stock, A1, b1, [] ,[] , xMin, []);
    end
    figure(3);
    plot(1:100,Y);
    title('Responsable du stock');
    ylabel('Nombre stock');
    xlabel('Pourcentage du benefice maximal');
    pos=63;
    x_stock = X(:,pos);
    nbStock = Y(1,pos);
end