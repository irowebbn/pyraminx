# README
A* on the (Master) Pyraminx

University of Kentucky

CS463G Fall 2020

### Author's Notes

This document will briefly describe how the desired behavior is implemented.
This document is written in Markdown style, which can be read in its raw text
form or rendered with header and list styling. If you prefer to read with the
rendered styles, you can open this file in an editor/viewer that supports it, or
you can use a utility such as pandoc to convert to the format of your choosing. 

For an introduction to the pyraminx puzzle, the notation for solving used in the
autosolver output, or an overview of how the puzzle is implemented, see 
`README_Assignment1_Model.md`

## How to run
To build this program, ensure you have a C++ compiler that supports C++11. Run
`make` to build the program. `make release` or `make debug` will also build the 
program, but with/without optimization and with/without debugging symbols. 
To run the autosolver, run `./autosolve`. Logs are written to log.txt.
To run the interactive puzzle from assignment 1, run `./interactive`.

This has been successfully tested with the Apple clang version 11.0.3 compiler
on a x86_64-apple-darwin19.5.0 architecture.

## How it works

The autosolver begins by instantiating a `Pyraminx` object to represent the
puzzle being solved. It will sequentially generate and solve 5 puzzles that have
been scrambled *k* times with `Pyraminx::scramble()`. The value *k* starts at 3 
and increases to 20, although at this point in time, most machines will run out
of memory near the k=8 mark.

The solver uses A* search, which requires a heuristic, which is described below.
It can be visualized as a tree search, where we explore the branches with the
lowest heuristic value. Each node represents a certain configuration of the
puzzle, and the edges are moves that can be applied to the puzzle. This is 
represented in the code as the `node` structure. It has 3 members: the cost to
of the node, a pointer to the parent node, and the move type required to
move from the parent node to itself.

The cost of the node is the function *f = g + h* where *g* is the cost of 
reaching the node, which is equivalent to the number of the layer at which the 
node exists i.e., the number of moves to that point, and *h* is the heuristic 
value estimating the number of moves remaining to solve.

The pointer is the address of the parent node, so the path from root to leaf 
nodes can be created. Each node is manually allocated when it is first visited, 
and all nodes release their memory when a solved state is reached.

The move type is one of the 32 choices described in assignment 1: 4 corners, 
each with 4 layers to turn in 2 directions. It exists as the structure 
`move_type` in the code.

As nodes are created, they are added to a priority queue, called `min_heap`
because it uses a compare function to pop a node with a minimum-value 
heuristic (there may be more than one with the same minimum, it doesn't matter
which is returned). This queue represents the "search frontier".

Once the pyraminx is scrambled, the root node (no move applied, no parent node)
is inserted into the queue, and the search begins.  A node is popped from the 
queue: in this first case it is the root node, in future iterations, it will be 
the most promising option from the search frontier. The sequence of moves from 
the root to the popped node is applied using `apply_moves`. `apply_moves` works 
by adding the move for a node to the move list, and then repeats for its parent 
node, until the root node is reached. The pyraminx is then checked to see if it 
is in a solved state. If it is, the instance is finished. If not, the popped
node is expanded, meaning the node representing the next 32 possible moves are
generated and added to the queue. We then undo the "test moves" applied to the 
pyraminx to reset to the original scrambled state before we try the next path.
We then pop another node from the queue to repeat the process.

When a puzzle is solved, the sequence of moves that serve as the solution are 
printed below the initial visualization of the scrambled puzzle. The solution 
can be easily verified by opening another terminal and running `./interactive`, 
which runs the program from assignment 1. If starting from a solved state, you 
can apply the listed moves in reverse, with the directions flipped 
(i.e., U3` = U3), and the final result will be the scrambled puzzle that was 
printed at the start of the autosolver instance.

## Heuristic
The heuristic implemented here is the same one proposed in assignment one.
First, we must count all the out of place pieces. The term "piece" here is
distinct from the concept of a face position as described in the data structure,
because on the physical cube, faces from adjacent sides can both be on the same
three-dimensional piece (a small tetrahedron with 2 or 3 faces exposed). We can
classify these pieces four ways:  4 centers (one face exposed, fixed), 24
interior pieces (one face exposed, movable), 12 edge pieces (two faces exposed),
and 4 corner pieces (three faces exposed). We optimistically assume that each
piece is only one move away from its correct location. Therefore, for each type
of piece, we divide the number that are out of place by the maximum number that
can be moved in one turn. In this case, it is a level 3 or "base" turn that
rotates an entire face. This moves 3 corners, 6 edges, and 9 interiors. We then
sum the integer floor of these values to get our heuristic. It is worth noting
that in such a turn, the interior pieces on the face being rotated (the base) do
not actually change faces, which means if they are out of place, they will not
be returned to the correct location. Therefore, they are not counted in the
divisor. The resulting value is an integer from 0 to 5, where a lower integer
indicates that the puzzle is closer to a solution.

## Performance
I opted to attempt to solve this problem in a way that most closely models how
solving a real Pyraminx would work. This meant I did not limit the solver to any
subset of moves, and each expansion added 32 nodes to the queue. This means that
the puzzle can be scrambled by any process, and the solver will find the
solution given enough time and memory. The problem, of course, is that memory
constraints are encountered quite quickly. The files `plot1.png` and `plot2.png`
show the average number of nodes expanded for puzzles scrambled with different 
numbers of moves. We can see that the number of nodes increases exponentially 
as the complexity of the puzzle increases. Even if the nodes are small in size,
we quickly end up with so many the program crashes because it fills all the 
available memory. This has been verified wby monitoring system logs.

![Node plot](plot1.png)
![Node plot logarithmic](plot2.png)

The curve shown in these plots will eventually level off, as the number of 
scrambled turns is only an upper bound on the number of turns actually required 
to solve the puzzle, as certain sequences of moves can be minimized into single 
moves. At some point, increasing the number of scrambled moves cannot increase
the number of moves to reach a solution- this upper bound is called "God's 
number", meaning that God could always solve the puzzle in that number of moves.
The Rubik's Cube God Number is 20 (https://cube20.org/), 11 for the pyraminx 
(https://www.jaapsch.net/puzzles/pyraminx.htm) and is theorized to be between 20 
and 26 for the Master Pyraminx 
(https://www.speedsolving.com/threads/master-pyraminx-gods-number-bounds.74056/)
Unfortunately, the program could not run long enough to verify this.

## Learning outcome

There are a few things I learned from working on this assignment.
1. Most importantly, I have clearer picture of how A* works. A simple heuristic 
can still be helpful! I was surprised at how the use of the heuristic suddenly 
made the search to appear somewhat "intelligent".
2. Computing constraints are real! I didn't expect to hit a wall with RAM usage
so quickly, and really underestimated the exponential growth of the problem.

## Possible future improvements
1. Limiting expansion of the search tree by preventing the generation of the 
inverse move to a parent (i.e., a U3` after a U3). This would slightly limit 
growth and also not impact the fidelity of the model.
2. Writing search the tree data structure to hard disk to trade speed for more 
memory.
3. Using a cloud instance with a lot of RAM and a large disk as swap space. 
I tried running on a Google Cloud Compute VM with 128 GB of RAM using some free 
student credits, but was still only able to reach k=10. I believe adding a
solid state drive to the VM and setting it as swap space could give enough 
memory to reach k = 20 without modifications.
4. Updating `apply_moves()`. As it is currently implemented, we travel all the
way to the root and back any time we want to apply a new node. This is obviously
not efficient. One solution would be to keep track of the current layer, and 
only backtrack up to a common ancestor node before travelling back down to try 
the newest popped node.