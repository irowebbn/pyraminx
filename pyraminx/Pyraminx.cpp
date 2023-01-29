#include "Pyraminx.h"

struct Move{
    Vertex vertex;
    Direction direction;
    int layer;

    Move(){
        assert(layer<=0 && layer <=3);
    }
};

// class Piece{

// }
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
   set_neighbors();
}
void Pyraminx::turn_layer(Vertex corner, int layer, Direction dir){
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
        if(corner == Vertex::B){
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

std::shared_ptr<Face> Pyraminx::get_opposite_face(Vertex corner){
    switch (corner)
    {
    case Vertex::U:
        return green_face;
        break;
    case Vertex::L:
        return yellow_face;
        break;
    case Vertex::R:
        return red_face;
        break;
    case Vertex::B:
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
        Vertex random_corner = static_cast<Vertex>(corner_layer_dist(generator));
        Direction random_direction = static_cast<Direction>(dir_dist(generator));
        turn_layer(random_corner, random_layer, random_direction);
    }
}

int Pyraminx::get_heuristic(){
    int num_interior = 24;
    int num_corner = 4;
    int num_edge = 12;
    int interior_correct = count_correct_interior();
    int corner_correct = count_correct_corner();
    int edge_correct = count_correct_edge();

    int h = ((num_corner - corner_correct)/3) + 
        ((num_edge - edge_correct)/6) + 
        ((num_interior - interior_correct)/9);
    return h;
}

int Pyraminx::count_correct_interior(){
    int count = 0;
    int interior_loc[6] = {2, 5, 7, 10, 12, 14};
    int interior_len = sizeof(interior_loc)/ sizeof(int);
    for(int i = 0; i < face_list.size(); i++){
        Color center_color = face_list[i]->get_center_color();
        for(int j = 0; j< interior_len; j++){
            if(face_list[i]->color_data[interior_loc[j]] == center_color){
                count++;
            }
        }
    }
    //printf("Correct interior pieces: %i/24\n", count);
    return count; 
}

int Pyraminx::count_correct_corner(){
    Color blue_upper =     blue_face->get_triangle_color(Vertex::U,0,0);
    Color red_upper =       red_face->get_triangle_color(Vertex::U,0,0);
    Color yellow_upper = yellow_face->get_triangle_color(Vertex::U,0,0);

    Color blue_left =    blue_face->get_triangle_color(Vertex::L,0,0);
    Color red_left =      red_face->get_triangle_color(Vertex::L,0,0);
    Color green_left =  green_face->get_triangle_color(Vertex::L,0,0);

    Color blue_right =     blue_face->get_triangle_color(Vertex::R,0,0);
    Color green_right =   green_face->get_triangle_color(Vertex::R,0,0);
    Color yellow_right = yellow_face->get_triangle_color(Vertex::R,0,0);

    Color red_back =       red_face->get_triangle_color(Vertex::B,0,0);
    Color yellow_back = yellow_face->get_triangle_color(Vertex::B,0,0);
    Color green_back =   green_face->get_triangle_color(Vertex::B,0,0);
    
    int upper_correct = (blue_upper == Color::blue) && (red_upper == Color::red) && (yellow_upper == Color::yellow);
    int left_correct = (blue_left == Color::blue) && (red_left == Color::red) && (green_left == Color::green); 
    int right_correct = (blue_right == Color::blue) && (yellow_right == Color::yellow) && (green_right == Color::green); 
    int back_correct = (yellow_back == Color::yellow) && (red_back == Color::red) && (green_back == Color::green);  
    //printf("Correct corner pieces: %i/4\n", upper_correct + left_correct + right_correct + back_correct);
    return upper_correct + left_correct + right_correct + back_correct;
}

int Pyraminx::count_correct_edge(){
    int count = 0;
    // Check the edges between faces sharing the upper corner
    std::shared_ptr<Face> current_face = blue_face;
    for(int i = 0; i < 3; i++){
        std::shared_ptr<Face> adjacent_face = current_face->neighbors.U_neighbor_right;
        Color current_top = current_face->get_triangle_color(Vertex::U, 1, 2);
        Color current_bottom = current_face->get_triangle_color(Vertex::U, 2, 4);
        Color adjacent_top = adjacent_face->get_triangle_color(Vertex::U, 1, 0);
        Color adjacent_bottom = adjacent_face->get_triangle_color(Vertex::U, 2, 0);
        int top_correct = (current_top == current_face->get_center_color()) 
            && (adjacent_top == adjacent_face->get_center_color());
        int bottom_correct = (current_bottom == current_face->get_center_color()) 
            && (adjacent_bottom == adjacent_face->get_center_color());
        count = count + top_correct + bottom_correct;
        current_face = adjacent_face;
    }
    // Check the edges between the bottom face (green) and the others
    for(int i = 0; i < 3; i++){
        std::shared_ptr<Face> adjacent_face;
        Vertex current_corner;
        switch (i)
        {
        case 0:
            current_corner = Vertex::L;
            adjacent_face = yellow_face;
            break;
        case 1:
            current_corner = Vertex::R;
            adjacent_face = red_face;
            break;
        case 2:
            current_corner = Vertex::B;
            adjacent_face = blue_face;
            break;
        }

        Color current_left  = green_face->get_triangle_color(current_corner, 3, 2);
        Color current_right = green_face->get_triangle_color(current_corner, 3, 4);

        Color adjacent_right = adjacent_face->get_triangle_color(Vertex::U, 3, 2);
        Color adjacent_left = adjacent_face->get_triangle_color(Vertex::U, 3, 4);

        int left_correct = (current_left == Color::green) && (adjacent_left == adjacent_face->get_center_color());
        int right_correct = (current_right == Color::green) && (adjacent_right == adjacent_face->get_center_color());

        count = count + left_correct + right_correct;
    }
    //printf("Correct edge pieces: %i/12\n", count);
    return count;
}

bool Pyraminx::is_solved(){
    int interior_correct = count_correct_interior();
    int corner_correct = count_correct_corner();
    int edge_correct = count_correct_edge(); 

    return (interior_correct == 24) && (corner_correct == 4) && (edge_correct == 12);
}

void Pyraminx::print(){
    for(int layer = 0; layer < 4; layer++){
        int top_row_len = 2*layer +1;
        int bottom_row_len = (7- top_row_len) + 1;
        for(int j = 0; j < (7-bottom_row_len)/2; j++){
            printf(" ");
        }
        for(int entry = 0; entry < bottom_row_len; entry++){
            red_face->print_triangle(Vertex::L, 3-layer, (bottom_row_len - 1) - entry);
        }
        printf(" ");
        for(int entry = 0; entry < top_row_len; entry++){
            blue_face->print_triangle(Vertex::U, layer, entry);
        }
        printf(" ");
        for(int entry = 0; entry < bottom_row_len; entry++){
            yellow_face->print_triangle(Vertex::R, 3-layer, (bottom_row_len - 1) - entry);
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
            green_face->print_triangle(Vertex::B, layer, entry);
        }
        for(int j = 0; j < (7 - row_len)/2; j++){
                printf(" ");
        }
        printf("\n");
    }
}
void Pyraminx::set_neighbors(){
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
