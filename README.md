# FTOSearch

Some code for the FTO. Ideally I'd like to have an optimal solver that takes a reasonable amount of time. This is also going to be for computing subgroup distance distributions and probably looking at multi phase solver ideas as well.

Below is the numbering of pieces that I use for defining permutations of pieces. Since the triangles of the second tetrad are equivalent to the first through a z rotation we can use the same numbering and conjugate the moves (e.g. Doing an F move on the second tetrad achieves the same as doing an R move on the first)

![numbering](./draft/numbering.jpg)

## Compiling

This should do the trick if you have a c++ compiler and make installed

make fto

## Running

./obj/fto

## Pruning tables

Full pruning distances for corners and edges are shown below:

Corners (permutation + orientation)
Table size = 11520
0 1
1 16
2 204
3 1893
4 7228
5 2174
6 4
Mean value: 3.98516

Edges (only one possible orientation)
Table size = 239500800
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

Triangles of one tetrad
Table size = 369600
 0 1
 1 8
 2 96
 3 1020
 4 10354
 5 83779
 6 240962
 7 33374
 8 6
Mean value: 5.79818