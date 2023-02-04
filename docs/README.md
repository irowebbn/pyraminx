# Quickstart:
```bash
git clone git@github.com:irowebbn/pyraminx.git
cd pyraminx
mkdir build
cd build
cmake -S .. -B . # You can also specify a specific build generator with `-G` option
cmake --build . --config Release  # You can also run your specified build tool directly, e.g. `make` or `ninja`
``

Currently, there are two executables build under `examples/cli` and `examples/autosolve`. 

The `pyraminx-cli` executable is a interactive puzzle which takes move inputs and simpy prints an unfolded view of the puzzle to
the terminal. This allows the user to see the state of the puzzle after each
step. The terminal to which output is written must support ANSI color codes. To
check if your terminal supports this, run `tput colors`. The value returned
should be at least 8 for the coloring to work. If you see garbage characters
printed to the output, your terminal probably doesn't support this. 

The `pyraminx-autosolve` executable will solve consecutive instances of scrambled puzzles, starting with 3 scrambling moves and working up to 20 scrambling moves. You can edit these parameters in `examples/autosolve/autosolve.cpp`. Future work will allow this to be set at runtime. The algorithm used is optimal, but currently runs out of memory for deeply scrambled cubes. Future work will attempt to improve this performance.

For the description of how the Pyraminx model was implemented, see 
`docs/README_Assignment1_Model.md`. For the description on how the A* heuristic-informed
search solver works, see `docs/README_Assignment2_AStar.md`
