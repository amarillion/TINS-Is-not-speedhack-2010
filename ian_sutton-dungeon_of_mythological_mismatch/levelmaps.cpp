#include <vector>
#include <stdlib.h>
#include "logging.hpp"

const float civilian_rarity = 0.04;
const float enemy1_rarity = 0.02;
const float enemy2_rarity = 0.01;
const float enemy3_rarity = 0.005;
const float ammo_stash_rarity = 0.001;


int generated_tilemap[91] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -1
};

int generated_objectmap[91] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -1
};


void GenerateTilemap(int seed) // obviously this seed will only work with a given implementation of rand, but I don't have time to add in my own pseudorandomness
{
    erlog("Generating new tile map");
    if (seed != -1)
        srand(seed);
    int maze_width = 6;
    int maze_height = 5;
    int width = 2 * maze_width+1;
    int height = 2 * maze_height+1;
    std::vector<std::vector<int> > maze;
    std::vector<int> moves;
	maze.resize(height);

    for(int x = 0; x < height; ++x) {
		maze[x].resize(width);
        for(int y = 0; y < width; y++) {
            maze[x][y] = 1;
        }
    }
    int x_pos = 1;
    int y_pos = 1;
    maze[x_pos][y_pos] = 0;
    moves.push_back(y_pos+(x_pos*width));


    while(moves.size() > 0) {

        std::vector<char> possible_directions;
        if(x_pos+2 < (int)maze.size() && maze[x_pos+2][y_pos] == 1 && x_pos+2 != 0 && x_pos+2 != height-1) {
            possible_directions.push_back('S');
        }
        if((int)x_pos-2 > 0 && maze[x_pos-2][y_pos] == 1 && x_pos-2 != height-1) {
            possible_directions.push_back('N');
        }
        if((int)y_pos-2 > 0 && maze[x_pos][y_pos-2] == 1 && y_pos-2 != width-1) {
            possible_directions.push_back('W');
        }
        if(y_pos+2 < (int)maze[x_pos].size() && maze[x_pos][y_pos+2] == 1 && y_pos+2 != 0 && y_pos+2 != width-1) {
            possible_directions.push_back('E');
        }
        if(possible_directions.size() > 0) {
            int move = rand() % possible_directions.size();
            switch (possible_directions[move]){
                case 'N':
                    maze[x_pos - 2][y_pos] = 0;
                    maze[x_pos - 1][y_pos] = 0;
                    x_pos -= 2;
                    break;
                case 'S':
                    maze[x_pos + 2][y_pos] = 0;
                    maze[x_pos + 1][y_pos] = 0;
                    x_pos +=2;
                    break;
                case 'W':
                    maze[x_pos][y_pos - 2] = 0;
                    maze[x_pos][y_pos - 1] = 0;
                    y_pos -= 2;
                    break;
                case 'E':
                    maze[x_pos][y_pos + 2] = 0;
                    maze[x_pos][y_pos + 1] = 0;
                    y_pos += 2;
                    break;
            }
            moves.push_back(y_pos + (x_pos*width));
        }
        else {
            int back = moves[moves.size() - 1];
			moves.pop_back();
            x_pos = back / width;
            y_pos = back % width;
        }
    }
    erlog("Did all the generation");

    // Fill out the actual tile map
    for (int i=0; i<90; ++i) {
        // Put a random floor tile in, 3, 4, or 5
        generated_tilemap[i] = 3 + rand() % 3;
        generated_objectmap[i] = 0;
    }
    generated_tilemap[90] = -1;
    generated_objectmap[90] = -1;


    for (int i=1; i<10; ++i) {
        for (int j=0; j<=9; ++j) {
            if (maze[i][j] == 1)
                generated_tilemap[(i-1)*10+j] = rand() % 3;
        }
    }
    // Punch some holes in the walls, becuase we want some freedom to move around a bit
    int how_many_holes = rand() % 90;
    for (int i=0; i<how_many_holes; ++i) {
        generated_tilemap[rand() % 90] = 3 + rand() % 3;
    }


    // Position the player and the exit
    for (int i=82; i<90; ++i) {
        if (generated_tilemap[i] >= 3) {
            generated_objectmap[i] = 1;
            break;
        }
    }

    for (int i=7; i>=0; --i) {
        if (generated_tilemap[i] >= 3) {
            generated_objectmap[i] = 2;
            break;
        }
    }

    //const float civilian_rarity = 0.01;
    //const float enemy1_rarity = 0.01;
    //const float enemy2_rarity = 0.005;
    //const float enemy3_rarity = 0.001;
    //const float ammo_stash_rarity = 0.005;
    // Add some general items
    for (int i=0; i<90; ++i) {
        if (generated_tilemap[i] < 3 || generated_objectmap[i] != 0)
            continue;
        if ((float)rand()/(float)RAND_MAX < civilian_rarity)
            generated_objectmap[i] = 7;
        else if ((float)rand()/(float)RAND_MAX < enemy1_rarity)
            generated_objectmap[i] = 4;
        else if ((float)rand()/(float)RAND_MAX < enemy2_rarity)
            generated_objectmap[i] = 5;
        else if ((float)rand()/(float)RAND_MAX < enemy3_rarity)
            generated_objectmap[i] = 6;
        else if ((float)rand()/(float)RAND_MAX < ammo_stash_rarity)
            generated_objectmap[i] = 8;
    }



    erlog("Generated tilemap");
}
