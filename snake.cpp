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
struct position{
	int x,y;

    position(){}
	position(int a, int b):x(a),y(b){}
};

// Board class definition
class Board{
private:
	int height,width; // height(along y), width(along x) of Board
	char** field;	  // char 2D-array for storing values present on Board
public:
	// Constructor
	Board(){
		height = 20; // initializing height of board
		width = 50;  // initializing width of board

		// dynamic memory allocation for board
		field = new char*[height];
		for(int i=0;i<height;i++)
			field[i] = new char[width];
	}

	//Destructor
	~Board(){
		// deallocating dynamic memory allocated previously
		for(int i=0;i<height;i++)
			delete[] field[i];

		delete[] field;
	}

	// print board on screen
	void show_board(){
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				cout<<field[i][j];
			}
			cout<<endl;
		}
	}

	// clean the board
	void clear_board(){
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++)
				field[i][j]=' ';
		}
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
		return food_point.x;
	}

	// returns y-coordinate of food
	int get_food_y(){
		return food_point.y;
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
public:
	// Constructor
	Snake(int x = 3, int y = 3):body_head_symbol('@'),body_part_symbol('o'),dir(DOWN),body_size(1){
		// set position of head
		position tmp(x,y);
		body.push_back(tmp);

		head = body[0];
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
	// windows.h --> GetAsyncKeyState
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
		//speed = 1

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

		// update snake position
		for(int i=body_size-1; i>0; i--)
			body[i] = body[i-1];
		head = new_head;
		body[0]=head;

		// Kills snake if it hits any wall
		if(head.x < 0 || head.y <0 || head.x>=field.get_board_width() || head.y>=field.get_board_height()){
			cout<<"SNAKE DEAD.....!!!!"<<endl;
			exit(0);
		}

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


// main function
int main(){

	// random seed for rand function
	srand(time(0));

	//sets initial food position
	eatable.set_food_position(6,6,field);

	// loop until the player kills snake
	while(1){
		field.clear_board(); // clears board
		player.get_input();  // finds if user has pressed any key until previous execution of loop

		player.move();		 // moves snake

        field.set_on_board(eatable.get_food_y(),eatable.get_food_x(),eatable.get_food_symbol()); //set food on board
		player.set_snake_onboard(field); // set snake on board

		// if snake(head) has found food resets food randomly
		if(player.food_found(eatable)){
			eatable.reset_food_position(field);
		}

		field.show_board(); // prints board

		system("cls"); // clear screen

	}

	return 0;
}
