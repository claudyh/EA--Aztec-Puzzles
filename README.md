## Aztec Puzzles
*in C++*

&nbsp;

### 📋 Problem A: Aztec Vaults
---

Given a grid where each 2x2 block shares a rotation handle, and rotating a handle cycles the surrounding 4 cells, find the minimum number of rotations needed to sort the grid so that row i contains only the value i+1, within a maximum number of allowed moves. Solved with backtracking, using several pruning strategies: skipping redundant/diagonal handles, freezing already-solved rows, and estimating a lower bound on moves from the total misplaced-cell distance.

**Input**
- The input starts with one line containing one integer T that corresponds to the number of test cases. Then, T input blocks follow. Each input block starts with a line containing the number of rows R and columns C of the grid G, and the maximum number of moves allowed M, separated by whitespace. Then, R lines follow, each representing a row of the vault grid, containing C integers separated by whitespace.

**Output**
- For each test case, print the minimum number of moves S required to restore the vault grid to its correct position in the format "S\n". If no solution is found, output "the treasure is lost!\n".

**Constraints:**
- 2 <= R <= 5 (rows)
- 2 <= C <= 5 (columns)
- R <= C
- 1 <= M <= 7 (max moves)
- 1 <= G[i][j] <= R

**Example:**
- For the 2x3 grid below with a maximum of 5 moves, only two vault handles exist to rotate. Rotating the first handle twice and the second handle once (3 moves total) sorts the grid:
```
2 2 1        2 2 1        1 2 1        1 1 1
       -->           -->         -->
2 1 1        1 2 1        2 2 1        2 2 2
```

&nbsp;

### 📋 Problem B: Aztec Riddle
---

Given a grid, count the number of distinct ways to place skulls in it so that each row contains an exact number of skulls and each column contains an exact number of skulls. Solved with backtracking enumeration, optimized with dynamic programming: identical remaining row/column constraint states (regardless of column order) are memoized so repeated subproblems aren't recomputed.

**Input**
- The input starts with one line containing one integer T that corresponds to the number of test cases. Then, T input blocks follow. Each input block starts with a line containing two integers, the number of rows and columns of the grid. Then, a line follows with two integers: the exact number of skulls required in each row, and the exact number of skulls required in each column, respectively.

**Output**
- For each test case, print the total number of distinct valid skull placements satisfying the row and column constraints.

**Constraints:**
- Number of skulls required per column or row never exceeds 7
- Number of rows and columns, and number of skulls required per row, are only bounded implicitly (must be non-negative and no larger than the grid itself)

**Example:**
- For a 2x2 grid needing exactly 1 skull per row and exactly 1 skull per column, there are 2 valid configurations:
```
1 0         0 1
0 1   and   1 0
```

&nbsp;

### 📋 Problem C: Aztec Maze
---

Given a maze with a door, an exit, and manholes that flood the maze with water once entered, find where to place a single floodgate (on an edge) and a limited number of manhole covers so that a safe path exists from the door to the exit. Solved by modeling the maze as a graph, finding bridges with a DFS-based algorithm, and identifying a bridge that disconnects enough manholes from the path side to make the remaining ones coverable.

**Input**
- The first line gives the number of test cases (at most 10). Each test case starts with a line containing two integers N and M, the number of rows and columns of the maze. The following N lines give the maze itself, where each character is `#` (wall), `.` (empty space), `E` (exit, always on the first line), `D` (door, always on the last line), or `M` (manhole). The last line of the test case gives an integer C, the number of manhole covers available (guaranteed to be less than the number of manholes in the maze).

**Output**
- Four space-separated integers r1 c1 r2 c2 giving the location of the floodgate (between two adjacent cells). Then a line with the number of manhole covers used, followed by that many lines with their coordinates. Then a line with the number of steps in the safe path, followed by that many lines with the path's coordinates, starting at the door and ending at the exit.

**Constraints:**
- 1 <= N, M <= 500
- C is guaranteed to be less than the number of manholes in the maze

&nbsp;

---

**NOTE:** The first two problems were correctly solved, however the last one still has a bug: the bridge-finding DFS should start from the door node, but it doesn't, so the chosen floodgate can be wrong on some inputs. This project was developed within the scope of a Computer Science course by Cláudia Torres and Diogo Silva.
