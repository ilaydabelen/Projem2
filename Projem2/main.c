#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define X_LENGTH 5
#define Y_LENGTH 5

typedef struct {
    int x;
    int y;
} Location;

typedef struct {
    int points;
    Location location;
} Hunter;

typedef struct {
    int type;
    Location location;
} Animal;

typedef struct {
    int type;
} Grid;

Grid initGrid(int xlength, int ylength) {
    Grid grid[xlength][ylength];
    for (int i = 0; i < xlength; i++) {
        for (int j = 0; j < ylength; j++) {
            grid[i][j].type = rand() % 4;
        }
    }
    return **grid;
}

void printGrid(Grid grid) {
    printf("| Ayý: %d, Kuþ: %d, Panda: %d, Avcý: %d |\n",
           grid.type, grid.type, grid.type, grid.type);
}

void moveAnimal(Animal *animal) {
    int direction = rand() % 4;
    switch (direction) {
        case 0:
            animal->location.x = (animal->location.x + 1) % X_LENGTH;
            break;
        case 1:
            animal->location.x = (animal->location.x - 1 + X_LENGTH) % X_LENGTH;
            break;
        case 2:
            animal->location.y = (animal->location.y + 1) % Y_LENGTH;
            break;
        case 3:
            animal->location.y = (animal->location.y - 1 + Y_LENGTH) % Y_LENGTH;
            break;
    }
}

void moveHunter(Hunter *hunter) {
    int direction = rand() % 4;
    switch (direction) {
        case 0:
            hunter->location.x = (hunter->location.x + 1) % X_LENGTH;
            break;
        case 1:
            hunter->location.x = (hunter->location.x - 1 + X_LENGTH) % X_LENGTH;
            break;
        case 2:
            hunter->location.y = (hunter->location.y + 1) % Y_LENGTH;
            break;
        case 3:
            hunter->location.y = (hunter->location.y - 1 + Y_LENGTH) % Y_LENGTH;
            break;
    }
}

void *simulateAnimal(void *args) {
    Animal *animal = (Animal *) args;
    while (1) {
        moveAnimal(animal);
        usleep(1000);
    }
    return NULL;
}

void *simulateHunter(void *args) {
    Hunter *hunter = (Hunter *) args;
    while (1) {
        moveHunter(hunter);
        usleep(1000);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int numHunters = atoi(argv[1]);

    Grid grid = initGrid(X_LENGTH, Y_LENGTH);
    printGrid(grid);

    Animal animals[3];
    pthread_t animalThreads[3];

    for (int i = 0; i < 3; i++) {
        animals[i].type = i;
        animals[i].location.x = rand() % X_LENGTH;
        animals[i].location.y = rand() % Y_LENGTH;

        pthread_create(&animalThreads[i], NULL, simulateAnimal, (void *) &animals[i]);
    }

    Hunter hunters[numHunters];
    pthread_t hunterThreads[numHunters];

    for (int i = 0; i < numHunters; i++) {
        hunters[i].points = 0;
        hunters[i].location.x = rand() % X_LENGTH;
        hunters[i].location.y = rand() % Y_LENGTH;

        pthread_create(&hunterThreads[i], NULL, simulateHunter, (void *) &hunters[i]);
    }

    sleep(1); // Simülasyonu 1 saniye çalýþtýr

    for (int i = 0; i < 3; i++) {
        pthread_cancel(animalThreads[i]);
    }

    for (int i = 0; i < numHunters; i++) {
        pthread_cancel(hunterThreads[i]);
    }

    return 0;
}
