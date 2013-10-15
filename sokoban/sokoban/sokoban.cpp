// control_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "conio.h"
#include <string>
#include <ctime>
#include <vector>

using namespace std;

typedef unsigned int uint;

enum objects  
{
	FREE_SPACE = 0,
	WALL,
	PLAYER,
	BOX,
	BOX_PLACE,
	BOX_ON_PLACE,
	PLAYER_ON_BOX_PLACE
};

enum control 
{	
	KEY_UP = 72,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,	
	KEY_DOWN = 80	
};

struct modes 
{
	int next;
	int position;
};

//Objects size set
const int width = 7;
const int height = 5;

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

int* startLavel = 0;
int* boxes = 0;
int* current_lavel = 0;

int boxes_on_place = 0;

void text_color(char symbol, uint foreground, uint background) 
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, foreground | background | BACKGROUND_INTENSITY);
	cout << symbol;
	SetConsoleTextAttribute(hConsole, 7);
}

void string_color(string text) 
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	cout << text;
	SetConsoleTextAttribute(hConsole, 7);
}

void printObject(uint posX, uint posY) 
{
	HANDLE hConsole;
	COORD cursorPos;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorPos.X = posX;
	cursorPos.Y = posY;
	SetConsoleCursorPosition(hConsole, cursorPos);
}

void debug_menu(uint position_x, uint position_y, int* lavel, uint current_position) 
{
	printObject(position_x, position_y);
	cout << "Position: " << current_position;
	printObject(position_x, position_y + 2);
	cout << "left: " << lavel[current_position-1];
	printObject(position_x, position_y + 3);
	cout << "up: " << lavel[current_position-MAP_HEIGHT];
	printObject(position_x, position_y + 4);
	cout << "right: " << lavel[current_position+1];
	printObject(position_x, position_y + 5);
	cout << "down: " << lavel[current_position+MAP_HEIGHT];
	
	printObject(position_x, position_y + 7);
	cout << "boxes: " << boxes;

	int j = 10;
	for (int i = 0; i < 100; i++) 
	{
		if (i % 10 == 0) 
		{
			printObject(position_x, position_y + j);
			j++;
		}
		cout << lavel[i];
		
	}

}

void object(int posX, int posY, char symbol, int foreground, int background) 
{
	for (int i = 0; i < height; i++) 
	{
		printObject(posX, posY);
		for (int j = 0; j < width; j++)
		{
			text_color(symbol, foreground, background);
		}
		posY++;
	}
}

void wall(int position) 
{
	uint posX, posY;

	posX = (position % MAP_WIDTH) * width;
	posY = (position / MAP_HEIGHT) * height;

	object(posX, posY, 219, 7, 7);
}

void free_space(int position) 
{
	int posX, posY;

	posX = (position % MAP_WIDTH) * width;
	posY = (position / MAP_HEIGHT) * height;

	char symbol;
	symbol = 32;

	for (int i = 0; i < height; i++) 
	{
		printObject(posX, posY);
		for (int j = 0; j < width; j++)
		{
			cout << symbol;
		}
		posY++;
	}
}

void player(int position) 
{
	int posX, posY;

	posX = (position % MAP_WIDTH) * width;
	posY = (position / MAP_HEIGHT) * height;

	object(posX, posY, 219, 2, 2);
}

void box(int position)
{
	int posX, posY;
	posX = (position % MAP_WIDTH) * width;
	posY = (position / MAP_HEIGHT) * height;

	object(posX, posY, 32, 300, 300);
}

void box_place(int position)
{
	int posX, posY;
	posX = (position % MAP_WIDTH) * width;
	posY = (position / MAP_HEIGHT) * height;

	object(posX, posY, 32, 0, 0);
}

void box_on_the_box_place(int position)
{
	int posX, posY;
	posX = (position % MAP_WIDTH) * width;
	posY = (position / MAP_HEIGHT) * height;

	object(posX, posY, 32, 500, 500);
}

void player_on_the_box_place(int position)
{
	int posX, posY;
	posX = (position % MAP_WIDTH) * width;
	posY = (position / MAP_HEIGHT) * height;

	object(posX, posY, 32, 0, 0);
}

void world_draw(int* map) 
{
	int i = 0;
	while (i < 100) 
	{
		switch(map[i]) 
		{
		case 1:
			wall(i);
			break;
		}

		i++;
		if (i % MAP_WIDTH == 0) 
		{
			cout << endl;
		}
	}
}


int play_zone_draw(int* map) 
{
	int i = 0;
	modes play_zone;
	while (i < 100)
	{
		switch(map[i]) 
		{
		case FREE_SPACE:
			free_space(i);
			break;			
		case PLAYER:
		case PLAYER_ON_BOX_PLACE:
			play_zone.position = i;
			player(i);
			break;
		case BOX:
			play_zone.next = i;
			box(i);
			break;
		case BOX_PLACE:
			play_zone.next = i;
			box_place(i);
			break;
		case BOX_ON_PLACE:
			play_zone.next = i;
			box_on_the_box_place(i);
			break;
		}

		i++;

		if (i % MAP_WIDTH == 0) 
		{
			cout << endl;
		}
	}
	return play_zone.position;
}

