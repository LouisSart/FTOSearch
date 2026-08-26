
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