#include<bits/stdc++.h>
using namespace std;

class Board;
class Food;

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
