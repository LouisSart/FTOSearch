
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