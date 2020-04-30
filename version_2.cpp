// ******** Snake Video Game ************
// Only for MS Windows

// UP, DOWN, RIGHT, LEFT keys for controlling snake

// Directions of X, Y axis
// ^ -y(row)
// |
// |
// ---> x(col)


#include<bits/stdc++.h>
#include<windows.h> // For MS Windows only
using namespace std;

// Classes declaration
class Board;
class Food;
class Snake;

// Point on screen/board in terms of (x,y)
// 1-based indexing/positioning
struct position{
	int x,y;

	// Constructors
    position(){}
	position(int a, int b):x(a),y(b){}

	bool operator== (const position& other) const{
		if(x == other.x && y == other.y)
			return true;

		return false;
	}
};

// Class implementing hash function to store objects of class "position"
class MyHash{
public:
	size_t operator()(const position& p) const{
		return (hash<int>()(p.x))^(hash<int>()(p.y)); // xor of two hashes as a new hash
	}
};

// Board class definition
class Board{
private:
	int height,width; // height(along y), width(along x) of Playable Board
	char symbol;
	char** field;	  // char 2D-array for storing values present on Board
public:
	// Constructor
	Board(){
		height = 20; // initializing height of board
		width = 50;  // initializing width of board

		symbol = '*';

		// dynamic memory allocation for board
		field = new char*[height+2];
		for(int i=0;i<height+2;i++)
			field[i] = new char[width+2];
	}

	//Destructor
	~Board(){
		// deallocating dynamic memory allocated previously
		for(int i=0;i<height+2;i++)
			delete[] field[i];

		delete[] field;
	}

	// print board on screen
	void show_board(){
		for(int i=0;i<height+2;i++){
			for(int j=0;j<width+2;j++){
				cout<<field[i][j];
			}
			cout<<endl;
		}
	}

	// clean the board
	void clear_board(){
		for(int i=0;i<height+2;i++){
			for(int j=0;j<width+2;j++)
				field[i][j]=' ';
		}

		//set boundary
		for(int i=0;i<width+2;i++)
			field[0][i]=symbol;
		for(int i=0;i<width+2;i++)
			field[height+1][i]=symbol;
		for(int i=0;i<height+2;i++)
			field[i][0]=symbol;
		for(int i=0;i<height+2;i++)
			field[i][width+1]=symbol;
	}

	// returns board height
	int get_board_height(){
		return height;
	}

	// returns board width
	int get_board_width(){
		return width;
	}

	// set value on board
	void set_on_board(int row, int col, char c){
		field[row][col]=c;
	}

	// return boundary symbol
	char get_boundary_symbol(){
		return symbol;
	}

}field; //object with name "field" of class "Board"

// Food class definition
class Food{
private:
	position food_point; // position of food on the board
	char symbol;		 // character symbol for representing food

public:
	// Constructor
	Food():symbol('X'){}

	// set food on board when given food's coordinates
	void set_food_position(int x, int y, Board& field){
		food_point.x=x;
		food_point.y=y;
	}

	// randomly allocating food position on board with rand() function
	void reset_food_position(Board& field){
		food_point.x = rand()%field.get_board_width();
		food_point.y = rand()%field.get_board_height();
	}

	// returns x-coordinate of food
	int get_food_x(){
		return 1+food_point.x;
	}

	// returns y-coordinate of food
	int get_food_y(){
		return 1+food_point.y;
	}

	// return food symbol
	char get_food_symbol(){
		return symbol;
	}

}eatable;

// Few important points w.r.t functioning :-
// Snake lenght increases after eating food,
// so here also it increases after it has passed food block i.e eaten food

// Snake class definition
class Snake{
private:
	char body_head_symbol,body_part_symbol; // symbols for head and body(except) head of snake
	vector<position> body; 					// storing body of snake body[0]->head, body[size-1]->tail
	int body_size;							// size of snake's body
	position head;							// position of snake's head on board
	enum direction {UP,DOWN,LEFT,RIGHT};	// directions of motion(w.r.t player in front of screen)
	direction dir;							// current direction of motion of snake's head
	unordered_set<position, MyHash> uset;	// set stores unique body parts
public:
	// Constructor
	Snake(int x = 3, int y = 3):body_head_symbol('@'),body_part_symbol('o'),dir(DOWN),body_size(1){
		// set position of head
		position tmp(x+1,y+1);
		body.push_back(tmp);

		head = body[0];
		uset.insert(head);
	}

	// returns x-coordinate of head
	int get_head_x(){
		return head.x;
	}

	// returns y-coordinate of head
	int get_head_y(){
		return head.y;
	}

