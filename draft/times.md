
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