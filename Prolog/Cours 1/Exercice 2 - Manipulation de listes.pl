% append an element to list
append([ ], L1, L1).
append([A|L1],L2,[A|L3]) :- append(L1,L2,L3).

% delete an element from list
del(_,[],[]) :- !.
del(X,[Y|L1],L2) :- X=Y,del(X,L1,L2).
del(X,[Y|L1],[Y|L2]) :-del(X,L1,L2).

% inverse a list
inv([],[]).
inv([X|L1],L2) :- inv(L1,Ret),append(Ret,[X],L2).

% substitution of an element in a list
subsAll(_,_,[],[]).
subsAll(P1,P2,[P1|L1],[P2|L2]) :- subsAll(P1,P2,L1,L2).
subsAll(P1,P2,[Y|L1],[Y|L2]) :- Y\=P1,subsAll(P1,P2,L1,L2).
