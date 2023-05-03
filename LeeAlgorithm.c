#include <stdio.h>
#include <time.h>

#define M -1
#define startmap   {{-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},  \
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},  \
                    {-1, -1,  0,  0,  0,   0,  0,  0,  0,  0,  0, -1, -1},  \
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},  \
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},  \
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},  \
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},  \
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},  \
                    { 0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0},  \
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},  \
                    {-1, -1,  0,  0,  0,   0,  0,  0,  0,  0,  0, -1, -1},  \
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},  \
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1}};

int stationmap[12][2] ={{12, 4},
                        {12, 6},
                        {12, 8},
                        {8, 12},
                        {6, 12},
                        {4, 12},
                        {0, 8},
                        {0, 6},
                        {0, 4},
                        {4, 0},
                        {6, 0},
                        {8, 0}};

int map[13][13] = startmap;
int dir[13][13] = startmap;

int processedcells;
int todo[90][2];

int posx, posy, endx, endy, stationstart, stationend;

void InitAssignmentA() {
    printf("Robot position (station no.):  ");
    scanf("%d", &stationstart);
    printf("target position (station no.): ");
    scanf("%d,", &stationend);

    posy = stationmap[stationstart -1][0];
    posx = stationmap[stationstart -1][1];
    endy = stationmap[stationend   -1][0];
    endx = stationmap[stationend   -1][1];

    printf("posy = %d,  posx = %d \n", posy, posx);
    printf("endy = %d,  endx = %d \n", endy, endx);

}

/*  Function for multiple inputs (assignment B)
*   And redo if hit mine
*/



void showmap(int array[13][13]) { //shows the map's data (loveyou chisto for making this better)
    int i, j = 0;
    printf("\n---------------------------------------");
    for(i=0; i<13; i++) {
        printf("\n");
        for(j=0; j<13; j++) {

            if(array[i][j] == -1 || array[i][j] >9) {
                printf(" %d,", array[i][j]); }
            else {
                printf("  %d,", array[i][j]); }
        }

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

int calcmaps(int endy, int endx) { //returns taxicab distance, fills map and dir

    int line = 0;

    todo[0][0] = endy;
    todo[0][1] = endx;
    map[endy][endx] = 1;

    if(map[endy][endx] == -1) {
        printf("invalid destination");
    }

    while(map[posy][posx] == 0){
        handlecell(line);
        line++;
    }

}

void relative(int olddir, int posy, int posx) {

    int newdir = dir[posy][posx];
    printf("%d, %d, ", posy, posx);

    if(newdir == olddir) {
        printf("straight\n");
    }
    if(newdir == (olddir + 1) || newdir == (olddir - 3)) {
        printf("right\n");
    }
    if(newdir == (olddir + 3) || newdir == (olddir -1)) {
        printf("left\n");
    }

}

void walk(int starty, int startx){

    int posx = startx;
    int posy = starty;
    //int i;
    //routelength = map[starty][startx]; //number on map directly represents distance to target

    while (dir[posy][posx] != 0) {  //for each i in routelength, get direction of current cell and move accordingly

        int olddir = dir[posy][posx];

        switch(dir[posy][posx]) {

            case 1:
                posy-=2;
                relative(olddir, posy, posx);
                //printf("north\n");
                break;
            case 2:
                posx+=2;
                relative(olddir, posy, posx);
                //printf("east\n");
                break;
            case 3:
                posy+=2;
                relative(olddir,posy, posx);
                //printf("south\n");
                break;
            case 4:
                posx-=2;
                relative(olddir, posy, posx);
                //printf("west\n");
                break;
        }

    }

    printf("arrived\n");

}

int main() {

    InitAssignmentA();

    int i;
    for(i = 0; i < 90; i++) {
        todo[i][0] = -99;
        todo[i][1] = -99;
    }

    calcmaps(endy, endx); // x, y location of target

    showmap(map);
    showmap(dir);
    printf("\nprocessed cells: %d\n", processedcells);

    walk(posy , posx); //x , y location of robot

}



