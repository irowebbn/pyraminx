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
void Pyraminx::turn_layer(Corner corner, int layer, Direction dir){
    // To keep the middle piece fixed in our view,
    // treat middle layer twists as counter-twists of all other layers
    if(layer == 2){
        if(dir == Direction::clockwise){
            dir = Direction::counterclockwise;
        }
        else{
            dir = Direction::clockwise;
        }
        turn_layer(corner, 0, dir);
        turn_layer(corner, 1, dir);
        turn_layer(corner, 3, dir);
    }
    else{
        // If we are turning from the back corner, treat 
        // the green face as the reference face. 
        // Otherwise use the blue face as the reference.
        std::shared_ptr<Face> ref_face;
        if(corner == Corner::B){
            ref_face = green_face;
        }
        else{
            ref_face = blue_face;
        }
        std::shared_ptr<Face> sending_face = ref_face->get_sending_neighbor(corner, dir);
        std::shared_ptr<Face> receiving_face = ref_face->get_receiving_neighbor(corner, dir);
        std::vector<Color> sender_color = sending_face->get_layer(corner, layer);
        std::vector<Color> old_color = ref_face->update_layer(corner, layer, sender_color);
        std::vector<Color> receiver_old_color = receiving_face->update_layer(corner, layer, old_color);
        sending_face->update_layer(corner, layer, receiver_old_color);

        // If we rotate a base layer, we are also rotating the face 
        // opposite the corner in the reverse direction
        if(layer == 3){
            if(dir == Direction::clockwise){
                dir = Direction::counterclockwise;
            }
            else{
                dir = Direction::clockwise;
            }
            get_opposite_face(corner)->turn_face(dir);
        }
    }
}

std::shared_ptr<Face> Pyraminx::get_opposite_face(Corner corner){
    switch (corner)
    {
    case Corner::U:
        return green_face;
        break;
    case Corner::L:
        return yellow_face;
        break;
    case Corner::R:
        return red_face;
        break;
    case Corner::B:
        return blue_face;
        break;
    default:
        return blue_face;
        printf("Invalid corner specified.\n");
        break;
    }
}

void Pyraminx::scramble(int depth){
    std::random_device generator;
    std::uniform_int_distribution<int> corner_layer_dist(0, 3);
    std::uniform_int_distribution<int> dir_dist(0, 1);

    // Some help here from zildjohn01, cc by-sa 
    // https://stackoverflow.com/questions/2999012/generating-random-enums
    // Tip is to use static_cast to castrandom int to an enum
    // That way I can randomly choose a corner and direction.
    for(int i = 0; i < depth; i++){
        int random_layer = corner_layer_dist(generator);
        Corner random_corner = static_cast<Corner>(corner_layer_dist(generator));
        Direction random_direction = static_cast<Direction>(dir_dist(generator));
        turn_layer(random_corner, random_layer, random_direction);
    }
}

void Pyraminx::print(){
    for(int layer = 0; layer < 4; layer++){
        int top_row_len = 2*layer +1;
        int bottom_row_len = (7- top_row_len) + 1;
        for(int j = 0; j < (7-bottom_row_len)/2; j++){
            printf(" ");
        }
        for(int entry = 0; entry < bottom_row_len; entry++){
            red_face->print_triangle(Corner::L, 3-layer, (bottom_row_len - 1) - entry);
        }
        printf(" ");
        for(int entry = 0; entry < top_row_len; entry++){
            blue_face->print_triangle(Corner::U, layer, entry);
        }
        printf(" ");
        for(int entry = 0; entry < bottom_row_len; entry++){
            yellow_face->print_triangle(Corner::R, 3-layer, (bottom_row_len - 1) - entry);
        }
        for(int j = 0; j < (7-bottom_row_len)/2; j++){
                printf(" ");
        }
        printf("\n");
    }
    for(int layer = 3; layer >=0; layer--){
        int row_len = layer*2 + 1;
        printf("     "); // Offset so green appears below blue
        for(int j = 0; j < (7 - row_len)/2; j++){
                printf(" ");
        }
        for(int entry = row_len - 1; entry >= 0; entry--){
            green_face->print_triangle(Corner::B, layer, entry);
        }
        for(int j = 0; j < (7 - row_len)/2; j++){
                printf(" ");
        }
        printf("\n");
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
