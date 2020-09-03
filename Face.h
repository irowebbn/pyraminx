#ifndef FACE_H
#define FACE_H

#include <memory>
#include <map>
#include <cstdio>
#include <vector>

#include "Eq_Pos_Table.h"

class Face; // Forward declaration for neighborhood
class Eq_Pos_Table;

enum class Color{
    green,
    blue,
    yellow,
    red
};

enum class Direction{
    clockwise,
    counterclockwise
};

enum class Corner{
    U,
    L,
    R,
    B
};

struct Position{
    Corner reference_corner;
    int layer;
    int entry;
};


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
    //void turn(Corner corner, int level, Direction dir);
    std::vector<Color> get_layer(Corner corner, int layer);
    std::vector<Color> update_layer(Corner corner, int layer, std::vector<Color> new_layer);
    void print();
    void set_neighbors(Neighborhood neighbors);
    Color get_center_color(){return center_color;};
    std::shared_ptr<Face> get_sending_neighbor(Corner corner, Direction dir);
    std::shared_ptr<Face> get_receiving_neighbor(Corner corner, Direction dir);
private:
    Color center_color;
    std::map< Position, Color* > current_state;
    Color color_data[16];
    Neighborhood neighbors;
    void update_triangle(Position pos_to_update);
    void build_map(Corner reference_corner, Eq_Pos_Table& eq_pos_table);
};

#endif // FACE_H