void movePlayer(int step, int currentPosition, int* map, objects current, objects next) 
{
	map[currentPosition] = current;
	currentPosition = currentPosition + step;
	map[currentPosition] = next;
}

// 0 - free space
// 1 - wall
// 2 - player
// 3 - box
// 4 - box place
// 5 - box on place
// 6 - player on place

int lavel1[] = 
{
	1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,4,1,
	1,0,3,0,0,2,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,4,4,0,0,0,1,
	1,0,0,0,4,4,0,4,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,3,0,0,0,0,3,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1
}; 

int lavel2[] = 
{
	1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,1,1,1,0,1,
	1,1,0,0,0,0,1,1,0,1,
	1,1,1,0,0,3,0,1,0,1,
	1,2,1,0,0,0,0,1,0,1,
	1,0,4,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1
};

void boxes_count(int* map) 
{
	int i = 0;
	while(i < 100) 
	{
		if(map[i] == BOX) 
		{
			boxes++;
		}
		i++;
	}
}

void lavel_choose(int lavel_number) 
{
	switch(lavel_number) 
	{
	case 1:
		current_lavel = lavel1;
		break;
	case 2:
		current_lavel = lavel2;
		break;
	};
}

void player_control(int currentPosition, int* map, char input) 
{
	int step;
	switch(input) 
		{
		case KEY_LEFT:
			step = -1;
			break;
		case KEY_DOWN:
			step = MAP_HEIGHT;
			break;
		case KEY_RIGHT:
			step = 1;
			break;
		case KEY_UP:
			step = -(MAP_HEIGHT);				
			break;
		}

		//move box to place
		if (map[currentPosition + step] == BOX && map[currentPosition + 2 * step] == BOX_PLACE) 
		{
			map[currentPosition + step] = FREE_SPACE;
			map[currentPosition + 2 * step] = BOX_ON_PLACE;
			boxes--;
		}

		//move box
		if (map[currentPosition + step] == BOX && map[currentPosition + 2 * step] == FREE_SPACE) 
		{
			map[currentPosition + step] = FREE_SPACE;
			map[currentPosition + 2 * step] = BOX;
		}

		//move box from place to place
		if(map[currentPosition + step] == BOX_ON_PLACE && map[currentPosition + 2 * step] == BOX_PLACE) 
		{
			map[currentPosition + step] = BOX_PLACE;
			map[currentPosition + 2 * step] = BOX_ON_PLACE;
		}

		//move box from place to empty place
		if(map[currentPosition + step] == BOX_ON_PLACE && map[currentPosition + 2 * step] == FREE_SPACE) 
		{
			map[currentPosition + step] = BOX_PLACE;
			map[currentPosition + 2 * step] = BOX;
			boxes++;
		}

		//move player
		if (map[currentPosition + step] == FREE_SPACE || map[currentPosition + step] == BOX_PLACE) 
		{
			if (map[currentPosition + step] == FREE_SPACE && map[currentPosition] == PLAYER) {
				movePlayer(step, currentPosition, map, FREE_SPACE, PLAYER); }
					
			if (map[currentPosition + step] == BOX_PLACE && map[currentPosition] == PLAYER) {
				movePlayer(step, currentPosition, map, FREE_SPACE, PLAYER_ON_BOX_PLACE); }

			if (map[currentPosition + step] == BOX_PLACE && map[currentPosition] == PLAYER_ON_BOX_PLACE) {
				movePlayer(step, currentPosition, map, BOX_PLACE, PLAYER_ON_BOX_PLACE); }

			if (map[currentPosition + step] == FREE_SPACE && map[currentPosition] == PLAYER_ON_BOX_PLACE) {
				movePlayer(step, currentPosition, map, BOX_PLACE, PLAYER); } 
		}		
}

void Display (int start_lavel) 
{
	unsigned char input;
	int currentPosition;

	lavel_choose(start_lavel);
	int* Lavel;

	Lavel = current_lavel;	
	boxes_count(Lavel);

	world_draw(Lavel);

	while (boxes != 0) 
	{
		currentPosition = play_zone_draw(Lavel);
		debug_menu(72, 5, Lavel, currentPosition);

		input = _getch();

		if (input == 224) {
			input = _getch(); }

		player_control(currentPosition, Lavel, input);

	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//screen initialize
	CONSOLE_CURSOR_INFO cursor_info;
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cursor_info.dwSize = 1;
	cursor_info.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursor_info);
	system("mode con cols=100 lines=55");
	////////////////////////////////////////////////////////

	for (int i = 1; i < 3; i ++) 
	{
		Display(i);
	}

	return 0;
}