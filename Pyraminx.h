#ifndef PYRAMINX_H
#define PYRAMINX_H

#include <random>
#include "Face.h"

class Pyraminx{
    public:
    Pyraminx();
    void print();
    void turn_layer(Corner corner, int layer, Direction dir);
    void scramble(int depth);
    void setNeighbors();
    private:
    std::shared_ptr<Face> get_opposite_face(Corner corner);
    std::vector<std::shared_ptr<Face> > face_list;
    std::shared_ptr<Face> blue_face;
    std::shared_ptr<Face> red_face; 
    std::shared_ptr<Face> yellow_face;
    std::shared_ptr<Face> green_face; 
};

#endif