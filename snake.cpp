#include<bits/stdc++.h>
#include<windows.h> // windows only
using namespace std;

// ^ -y(row)
// |
// |
// ---> x(col)

class Board;
class Food;
class Snake;

struct position{
	int x,y;

    position(){}
	position(int a, int b):x(a),y(b){}
};

class Board{
private:
	int height,width;
	char** field;
public:
	Board(){
		height = 20;
		width = 50;

		field = new char*[height];
		for(int i=0;i<height;i++)
			field[i] = new char[width];
	}

	~Board(){
		for(int i=0;i<height;i++)
			delete[] field[i];

		delete[] field;
	}

	void show_board(){
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				cout<<field[i][j];
			}
			cout<<endl;
		}
	}

	void clear_board(){
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++)
				field[i][j]=' ';
		}
	}

	int get_board_height(){
		return height;
	}

	int get_board_width(){
		return width;
	}

	void set_on_board(int row, int col, char c){
		field[row][col]=c;
	}

}field;


class Food{
private:
	position food_point;
	char symbol;

public:
	Food():symbol('X'){}

	void set_food_position(int x, int y, Board& field){
		food_point.x=x;
		food_point.y=y;
	}

	void reset_food_position(Board& field){
		food_point.x = rand()%field.get_board_width();
		food_point.y = rand()%field.get_board_height();
	}

	int get_food_x(){
		return food_point.x;
	}

	int get_food_y(){
		return food_point.y;
	}

	char get_food_symbol(){
		return symbol;
	}

}eatable;

//snake lenght increases after eating food
//so here also it increases after it has passed food block i.e eaten food
class Snake{
private:
	char body_head_symbol,body_part_symbol;
	vector<position> body;
	int body_size;
	position head;
	enum direction {UP,DOWN,LEFT,RIGHT};
	direction dir;
public:
	Snake(int x = 3, int y = 3):body_head_symbol('@'),body_part_symbol('o'),dir(DOWN),body_size(1){
		position tmp(x,y);
		body.push_back(tmp);

		head = body[0];
	}

	int get_head_x(){
		return head.x;
	}

	int get_head_y(){
		return head.y;
	}

	char get_head_symbol(){
		return body_head_symbol;
	}

	char get_nonhead_symbol(){
		return body_part_symbol;
	}

	//windows.h
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

	void move(){
		//speed = 1
		position head_modify(0,0);
		if(dir == UP)
			head_modify.y = -1;
		else if(dir == DOWN)
			head_modify.y = 1;
		else if(dir == RIGHT)
			head_modify.x = 1;
		else if(dir == LEFT)
			head_modify.x = -1;

		position new_head(0,0);
		new_head.x = body[0].x + head_modify.x;
		new_head.y = body[0].y + head_modify.y;

		for(int i=body_size-1; i>0; i--)
			body[i] = body[i-1];
		head = new_head;
		body[0]=head;

		if(head.x < 0 || head.y <0 || head.x>=field.get_board_width() || head.y>=field.get_board_height()){
			cout<<"SNAKE DEAD.....!!!!"<<endl;
			exit(0);
		}

	}

	bool food_found(Food& foodie){
		if(foodie.get_food_x() == head.x && foodie.get_food_y() == head.y){
			//size increment
			body_size+=1;

			position tmp(0,0);
			body.push_back(tmp);
			return true;
		}
		return false;
	}

	void set_snake_onboard(Board& draw_here){

		field.set_on_board(head.y,head.x,body_head_symbol);

		for(int i=1; i<body.size(); i++)
			field.set_on_board(body[i].y,body[i].x,body_part_symbol);

	}

}player;


int main(){

	srand(time(0));

	eatable.set_food_position(6,6,field);

	while(1){
		field.clear_board();
		player.get_input();

		player.move();

        field.set_on_board(eatable.get_food_y(),eatable.get_food_x(),eatable.get_food_symbol());
		player.set_snake_onboard(field);

		if(player.food_found(eatable)){
			eatable.reset_food_position(field);
		}

		field.show_board();

		system("cls");//clear screen

	}

	return 0;
}
