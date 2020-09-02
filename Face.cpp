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
        build_map(Corner::U, eq_pos_table);
        break;
    // Different case for Green since it doesn't have a U corner
    case Color::green:
        build_map(Corner::B, eq_pos_table);
        break;
    default:
        printf("Invalid color for face.\n");
        break;
    }
}

void Face::build_map(Corner reference_corner, Eq_Pos_Table& eq_pos_table){
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

void Face::turn(Corner corner, int level, Direction dir){
    
}

void Face::update_triangle(Position pos_to_update){
    // If face center color is blue, yellow, or red
    // upper corner can be dereferenced immediately
    // other corners need to be double dereferenced

    // if face color is green
    // Back corner can be dereferenced immediately
    // other corners need to be double dereferenced
}

void Face::print(){

    int entry = 0;
    for(int row_len = 1; row_len <= 7; row_len = row_len + 2){
        for(int j = 0; j < (7-row_len)/2; j++){
            printf(" ");
        }
        for(int j = 0; j < row_len; j++){
            printf("%i", color_data[entry]);
            entry++;
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