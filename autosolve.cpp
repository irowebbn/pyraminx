#include <queue>
#include <utility> 

#include "Pyraminx.h"

struct move_type{
    Corner ref_corner;
    int layer;
    Direction dir;
};

typedef std::vector<move_type> move_list;
typedef std::pair<int, move_list> fval_state_pair;

struct compare{
    bool operator()(std::pair<int, move_type> & p1, std::pair<int, move_type> & p2 ){
        return p1.first > p2.first;
    }
};

void enqueue_children(fval_state_pair);

int main(){
    int k_start = 3;
    int k_stop = 20;
    for(int k = k_start; k <= k_stop; k++){
        Pyraminx myPyraminx;
        myPyraminx.scramble(k);
        int g = 0;
        int h = myPyraminx.get_heuristic();
        int f = g + h;
        move_list moves_so_far;
        std::priority_queue<fval_state_pair, std::vector<fval_state_pair>,compare> to_expand;
        to_expand.push(fval_state_pair(f, moves_so_far));
        
    }
    
}

void enqueue_children(fval_state_pair, Pyraminx& pyraminx){
    for(int i = 0; i < 4; i++){
        Corner ref_corner;
        switch (i)
        {
        case 0:
            ref_corner = Corner::U;
            break;
        case 1:
            ref_corner = Corner::L;
            break;
        case 2:
            ref_corner = Corner::R;
            break;
        case 3:
            ref_corner = Corner::B;
            break;
        }
    }
}

void enqueue_corner(){
    // iterate through layers
    // iterate through directions
    // apply move
    // calculate h
    // enqueue
    // undo move
}