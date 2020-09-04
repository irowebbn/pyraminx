# README
Modeling the (Master) Pyraminx

Isaac Rowe, University of Kentucky

CS463G Fall 2020

## Puzzle Description
The master Pyraminx is a puzzle similar to the Rubik's Cube, but is a
tetrahedron rather than a cube. There are 4 triangular faces each with 4 layers
of smaller triangle pieces that rotate about the axis through each corner.

## How to run
To build this program, ensure you have a C++ compiler that supports C++11. Run
`make run` to build and run the program. When the program is launched, enter a
command of the form LX[`] where L is the letter corresponding with a corner of
the Pyraminx (U, L, R, B),  X is a integer from 0-3 specifying the layer of the
row relative to the tip (layer 0) at the corner given, and the optional ` mark
indicates whether the layer should be turned counterclockwise. The default
rotation is clockwise. The corner characters are case insensitive. There should
be no spaces in your command. Enter 's' to scramble, 'q' to quit. Use `make test`
to run the program with example input in `test_input`.

This has been successfully tested with the Apple clang version 11.0.3 compiler
on a x86_64-apple-darwin19.5.0 architecture.

## Notation Convention
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

See the image `standard_view.jpg` for a visual aid or read the text description
below. 
![Standard view](standard_view.jpg)

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
functions `print()`, which prints the puzzle's current configuration,
`turn_layer()`, which enacts a user-specified move, `scramble()`, which sets the
puzzle to a random (solvable) state, and `setNeighbors()`, which establishes the
relationship between the faces. 

The `Face` object contains information about the colors and their positions on that particular face of the puzzle, which is fixed about a center piece. There are a few main pieces of data managed by the
`Face`

**Memory Layout**:

![memory layout](mem_layout.png)


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

## Heuristic

## GUI
In addition to the input interface described above, this program provides a
basic graphical output, which simply prints an unfolded view of the puzzle to
the terminal. This allows the user to see the state of the puzzle after each
step. The terminal to which output is written must support ANSI color codes. To
check if your terminal supports this, run `tput colors`. The value returned
should be at least 8 for the coloring to work. If you see garbage characters
printed to the output, your terminal probably doesn't support this.

You can see an example of the visualization in the file `GUI_Output.png`. This
shows the state of the puzzle after the moves B3, R0, U3.

![GUI output](GUI_Output.png)

It can be difficult to read diagonal layers, since unlike interlocking
triangles, characters must exclusively occupy their own discrete space. Future
work may include a 3D visualization of the puzzle to mitigate this issue.

## Randomizer

The randomizer is implemented in the Pyraminx.scramble() routine. It takes an
integer argument, which is how many random moves to apply. It is implemented
with the C++ Standard Library uniform number distribution and random number
generator. Three different random integers are generated: one in the range [0,4]
for the corner, one in the range [0,3] for the layer, and one in the range [0,1]
for the direction. The corresponding move is then applied. Entering `s` in the
interactive program will run this function for 20 moves. More work is needed to
determine if this is an appropriate value for true randomness, but to the
untrained eye, the puzzle appears to be in a random state.