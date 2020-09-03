#include "Eq_Pos_Table.h"

Eq_Pos_Table::Eq_Pos_Table(){
    // Build the equivalent position tables
    for(int layer = 0; layer < 4; layer++){
        for(int entry = 0;  entry < (2*layer + 1); entry++){
            //int flat_entry = (layer*layer) + entry;
            Position eq1, eq2;
            switch (entry)
            {
            case 0:
                eq1.layer = 3;
                eq2.layer = 3-layer;
                break;
            case 1:
                eq1.layer = 3;
                eq2.layer = 4-layer;
                break;
            case 2:
                eq1.layer = 2;
                eq2.layer = 4-layer;
                break;
            case 3:
                eq1.layer = 2;
                eq2.layer = 5-layer;
                break;
            case 4:
                eq1.layer = 1;
                eq2.layer = 5-layer;
                break;
            case 5:
                eq1.layer = 1;
                eq2.layer = 6-layer;
                break;
            case 6:
                eq1.layer = 0;
                eq2.layer = 6-layer;
                break;
            default:
                printf("Invalid position\n");
                break;
            }
            eq1.entry = 2*layer  - entry;
            eq2.entry = 2*(eq2.layer)  - entry;

            
            eq1.reference_corner = Corner::R;
            eq2.reference_corner = Corner::L;
            std::vector<Position> blue_insert{eq1, eq2};
            blue_eq.push_back(std::vector<Position>{eq1, eq2});
            eq1.reference_corner = Corner::L;
            eq2.reference_corner = Corner::B;
            red_eq.push_back(std::vector<Position>{eq1, eq2});
            eq1.reference_corner = Corner::B;
            eq2.reference_corner = Corner::R;
            yellow_eq.push_back(std::vector<Position>{eq1, eq2});
            eq1.reference_corner = Corner::L;
            eq2.reference_corner = Corner::R;
            green_eq.push_back(std::vector<Position>{eq1, eq2});
        }
    }
}

std::vector<Position> Eq_Pos_Table::get_equivalent_positions(Color center_color, int flat_entry){
    switch (center_color)
    {
    case Color::blue:
        return blue_eq[flat_entry];
        break;
    case Color::yellow:
        return yellow_eq[flat_entry];
        break;
    case Color::red:
        return red_eq[flat_entry];
        break;
    case Color::green:
        return green_eq[flat_entry];
        break;
    default:
        printf("Invalid color\n");
        break;
    }
};