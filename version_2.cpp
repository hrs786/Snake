#include <bits/stdc++.h>
#include <windows.h> // For MS Windows only


// Classes declaration
class Board;
class Element;
class Food;
class Snake;


// Point on screen/board in terms of (x,y)
// 1-based indexing/positioning
struct Coordinate {
	int x;
	int y;

    Coordinate() {}
	Coordinate(int a, int b) : x(a), y(b) {}

	bool operator==(const Coordinate& other) const{
		return (x == other.x && y == other.y);
	}
};


// Class implementing hash function to store objects of class "Coordinate"
class MyHash {
private:
	std::hash<int> hash_; // hash function for int type

public:
	size_t operator()(const Coordinate& p) const{
		return hash_(p.x)^hash_(p.y); // xor of two hashes as a new hash
	}
};


// Board class definition
class Board {
private:
	int height, width; // height(along y), width(along x) of playable Board
	char symbol; // symbol for boundary of Board
	char** field; // char 2D-array for storing values present on Board

public:
	// Constructor
	Board() : height(20), width(50), symbol('*') {
		// dynamic memory allocation for board
		field = new char*[height+2];
		for (int i=0;i<height+2;i++)
			field[i] = new char[width+2];
	}

	~Board() {
		// deallocating dynamic memory allocated previously
		for (int i=0;i<height+2;i++)
			delete[] field[i];
		delete[] field;
	}

	// print board on screen
	void show_board() {
		for (int i=0; i<height+2; i++) {
			for (int j=0; j<width+2; j++)
				std::cout << field[i][j];
			std::cout << '\n';
		}
		std::cout << std::endl;
	}

	// clean the board
	void clear_board() {
		for (int i=0;i<height+2;i++) {
			for (int j=0;j<width+2;j++)
				field[i][j]=' ';
		}

		//set boundary
		for (int i=0;i<width+2;i++) {
			field[0][i]=symbol;
			field[height+1][i]=symbol;
		}
		for (int i=0;i<height+2;i++) {
			field[i][0]=symbol;
			field[i][width+1]=symbol;
		}
	}

	// returns board height
	int get_board_height() {
		return height;
	}

	// returns board width
	int get_board_width() {
		return width;
	}

	// set character on board
	void set_on_board(int row, int col, char c) {
		field[row][col] = c;
	}

	// return boundary symbol
	char get_boundary_symbol() {
		return symbol;
	}

};
Board field; //object with name "field" of class "Board"


class Element {
protected:
	Coordinate pos; // Coordinate of element on board
	char symbol;  // character symbol for representing element

public:
	// Constructor
	Element() {}
	Element(char symbol_) : symbol(symbol_) {}
	Element(Coordinate pos_, char symbol_) : pos(pos_), symbol(symbol_) {}

	// set Coordinate of element on board
	void set_position(int x, int y) {
		pos.x = x;
		pos.y = y;
	}

	// returns x-coordinate of element
	virtual int get_x() {
		return pos.x;
	}

	// returns y-coordinate of element
	virtual int get_y() {
		return pos.y;
	}

	// return element symbol
	char get_symbol() {
		return symbol;
	}
};


// Food class definition
class Food : public Element {
private:

public:
	// Constructor
	Food() : Element('X') {}

	// set food on board when given food's coordinates
	void set_food_position(int x, int y, Board& field) {
		pos.x = x;
		pos.y = y;
	}

	// randomly allocating food Coordinate on board with rand() function
	void reset_food_position(Board& field) {
		pos.x = rand() % field.get_board_width();
		pos.y = rand() % field.get_board_height();
	}

	// returns x-coordinate of food
	int get_x() override {
		return 1+pos.x;
	}

	// returns y-coordinate of food
	int get_y() override {
		return 1+pos.y;
	}
};
Food eatable;

// Few important points w.r.t functioning :-
// Snake length increases after eating food,
// so here also it increases after it has passed food block i.e eaten food

// Snake class definition
class Snake : public Element {
private:
	char body_head_symbol,body_part_symbol; // symbols for head and body(except) head of snake
	std::vector<Coordinate> body; // storing body of snake body[0]->head, body[size-1]->tail
	int body_size; // size of snake's body
	Coordinate pos;	// Coordinate of snake's head on board
	enum Direction {UP, DOWN, LEFT, RIGHT};	// Directions of motion(w.r.t player in front of screen)
	Direction dir; // current Direction of motion of snake's head
	std::unordered_set<Coordinate, MyHash> uset;	// set stores unique body parts

public:
	// Constructor
	Snake(int x=3, int y=3) : body_head_symbol('@'), body_part_symbol('o'), dir(DOWN), body_size(1), Element(Coordinate(x+1, y+1), body_head_symbol) {
		// set Coordinate of head
		Coordinate tmp(x+1, y+1);
		body.push_back(tmp);
		pos = body[0];
		uset.insert(pos);
	}

