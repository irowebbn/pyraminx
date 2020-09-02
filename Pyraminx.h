#ifndef PYRAMINX_H
#define PYRAMINX_H

#include "Face.h"

class Pyraminx{
    public:
    Pyraminx();
    void print();
    void turn(Corner corner, int level, Direction dir);
    void scramble();
    void setNeighbors();
    private:
    std::vector<std::shared_ptr<Face> > face_list;
    std::shared_ptr<Face> blue_face;
    std::shared_ptr<Face> red_face; 
    std::shared_ptr<Face> yellow_face;
    std::shared_ptr<Face> green_face; 
};

#endif