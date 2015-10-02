function [f_compta x_compta benefice] = Comptable(A,b,xMin)
    f_compta = [35/6 697/60 73/6 13/10 633/20 1649/60];
    [x_compta,benefice] = linprog(-f_compta, A, b, [], [], xMin, []);
    benefice=-benefice;
end