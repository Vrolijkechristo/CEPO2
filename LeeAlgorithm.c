//
// Created by Christo, made by Gyan on 3-5-2023.
//

#include <stdio.h>
#include <time.h>

int map[13][13] =  {{-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},
                    {-1, -1,  0,  0,  0,   0,  0,  0,  0,  0,  0, -1, -1},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    {-1, -1,  0,  0,  0,   0,  0,  0,  0,  0,  0, -1, -1},
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1}};

int dir[13][13] =  {{-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},
                    {-1, -1,  0,  0,  0,   0,  0,  0,  0,  0,  0, -1, -1},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},
                    {-1, -1,  0,  0,  0,   0,  0,  0,  0,  0,  0, -1, -1},
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1}};

int processedcells, routelength = 0;
int todo[90][2] = {};

void showmap() { //shows the currect map with data about taxicab distance
    int i, j = 0;
    for(i=0; i<13; i++) {
        printf(";\n");
        for(j=0; j<13; j++) {

            if(map[i][j] == -1 || map[i][j] >9) {
                printf(" %d,", map[i][j]); }
            else {
                printf("  %d,", map[i][j]); }
        }

    }

}

void showdir() { //shows direction map to get from to start coords
    int i, j = 0;
    for(i=0; i<13; i++) {
        printf(";\n");
        for(j=0; j<13; j++) {

            if(map[i][j] == -1) {
                printf(" %d,", dir[i][j]); }
            else {
                printf("  %d,", dir[i][j]); }
        }

    }

}

void showtodo() { //shows sequential list of cells coords at which to evaluate neighbours
    int i, j = 0;
    for(i=0; i<90; i++) {
        printf("\n");
        for(j=0; j<2; j++) {

            printf(" %d", todo[i][j]); }

    }

}

void handleneighbour(int cellx, int celly, int cellnum, int direction) {

    if(map[celly][cellx] == 0 && -1 < cellx && cellx < 13 && -1 < celly && celly < 13) {

        map[celly][cellx] = cellnum + 1;
        dir[celly][cellx] = direction;

        processedcells++;

        todo[processedcells][0] = celly;
        todo[processedcells][1] = cellx;

    }

}

void handlecell(int line) {

    int y = todo[line][0];
    int x = todo[line][1];

    int cellnum = map[y][x];

    handleneighbour(x, y +1, cellnum, 1);
    handleneighbour(x +1, y, cellnum, 4);
    handleneighbour(x, y -1, cellnum, 3);
    handleneighbour(x -1, y, cellnum, 2);

}

int calcmaps(int endx, int endy) { //returns taxicab distance, fills map and dir

    int line = 0;

    todo[0][0] = endy;
    todo[0][1] = endx;
    map[endy][endx] = 1;

    if(map[endy][endx] == -1) {
        printf("invalid points");
    }

    while(todo[line][0] != -99) {
        handlecell(line);
        line++;
    }

}

void walk(int startx, int starty){

    int posx = startx;
    int posy = starty;
    int i;
    routelength = map[starty][startx]; //number on map directly represents distance to target

    for(i = routelength; 0 < i; i--) {  //for each i in routelength, get direction of current cell and move accordingly
        printf("%d, %d, ", posy, posx);
        switch(dir[posy][posx]) {

            case 1:
                posy--;
                printf("north\n");
                break;
            case 2:
                posx++;
                printf("east\n");
                break;
            case 3:
                posy++;
                printf("south\n");
                break;
            case 4:
                posx--;
                printf("west\n");
                break;
            case 0: //when the zero is reached, destination is reached
                printf("arrived\n");

        }

    }

}

int main() {

    clock_t start_t, end_t;
    double total_t;
    start_t = clock();

    int i;

    for(i = 0; i < 90; i++) {
        todo[i][0] = -99;
        todo[i][1] = -99;
    }

    calcmaps(8, 12); // x, y location of target

    walk(12 , 4);
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    showtodo();
    printf("\n---------------------------------------");
    showmap();
    printf("\n---------------------------------------");
    showdir();

    printf("\nprocessed cells: %d\n", processedcells);

    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken: %.1f us\n\n", total_t*1000000  );

    walk(12 , 4); //x , y location of robot
}
