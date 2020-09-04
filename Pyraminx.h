#ifndef PYRAMINX_H
#define PYRAMINX_H

#include <random>
#include "Face.h"

class Pyraminx{
    public:
    Pyraminx();
    // Formatted print of unfolded puzzle
    void print();
    // Turns the specified layer by 120 degrees
    void turn_layer(Corner corner, int layer, Direction dir);
    // Randomly turn the cube a specified number of times
    void scramble(int depth);
    // Set up the relationship between the faces
    void setNeighbors();
    private:
    // Get the face opposite a given corner. 
    // Used if we rotate layer 3.
    std::shared_ptr<Face> get_opposite_face(Corner corner);
    // Pointers to the Face objects used in the puzzle
    std::vector<std::shared_ptr<Face> > face_list;
    std::shared_ptr<Face> blue_face;
    std::shared_ptr<Face> red_face; 
    std::shared_ptr<Face> yellow_face;
    std::shared_ptr<Face> green_face; 
};

#endif