indexFromCharacter(1, X,[X|_]).
indexFromCharacter(A, X,[_|L]):- indexFromCharacter(A1,X,L),!,A is A1+1.

% get the character on index
characterFromIndex(1,X,[X|_]) :- !.
characterFromIndex(A,X,[_|L]) :- A1 is A-1,characterFromIndex(A1,X,L).