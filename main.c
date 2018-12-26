#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXNUM 100
typedef enum
{
    north = 0,
    east = 1,
    south = 2,
    west = 3
} direction;
struct car_info
{
    int id;
    direction dir;
};
struct queue
{
    int num;           //the number of the car in this direction
    direction dir;     //the direction
    int carID[MAXNUM]; //the ID of the car in this direction
    int front;         //the front pointer of the queue
    int rear;          //the rear pointer of the queue
};
pthread_mutex_t mutex_source[4]; //represent the a b c d source，the order is c b a d

pthread_mutex_t mutex_dir[4];

pthread_cond_t first_dir[4];
struct queue dirQueue[4];

int wait_num = 0;

void initMut()
{
    for (int i = 0; i < 4; i++)
    {
        pthread_mutex_init(&mutex_source[i], NULL);
        pthread_mutex_init(&mutex_dir[i], NULL);
    }
}
void initCond()
{
    for (int i = 0; i < 4; i++)
        pthread_cond_init(&first_dir[i], NULL);
}
void initQueue()
{
    for (int i = 0; i < 4; i++)
    {
        dirQueue[i].num = 0;
        dirQueue[i].dir = i;
        dirQueue[i].front = 0;
        dirQueue[i].rear = -1;
    }
}
void *car_thread_handle(void *pParam)
{

    struct car_info *car = (struct car_info *)pParam;

    //dirQueue[car->dir].carID[++dirQueue[car->dir].rear] = car->id; //push_back the car id into the queue

    //dirQueue[car->dir].num++; //let the num++

    char dir_s[10], dir_left_s[10];
    int right_dir;
    int wait_flag = 0;
    //get the dir string which will be used to print the information
    switch (car->dir)
    {
    case 0:
        strcpy(dir_s, "North");
        strcpy(dir_left_s, "East");
        break;
    case 1:
        strcpy(dir_s, "East");
        strcpy(dir_left_s, "South");
        break;
    case 2:
        strcpy(dir_s, "South");
        strcpy(dir_left_s, "West");
        break;
    case 3:
        strcpy(dir_s, "West");
        strcpy(dir_left_s, "North");
        break;
    }

    while (dirQueue[car->dir].carID[dirQueue[car->dir].front] != car->id)
        ;
    //If a car is passing the cross, others should wait in this waitting queue.

    pthread_mutex_lock(&mutex_dir[car->dir]);
    usleep(2000);
    printf("Car %d from %s arrives at crossing\n", car->id, dir_s); //print the arriving info

    if (wait_num == 3) // Now The car want to occupy the source, but if there will be a deadlock, it can not occupy
    {
        printf("DEADLOCK: car jam detected, signaling %s to go\n", dir_left_s);
        usleep(4000);
        pthread_cond_signal(&first_dir[(car->dir + 1) % 4]);
    }

    while (wait_num == 3)
        ; //let the thread block here,until the left car has occupied the source
    wait_num++;
    pthread_mutex_lock(&mutex_source[car->dir]);
    usleep(2000);

    right_dir = (car->dir + 3) % 4;

    if (dirQueue[right_dir].num) //There is at least a car at right
    {

        pthread_cond_wait(&first_dir[car->dir], &mutex_dir[car->dir]); //wait the right to go first   Maybe BUG
    }

    pthread_mutex_lock(&mutex_source[(car->dir - 1) % 4]);
    dirQueue[car->dir].num--;
    usleep(2000);
    wait_num--;
    printf("Car %d from %s leaving crossing\n", car->id, dir_s);

    pthread_mutex_unlock(&mutex_source[car->dir]);

    pthread_cond_signal(&first_dir[(car->dir + 1) % 4]);
    usleep(4000);

    dirQueue[car->dir].front++;
    pthread_mutex_unlock(&mutex_source[(car->dir - 1) % 4]);
    usleep(2000);
    //unlock
    pthread_mutex_unlock(&mutex_dir[car->dir]);
}
int main(int argc, char **argv)
{
    char carList[100];
    strcpy(carList, argv[1]);
    int carNum = strlen(carList);
    pthread_t car_thread[carNum];
    struct car_info *car[carNum];

    //initialize the mutex and cond
    initMut();
    initCond();
    //initialize the counter of each direction
    initQueue();
    //creat the threads
    for (int i = 0; i < carNum; i++)
    {
        car[i] = (struct car_info *)malloc(sizeof(struct car_info));
        //car[i].id = i;
        car[i]->id = i;
        switch (carList[i])
        {
        case 'n':
            //car[i].dir = north;
            car[i]->dir = north;
            break;
        case 'e':
            //car[i].dir = east;
            car[i]->dir = east;
            break;
        case 'w':
            //car[i].dir = west;
            car[i]->dir = west;
            break;
        case 's':
            //car[i].dir = south;
            car[i]->dir = south;
            break;
        default:
            break;
        }
        //pthread_create(&car_thread[i], NULL, car_thread_handle, car);
        dirQueue[car[i]->dir].carID[++dirQueue[car[i]->dir].rear]=i;
        dirQueue[car[i]->dir].num++;
    }
    for(int i=0;i<carNum;i++)
    {
        pthread_create(&car_thread[i], NULL, car_thread_handle, car[i]);
    }
    //wait for the threads to quit
    for (int i = 0; i < carNum; i++)
    {
        pthread_join(car_thread[i], NULL);
    }
}
