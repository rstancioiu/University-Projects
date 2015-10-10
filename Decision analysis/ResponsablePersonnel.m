function [f_perso x_perso tempsM4] = ResponsablePersonnel(beneficeMax,f_compta)
    [A,b,Aeq,beq,xMin,xMax] = Initialiser();
    f_perso = [2 10 5 4 13 7];
    X=zeros(6,100);
    Y=zeros(1,100);
    f_compta
    for i=1:100
        b1 = [b; -beneficeMax*i/100];
        A1 = [A; -f_compta];
        [X(:,i),Y(1,i)] = linprog(f_perso, A1, b1, [], [] , xMin, []);
    end
    figure(2);
    plot(1:100,Y);
    title('Responsable du personnel');
    ylabel('Temps');
    xlabel('Pourcentage du benefice maximal');
    pos=63;
    x_perso = X(:,pos);
    tempsM4 = Y(1,pos);
end