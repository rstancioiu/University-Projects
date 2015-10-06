
del(_,[],[]) :- !.
del(X,[Y|L1],L2) :- X=Y,del(X,L1,L2).
del(X,[Y|L1],[Y|L2]) :-del(X,L1,L2).

append([ ], L1, L1).
append([A|L1],L2,[A|L3]) :- append(L1,L2,L3).

% delete duplicates
list2ens([],[]):-!.
list2ens([X|L1],L2):- list2ens(L1,Ret),del(X,Ret,L3),append([X],L3,L2).
