#      FTOSearch

Some code for the FTO. Ideally I'd like to have an optimal solver that takes a reasonable amount of time. This is also going to be for computing subgroup distance distributions and probably looking at multi phase solver ideas as well.

Below is the numbering of pieces that I use for defining permutations of pieces. Since the triangles of the second tetrad are equivalent to the first through a z rotation we can use the same numbering and conjugate the moves (e.g. Doing an F move on the second tetrad achieves the same as doing an R move on the first)

##     Compiling

This should do the trick if you have a c++ compiler and make installed

`make fto`

##     Running

`/obj/fto <option>`

Note that move tables and pruning tables will be generated on the first run and stored on disk in directories `./move_tables` and `./pruning_tables`.

Available options : 
- `optimal` : Generate a 13 random move scramble and solve it optimally
- `rlbd` : Generate a random scramble of moves from {R,L,B,D} and solve it optimally
- `reduction` : Generate a 13 random move scramble and bring it back to the rlbd subgroup

##     Pruning tables

Full pruning distances for corners and edges are shown below:

```
 - Corners (permutation + orientation)
        Table size = 11520
        0 1
        1 16
        2 208
        3 1764
        4 6439
        5 2957
        6 135
        Mean value: 4.08637

 - Edges (only one possible orientation)
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

 - Edge combination for the RLBD orbit
       0 1
       1 8
       2 72
       3 560
       4 3642
       5 19470
       6 74184
       7 153339
       8 106923
       9 11337
       10 64
       Mean value: 7.00834


 - Triangles of one tetrad
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
```

###    Triplets

Since the corner and triangle space for one tetrad (orbit) are small enough, we can combine them to get a bigger table (4.3 GB) and better value :

```
 - Triplets table (corners X triangles of one tetrad)
        Table size = 4257792000
        0 1
        1 16
        2 208
        3 2688
        4 34308
        5 423596
        6 5050478
        7 55900941
        8 502136400
        9 2212175901
        10 1467208642
        11 14858685
        12 136
        Mean value: 9.20338
```

One big advantage of using the triplet coordinate is that it can be reused for the second tetrad after applying a z conjugation to the corners. This gives two pruning values for a given position from the same table.

### Checkers

When solving the corners and placing the edges on their respective RLBD faces (regardless of permutation), we get a triangular "checkerboard" pattern. This is what I call the checker pruning value of size 4.3 GB. Similarly to the triplet value, it can be reused for the second orbit using a z shift of the corners.

```
- Checker table (corners X edge comb of one tetrad)
        Table size = 4257792000
       0 1
       1 16
       2 208
       3 2688
       4 34332
       5 424659
       6 5040598
       7 54708259
       8 473608911
       9 2093331215
       10 1605069411
       11 25571546
       12 156
       Mean value: 9.24806
```

##     The RLBD subgroup

When you scramble the FTO using only moves from the subset <R,R',L,L',B,B',D,D'> you get a position that belongs to a subgroup of the full space that I call the RLBD subgroup. Here is the distribution of positions in this subgroup.

```
- RLBD table
       Table size = 933120
       0 1
       1 8
       2 48
       3 288
       4 1728
       5 9896
       6 51808
       7 220111
       8 480467
       9 166276
       10 2457
       11 32
       Mean value: 7.79549
```

##     RLBD reduction

One idea for a two phase solver of the FTO is to bring the scrambled state into the RLBD subgroup, because then the optimal finish will be very fast to compute. RLBD reduction comes down to:

 - Solving the triangles of the RLBD tetrad (orbit)
 - Bringing back the edges to their respective RLBD face (in a way that they can be solved with only one move)
 - Forming the triplets of the second tetrad (connecting the corners with their respective triangles)

Here are the distributions of distance to the RLBD subgroup for some groups of pieces :

```
 - Full edge table (size 239500800)
       0 81
       1 648
       2 5832
       3 49248
       4 360612
       5 2375406
       6 13472244
       7 56072493
       8 114094818
       9 51533010
       10 1534788
       11 1539
       12 81
       Mean value: 7.84441

 - Edge comb table for the second orbit (size 369600)
       0 81
       1 624
       2 5070
       3 31040
       4 119591
       5 180659
       6 32362
       7 170
       8 3

 - Triplet table (size 4257792000)
       0 11520
       1 89856
       2 1000800
       3 10102680
       4 94276584
       5 647412036
       6 2146831308
       7 1337965452
       8 20101764
       Mean value: 6.11916
```