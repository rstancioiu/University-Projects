function [A,b,Aeq,beq,xMin,xMax] = Initialiser()

    xMin = [5 5 0 0 0 0];
    MP1 = [1 2 1 5 0 2];
    MP2 = [2 2 1 2 2 1];
    MP3 = [1 0 3 2 2 0];
    tps1 = [8 15 0 5 0 10];
    tps2 = [7 12 2 15 7 12];
    tps3 = [8 1 11 0 10 25];
    tps4 = [2 10 5 4 13 7];
    tps5 = [5 0 8 7 10 25];
    tps6 = [5 5 3 12 8 6];
    tps7 = [5 3 5 8 0 7];
    b = [4800; 4800; 4800; 4800; 4800; 4800; 4800; 350; 620; 485];
    A = [tps1; tps2; tps3; tps4; tps5; tps6; tps7; MP1; MP2; MP3];
    xMax=[];
    beq=[];
    Aeq=[];

end