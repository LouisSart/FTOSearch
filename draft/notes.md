
# Edge table on the Cubie level

26 août 2026 : génération de la table d'edges avec CubieFTO : 1min38s

main.cpp:
```c++
#include "table.hpp"
#include "utils.hpp"

int main(int argc, const char* argv[]) {

    time_fn(generate_edge_table, 1);

    return 0;
}
```

```shell
epicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
g++ -std=c++20 -O1 -Isrc/ -c src/main.cpp -o obj/main.o
g++ -std=c++20 -O1 -Isrc/ obj/main.o obj/fto.o obj/move.o obj/move_table.o obj/solve.o obj/table.o  -o obj/fto
0 1
1 16
2 160
3 1408
4 11712
switch to forwards scan
5 90912
6 644756
7 4070826
8 21433009
9 76410122
10 109897795
switch to backwards scan
11 26611502
12 328215
13 366
Time taken:  98679262  microseconds
```

# Edge table on the coordinate level

28 aout 2026 : génération de la table d'edges avec FTO : 13s (on gagne un facteur 7.5 !)

Attention les chiffres sont pas bons kevin ! En fait il ne s'agit pas de la table exacte mais d'une version
altérée car la parité de la seconde permutation de coordonnée e2 n'est pas prise en compte. On ne calcule
l'index que pour les N - 2 premiers éléments.

```shell
epicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
g++ -std=c++20 -O1 -Isrc/ -c src/main.cpp -o obj/main.o
g++ -std=c++20 -O1 -Isrc/ obj/main.o obj/fto.o obj/move.o obj/move_table.o obj/solve.o obj/table.o  -o obj/fto
0 1
1 16
2 165
3 1565
4 14455
switch to forwards scan
5 126173
6 1006400
7 6980326
8 37432519
9 110062666
10 79992251
switch to backwards scan
11 3883772
12 491
Time taken:  13141790  microseconds
```

On perd un peu en accurracy parce que dans le cas où deux permutations
différentes (aux deux derniers éléments près) ont le même index
mais pas la même distance to solved alors on ne garde que celle qui a la plus basse valeur.
On a donc une heuristique toujours admissible mais de moins bonne qualité.
L'idée de génie qui va me permettre de conserver la permutation exacte après avoir splitté les arêtes est encore à venir.

Je pense m'en satisfaire pour l'instant vu que j'obtiens une mean value assez ok (9.13679 au lieu de 9.55184)
et que j'ai sacrément accéléré la génération de la table.

# Edge index conversion

Bon en fait l'heuristique précédente n'était pas admissible, donc il arrivait que IDA* ne trouve pas de solution à la profondeur de l'optimal. J'ai fini par tabler sur une solution intermédiaire qui ne m'enchante pas : j'ai construit une table de conversion qui transforme la coordonnée de mon split 6/6 (donc sparse car elle autorise toutes les parités) en coordonnée creuse, e.g. la coordonnée équivalente de la permutation complète avec parité paire. Du coup je retombe sur la pruning table optimale, avec comme compromis la construction d'une table de conversion de taille 12! * sizeof(unsigned) (raté pour les économies de RAM) + la fonction set_from_index(c_dense) qui repasse par le cubie level, nécessaire au forward et backward scan. (Trop long avec IDDFS seul).

```shell
epicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
 0 1
 1 16
 2 160
 3 1408
 4 11712
 5 90912
 6 644756
 7 4070826
 8 21433009
 9 76410122
10 109897795
11 26611502
12 328215
13 366
Mean value: 9.55184
Time taken:  62506042  microseconds
```
3 septembre 2026:

Résultat : la table prend 60 sec à être générée, plus la génération de la table de conversion (45s). En gros en temps de calcul je n'ai rien gagné à la génération de table, l'accélération ne se verra qu'en solve. Tout ça pour ça.

Autre idée : Résoudre les arêtes revient à placer les arêtes de chaque tétrade sur leur face (en ignorant la parité). Eh oui car placer l'arête jaune-orange simultanément sur la face jaune et sur la face orange revient à la résoudre entièrement. Ça revient bêtement à construire deux pruning tables de taille 369000, dont l'une est symétrique de l'autre par rotation z. Bon par contre la pruning value associée doit pas être fofolle.

## Comparaison Cubie-level vs Coordinate-level

```shell
eepicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
g++ -std=c++20 -O1 -Isrc/ -Ilib obj/main.o obj/coordinate_fto.o obj/fto.o obj/solve.o  -o obj/fto
D' L bR B D B' D' D' R U bL bR' F' bR // Scramble random moves

// Coordinate-level solve
Searching at depth 8
Nodes generated: 111
Searching at depth 9
Nodes generated: 961
Searching at depth 10
Nodes generated: 9915
Searching at depth 11
Nodes generated: 94425
Searching at depth 12
Nodes generated: 1088675
Searching at depth 13
Nodes generated: 11816273
Solutions found
Time taken:  1173692  microseconds
bR' F bR bL' U' R' D' B D' B' L' bR' D (13)

// Cubie-level solve
Searching at depth 8
Nodes generated: 111
Searching at depth 9
Nodes generated: 961
Searching at depth 10
Nodes generated: 9915
Searching at depth 11
Nodes generated: 94425
Searching at depth 12
Nodes generated: 1088675
Searching at depth 13
Nodes generated: 11816273
Solutions found
Time taken:  5235784  microseconds
bR' F bR bL' U' R' D' B D' B' L' bR' D (13)
```

