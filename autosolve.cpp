#include <queue>
#include <utility> 
#include <fstream>
#include <iostream>

#include "Pyraminx.h"

// Simple encapsulations for the moves we will apply to pyraminx
struct move_type{
    Corner ref_corner;
    int layer;
    Direction dir;
};

struct node{
    // Special constructor for root node
    node(int cost){
        this->cost = cost;
        this->parent = nullptr;
    }

    // General constructor for node
    node(int cost, move_type to_apply, const node* parent){
        this->cost = cost;
        this->to_apply = to_apply;
        this->parent = parent;
    }
    int cost;
    move_type to_apply;
    const node* parent;
};

// Nice names for our weird data structures
// A list of moves that lead to a node
//typedef std::vector<move_type> move_list;
// A node, which has an f value (cost) and a move list leading to it
//typedef std::pair<int, move_list> node;

// Comparison operator for our min-heap
struct Compare{
    bool operator()(node* p1, node* p2 ){
        return p1->cost > p2->cost;
    }
};

// Create a min-heap using a STL priority queue
// Make this hold pointers!!
typedef std::priority_queue<node*, std::vector<node*>, Compare> min_heap;

// Given a node, generate the children nodes, calculate their f values (costs), 
// and put them in the min-heap
void enqueue_children(min_heap& to_expand, const node* parent, Pyraminx& pyraminx, int g);
// Helper function for enqueue children that enqueues the children corresponding 
// to all the possible moves from a given corner of the puzzle
void enqueue_corner(min_heap& to_expand, const node* parent, Pyraminx& pyraminx, const Corner& ref_corner, int g);
// Prints the move list of a node
void print_move_list(const node& current);
// Applies the move list needed to reach a node. Returns cost to reach node.
int apply_moves(const node* newest_node, Pyraminx& pyraminx);
// Undoes the move list needed to reach a node
void undo_moves(const node* newest_node, Pyraminx& pyraminx);
std::vector<move_type> get_move_list(const node* newest_node);

// Driver for autosolve test
int main(){
    // Open file to write out number of expanded nodes:
    std::ofstream logfile;
    logfile.open ("log.txt", std::ios::out | std::ios::app );
    logfile << "k, nodes expanded\n";
    logfile.close();

    int k_start = 3;
    int k_stop = 20;
    int num_instances = 5;
    // For each k, solve 5 puzzles randomized with k moves
    for(int k = k_start; k <= k_stop; k++){
        float count = 0;
        for(int i = 0; i < num_instances; i++){
            Pyraminx myPyraminx;
            myPyraminx.scramble(k);
            printf("Solving puzzle randomized with %i moves...\n", k);
            myPyraminx.print();

            // Create and enqueue root node
            int g = 0;
            int h = myPyraminx.get_heuristic();
            int f = g + h;
    
            min_heap to_expand;
            
            to_expand.push(new node(f));

            // Explore tree
            bool finished;
            while(!to_expand.empty()){
                node* current = to_expand.top();
                to_expand.pop();
                g = apply_moves(current, myPyraminx);
                finished = myPyraminx.is_solved();
                if(finished){
                    printf("Solved! Solution is: ");
                    print_move_list(*current);
                    break;
                }
                else{
                    count++;
                    enqueue_children(to_expand, current, myPyraminx, g);
                    undo_moves(current, myPyraminx);
                }
            }
        }// i for loop (5 instances)
        // We reopen and close the file each time because we don't want to lose
        // the data if termination is early
        logfile.open ("log.txt", std::ios::out | std::ios::app );
        float average = count / num_instances;
        logfile << k << ", " << average << "\n";
        logfile.close();
    }// k for loop (k from 3 to 20) 
}

// Given a node, add nodes for all the possible move types.
void enqueue_children(min_heap& to_expand, const node* parent, Pyraminx& pyraminx, int g){
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
        enqueue_corner(to_expand, parent, pyraminx, ref_corner, g);
    }
}

// For a given corner, create a node for all the possible layers and directions
void enqueue_corner(min_heap& to_expand, const node* parent, Pyraminx& pyraminx, const Corner& ref_corner, int g){
    // for each possible layer and direction:
    // apply move
    // calculate new f value
    // enqueue new move list and corresponding value
    // undo move
    Direction move_dir;
    Direction undo_dir;
    int h_child;
    int g_child;
    int f_child; 
    for(int layer = 0; layer < 4; layer++){ // iterate layers
        for(int dir = 0; dir < 2; dir++){ // iterate directions
            if(dir == 0){
                move_dir = Direction::clockwise;
                undo_dir = Direction::counterclockwise;
            }
            else{
                move_dir = Direction::counterclockwise;
                undo_dir = Direction::clockwise;
            }
            // Get cost by applying move to pyraminx
            pyraminx.turn_layer(ref_corner, layer, move_dir); 
            h_child = pyraminx.get_heuristic(); 
            g_child = g + 1;
            f_child = g_child + h_child;
            move_type child_move{ .ref_corner=ref_corner, .layer= layer, .dir=move_dir};
        
            //node child{.cost = f_child, .to_apply = child_move, .parent= parent};//, child_move_list);
            // Add to min-heap
            to_expand.push(new node(f_child, child_move, parent));
            // Undo test to calculate cost
            pyraminx.turn_layer(ref_corner, layer, undo_dir);
        }
    }
}

int apply_moves(const node* newest_node, Pyraminx& pyraminx){
    std::vector<move_type> move_list = get_move_list(newest_node);
    for(auto it = move_list.rbegin(); it != move_list.rend(); ++it){
        pyraminx.turn_layer(it->ref_corner, it->layer, it->dir);
    }
    return move_list.size(); // new g value
}

void undo_moves(const node* newest_node, Pyraminx& pyraminx){
    Direction undo_dir;
    std::vector<move_type> move_list = get_move_list(newest_node);
    for(move_type move : move_list){
        if (move.dir == Direction::clockwise){
            undo_dir = Direction::counterclockwise;
        }
        else{
            undo_dir = Direction::clockwise;
        }
        pyraminx.turn_layer(move.ref_corner, move.layer, undo_dir);
    }
}
std::vector<move_type> get_move_list(const node* newest_node){
    std::vector<move_type> move_list;
    const node* current = newest_node;
    while(current->parent != nullptr){
        move_list.push_back(current->to_apply);
        current = current->parent;
    }
    return move_list;
}

void print_move_list(const node& current){
    char corner;
    int layer;
    char dir = ' ';
    std::vector<move_type> move_list = get_move_list(&current);
    for(auto it = move_list.rbegin(); it != move_list.rend(); ++it){
        layer = it->layer;
        switch (it->ref_corner)
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
        if(it->dir == Direction::counterclockwise){
            dir = '`';
        }
        else{
            dir = ' ';
        }
        printf("%c%i%c ", corner, layer, dir);
    }
    printf("\n");
}

