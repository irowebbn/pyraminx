# README
Modeling the (Master) Pyraminx

Isaac Rowe, University of Kentucky

CS463G Fall 2020

# Puzzle Description

# Notation Convention
The assignment specifies "To make a move, you should specify the face that is
rotated and the direction of rotation." This language does not account for all
the types of moves possible with the Pyraminx, so moves are specified by the
layer that is rotated and the direction of rotation. This interpretation was
supported by comments made in lecture by Dr. Goldsmith in response to a
question.

In order to create a standard notation for making moves, I have defined a
a standard view. This is consistent with techniques used in the twisty-puzzle
community, where the puzzle stays fixed with one side towards the solver for the
duration of a solving stage. 

We imagine always viewing the Pyraminx such that the blue face (Go Cats!) 
is always facing us. The corners are therefore fixed and referred to as 
U, L, R, or B (Upper, Left, Right, Back). To make a turn, the user specifies
the corner, and the layer, which is counted up from zero starting with tip layer
at the corner specified. The layer is turned clockwise from the perspective of
of a viewer at the specified corner tip, or counter-clockwise if the user
appends a tick (`) to the end of the turn to specify an inverted turn.

See the image `standard_view.jpeg` for a visual aid or read the text description
below. 
![Standard view](standard_view.jpeg)

**Standard view, unfolded**: Blue in middle, point up. Left side shares an edge
with red, right side shares an edge with yellow, bottom side shares an edge with
green. Top point labeled U, bottom point labelled L, right point labelled R.
Exterior points of unfolded pyramid all labelled B. Layers are labelled 0-3
starting at each tip.

## Data Structure
The source code is organized according to the object-oriented programming (OOP)
paradigm. There are 2 main classes: `Pyraminx` and `Face`. Pyraminx is a data
structure representing the puzzle as a whole. It has 4 members with pointers
`Face` objects corresponding to the faces of the puzzle. It provides the
functions `print()`, which prints the puzzle's current configuration, `turn()`,
which enacts a user-specified move, `scramble()`, which sets the puzzle to a
random (solvable) state, and `setNeighbors()`, which establishes the
relationship between the faces.


Reference corner: U for Blue, Yellow, Red. B for Green (doesn't have a U corner)

**Memory Layout**:
         0
      1  2  3
   4  5  6  7  8
9 10 11 12 13 14 15

Each face has neighbors. These are defined relative to the tip of each face. At
each tip on a face (3 from the set U, L, R, B), there will be a left neighbor
and a right neighbor. Left an right are defined as if the face was viewed
straight-on at an orientation with the tip in question at the top. There are 8
possible types of neighbors, but a given face will only have 6.

For example, the blue face has the following neighbor:
        U_neighbor_left: Red
        U_neighbor_right: Yellow
        L_neighbor_left: Green 
        L_neighbor_right: Red 
        R_neighbor_left: Yellow
        R_neighbor_right: Green
The neighbors for the other faces can be easily determined by looking at the 
layout of the cube provided.

# Heuristic

# GUI

The terminal to which output is written must support ANSI color codes. To check
if your terminal supports this, run `tput colors`. The value returned should be
at least 8 for the coloring to work. If you see garbage characters printed to
the output, your terminal probably doesn't support this.

It can be difficult to read diagonal layers, since unlike interlocking
triangles, characters must exclusively occupy their own discrete space. Future
work may include a 3D visualization of the puzzle to mitigate this issue.