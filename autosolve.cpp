#include <queue>
#include <utility> 

#include "Pyraminx.h"

struct move_type{
    Corner ref_corner;
    int layer;
    Direction dir;
};

typedef std::vector<move_type> move_list;
typedef std::pair<int, move_list> node;

struct Compare{
    bool operator()(node& p1, node& p2 ){
        return p1.first > p2.first;
    }
};

typedef std::priority_queue<node, std::vector<node>, Compare> min_heap;

void enqueue_children(min_heap& to_expand, const node& parent, Pyraminx& pyraminx);
void enqueue_corner(min_heap& to_expand, const node& parent, Pyraminx& pyraminx, const Corner& ref_corner);
bool goal_test(const node& current, Pyraminx& pyraminx);
void print_move_list(const node& current);
void apply_moves(const node& current, Pyraminx& pyraminx);
void undo_moves(const node& current, Pyraminx& pyraminx);

int main(){
    int k_start = 3;
    int k_stop = 20;
    for(int k = k_start; k <= k_stop; k++){
        Pyraminx myPyraminx;
        myPyraminx.scramble(k);
        printf("Solving puzzle randomized with %i moves...\n", k);
        myPyraminx.print();
        int g = 0;
        int h = myPyraminx.get_heuristic();
        int f = g + h;
        move_list moves_so_far;
        min_heap to_expand;
        to_expand.push(node(f, moves_so_far));
        bool finished;
        while(!to_expand.empty()){
            node current = to_expand.top();
            to_expand.pop();
            finished = goal_test(current, myPyraminx);
            if(finished){
                // printf("Initial:\n");
                // myPyraminx.print();
                // apply_moves(current, myPyraminx);
                // myPyraminx.print();
                printf("Solved! Solution is: ");
                print_move_list(current);
                break;
            }
            else{
                // printf("Not solved. Reverting.\n");
                enqueue_children(to_expand, current, myPyraminx);
            }
        }
    }
    
}

void enqueue_children(min_heap& to_expand, const node& parent, Pyraminx& pyraminx){
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
        enqueue_corner(to_expand, parent, pyraminx, ref_corner);
    }
}

void enqueue_corner(min_heap& to_expand, const node& parent, Pyraminx& pyraminx, const Corner& ref_corner){
    // for each possible layer and direction:
    // apply move
    // calculate new f value
    // enqueue new move list and corresponding value
    // undo move
    Direction move_dir;
    Direction undo_dir;
    int h_child;
    int g_parent;
    int g_child;
    int f_child; 
    for(int layer = 0; layer < 4; layer++){
        for(int dir = 0; dir < 2; dir++){
            if(dir == 0){
                move_dir = Direction::clockwise;
                undo_dir = Direction::counterclockwise;
            }
            else{
                move_dir = Direction::counterclockwise;
                undo_dir = Direction::clockwise;
            }
            pyraminx.turn_layer(ref_corner, layer, move_dir);
            h_child = pyraminx.get_heuristic();
            g_parent = parent.second.size();
            g_child = g_parent + 1;
            f_child = g_child + h_child;
            move_type child_move{ .ref_corner=ref_corner, .layer= layer, .dir=move_dir};
            move_list child_move_list = parent.second;
            child_move_list.push_back(child_move);
            node child(f_child, child_move_list);
            to_expand.push(child);
            pyraminx.turn_layer(ref_corner, layer, undo_dir);
        }
    }
}

bool goal_test(const node& current, Pyraminx& pyraminx){
    // printf("Trying the following moves: ");
    // print_move_list(current);
    apply_moves(current, pyraminx);
    bool solved = pyraminx.is_solved();
    // pyraminx.print();
    undo_moves(current, pyraminx);
    return solved;
}

void apply_moves(const node& current, Pyraminx& pyraminx){
    for(move_type move : current.second){
        pyraminx.turn_layer(move.ref_corner, move.layer, move.dir);
    }
}

void undo_moves(const node& current, Pyraminx& pyraminx){
    Direction undo_dir;
    for(auto it = current.second.rbegin(); it != current.second.rend(); ++it){
        if((*it).dir == Direction::clockwise){
            undo_dir = Direction::counterclockwise;
        }
        else{
            undo_dir = Direction::clockwise;
        }
        pyraminx.turn_layer((*it).ref_corner, (*it).layer, undo_dir);
    }
}

void print_move_list(const node& current){
    char corner;
    int layer;
    char dir = ' ';
    for(move_type move : current.second){
        layer = move.layer;
        switch (move.ref_corner)
        {
        case Corner::U:
            corner = 'U';
            break;
        case Corner::L:
            corner = 'L';
            break;
        case Corner::R:
            corner = 'R';
            break;
        case Corner::B:
            corner = 'B';
            break;
        }
        if(move.dir == Direction::counterclockwise){
            dir = '`';
        }
        printf("%c%i%c ", corner, layer, dir);
    }
    printf("\n");
}

