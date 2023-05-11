#include <stdio.h>
#include <Windows.h>
#include <string.h>

#define COMPORT "COM4"
#define BAUDRATE CBR_9600

char byteBuffer[BUFSIZ+1];

HANDLE hSerial;

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
int todo[90][2];
int orderA[3];
int olddir;

int Y1, Y2, Y3, X1, X2, X3;
int Stat1 = 0;
int Stat2 = 0;
int Stat3 = 0;
int Station1, Station2, Station3;
int StationStart = 1;


int processedcells, posx, posy;
int arrived = 0;

void initSio(HANDLE hSerial){

    COMMTIMEOUTS timeouts ={0};
    DCB dcbSerialParams = {0};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        //error getting state
        printf("error getting state \n");
    }

    dcbSerialParams.BaudRate = BAUDRATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if(!SetCommState(hSerial, &dcbSerialParams)){
        //error setting serial port state
        printf("error setting state \n");
    }

    timeouts.ReadIntervalTimeout = 1000;
    timeouts.ReadTotalTimeoutConstant = 1000;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if(!SetCommTimeouts(hSerial, &timeouts)){
        //error occureed. Inform user
        printf("error setting timeout state \n");
    }
}

int readByte(HANDLE hSerial, char *buffRead) {

    DWORD dwBytesRead = 0;

    if (!ReadFile(hSerial, buffRead, 1, &dwBytesRead, NULL))
    {
        printf("error reading byte from input buffer \n");
    }

    return(0);
}