On gagne un facteur 4 sur un solve à la profondeur 13 à utiliser les coordonnées. C'est bien mais c'est pas aussi fort que d'améliorer la pruning value...

# RLBD Coset size

vendredi 4 septembre 2026 : je m'assure que les tailles du subset RLBD sont bien celles que je pense.

```shell
epicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
g++ -std=c++20 -O1 -Isrc/ -Ilib -c src/main.cpp -o obj/main.o
g++ -std=c++20 -O1 -Isrc/ -Ilib obj/main.o obj/coordinate_fto.o obj/fto.o obj/solve.o  -o obj/fto
Triplet space size 11520 ==> OK
Edge space size 81 ==> OK
```

# Triplet size

8 septembre 2026 : Génération de la table des triplets RLBD, c'est-à-dire coins x triangles de la deuxième orbite.  Ça prend 2 minutes pour une table de 4 Go, c'est raisonnable.

```shell
epicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
g++ -std=c++20 -O1 -Isrc/ -Ilib -c src/solve.cpp -o obj/solve.o
g++ -std=c++20 -O1 -Isrc/ -Ilib -c src/main.cpp -o obj/main.o
g++ -std=c++20 -O1 -Isrc/ -Ilib obj/main.o obj/coordinate_fto.o obj/fto.o obj/solve.o  -o obj/fto
Generating triplet pruning table
0 1
1 16
2 208
3 2688
4 34308
5 423596
6 5050478
switch to forwards scan
7 55900941
8 502136400
9 2212175901
switch to backwards scan
10 1467208642
11 14858685
12 136
Time taken:  129237009  microseconds
```

