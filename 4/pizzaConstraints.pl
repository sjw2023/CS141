chalkboard([[hawaiian, HT1, HT2, HP],
	    [marcopolo, MPT1, MPT2, MPP],
	    [pepperoni, PT1, PT2, PP],
	    [super, ST1, ST2, SP],
	    [ninja, NT1, NT2, NP]]) :-
permutation([HT1, MPT1, PT1, ST1, NT1], [ham, mussels, prawns, salami, tuna]),
permutation([HT2, MPT2, PT2, ST2, NT2], [avocado, corn, olives, pineapple, tomato]),
permutation([HP, MPP, PP, SP, NP], [5, 6, 7, 8, 9]),
\+ error(hawaiian, HT1, HT2, HP),
\+ error(marcopolo, MPT1, MPT2, MPP),
\+ error(pepperoni, PT1, PT2, PP),
\+ error(super, ST1, ST2, SP),
\+ error(ninja, NT1, NT2, NP), !.

%facts!
error(hawaiian, T1, _, P) :- \+ T1 == mussels; P < 7.
                        %error if T1 is not mussels,*or*P is not over 6
error(marcopolo, T1, T2, _) :- \+ T2 == tomato ; T1 == ham.
                        %error exists if T2 is not tomato *or*T1 is ham
error(_, ham, _, P) :- \+ P == 8.
                        %error exists if the pizza with ham is not $8
error(pepperoni, _, _, P) :- \+ P == 7.
			%error exists if the pizza with pepperoni is not $7
error(super, _, pineapple, _).
                  %error exists if the super pizza has pineapple
error(_, tuna, corn, 6).
                  %error exists if the pizza with tuna & corn is $6
error(_, tuna, T2, _) :- \+ T2 == corn.
                  %error exists if the pizza with tuna
error(_, T1, corn, _) :- \+ T1 == tuna.
error(_, _, olives, P) :- \+ P == 5.
error(_, salami, olives, _).
error(_, _, pineapple, 9).

% extra functions.
permutation([H1|T1], L2) :- member(H1, L2), remove(H1, L2, L22), permutation(T1, L22).
permutation([H1], [H1]).

member(E, [E|_]).
member(E, [_|T]) :- member(E, T).

remove(E, [E|T], T).
remove(E, [X|T], [X|R]) :- remove(E, T, R).
