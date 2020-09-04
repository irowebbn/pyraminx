#ifndef FACE_H
#define FACE_H

#include <memory>
#include <map>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Eq_Pos_Table.h"

class Face; // Forward declaration for neighborhood
class Eq_Pos_Table;

// Define our possible colors
enum class Color{
    green,
    blue,
    yellow,
    red
};

// Define our possible directions
enum class Direction{
    clockwise,
    counterclockwise
};

// Define our possible corners
enum class Corner{
    U,
    L,
    R,
    B
};

// Holds data to refer to a specific position on a face
struct Position{
    Corner reference_corner;
    int layer;
    int entry;
};

// Set of pointers to adjacent faces
struct Neighborhood{
    std::shared_ptr<Face> U_neighbor_left;
    std::shared_ptr<Face> U_neighbor_right;
    std::shared_ptr<Face> L_neighbor_left;
    std::shared_ptr<Face> L_neighbor_right;
    std::shared_ptr<Face> R_neighbor_left;
    std::shared_ptr<Face> R_neighbor_right;
    std::shared_ptr<Face> B_neighbor_left;
    std::shared_ptr<Face> B_neighbor_right;
};

class Face{
public:
    Face(Color center_color, Eq_Pos_Table& eq_pos_table);
    // Return the color values at they layer specified
    std::vector<Color> get_layer(Corner corner, int layer);
    // Change the color values at the layer specified.
    // Returns the old values (to be given to the adjacent face)
    std::vector<Color> update_layer(Corner corner, int layer, std::vector<Color> new_layer);
    void print();
    // Print just the entry at the specified location
    void print_triangle(Corner ref_corner, int layer, int entry);
    // Used by Pyraminx constructor to define relationship between faces
    void set_neighbors(Neighborhood neighbors);
    // Return the center color of this face
    Color get_center_color(){return center_color;};
    // Get the correct adjacent face when a turn is specified
    std::shared_ptr<Face> get_sending_neighbor(Corner corner, Direction dir);
    std::shared_ptr<Face> get_receiving_neighbor(Corner corner, Direction dir);
    // Rotates the whole face the desired direction. 
    // Used when a level 3 turn is specified.
    void turn_face(Direction dir);
private:
    Color center_color;
    // Map of different position names to the correct pointers to `color_data`
    std::map< Position, Color* > current_state;
    // These are the "magic" mappings of the positions of a face
    // from before (index) to after (value) a turn.
    int cw_rotation[16] = { 9, 11, 10,  4, 13, 12, 6,  5,  1, 15, 14, 8, 7, 3,  2, 0};
    int ccw_rotation[16] = {15,  8, 14, 13,  3,  7, 6, 12, 11,  0,  2, 1, 5, 4, 10, 9 };
    // The canonical reference to the underlying color values on the face
    Color color_data[16];
    // Set of pointers to adjacent faces
    Neighborhood neighbors;
    // Helper function for constructor
    void build_map(Corner reference_corner, Eq_Pos_Table& eq_pos_table);
};

#endif // FACE_H