

# 8-Puzzle-Solver
This program asks the user to input unique numbers from 0-8 in a 3x3 grid, that will serve as the input for the 8- puzzle. The program will then utilize IDS and A* search to find the solution, solution path, solution cost, number of nodes expanded, and the running time.

<br>

<img src="assets/ui.png" alt="UI" width="80%">

<img src="assets/worst.png" alt="Astar_worst_mode" width="80%"><br>

---

<br>

## **GAME RULES**

|  |  |  |
|:--:|:--:|:--:|
|  | ↑ |  |
| ← | `_` | → |
|  | ↓ |  |


An 8 puzzle is a simple game consisting of a 3 x 3 grid (containing 9 squares). One of the squares is empty. The blank tile can only be moved to the adjacent cells.
The objective is to move the empty square around into different positions and reaching the state of the numbers as displayed in the ***"goal state"***.

<br>

## **GAME MODES**

<table>
<tr><th>Default Goal State </th><th>[1] Easy mode</th></th><th>[2] Medium mode</th></th><th>[3] Hard mode</th></th><th>[4] Worst mode</th></tr>
<tr><td>

|  |  |  |
|:--:|:--:|:--:|
| 1 | 2 | 3 |
| 8 | `_` | 4 |
| 7 | 6 | 5 |

</td><td>

|  |  |  |
|:--:|:--:|:--:|
| 1 | 3 | 4 |
| 8 | 6 | 2 |
| 7 | `_` | 5 |

</td><td>

|  |  |  |
|:--:|:--:|:--:|
| 2 | 8 | 1 |
| `_` | 4 | 3 |
| 7 | 6 | 5 |

</td><td>

|  |  |  |
|:--:|:--:|:--:|
| 2 | 8 | 1 |
| 4 | 6 | 3 |
| 7 | 5 | `_` |

</td><td>

|  |  |  |
|:--:|:--:|:--:|
| 5 | 6 | 4 |
| 4 | `_` | 8 |
| 3 | 2 | 1 |

</td></tr>

</table>

<br>
Aside from the pre-set starting configuration of the puzzle, the user can also give their desired starting config.<br>
Similarly, the user can also provide their own desired goal state to try to reach.<br>
The program will promptly give a warning if the goal state is impossible to reach, from the starting position.<br>

<br>

- ## mode [5] - Preferred Initial Configuration
    - The user shall input the desired Initial state. The program will alert the user if ever the default goal state is impossible to reach when starting from the chosen initial state.u<br><img src="assets/initial.png" alt="" width="70%"><br>

- ## mode [6] - Preferred Initial & Final State
    - The user may choose and enter their own desired Initial state, as well as the targe Goal state. The program will alert the user if ever the chosen goal state is impossible to reach when starting from the chosen initial state.<br><img src="assets/goal.png" alt="" width="70%"><br>


- ## mode [7] - Debug mode
    - Additional information about how the algorithm is solving the problem is printed. This helps make sure the program is behaving as intended, especially during development.<br><img src="assets/ids_debug.png" alt="" width="70%"><br>

<br>

---

## Algorithms used:

<br><img src="assets/ids.png" alt="IDS pseudocode" width="80%"><br>

- **Iterative Deepening Search(IDS)**<br>
IDS (or IDDFS) is a state space/graph search strategy in which a depth-limited version of depth-first search is run repeatedly with increasing depth limits until the goal is found. It isa a graph traversal and path search algorithm, which is used in many fields of computer science due to its completeness, optimality, and optimal efficiency. One major practical drawback is its O(b^d) space complexity, as it stores all generated nodes in memory.

<br><img src="assets/astar.png" alt="A* pseudocode" width="80%"><br>

- **A * Search Algorithm**<br>
A* search algorithm is an informed search algorithm using heuristics (in this case, manhattan distance) to estimate the cost to reach the goal, and making informed decisions based on that cost. It keeps track of the nodes based on their estimated cost (f-value), where f(n) = g(n) + h(n), g(n) is the cost of the path from the start node to node n, and h(n) is the heuristic estimate of the cost from node n to the goal.


Using IDS on problems that require more than 16 steps is will take an unreasonable long time because there is no optimization to prune repeated states in the search tree.
*The algorithms in this program could be further optimized to remember, then ignore repeated states (pruning).*

---

<br>

## **FINDINGS**

| Algorithm | Completeness | Time Complexity | Space Complexity | Optimality |
|:--:|:--:|:--:|:--:|:--:|
| IDS | YES (no infinite paths) | *O* **(b^d)** | *O* **(bd)** | Yes **if step cost is constant* |
| A* | Yes | *O* **(b^d)** | *O* **(b^d)** | Yes |

**where b is the average branching factor of the search tree and d is the depth of the goal node.*

<br>

- **IDS** is a variation of depth-first search (DFS) that addresses the limitations of DFS, such as potentially getting stuck in deep branches and not being able to find a solution within a reasonable time or memory constraint. needs to search until the end of the tree branch. Repeated generation of already explored states makes the time complexity expensive.

- Although **A*** search is optimal, the space complexity is a drawback because it needs to store all the explored states. In the worst case, where the A* algorithm explores the entire search space, the time complexity of A* search with the Manhattan distance heuristic is exponential. However, in practice, A* search is often much more efficient than the worst-case scenario because of the heuristic function. The Manhattan distance heuristic is admissible, meaning it never overestimates the cost of reaching the goal state. With an admissible heuristic, A* search is guaranteed to find the optimal solution. Additionally, the heuristic helps guide the search towards the goal state, reducing the number of unnecessary states explored.


<br><br>

---

# MEMBERS

- BALANA, Renmar
- BORRERO, Jan Lance
- SABAS, Jessa Lorenza

*BSCS 3-C (2021-2022)*
