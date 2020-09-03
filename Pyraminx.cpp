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
void Pyraminx::turn(Corner corner, int layer, Direction dir){
    // TODO: account for middle layer twist
    // TODO: fix back corner twists- returns nullptr from blue_face
    // take corner, lookup 3 adjacent faces, do ring below for them
    std::shared_ptr<Face> sending_face = blue_face->get_sending_neighbor(corner, dir);
    std::shared_ptr<Face> receiving_face = blue_face->get_receiving_neighbor(corner, dir);
    std::vector<Color> sender_color = sending_face->get_layer(corner, layer);
    std::vector<Color> old_color = blue_face->update_layer(corner, layer, sender_color);
    std::vector<Color> receiver_old_color = receiving_face->update_layer(corner, layer, old_color);
    sending_face->update_layer(corner, layer, receiver_old_color);
}

void Pyraminx::print(){
    for(int i = 0; i < face_list.size(); i++){
        face_list[i]->print();
    }
}
void Pyraminx::setNeighbors(){
    Neighborhood neighbors;
    for(int i = 0; i < face_list.size(); i++){
        Color center_color = face_list[i]->get_center_color();
        switch (center_color)
        {
        case Color::blue:
            neighbors.U_neighbor_left = red_face;
            neighbors.U_neighbor_right = yellow_face;
            neighbors.L_neighbor_left = green_face;
            neighbors.L_neighbor_right = red_face;
            neighbors.R_neighbor_left = yellow_face;
            neighbors.R_neighbor_right = green_face;
            neighbors.B_neighbor_left = nullptr; 
            neighbors.B_neighbor_right = nullptr;
            blue_face->set_neighbors(neighbors);
            break;
        case Color::red:
            neighbors.U_neighbor_left = yellow_face;
            neighbors.U_neighbor_right = blue_face;
            neighbors.L_neighbor_left = blue_face;
            neighbors.L_neighbor_right = green_face;
            neighbors.R_neighbor_left = nullptr;
            neighbors.R_neighbor_right = nullptr;
            neighbors.B_neighbor_left = green_face; 
            neighbors.B_neighbor_right = yellow_face;
            red_face->set_neighbors(neighbors);
            break;
        case Color::yellow:
            neighbors.U_neighbor_left = blue_face;
            neighbors.U_neighbor_right = red_face;
            neighbors.L_neighbor_left = nullptr;
            neighbors.L_neighbor_right = nullptr;
            neighbors.R_neighbor_left = green_face;
            neighbors.R_neighbor_right = blue_face;
            neighbors.B_neighbor_left = red_face; 
            neighbors.B_neighbor_right = green_face;
            yellow_face->set_neighbors(neighbors);
            break;
        // Different case for Green since it doesn't have a U corner
        case Color::green:
            neighbors.U_neighbor_left = nullptr;
            neighbors.U_neighbor_right = nullptr;
            neighbors.L_neighbor_left = red_face;
            neighbors.L_neighbor_right = blue_face;
            neighbors.R_neighbor_left = blue_face;
            neighbors.R_neighbor_right = yellow_face;
            neighbors.B_neighbor_left = yellow_face; 
            neighbors.B_neighbor_right = red_face;
            green_face->set_neighbors(neighbors);
            break;
        default:
            printf("Invalid color for face.\n");
            break;
        }
    
    }
} 