	// returns symbol of snake's non-head
	char get_nonhead_symbol() {
		return body_part_symbol;
	}

	// gets player input for Direction of head and store in dir
	// windows.h --> GetAsyncKeyState --> checks if key was pressed after a previous call
	// VK_[key_name] refers to virtual key code
	void get_input() {
		if (GetAsyncKeyState(VK_UP) && dir != DOWN)
			dir = UP;
		else if (GetAsyncKeyState(VK_DOWN) && dir != UP)
			dir = DOWN;
		else if (GetAsyncKeyState(VK_LEFT) && dir != RIGHT)
			dir = LEFT;
		else if (GetAsyncKeyState(VK_RIGHT) && dir != LEFT)
			dir = RIGHT;
	}

	// movement of snake
	void move() {
		// flag to check if snake has eaten food
		bool is_size_increased = false;
		Coordinate tmp(0,0);
		if (body[body_size-1] == tmp)
			is_size_increased = true; // (0,0) is never part of snake, so is used to set flag

		// stores modification in head
		Coordinate head_modify(0,0);
		if (dir == UP)
			head_modify.y = -1;
		else if (dir == DOWN)
			head_modify.y = 1;
		else if (dir == RIGHT)
			head_modify.x = 1;
		else if (dir == LEFT)
			head_modify.x = -1;

		// finds new head Coordinate
		Coordinate new_head(body[0].x + head_modify.x, body[0].y + head_modify.y);

		if (is_size_increased) {
			uset.insert(new_head); // if food eaten increase size by adding new_head as extra element to body
		} else {
			uset.erase(body[body_size-1]); // if food not eaten remove tail, add new_head
			uset.insert(new_head);
		}

		// to check if snake's head collided it's body
		// by comparing size of body and set of it's body part, as set needs to have unique elements
		bool collide = false;
		if (body_size != uset.size())
			collide = true;

		// update snake Coordinate
		for (int i=body_size-1; i>0; i--)
			body[i] = body[i-1];
		pos = new_head;
		body[0]=pos;

		// Kills snake if it hits any wall or itself
		if (pos.x <= 0 || pos.y <= 0 || pos.x >= 1+field.get_board_width() || pos.y >= 1+field.get_board_height() || collide) {
			throw (std::string)"SNAKE DEAD.....!!!!";
		}
	}

	// tells if snake(head) has reached food
	bool food_found(Food& foodie) {
		if (foodie.get_x() == pos.x && foodie.get_y() == pos.y) {
			body_size++; // increase size of snake
			// adds a temporary Coordinate at end which is helpful while movement of snake(move method) after eating
			Coordinate tmp(0,0);
			body.push_back(tmp);
			return true;
		}
		return false;
	}

	// set the snake's symbols on board at it's Coordinate
	void set_snake_onboard(Board& draw_here) {
		field.set_on_board(pos.y, pos.x, body_head_symbol);
		for (int i=1; i<body.size(); i++)
			field.set_on_board(body[i].y, body[i].x, body_part_symbol);
	}
};
Snake player; // object "player" of class "Snake"


int main() {
	std::cout << "\033[?25l"; // hides cursor

	// current unix time in seconds as seed for rand function
	srand(time(0));

	//sets initial food Coordinate
	eatable.set_food_position(6,6,field);

	// loop until the player kills snake
	while (true) {
		field.clear_board(); // clears board
		player.get_input(); // finds if user has pressed any key until previous execution of loop

		// moves snake
		try {
			player.move();
		} catch(std::string err) { // if snake dies
			field.clear_board();
			std::cout << err << std::endl; // prints error message
			system("pause > nul"); // pause system and wait for key press, MS Windows only
			return 0; // exit program
		}

        field.set_on_board(eatable.get_y(),eatable.get_x(),eatable.get_symbol()); //set food on board
		player.set_snake_onboard(field); // set snake on board

		// if snake(head) has found food resets food randomly
		if (player.food_found(eatable))
			eatable.reset_food_position(field);

		field.show_board(); // prints board

		Sleep(200);	// Windows.h --> milliseconds for which to stop execution, slows speed of snake
        std::cout << "\033[2J\033[1;1H"; // clear screen, cross-platform
	}

	std::cout << "\033[?25h"; // shows cursor
	return 0;
}
