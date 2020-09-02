#include "Pyraminx.h"

Pyraminx::Pyraminx(){
    Eq_Pos_Table eq_pos_table;

    blue_face = std::make_shared<Face>(Color::blue, eq_pos_table);
    red_face = std::make_shared<Face>(Color::red, eq_pos_table);
    yellow_face = std::make_shared<Face>(Color::yellow, eq_pos_table);
    green_face = std::make_shared<Face>(Color::green, eq_pos_table);

    face_list.push_back(blue_face);
    face_list.push_back(red_face);
    face_list.push_back(yellow_face);
    face_list.push_back(green_face); 

   setNeighbors();
}

void Pyraminx::setNeighbors(){
    
/*     neighbors.U_neighbor_left = 
    neighbors.U_neighbor_left;
    neighbors.U_neighbor_right;
    neighbors.L_neighbor_left;
    neighbors.L_neighbor_right;
    neighbors.R_neighbor_left;
    neighbors.R_neighbor_right;
    neighbors.B_neighbor_left;
    neighbors.B_neighbor_right; */

    Neighborhood neighbors;
    for(int i = 0; i < face_list.size(); i++){
        Color center_color = face_list[i]->get_center_color();
        switch (center_color)
        {
        case Color::blue:
            break;
        case Color::red:
            break;
        case Color::yellow:
            
            break;
        // Different case for Green since it doesn't have a U corner
        case Color::green:
            
            break;
        default:
            printf("Invalid color for face.\n");
            break;
        }
    
    }
} 
