#include "Face.h"

Face::Face(Color center_color, Eq_Pos_Table& eq_pos_table){
    this->center_color = center_color;
    for(int i = 0; i < 16; i++){
        color_data[i] = center_color;
    }

    switch (center_color)
    {
    case Color::blue:
    case Color::red:
    case Color::yellow:
        build_map(Vertex::U, eq_pos_table);
        break;
    // Different case for Green since it doesn't have a U corner
    case Color::green:
        build_map(Vertex::B, eq_pos_table);
        break;
    default:
        printf("Invalid color for face.\n");
        break;
    }
}

void Face::build_map(Vertex reference_corner, Eq_Pos_Table& eq_pos_table){
    Position current_position;
    current_position.reference_corner = reference_corner;
    for(int layer = 0; layer < 4; layer++){
        current_position.layer = layer;
        for(int entry = 0; entry < (2*layer + 1); entry++){
            current_position.entry = entry;
            int flat_entry = (layer*layer) + entry;
            std::vector<Position> eq_positions = eq_pos_table.get_equivalent_positions(center_color, flat_entry);
            current_state.insert(std::pair<Position, Color*>(current_position, &color_data[flat_entry]));
            for(int i = 0; i < eq_positions.size(); i++){
                current_state.insert(std::pair<Position, Color*>(eq_positions[i], &color_data[flat_entry]));
            }
        }
    }
}
std::vector<Color> Face::get_layer(Vertex corner, int layer){
    std::vector<Color> desired_layer;
    Position to_access;
    to_access.layer =  layer;
    to_access.reference_corner = corner;
    for(int entry = 0; entry < 2*layer + 1; entry++){
        to_access.entry = entry;
        desired_layer.push_back(*current_state.find(to_access)->second);
    }
    return desired_layer;
}

// There's some repetition here, but I didn't want to have to transverse the 
// layer twice by calling get_layer.
std::vector<Color> Face::update_layer(Vertex corner, int layer, std::vector<Color> new_layer){
    std::vector<Color> old_layer;
    Position to_access;
    to_access.layer =  layer;
    to_access.reference_corner = corner;
    for(int entry = 0; entry < 2*layer + 1; entry++){
        to_access.entry = entry;
        old_layer.push_back(*current_state.find(to_access)->second);
        *(current_state[to_access]) = new_layer[entry];
    }
    return old_layer;
}

void Face::turn_face(Direction dir){
    Color color_tmp[16];
    std::memcpy(color_tmp, color_data, 16*sizeof(Color));
    int turn_map[16];
    if(dir == Direction::clockwise){
        std::memcpy(turn_map, cw_rotation, 16*sizeof(int));
    }
    else{
        std::memcpy(turn_map, ccw_rotation, 16*sizeof(int));
    }
    for(int i = 0; i < 16; i++){
        color_data[i] = color_tmp[turn_map[i]];
    }
}

std::shared_ptr<Face> Face::get_sending_neighbor(Vertex corner, Direction dir){
    switch (corner)
    {
    case Vertex::U:
        if(dir == Direction::clockwise){
            return neighbors.U_neighbor_right;
        }
        else{
            return neighbors.U_neighbor_left;
        }
        break;
    case Vertex::L:
        if(dir == Direction::clockwise){
            return neighbors.L_neighbor_right;
        }
        else{
            return neighbors.L_neighbor_left;
        }
        break;
    case Vertex::R:
        if(dir == Direction::clockwise){
            return neighbors.R_neighbor_right;
        }
        else{
            return neighbors.R_neighbor_left;
        }
        break;
    case Vertex::B:
        if(dir == Direction::clockwise){
            return neighbors.B_neighbor_right;
        }
        else{
            return neighbors.B_neighbor_left;
        }
        break;
    default:
        printf("Invalid move specified");
        break;
    }
}

 std::shared_ptr<Face> Face::get_receiving_neighbor(Vertex corner, Direction dir){
    if(dir == Direction::clockwise){
        dir = Direction::counterclockwise;
    }
    else{
        dir = Direction::clockwise;
    }
    return get_sending_neighbor(corner, dir);
 }

Color Face::get_triangle_color(Vertex ref_corner, int layer, int entry){
    Position lookup_pos{.reference_corner = ref_corner, .layer = layer, .entry = entry};
    return  *(current_state.find(lookup_pos)->second);
}
void Face::print_triangle(Vertex ref_corner, int layer, int entry){
    Color triangle_color = get_triangle_color(ref_corner, layer, entry);
    switch (triangle_color)
    {
    case Color::blue:
        printf("\033[1;34mB\033[0m");
        break;
    case Color::red:
        printf("\033[1;31mR\033[0m");
        break;             
    case Color::yellow:
        printf("\033[1;33mY\033[0m");
        break;
    case Color::green:
        printf("\033[1;32mG\033[0m");
        break;
    default:
        break;
    }
}
void Face::print(){
    Vertex ref_corner;
    if(center_color == Color::green){
        ref_corner = Vertex::B;
    }
    else{
        ref_corner = Vertex::U;
    }
    for(int layer = 0; layer < 4; layer++){
        int row_len = 2*layer + 1;
        for(int j = 0; j < (7-row_len)/2; j++){
            printf(" ");
        }
        for(int entry = 0; entry < row_len; entry++){
            print_triangle(ref_corner, layer, entry);
        }
        for(int j = 0; j < (7-row_len)/2; j++){
            printf(" ");
        }
        printf("\n");
    }
}

void Face::set_neighbors(Neighborhood neighbors){
    this->neighbors = neighbors;
}

bool operator< (const Position &pos1, const Position &pos2)
{
    if(pos1.reference_corner < pos2.reference_corner){
        return true;
    }
    else if(pos1.reference_corner == pos2.reference_corner){
        if(pos1.layer < pos2.layer){
            return true;
        }
        else if(pos1.layer == pos2.layer){
            return pos1.entry < pos2.entry;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}