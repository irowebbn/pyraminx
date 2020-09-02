README

Standard view, unfolded: Blue in middle, point up. Left side shares an edge with red, 
right side shares an edge with yellow, bottom side shares an edge with green. 
Top point labeled U, bottom point labelled L, right point labelled R. Exterior
points of unfolded pyramid all labelled B. Layers are labelled 1-4 starting at
each tip.

Each face has neighbors. These are defined relative to the tip of each face.
At each tip on a face (3 from the set U, L, R, B), there will be a left neighbor
and a right neighbor. Left an right are defined as if the face was viewed 
straight-on at an orientation with the tip in question at the top.
There are 8 possible types of neighbors, but a given face will only have 6. 
The neighbor sets are as follows:
    Blue:
        U_neighbor_left: Red
        U_neighbor_right: Yellow
        L_neighbor_left: Green 
        L_neighbor_right: Red 
        R_neighbor_left: Yellow
        R_neighbor_right: Green
    Red:
        U_neighbor_left: Yellow
        U_neighbor_right: Blue
        L_neighbor_left: Blue 
        L_neighbor_right: Green 
        R_neighbor_left: Yellow
        R_neighbor_right: Green
    Green:
    Yellow:

Turns are referred to by their layer in the pyramid, from 1 (tip) to 4 (base),
the corner from which the layers are counted (U, L, R, or B), and the direction
(clockwise or counter-clockwise).

Reference corner: U for Blue, Yellow, Red. B for Green (doesn't have a U corner)
Memory Layout:
         0
      1  2  3
   4  5  6  7  8
9 10 11 12 13 14 15