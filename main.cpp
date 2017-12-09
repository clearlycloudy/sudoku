#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <utility>
#include <cstring>
#include <limits>

class move_info {
public:
    int digit;
    int pos_x;
    int pos_y;
};

class solver {
public:
    std::vector<std::vector<int> > board_fin;
    std::vector<move_info> move_stack;
    int nums_filled;
    bool solved;
    solver( std::vector<std::vector<int> > input ){
	solved = false;
	nums_filled = 0;
	board_fin.resize(9);
	for(size_t i = 0; i < 9; ++i ){
	    board_fin[i] = std::vector<int>( 9, 0 );
	}
	assert( input.size() == 9 );
	assert( input[0].size() == 9 );
	for(size_t i = 0; i < 9; ++i ){
	    for( size_t j = 0; j < 9; ++j ){
		int num = input[i][j];
		board_fin[i][j] = input[i][j];
		if( num != 0 ){
		    nums_filled++;
		}   
	    }
	}
    }
    bool is_solved(){
	if( nums_filled == 81 ){
	    solved = true;
	    return true;
	}else{
	    return false;
	}
    }
    std::pair<int,int> get_free_cell(){ //O(dim^3)
	//pick a free cell from the board using most constrained heurestic
	int pos_x = -1;
	int pos_y = -1;
	int constraints[9][9];
	for(size_t i = 0; i < 9; ++i ){
	    for( size_t j = 0; j < 9; ++j ){
		if( board_fin[i][j] == 0 ){
		    pos_x = j;
		    pos_y = i;
		    auto candidates = generate_candidate( pos_x, pos_y );
		    constraints[pos_y][pos_x] = candidates.size();
		}else{
		    constraints[i][j] = std::numeric_limits<int>::max();
		}
	    }
	}

	int count_best = std::numeric_limits<int>::max();
	
	for(size_t i = 0; i < 9; ++i ){
	    for( size_t j = 0; j < 9; ++j ){
		if( count_best > constraints[i][j] ){
		    count_best = constraints[i][j];
		    pos_x = j;
		    pos_y = i;
		}
	    }
	}
	assert( pos_x > -1 );
	assert( pos_y > -1 );
	return { pos_x, pos_y };
    }
    std::vector< move_info > generate_candidate( int pos_x, int pos_y ){ //O(dim)
	std::vector< move_info > m;
	//check row constraint
	bool digits[10];
	for( auto &i : digits ){
	    i = true;
	}
	for( int c = 0; c < 9; ++c ){
	    if( board_fin[pos_y][c] != 0 ){
		digits[ board_fin[pos_y][c] ] = false;
	    }
	}	
	//check column constraint
	for( int r = 0; r < 9; ++r ){
	    if( board_fin[r][pos_x] != 0 ){
		digits[ board_fin[r][pos_x] ] = false;
	    }
	}
	//check 3x3 block constraint
	int seg_y = pos_y / 3;
	int seg_x = pos_x / 3;

	for(int i = 0; i < 3; ++i ){
	    for(int j = 0; j < 3; ++j ){
		int i_offset = i + seg_y*3;
		int j_offset = j + seg_x*3;
		if( board_fin[i_offset][j_offset] != 0 ){
		    digits[ board_fin[i_offset][j_offset] ] = false;
		}
	    }
	}
	for( int i = 1; i <= 9; ++i ){
	    if( digits[i] ){
		m.push_back( move_info{ i, pos_x, pos_y } );
	    }
	}
	return m;
    }
    void move( int digit, int pos_x, int pos_y ){
	assert( digit > 0 && digit < 10 );
	move_stack.push_back( move_info { digit, pos_x, pos_y } );
	board_fin[pos_y][pos_x] = digit;
	++nums_filled;
	// std::cout << "move: " << digit << ": " << pos_y << ", " << pos_x << std::endl;
	// std::cout << "increment fill count: " << nums_filled << std::endl;
	assert( nums_filled <= 81 );
    }
    void unmove(){
	assert( move_stack.size() > 0 );
	assert( nums_filled > 0 );
	auto & b = move_stack.back();
	board_fin[b.pos_y][b.pos_x] = 0;
	move_stack.pop_back();
	--nums_filled;
	// std::cout << "decrement fill count: " << nums_filled << std::endl;
	assert( nums_filled >= 0 );
    }
    void solve(){ //O((dim^2)! * dim^3)

	if( is_solved() ){
	    return;
	}

	// print_board();
	// getchar();

	auto freecell = get_free_cell();
	// std::cout << "selected free cell: " << freecell.second << ", " << freecell.first << std::endl;
	auto cand = generate_candidate( freecell.first, freecell.second );
	// std::cout << "candidate: ";
	// for(auto &i : cand){
	//     std::cout << i.digit << " ";
	// }
	// std::cout << std::endl;
	for(auto &i : cand){
	    move(i.digit, i.pos_x, i.pos_y);
	    solve();
	    if( solved ){
		return;
	    }
	    unmove();
	}
    }
    void print_board(){
	for(size_t i = 0; i < 9; ++i ){
	    for( size_t j = 0; j < 9; ++j ){
		if( board_fin[i][j] > 0 ){
		    std::cout << board_fin[i][j] << " ";
		}else{
		    std:: cout << "- ";
		}
	    }
	    std::cout << std::endl;
	}
    }
    void print_moves(){
	for( auto i : move_stack ){
	    std::cout << i.digit << ": " << i.pos_y << ", " << i.pos_x << std::endl;
	}
    }
};

int main(){
    std::vector<std::vector<int>> input( 9, std::vector<int>( 9, 0 ) );
    input[0][7] = 1;
    input[0][8] = 2;
    input[1][4] = 3;
    input[1][5] = 5;
    input[2][3] = 6;
    input[2][7] = 7;
    input[3][0] = 7;
    input[3][6] = 3;
    input[4][3] = 4;
    input[4][6] = 8;
    input[5][0] = 1;
    input[6][3] = 1;
    input[6][4] = 2;
    input[7][1] = 8;
    input[7][7] = 4;
    input[8][1] = 5;
    input[8][6] = 6;
    solver s( input );
    s.solve();
    if( s.solved ){
	std::cout << "solved!" << std::endl;
	s.print_board();
	s.print_moves();
    }else{
	std::cout << "no solution!" << std::endl;
    }
    return 0;
}
