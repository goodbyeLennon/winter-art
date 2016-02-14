/*
==Conway's Game of Life==
The goal of this program is play Conway's Game of Life on an arbitrarily sized finite grid
given a starting condition from a text file.  The game will be displayed in ASCII text.  In 
this version, if a live cell goes off the board, it wraps around the other edge.

desired future features:
-Have an unlimited number of steps and the game will end when the user hits ESC.

-detecting if the input file isn't formatted or sized properly and inserting 0s when the line ends early
 or skipping to the next line when there are extra characters on a line.

-Have the user define the intitial grid visually in the program using a cursor.

-Have an interactive menu for changing settings.


--------------------------rules---------------------------------
1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by over-population.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <Windows.h> //for Sleep()

const int XSIZE = 48;
const int YSIZE = 32;
const int STEPS = 1000;
const int FRAMEDELAY = 150; //time between frames in ms
const char LIVE = 'O';
const char DEAD = ' ';

void fillGrid(std::ifstream& in_s, int grid[][XSIZE], const int y, const int x); //gets the starting grid from a file and fills the base game grid
void updateGrid(int currentGrid[][XSIZE], int nextGrid[][XSIZE], const int y, const int x); //calculate the next grid iteration
void drawGrid(const int grid[][XSIZE], const int y, const int x); //draw the grid on screen.
bool cellChecker(const int grid[][XSIZE], const int ysize, const int xsize, int yLoc, int xLoc); //determines whether an individual cell is alive or dead.

int main()
{
	//variables
	std::ifstream startGrid; //the stream to open the starting grid file
	int currentGrid[YSIZE][XSIZE] = {}; //the current grid, which is displayed and used to calculate the next iteration
	int nextGrid[YSIZE][XSIZE] = {}; //the grid that is filled with the next iteration of the game
	int stepNumber = 0; // the current iteration of the game

	std::cout << "\t\t===Conway's Game of Life::";
	std::cout << XSIZE << " x " << YSIZE << " edition===\n\n";

	fillGrid(startGrid, currentGrid, YSIZE, XSIZE);

	while(stepNumber < STEPS)
	{
		drawGrid(currentGrid, YSIZE, XSIZE);
		updateGrid(currentGrid, nextGrid, YSIZE, XSIZE);
		stepNumber++;
		std::cout << "\n\n" << stepNumber;
	}

	return 0;

}

void fillGrid(std::ifstream& in_s, int grid[][XSIZE], int y, int x)
{
	using namespace std;

	string fileName;
	char next;
	int nextInt;

	cout << "Please enter the filename for the starting grid: ";
	cin >> fileName;

	in_s.open(fileName); 	 	//  Connect to the input file and test 
    if( in_s.fail() )
    { 
       cout << "Input file opening failed. \n"; 
       exit(1); 				 // if we couldn't open the file to read from we exit 
    }

	for(int i = 0; i < y; i++)
	{
		for(int j = 0 ; j < x; j++)
		{
			if( (in_s >> next) ) 
			{
				
				nextInt = next - '0'; //convert from char to int
				switch(nextInt)
				{
					case 1:
						grid[i][j] = 1;
						break;

					case 0:
					//default:						
						grid[i][j] = 0; //non-one numbers are saved as zeros.
						break;					
				}				
			}

			//if there are not enough numbers, fill the rest of the table with zeros.
			else
			{
				grid[i][j] = 0;
				cout << "\nThe input file is not formatted properly. Grid location ";
				cout << "(" << i << ", " << j << ") was automatically filled with a 0.";
				
			}
		}
	}

	in_s.close();

}

void drawGrid(const int grid[][XSIZE], const int y, const int x)
{
	using namespace std;
	int next;

	Sleep(FRAMEDELAY);  //wait for an amount of time between drawing frames
	system("CLS");		//clear the screen before drawing the next frame.
	cout << "\n\n";

	for(int i = 0; i < y; i++)
	{
		cout << endl;

		for(int j = 0; j < x; j++)
		{
			next = grid[i][j];
			if(next == 1)
			{
				cout << LIVE;
			}
			else
			{
				cout << DEAD;
			}
		}
	}

}

void updateGrid(int currentGrid[][XSIZE], int nextGrid[][XSIZE], const int y, const int x)
{
	bool cellState;

	//create the next grid
	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)
		{
			cellState = cellChecker(currentGrid, YSIZE, XSIZE, i, j);
			if(cellState == 1)
			{
				nextGrid[i][j] = 1;
			}
			else
			{
				nextGrid[i][j] = 0;
			}
		}
	}

	//copy the new grid to the current grid in preparation for the next iteration.
	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)
		{
			currentGrid[i][j] = nextGrid[i][j];
		}
	}
}

bool cellChecker(const int grid[][XSIZE], const int ysize, const int xsize, int yLoc, int xLoc)
{
	
	int yCheck; //the current y position being checked.
	int xCheck;
	int liveNeighbors = 0; //the running total number of live neighbor cells.

	for(int i = 0; i < 3; i++)
	{
		yCheck = ((yLoc + (i - 1) + ysize) % ysize); //calculate the current row to check.  Wrap around the grid if there is not a grid above/below the current one.
		for(int j = 0; j < 3; j++)
		{
			xCheck = ((xLoc + (j - 1) + xsize) % xsize);
			//std::cout << "\nLoc (" << yCheck << "," << xCheck << " is " << grid[yCheck][xCheck]; //for testing.
			if( grid[yCheck][xCheck] == 1 ) //if the cell is alive, add one to the running counter of live neighbors.
			{
				if(yCheck == yLoc && xCheck == xLoc)
				{
					//do nothing
				}
				else
				{
					liveNeighbors++;
				}
			}
		}
	}

	//for testing
	//std::cout << "\nliveNeighbors = " << liveNeighbors << "\n";

	//calc the return conditions if the cell is already alive.
	if( grid[yLoc][xLoc] == 1 )
	{
		if( liveNeighbors < 2 || liveNeighbors > 3 )
		{
			return 0;
		}

		if( liveNeighbors == 2 || liveNeighbors == 3 )
		{
			return 1;
		}
	}

	if( grid[yLoc][xLoc] == 0 )
	{
		if( liveNeighbors == 3 )
		{
			return 1;
		}
	}

	if( grid[yLoc][xLoc] != 0 && grid[yLoc][xLoc] != 1 )
	{
		std::cout << "\nSomething went wrong...\n";
		exit(1);
	}

}