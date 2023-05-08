#include <stdio.h>
#include <time.h>

//#define M -1
#define M 0
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



int calcmaps(int endy, int endx ) { //returns taxicab distance, fills map and dir

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
    printf("%d, %d, %d, ", posy, posx, map[posy][posx]);

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

// Willen we dat de volgorde van de punten elke keer als er een mijn gevonden word opnieuw word berekend of niet:
int CalculateLengthRoute(int Yend, int Xend, int Ybegin, int Xbegin) {
    // Clean all variables
    int line = 0;
    int i, j, g, h;
    processedcells = 0;
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 13; j++) {
            if (map[i][j] != -1) { // keep all walls (thus already found mines) untouched
                map[i][j] = 0;
                dir[i][j] = 0;
            }
        }
    }
    for (g = 0; g < 90; g++){
        for (h = 0; h < 2; h++){
            todo[g][h] = -99;
        }
    }

    todo[0][0] = Yend;
    todo[0][1] = Xend;
    map[Yend][Xend] = 1;

    while(map[Ybegin][Xbegin] == 0){
        int y = todo[line][0];
        int x = todo[line][1];

        int cellnum = map[y][x];

        handleneighbour(x, y +1, cellnum, 1);
        handleneighbour(x +1, y, cellnum, 4);
        handleneighbour(x, y -1, cellnum, 3);
        handleneighbour(x -1, y, cellnum, 2);
        line++;
    }


    int Length = map[Ybegin][Xbegin];
    return Length;
}

