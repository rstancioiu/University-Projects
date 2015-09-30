woman(marie).
woman(julie).
woman(beatrice).
woman(caroline).
woman(salome).
man(mathieu).
man(pierre).
man(raphael).
man(peter).
mother(caroline,julie).
mother(marie,julie).
mother(julie,beatrice).
father(marie,pierre).
father(julie,raphael).
father(caroline,mathieu).
father(mathieu,peter).
mother(mathieu,salome).


sisters(X,Y) :- woman(X), woman(Y), mother(X,Z), mother(Y,Z),X\==Y.
same_parents(X,Y) :- sisters(X,Y), father(X,Z), father(Y,Z).
grandmother(X,Y) :- (mother(X,Z),mother(Z,Y));(father(X,Z),mother(Z,Y)).
grandfather(X,Y) :- (mother(X,Z),father(Z,Y));(father(X,Z),father(Z,Y)).