	// returns symbol of snake's head
	char get_head_symbol(){
		return body_head_symbol;
	}

	// returns symbol of snake's non-head
	char get_nonhead_symbol(){
		return body_part_symbol;
	}

	// gets player input for direction of head and store in dir
	// windows.h --> GetAsyncKeyState --> checks if key was pressed after a previous call
	// VK_[key_name] refers to virtual key code
	void get_input(){
		if(GetAsyncKeyState(VK_UP) && dir != DOWN)
			dir = UP;
		else if(GetAsyncKeyState(VK_DOWN) && dir != UP)
			dir = DOWN;
		else if(GetAsyncKeyState(VK_LEFT) && dir != RIGHT)
			dir = LEFT;
		else if(GetAsyncKeyState(VK_RIGHT) && dir != LEFT)
			dir = RIGHT;
	}

	// movement of snake
	void move(){
		// flag to check if snake has eaten food
		bool is_size_increased = false;
		position tmp(0,0);
		if(body[body_size-1] == tmp)
			is_size_increased = true; // (0,0) is never part of snake, so is used to set flag

		// stores modification in head
		position head_modify(0,0);
		if(dir == UP)
			head_modify.y = -1;
		else if(dir == DOWN)
			head_modify.y = 1;
		else if(dir == RIGHT)
			head_modify.x = 1;
		else if(dir == LEFT)
			head_modify.x = -1;

		// finds new head position
		position new_head(0,0);
		new_head.x = body[0].x + head_modify.x;
		new_head.y = body[0].y + head_modify.y;

		if(is_size_increased){
			uset.insert(new_head); // if food eaten increase size by adding new_head as extra element to body
		}
		else{
			uset.erase(body[body_size-1]); // if food not eaten remove tail, add new_head
			uset.insert(new_head);
		}

		// to check if snake's head collided it's body
		// by comparing size of body and set of it's body part, as set needs to have unique elements
		bool collide = false;
		if(body_size != uset.size())
			collide = true;

		// update snake position
		for(int i=body_size-1; i>0; i--)
			body[i] = body[i-1];
		head = new_head;
		body[0]=head;

		// Kills snake if it hits any wall or itself
		if(head.x <= 0 || head.y <= 0 || head.x >= 1+field.get_board_width() || head.y >= 1+field.get_board_height() || collide){
			throw (string)"SNAKE DEAD.....!!!!";
		}
			// try-catch is better (than like exit()) as it does proper cleanup(calls destructors)
			// For more details - https://stackoverflow.com/questions/30250934/how-to-end-c-code

	}

	// tells if snake(head) has reached food
	bool food_found(Food& foodie){
		if(foodie.get_food_x() == head.x && foodie.get_food_y() == head.y){
			//size increment
			body_size+=1;

			// adds a temporary position at end which is helpful while movement of snake(move method) after eating
			position tmp(0,0);
			body.push_back(tmp);
			return true;
		}
		return false;
	}

	// set the snake's symbols on board at it's position
	void set_snake_onboard(Board& draw_here){

		field.set_on_board(head.y,head.x,body_head_symbol);

		for(int i=1; i<body.size(); i++)
			field.set_on_board(body[i].y,body[i].x,body_part_symbol);

	}

}player; // object "player" of class "Snake"


// hide cursor function only in windowed mode (i.e not full screen)
// Reference : https://superuser.com/questions/1496322/how-do-i-remove-hide-the-cursor-the-blinking-underscore-character-in-cmd-exe
void hide_cursor(){
	HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

// main function
int main(){

    //hides cursor
	hide_cursor();

	// cuurent unix time in seconds as seed for rand function
	srand(time(0));

	//sets initial food position
	eatable.set_food_position(6,6,field);

	// loop until the player kills snake
	while(1){
		field.clear_board(); // clears board
		player.get_input();  // finds if user has pressed any key until previous execution of loop

		// moves snake
		try{
			player.move();
		}
		catch(string err){
			field.clear_board();
			cout<<err<<endl;
			system("pause"); // pause system and wait for key press, MS Windows (NOT Linux)
			return 0;
		}

        field.set_on_board(eatable.get_food_y(),eatable.get_food_x(),eatable.get_food_symbol()); //set food on board
		player.set_snake_onboard(field); // set snake on board

		// if snake(head) has found food resets food randomly
		if(player.food_found(eatable)){
			eatable.reset_food_position(field);
		}

		field.show_board(); // prints board

		Sleep(100);		// Windows.h --> milliseconds for which to stop execution, slows speed of snake
		system("cls"); // clear screen

	}

	return 0;
}