int AssignmentA(int StationStart, int Station1, int Station2, int Station3){
    int Y1 = stationmap[Station1 -1][0];
    int X1 = stationmap[Station1 -1][1];
    int Y2 = stationmap[Station2 -1][0];
    int X2 = stationmap[Station2 -1][1];
    int Y3 = stationmap[Station3 -1][0];
    int X3 = stationmap[Station3 -1][1];
    int Ystart = stationmap[StationStart - 1][0];
    int Xstart = stationmap[StationStart - 1][1];

    int order[3];

    posy = Ystart;
    posx = Xstart;

    int lenS1 = CalculateLengthRoute(Y1, X1, Ystart, Xstart);
    int lenS2 = CalculateLengthRoute(Y2, X2, Ystart, Xstart);
    int lenS3 = CalculateLengthRoute(Y3, X3, Ystart, Xstart);

    int len12 = CalculateLengthRoute(Y1, X1, Y2, X2);
    int len23 = CalculateLengthRoute(Y2, X2, Y3, X3);
    int len13 = CalculateLengthRoute(Y1, X1, Y3, X3);

    int Stat1 = 0;
    int Stat2 = 0;
    int Stat3 = 0;

    if ((lenS1 <= lenS2) && (lenS1 <= lenS3)){
        printf("The route from %d to %d is the shortest\n", StationStart, Station1);
        order[0] = Station1;
        lenS1 = CalculateLengthRoute(Y1, X1, Ystart, Xstart);
        if (len12 <= len13){
            printf("The route from %d to %d is the shortest\n", Station1, Station2);
            order[1] = Station2;
            order[2] = Station3;
            //lenS1 = CalculateLengthRoute(Ystart, Xstart, Y1, X1);
        }
        else{
            printf("The route from %d to %d is the shortest\n", Station1, Station3);
            //lenS3 = CalculateLengthRoute(Ystart, Xstart, Y3, X3);
            order[1] = Station3;
            order[2] = Station2;
        }
    }
    else if((lenS2 <= lenS1) && (lenS2 <= lenS3)){
        printf("The route from %d to %d is the shortest\n", StationStart, Station2);
        lenS2 = CalculateLengthRoute(Y2, X2, Ystart, Xstart);
        order[1] = Station2;
        if (len23 <= len12){
            printf("The route from %d to %d is the shortest\n", Station3, Station2);
            order[1] = Station3;
            order[2] = Station1;
            //lenS1 = CalculateLengthRoute(Ystart, Xstart, Y1, X1);
        }
        else{
            printf("The route from %d to %d is the shortest\n", Station2, Station1);
            //lenS3 = CalculateLengthRoute(Ystart, Xstart, Y3, X3);
            order[1] = Station1;
            order[2] = Station3;
        }
    }
    else{
        printf("The route from %d to %d is the shortest\n", StationStart, Station3);
        lenS3 = CalculateLengthRoute(Y3, X3, Ystart, Xstart);
        order[2] = Station3;
        if (len23 <= len13){
            printf("The route from %d to %d is the shortest\n", Station1, Station2);
            order[1] = Station2;
            order[2] = Station1;
            //lenS1 = CalculateLengthRoute(Ystart, Xstart, Y1, X1);
        }
        else{
            printf("The route from %d to %d is the shortest\n", Station1, Station3);
            //lenS3 = CalculateLengthRoute(Ystart, Xstart, Y3, X3);
            order[1] = Station1;
            order[2] = Station2;
        }
    }
    for(int p = 0; p < 3; p++){
        printf("%d\n", order[p]);
    }

    showmap(map);
    printf("\n");
    while ((Stat1 != 1) || (Stat2 != 1) || (Stat3 != 1)){
        int next;
        if (posy == Y1 && posx == X1){
            Stat1 = 1;
            if (order[0] == Station1){
                next = CalculateLengthRoute(stationmap[order[1] - 1][0], stationmap[order[1] - 1][1],
                                            stationmap[order[0] - 1][0], stationmap[order[0] - 1][1]);
            }
            else if (order[1] == Station1){
                next = CalculateLengthRoute(stationmap[order[2] - 1][0], stationmap[order[2] - 1][1],
                                            stationmap[order[1] - 1][0], stationmap[order[1] - 1][1]);
            }
            else{
                return 99;
            }
        }
        if (posy == Y2 && posx == X2){
            Stat2 = 1;
            if (order[0] == Station2){
                next = CalculateLengthRoute(stationmap[order[1] - 1][0], stationmap[order[1] - 1][1],
                                            stationmap[order[0] - 1][0], stationmap[order[0] - 1][1]);
            }
            else if (order[1] == Station2){
                next = CalculateLengthRoute(stationmap[order[2] - 1][0], stationmap[order[2] - 1][1],
                                            stationmap[order[1] - 1][0], stationmap[order[1] - 1][1]);
            }
            else {
                return 99;
            }
        }
        if (posy == Y3 && posx == X3){
            Stat3 = 1;
            if (order[0] == Station3){
                next = CalculateLengthRoute(stationmap[order[1] - 1][0], stationmap[order[1] - 1][1],
                                            stationmap[order[0] - 1][0], stationmap[order[0] - 1][1]);
            }
            else if (order[1] == Station3){
                next = CalculateLengthRoute(stationmap[order[2] - 1][0], stationmap[order[2] - 1][1],
                                            stationmap[order[1] - 1][0], stationmap[order[1] - 1][1]);
            }
            else{
                return 99;
            }
        }
        step(2);

    }

}

    int main() {
/*
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

        printf("%d, %d, %d, ", posy, posx, map[posy][posx]);
        printf("straight\n");

        while (arrived == 0) {
            step(2);
        }

        printf("arrived\n");

        //printf("This is the length from station 1 to station 12: %d\n", CalculateLengthRoute(12, 4, 8, 0));
        //printf("This is the length from station 9 to station 4 : %d\n", CalculateLengthRoute(0, 4, 12, 8));
*/
        int Stat1, Stat2, Stat3;
        printf("Goal 1 position (station no.):  ");
        scanf("%d", &Stat1);
        printf("Goal 2 position (station no.): ");
        scanf("%d,", &Stat2);
        printf("Goal 3 position (station no.):  ");
        scanf("%d", &Stat3);
        AssignmentA(1, Stat1, Stat2, Stat3);
    }
