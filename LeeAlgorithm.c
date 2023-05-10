#include <stdio.h>

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
int orderA[3];
int olddir;

int processedcells, posx, posy;
int arrived = 0;

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
    printf("\n");
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

int relative() { //
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

void step(int steps, int minefound, int currentdir) { //to be called when the robot reaches a midpoint
    if(minefound == 1) {
        olddir = currentdir;
    }
    else {
        olddir = dir[posy][posx];
    }

    switch (olddir) {
        case 1:
            posy -= steps;
            relative();
            break;
        case 2:
            posx += steps;
            relative();
            break;
        case 3:
            posy += steps;
            relative();
            break;
        case 4:
            posx -= steps;
            relative();
            break;
        default:
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

int CheckPointCheck(int Y, int X, int Station, int Stat){
    if (posy == Y && posx == X){
        Stat = 1;
        if (orderA[0] == Station){
            CalculateLengthRoute(stationmap[orderA[1] - 1][0], stationmap[orderA[1] - 1][1],
                                 stationmap[orderA[0] - 1][0], stationmap[orderA[0] - 1][1]);
            showmap(map);
        }
        else if (orderA[1] == Station){
            CalculateLengthRoute(stationmap[orderA[2] - 1][0], stationmap[orderA[2] - 1][1],
                                 stationmap[orderA[1] - 1][0], stationmap[orderA[1] - 1][1]);
            showmap(map);
        }
        else{
            arrived = 1;
        }
    }
    return Stat;
}

void CheckLengthRouteVersus(int len1, int len2, int len3, int len4, int len5, int Sta1, int Sta2, int Sta3){
    if ((len1 <= len2) && (len1 <= len3)){
        orderA[0] = Sta1;
        if (len4 <= len5){
            orderA[1] = Sta2;
            orderA[2] = Sta3;
        }
        else{
            orderA[1] = Sta3;
            orderA[2] = Sta2;
        }
    }
}


int AssignmentA(int StationStart) {
    int Station1, Station2, Station3;
    printf("Goal 1 position (station no.):  ");
    scanf("%d", &Station1);
    printf("Goal 2 position (station no.): ");
    scanf("%d", &Station2);
    printf("Goal 3 position (station no.):  ");
    scanf("%d", &Station3);
    int Y1 = stationmap[Station1 - 1][0];
    int X1 = stationmap[Station1 - 1][1];
    int Y2 = stationmap[Station2 - 1][0];
    int X2 = stationmap[Station2 - 1][1];
    int Y3 = stationmap[Station3 - 1][0];
    int X3 = stationmap[Station3 - 1][1];
    int Ystart = stationmap[StationStart - 1][0];
    int Xstart = stationmap[StationStart - 1][1];
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
    CheckLengthRouteVersus(lenS1, lenS2, lenS3, len12, len13, Station1, Station2, Station3);
    CheckLengthRouteVersus(lenS2, lenS1, lenS3, len23, len12, Station2, Station3, Station1);
    CheckLengthRouteVersus(lenS3, lenS2, lenS1, len13, len23, Station3, Station1, Station2);

    for (int p = 0; p < 3; p++) {
        printf("%d\n", orderA[p]);
    }

    CalculateLengthRoute(stationmap[orderA[0] - 1][0], stationmap[orderA[0] - 1][1],
                         stationmap[StationStart - 1][0], stationmap[StationStart - 1][1]);
    showmap(map);
    printf("\n");

    while ((Stat1 != 1) || (Stat2 != 1) || (Stat3 != 1)) {
        Stat1 = CheckPointCheck(Y1, X1, Station1, Stat1);
        Stat2 = CheckPointCheck(Y2, X2, Station2, Stat2);
        Stat3 = CheckPointCheck(Y3, X3, Station3, Stat3);
        step(2, 0, 0);
    }

    printf("Arrived");
}

    int main() {
        AssignmentA(1);
    }