int writeByte(HANDLE hSerial, char *buffWrite){

    DWORD dwBytesWritten = 0;

    if (!WriteFile(hSerial, buffWrite, 1, &dwBytesWritten, NULL))
    {
        printf("error writing byte to output buffer \n");
    }
    printf("Byte written to write buffer is: %x \n", buffWrite[0]);

    return(0);
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

int relative(int olddir, int posy, int posx) { //

    int newdir = dir[posy][posx];
    printf("%d, %d, %d, ", posy, posx, map[posy][posx]);

    if(newdir == 0) {
        arrived = 1;
        printf("arrived\n");
        byteBuffer[0] = 0x00;
        writeByte(hSerial, byteBuffer);
        return 1000;
    }

    if(newdir == olddir) {
        printf("straight\n");
        byteBuffer[0] = 0x60;
        writeByte(hSerial, byteBuffer);
        return 1011;
    }
    if(newdir == (olddir + 1) || newdir == (olddir - 3)) {
        printf("right\n");
        byteBuffer[0] = 0x20;
        writeByte(hSerial, byteBuffer);
        return 1001;
    }
    if(newdir == (olddir + 3) || newdir == (olddir -1)) {
        printf("left\n");
        byteBuffer[0] = 0x40;
        writeByte(hSerial, byteBuffer);
        return 1010;
    }
    if(newdir == (olddir + 2) || newdir == (olddir -2)) {
        printf("turn180\n");
        byteBuffer[0] = 0x80;
        writeByte(hSerial, byteBuffer);
        return 1100;
    }

}

int step(int steps, int minefound, int currentdir) { //to be called when the robot reaches a midpoint

    if(minefound == 1) {
        olddir = currentdir;
    }
    else {
        olddir = dir[posy][posx];
    }

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
    for (i = 0; i < 90; i++){
        for (j = 0; j < 2; j++){
            todo[i][j] = -99;
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

void makemap() {

    Y1 = stationmap[Station1 - 1][0];
    X1 = stationmap[Station1 - 1][1];
    Y2 = stationmap[Station2 - 1][0];
    X2 = stationmap[Station2 - 1][1];
    Y3 = stationmap[Station3 - 1][0];
    X3 = stationmap[Station3 - 1][1];

    posy = stationmap[StationStart - 1][0];
    posx = stationmap[StationStart - 1][1];

    int lenS1 = CalculateLengthRoute(Y1, X1, posy, posx);
    int lenS2 = CalculateLengthRoute(Y2, X2, posy, posx);
    int lenS3 = CalculateLengthRoute(Y3, X3, posy, posx);

    int len12 = CalculateLengthRoute(Y1, X1, Y2, X2);
    int len23 = CalculateLengthRoute(Y2, X2, Y3, X3);
    int len13 = CalculateLengthRoute(Y1, X1, Y3, X3);

    CheckLengthRouteVersus(lenS1, lenS2, lenS3, len12, len13, Station1, Station2, Station3);
    CheckLengthRouteVersus(lenS2, lenS1, lenS3, len23, len12, Station2, Station3, Station1);
    CheckLengthRouteVersus(lenS3, lenS2, lenS1, len13, len23, Station3, Station1, Station2);

    for (int p = 0; p < 3; p++) {
        printf("Station %d: %d\n", p+1, orderA[p]);
    }

    CalculateLengthRoute(stationmap[orderA[0] - 1][0], stationmap[orderA[0] - 1][1],
                         stationmap[StationStart - 1][0], stationmap[StationStart - 1][1]);
    showmap(map);
    printf("\n");

}

void mine() { //to be called when robot reaches a mine, stores mine pos. in maps, recalcs route, creates next direction command

    int currentdir = dir[posy][posx];

    switch(currentdir) {

        case 1:
            map[posy -1][posx] = -1;
            dir[posy -1][posx] = -1;
            printf("\nMine found at %d, %d while driving NORTH: \n", posy -1, posx);
            break;
        case 2:
            map[posy][posx +1] = -1;
            dir[posy][posx +1] = -1;
            printf("\nMine found at %d, %d while driving EAST: \n", posy, posx +1);
            break;
        case 3:
            map[posy +1][posx] = -1;
            dir[posy +1][posx] = -1;
            printf("\nMine found at %d, %d while driving SOUTH: \n", posy +1, posx);
            break;
        case 4:
            map[posy][posx -1] = -1;
            dir[posy][posx -1] = -1;
            printf("\nMine found at %d, %d while driving WEST: \n", posy, posx -1);
            break;

    }

    makemap();
    //showtodo();
    //showmap(map);
    //showmap(dir);
    printf("processed cells: %d\n", processedcells);

    step(0, 1, currentdir);

}

int AssignmentAB() {

    printf("Goal 1 position (station no.):  ");
    scanf("%d", &Station1);
    printf("Goal 2 position (station no.): ");
    scanf("%d", &Station2);
    printf("Goal 3 position (station no.):  ");
    scanf("%d", &Station3);

    makemap();

    while ((Stat1 != 1) || (Stat2 != 1) || (Stat3 != 1)) {

        Stat1 = CheckPointCheck(Y1, X1, Station1, Stat1);
        Stat2 = CheckPointCheck(Y2, X2, Station2, Stat2);
        Stat3 = CheckPointCheck(Y3, X3, Station3, Stat3);

        readByte(hSerial, byteBuffer);

        if(byteBuffer[0] == 0x20) {
            printf("Mine!\n");
            byteBuffer[0] = 0x10;
            writeByte(hSerial, byteBuffer); //ack
            mine();
            byteBuffer[0] = 0x00;
            printf("\n");
        }
        if(byteBuffer[0] == 0x40) {
            printf("Midpoint!\n");
            byteBuffer[0] = 0x10;
            writeByte(hSerial, byteBuffer); //ack
            step(2, 0, 0);
            byteBuffer[0] = 0x00;
            printf("\n");
        }

    }

    printf("Arrived\n");
    byteBuffer[0] = 0x00;
    writeByte(hSerial, byteBuffer); //stop

}

int main() {

    hSerial = CreateFile(COMPORT,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0
    );

    if(hSerial == INVALID_HANDLE_VALUE){
        if(GetLastError()== ERROR_FILE_NOT_FOUND){
            //serial port does not exist. Inform user.
            printf(" serial port does not exist \n");
        }
        //some other error occurred. Inform user.
        printf(" some other error occured. Inform user.\n");
    }

    initSio(hSerial);

    AssignmentAB();

}