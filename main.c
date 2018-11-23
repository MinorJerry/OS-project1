#include <stdio.h>
#include <pthread.h>

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
    int num;//the number of the car in this direction
    direction dir; //the direction
    int carID[MAXNUM];//the ID of the car in this direction
    int front;//the front pointer of the queue
    int rear;//the rear pointer of the queue
};
pthread_mutex_t mutex_source[4];//represent the a b c d source，the order is c b a d

pthread_mutex_t mutex_dir[4];
pthread_mutex_t mutex_waitQueue[4];
pthread_cond_t cond_queue[4];

struct queue dirQueue[4];
int first_queue[4];//the first car's ID in each direction(queue)

void *car_thread_handle(void *pParam)
{
    struct car_info *car = (struct car_info *)pParam;
    dirQueue[car->dir].carID[++dirQueue[car->dir].rear] = car->id;//push_back the car id into the queue
    dirQueue[car->dir].num++;//let the num++
    char dir_s[10];
    //get the dir string which will be used to print the information
    switch(car->dir)
    {
        case 0:
            strcpy(dir_s,"North");break;
        case 1:
            strcpy(dir_s,"East");break;
        case 2:
            strcpy(dir_s,"South");break;
        case 3:
            strcpy(dir_s,"West");break;
    }
    //if the car is not the first car, it should wait
    while(first_queue[car->dir]!=car->id);
    //If a car is passing the cross, others should wait in this wait queue.
    pthread_mutex_lock(&mutex_dir[car->dir]);
    
    printf("Car %d from %s arrives at crossing",car->id,dir_s);//print the arriving info
    

    //wait for the signal to occupy one source
    pthread_cond_wait(&cond_queue[car->dir],&mutex_dir[car->dir]);

    //unlock
    pthread_mutex_unlock(&mutex_dir[car->dir]);

}
int main(int argc, char **argv)
{
    char carList[100] = argv[1];
    int carNum = strlen(carList);
    pthread_t car_thread[carNum];
    struct car_info car[carNum];
    //iniitial the counter of each direction 
    for (int i = 0; i < 4; i++)
    {
        dirQueue[i].num = 0;
        dirQueue[i].dir = i;
        dirQueue[i].front = 0;
        dirQueue[i].rear = -1;
    }
    //creat the threads
    for (int i = 0; i < carNum; i++)
    {
        car[i].id = i;
        switch (carList[i])
        {
        case 'n':
            car[i].dir = north;
            break;
        case 'e':
            car[i].dir = east;
            break;
        case 'w':
            car[i].dir = west;
            break;
        case 's':
            car[i].dir = south;
            break;
        default:
            break;
        }
        pthread_create(&car_thread[i], NULL, car_thread_handle, (void *)&car[i]);
    }
    //wait for the threads to quit
    for (int i = 0; i < carNum; i++)
    {
        pthread_join(car_thread[i], NULL);
    }
}
