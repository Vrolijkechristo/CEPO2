#include <stdio.h>
#include <time.h>

#define M -1
#define startmap   {{-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},  \
                    {-1, -1, -1, -1,  0,  -1,  0, -1,  0, -1, -1, -1, -1},  \
                    {-1, -1,  0,  0,  0,   0,  0,  M,  0,  0,  0, -1, -1},  \
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},  \
                    { 0,  0,  0,  0,  0,   0,  0,  M,  0,  0,  0,  0,  0},  \
                    {-1, -1,  0, -1,  M,  -1,  M, -1,  0, -1,  0, -1, -1},  \
                    { 0,  0,  0,  0,  0,   M,  0,  0,  0,  M,  0,  0,  0},  \
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},  \
                    { 0,  0,  0,  0,  0,   M,  0,  M,  0,  M,  0,  0,  0},  \
                    {-1, -1,  0, -1,  0,  -1,  0, -1,  0, -1,  0, -1, -1},  \
                    {-1, -1,  0,  0,  0,   0,  0,  0,  0,  M,  0, -1, -1},  \
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
int todo[90][2];

int processedcells, posx, posy, endx, endy, stationstart, stationend;
int arrived = 0;

void inputs() {
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
void showmap(int array[13][13]) { //shows the map's data (loveyou chisto for making this more efficient)
    int i, j;
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

void showtodo() { //shows sequential list of cells coords at which to evaluate neighbours
    int i, j;
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

int relative(int olddir, int posy, int posx) { //

    int newdir = dir[posy][posx];
    printf("%d, %d, ", posy, posx);

    if(newdir == 0) {
        arrived = 1;
        return 1000;
    }

        if(newdir == olddir) {
            printf("straight\n");
            return 1011;
        }
        if(newdir == (olddir + 1) || newdir == (olddir - 3)) {
            printf("right\n");
            return 1001;
        }
        if(newdir == (olddir + 3) || newdir == (olddir -1)) {
            printf("left\n");
            return 1010;
        }
        if(newdir == (olddir + 2) || newdir == (olddir -2)) {
            printf("turn180\n");
            return 1100;
        }

}

void step(int steps) { //to be called when the robot reaches a midpoint

    int olddir = dir[posy][posx];

    switch (olddir) {

        case 1:
            posy -= steps;
            relative(olddir, posy, posx);
            //printf("north\n");
            break;
        case 2:
            posx += steps;
            relative(olddir, posy, posx);
            //printf("east\n");
            break;
        case 3:
            posy += steps;
            relative(olddir, posy, posx);
            //printf("south\n");
            break;
        case 4:
            posx -= steps;
            relative(olddir, posy, posx);
            //printf("west\n");
            break;
    }
}

    int main() {

        inputs();

        int i;
        for (i = 0; i < 90; i++) {
            todo[i][0] = -99;
            todo[i][1] = -99;
        }

        calcmaps(endy, endx); // y, x location of target

        //showtodo();
        //showmap(map);
        //showmap(dir);
        printf("\nprocessed cells: %d\n", processedcells);

        //walk(posy , posx); //y , x location of robot

        printf("%d, %d, ", posy, posx);
        printf("straight\n");

        while (arrived == 0) {
            step(2);
        }

        printf("arrived\n");

    }