OK so this ran for about 5-8 hours (I didn't time it) but it found a solution up to depth 18. 

```shell
epicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/coordinate_fto.cpp -o obj/coordinate_fto.o
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/fto.cpp -o obj/fto.o
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/solve.cpp -o obj/solve.o
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/main.cpp -o obj/main.o
g++ -std=c++20 -O3 -Isrc/ -Ilib obj/main.o obj/coordinate_fto.o obj/fto.o obj/solve.o  -o obj/fto
bR' bL F R D bL F' L F bL' D F bL bR' U D' B' bL' (18) // random move scramble
Searching at depth 10
Nodes generated: 43
Searching at depth 11
Nodes generated: 871
Searching at depth 12
Nodes generated: 14487
Searching at depth 13
Nodes generated: 213979
Searching at depth 14
Nodes generated: 2893583
Searching at depth 15
Nodes generated: 38918947
Searching at depth 16
Nodes generated: 516419817
Searching at depth 17
Nodes generated: 2516961769
Searching at depth 18
Nodes generated: 3528804185
Solutions found
bL B U' D bR bL' F' D' bL F' L' F bL' D' R' F' bL' bR (18)
```

# Pruning improvements
(10 septembre 2026)

J'ai amélioré le pruning et ça me permet de générer environ 35-40% moins de noeuds aux profondeurs 15/16 sur le mélange de la section précédente. L'idée c'est que la triplet value (corners X triangles) est valables pour la première tétrade comme pour la deuxième, à une conjugaison par un z move près. Je transforme l'index de coins par une conjugaison z, que je combine pour calculer l'index des triplets de la deuxième tétrade. J'ai plus qu'à lookup dans la table des triplets pour avoir une nouvelle estimate à ajouter à la fonction générale d'estimation.

```shell
epicier@ACAB:~/Documents/FTOSearch$ make OPT="-O3" fto && ./obj/fto 
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/coordinate_fto.cpp -o obj/coordinate_fto.o
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/fto.cpp -o obj/fto.o
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/solve.cpp -o obj/solve.o
g++ -std=c++20 -O3 -Isrc/ -Ilib -c src/main.cpp -o obj/main.o
g++ -std=c++20 -O3 -Isrc/ -Ilib obj/main.o obj/coordinate_fto.o obj/fto.o obj/solve.o  -o obj/fto
bR' bL F R D bL F' L F bL' D F bL bR' U D' B' bL' (18) // même mélange que dans la section précédente
Searching at depth 10
Nodes generated: 43
Searching at depth 11
Nodes generated: 745
Searching at depth 12
Nodes generated: 11169
Searching at depth 13
Nodes generated: 151401
Searching at depth 14
Nodes generated: 1935473
Searching at depth 15
Nodes generated: 24835791
Searching at depth 16
Nodes generated: 316494387
Searching at depth 17
^C
```

EDIT : Je viens de tester un mélange à la profondeur 13 comme dans la section "Comparaison Cubie-level vs Coordinate-level". Avec la nouvelle heuristique on a accéléré la recherche d'un facteur ~22. Y a de quoi se féliciter :)

# Encore des algos

Swap les triangles de deux faces
bL' U' D' bR U' bR' R F' U' F U R' D bL U' (15)
bL F B bR' F bR R' U F U' F' R B' bL' F (15)
bR' F' B' bL F' bL' L U' F' U F L' B bR F' (15)
bR U D bL' U bL L' F U F' U' L D' bR' U (15)
F' bL B R' F U F' U' R bR' F' bR F' B' bL' (15)
F bR' B' L F' U' F U L' bL F bL' F B bR (15)
U' bR D L' U F U' F' L bL' U' bL U' D' bR' (15)
U bL' D' R U' F' U F R' bR U bR' U D bL (15)

Faire un U move mais qu'avec les triangles
R' L R F' U F U' R' L' R U' F' U F (14)

Le sune permet de faire une U perm
D' R D R D' R D R (8)
D R D' R D R D' R (8)
B' R B R B' R B R (8)
B R B' R B R B' R (8)
L' R L R L' R L R (8)
L R L' R L R L' R (8)
R D' R D R D' R D (8)
R D R D' R D R D' (8)
R B' R B R B' R B (8)
R B R B' R B R B' (8)
R L' R L R L' R L (8)
R L R L' R L R L' (8)

Un autre "3-cycle d'arêtes" (awkward):
B U F L F' U' B' D bR R' bR' D' (12)

Un sledge de coins pur en 12 (je crois que les triangles ne bougent pas)
R' L F L' R U' R' L F L' R U' (12) // marrant de regarder ce qui arrive à l'arête FD

Cette espèce de commutateur permet de faire deux cycles d'edges
sur deux faces de la même orbite
D R D' L D' B' D L' (8)
B' D L' D R D' L D' (8)
B' L' D L' R L D' L (8)
L' D L' R L D' L B' (8)
L' D R D' L D' B' D (8)
L' R L D' L B' L' D (8)
L D' B' D L' D R D' (8)
L D' L B' L' D L' R (8)
L B' L' D L' R L D' (8)
R D' L D' B' D L' D (8)
R L D' L B' L' D L' (8)

Un 2T2T qui peut servir pour TCP
D' B L' bL B' L U D L R' F L' R (13)
D R' L F' R L' U' D' L' B bL' L B' (13)
B L' bL B' L U D L R' F L' R D' (13)
B L' R D' B bR' D B' R' bL' B' L U' (13)
R' L B' D R' bR D' R F B R L' U (13)
R' L F' R L' U' D' L' B bL' L B' D (13)
U' L R' F' B' R' D bR' R D' B L' R (13)
U L' B R bL B D' bR B' D R' L B' (13)

# Nouvelle coordonnée d'arête

24 septembre 2026 : On reprend l'idée du 3 septembre. On définit une nouvelle coordonnée EComb qui représente la position des arêtes modulo leur appartenance à chacune des faces R, L, B et D. Ça nous donne donc un objet Center<12, 4> qui a un cardinal de 369600 (comme les triangles), qu'on pourra composer avec les coins pour faire une pruning value relativement bonne.

```
Generating edge comb pruning table
0 1
1 8
2 72
switch to forwards scan
3 560
4 3642
5 19470
6 74184
switch to backwards scan
7 153339
8 106923
9 11337
10 64
```

25 septembre 2026 : test de solve optimal avec la nouvelle checker pruning value
mon mélange de test : // auto scramble = Sequence<Move>{bR2, bL, F, R, D, bL, F2, L, F, bL2, D, F, bL, bR2, U, D2, B2, bL2};

```
Generating checker pruning table
0 1
1 16
2 208
3 2688
4 34332
5 424659
6 5040598
switch to forwards scan
7 54708259
8 473608911
9 2093331215
switch to backwards scan
10 1605069411
11 25571546
12 156
```

```
epicier@ACAB:~/Documents/FTOSearch$ make fto && ./obj/fto 
g++ -std=c++20 -O1  -Isrc/ -Ilib -c src/main.cpp -o obj/main.o
g++ -std=c++20 -O1  -Isrc/ -Ilib obj/main.o obj/coordinate_fto.o obj/fto.o obj/rlbd.o obj/solve.o  -o obj/fto
bR' bL F R D bL F' L F bL' D F bL bR' U D' B' bL' (18) // Mon mélange favori
Searching at depth 10
Nodes generated: 17
Searching at depth 11
Nodes generated: 633
Searching at depth 12
Nodes generated: 9383
Searching at depth 13
Nodes generated: 124145
Searching at depth 14
Nodes generated: 1618935
Searching at depth 15
Nodes generated: 21045765
Searching at depth 16
Nodes generated: 271568621
Searching at depth 17
Nodes generated: 3489569077
Searching at depth 18
```

On obtient un gain d'environ 15% noeuds par depth et on consomme 8.6 Go de RAM (deux tables de 4.3 Go).

# RLBD reduction avec la nouvelle coordonnée d'arêtes

Pruning value pour le deuxième set d'arêtes.

Generating edge reduction pruning table
0 81
switch to forwards scan
1 624
2 5070
3 31040
4 119591
switch to backwards scan
5 180659
6 32362
7 170
8 3