#include <pthread.h>  
#include <stdio.h>  
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define MAX 100


//mux of 4 quadrants
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;
pthread_mutex_t mutex_c;
pthread_mutex_t mutex_d;

//mutex of deadlock thread
pthread_mutex_t mutex_deadlock;
//cond to wake up the deadlock thread
pthread_cond_t cond_wake_deadlock;
//cond to wake up the car on the right
pthread_cond_t cond_right_lock;

//mutex of car waiting in queue
pthread_mutex_t mutex_wait_east;
pthread_mutex_t mutex_wait_west;
pthread_mutex_t mutex_wait_north;
pthread_mutex_t mutex_wait_south;

//mutex of car waiting in cross
pthread_mutex_t mutex_block_east;
pthread_mutex_t mutex_block_west;
pthread_mutex_t mutex_block_north;
pthread_mutex_t mutex_block_south;

//cond of each direction in queue
pthread_cond_t cond_west;
pthread_cond_t cond_east;
pthread_cond_t cond_north;
pthread_cond_t cond_south;

//cond of each direction in ready
pthread_cond_t firstEast;
pthread_cond_t firstWest;
pthread_cond_t firstSouth;
pthread_cond_t firstNorth;

typedef enum { west, north, south, east }dir_t;
dir_t dir;//indicate the direction of the new car that comes to the cross
int tot_car = 0;//count the number of car
sem_t empty;//the number of empty areas


//save all the thread
pthread_t car[MAX];

//4 queues to describe the car in each direction
struct queueCar
{
	pthread_t thread[MAX];
	int id[MAX];//the id of the car
	int front;//front pointer
	int rear;//rear pointer
	//indicate the current id of car in each direction
	int current;
	int count;
	//initialize
	queueCar() {
		front = rear = count = 0;
	}
	//push a car at the back with id n
	void push(int n) {
		count++;
		rear = (rear + 1) % MAX;
		id[rear] = n;
	}
	//pop a car in the front
	void pop() {
		count--;
		front = (front + 1) % MAX;
		current= id[front];
	}
};

//�ĸ���������
queueCar queueSouth;
queueCar queueEast;
queueCar queueNorth;
queueCar queueWest;

//if each direction has a car leaving the ready queue /or coming to the first cross
bool is_west;
bool is_south;
bool is_east;
bool is_north;
//�ж���������
bool is_deadlock = false;

//wake up all car in the front of the ready queue
void wakeupall()
{
	is_deadlock = false;
	if (queueSouth.count > 0) {
		 queueSouth.pop();
	
		pthread_cond_signal(&firstSouth);
	}
	if (queueNorth.count > 0) {
		queueNorth.pop();
		pthread_cond_signal(&firstNorth);
	}
	if (queueWest.count > 0) {
		 queueWest.pop();
		pthread_cond_signal(&firstWest);
	}
	if (queueEast.count > 0) {
		queueEast.pop();
		pthread_cond_signal(&firstEast);
	}
}
//define the thread for car from south
void* car_from_south(void* arg) {
	bool deadlock_flag = false;
	//add a wait lock
	pthread_mutex_lock(&mutex_wait_south);
	pthread_cond_wait(&firstSouth, &mutex_wait_south);
	pthread_mutex_unlock(&mutex_wait_south);

	//has come to the cross
	is_south = true;//set is_come to be true
	pthread_mutex_lock(&mutex_a);//apply for the mutex of a area
	printf("car %d from South arrives at crossing\n", queueSouth.current);
	//decrease the number of empty area
	sem_wait(&empty);
	//indicate current direction
	dir = south;

	bool flag = false;
	//if the resource is used up
	int sem;
	sem_getvalue(&empty, &sem);
	if (sem == 0) {
		//indicate that a deadlock happened
		pthread_cond_signal(&cond_wake_deadlock);
		//wait until the deadlock is solved
		pthread_cond_wait(&cond_right_lock, &mutex_a);

		usleep(2000);
		pthread_mutex_lock(&mutex_b);
		pthread_mutex_unlock(&mutex_a);
		printf("car %d from South leaves at crossing\n", queueSouth.current);
		is_south = false;
		//resource add 1.
		sem_post(&empty);
		usleep(2000);
		pthread_mutex_unlock(&mutex_b);
		wakeupall();
		return NULL;
	}
	else if (is_east) {
		//if it find a car on its right
		flag = true;
		//wait for rightcar to go first
		pthread_cond_wait(&cond_south, &mutex_block_south);
        //the car is gone, call a new car
        //wake up the left car
		if (is_deadlock) {
			usleep(2000);
			pthread_mutex_lock(&mutex_b);
			pthread_mutex_unlock(&mutex_a);
			printf("car %d from South leaves at crossing\n", queueSouth.current);
			if (dir == west)pthread_cond_signal(&cond_right_lock);//the car on its left is in deadlock
			else pthread_cond_signal(&cond_west);//wake up the left one
			is_south = false;
			sem_post(&empty);
			usleep(2000);
			pthread_mutex_unlock(&mutex_b);
			return NULL;
		}
	}
	usleep(2000);

	//come to the second cross
	pthread_mutex_lock(&mutex_b);

	//if flag, remember to call
	if (queueEast.count > 0 && flag) {
		 queueEast.pop();
		pthread_cond_signal(&firstEast);
	}

	//release the lock
	pthread_mutex_unlock(&mutex_a);
	is_south = false;
	//resource add 1.
	sem_post(&empty);
	printf("car %d from South leaves at crossing\n", queueSouth.current);

	//if a car is waiting on the left, let it go
	//and the waiting car in the queue is waked up by it
	if (is_west)pthread_cond_signal(&cond_west);
	//or it's waked up by itself
	else if (!is_south && queueSouth.count > 0) {
		 queueSouth.pop();
		pthread_cond_signal(&firstSouth);
	}
	usleep(2000);

	//unlock
	pthread_mutex_unlock(&mutex_b);
}
void* car_from_east(void* arg) {
	pthread_mutex_lock(&mutex_wait_east);
	pthread_cond_wait(&firstEast, &mutex_wait_east);
	pthread_mutex_unlock(&mutex_wait_east);
	is_east = true;
	pthread_mutex_lock(&mutex_b);
	printf("car %d from East arrives at crossing\n", queueEast.current);
	sem_wait(&empty);
	dir = east;
	bool flag = false;
	int sem;
	sem_getvalue(&empty, &sem);
	if (sem == 0) {
		pthread_cond_signal(&cond_wake_deadlock);
		pthread_cond_wait(&cond_right_lock, &mutex_b);

		usleep(2000);
		pthread_mutex_lock(&mutex_c);
		pthread_mutex_unlock(&mutex_b);
		printf("car %d from East leaves at crossing\n", queueEast.current);
		is_east = false;
		sem_post(&empty);
		usleep(2000);
		pthread_mutex_unlock(&mutex_c);
		wakeupall();
		return NULL;
	}
	else if (is_north) {
		flag = true;
		pthread_cond_wait(&cond_east, &mutex_block_east);
		if (is_deadlock) {
			usleep(2000);
			pthread_mutex_lock(&mutex_c);
			pthread_mutex_unlock(&mutex_b);
			printf("car %d from East leaves at crossing\n", queueEast.current);
			if (dir == south)pthread_cond_signal(&cond_right_lock);
			else pthread_cond_signal(&cond_south);
			is_east = false;
			sem_post(&empty);
			usleep(2000);
			pthread_mutex_unlock(&mutex_c);
			return NULL;
		}
	}

	usleep(2000);
	pthread_mutex_lock(&mutex_c);
	if (!is_north && queueNorth.count > 0 && flag) {
		 queueNorth.pop();
		pthread_cond_signal(&firstNorth);
	}
	sem_post(&empty);
	pthread_mutex_unlock(&mutex_b);
	is_south = false;

	printf("car %d from East leaves at crossing\n", queueEast.current);

	if (is_east)pthread_cond_signal(&cond_south);
	else if (!is_east && queueEast.count > 0) {
		queueEast.pop();
		pthread_cond_signal(&firstEast);
	}
	usleep(2000);
	pthread_mutex_unlock(&mutex_c);
}
void* car_from_north(void* arg) {
	pthread_mutex_lock(&mutex_wait_north);
	pthread_cond_wait(&firstNorth, &mutex_wait_north);
	pthread_mutex_unlock(&mutex_wait_north);
	is_north = true;
	pthread_mutex_lock(&mutex_c);
	printf("car %d from North arrives at crossing\n", queueNorth.current);
	sem_wait(&empty);
	dir = north;
	int sem;
	sem_getvalue(&empty,&sem);
	bool flag = false;
	if (sem==0) {
		pthread_cond_signal(&cond_wake_deadlock);
		pthread_cond_wait(&cond_right_lock, &mutex_c);
		pthread_cond_signal(&cond_east);
		usleep(2000);
		pthread_mutex_lock(&mutex_d);
		pthread_mutex_unlock(&mutex_c);
		printf("car %d from West leaves at crossing\n", queueNorth.current);
		is_north = false;
		sem_post(&empty);
		usleep(2000);
		pthread_mutex_unlock(&mutex_d);

		wakeupall();
		return NULL;
	}
	else if (is_west) {
		flag = true;
		pthread_cond_wait(&cond_north, &mutex_block_north);
		if (is_deadlock) {
			usleep(2000);
			pthread_mutex_lock(&mutex_d);
			pthread_mutex_unlock(&mutex_c);
			printf("car %d from North leaves at crossing\n", queueNorth.current);
			if (dir == east)pthread_cond_signal(&cond_right_lock);
			else pthread_cond_signal(&cond_east);
			is_north = false;
			sem_post(&empty);
			usleep(2000);
			pthread_mutex_unlock(&mutex_d);
			return NULL;
		}
	}
	usleep(2000);
	pthread_mutex_lock(&mutex_d);
	if (queueWest.count > 0 && flag) {
		 queueWest.pop();
		pthread_cond_signal(&firstWest);
	}
	sem_post(&empty);
	pthread_mutex_unlock(&mutex_c);
	is_north = false;
	printf("car %d from North leaves at crossing\n", queueNorth.current);
	if (is_east)pthread_cond_signal(&cond_east);
	else if (!is_north && queueNorth.count > 0) {
		 queueNorth.pop();
		pthread_cond_signal(&firstNorth);
	}
	usleep(2000);
	pthread_mutex_unlock(&mutex_d);
}

void* car_from_west(void* arg) {
	pthread_mutex_lock(&mutex_wait_west);
	pthread_cond_wait(&firstWest, &mutex_wait_west);
	pthread_mutex_unlock(&mutex_wait_west);
	is_west = true;
	pthread_mutex_lock(&mutex_d);
	printf("car %d from West arrives at crossing\n", queueWest.current);
	sem_wait(&empty);
	dir = west;
	bool flag = false;
	int sem;
	sem_getvalue(&empty, &sem);
	if (sem == 0) {
		pthread_cond_signal(&cond_wake_deadlock);
		pthread_cond_wait(&cond_right_lock, &mutex_d);
		usleep(2000);
		pthread_mutex_lock(&mutex_a);
		pthread_mutex_unlock(&mutex_d);
		printf("car %d from West leaves at crossing\n", queueWest.current);
		is_south = false;
		sem_post(&empty);
		usleep(2000);
		pthread_mutex_unlock(&mutex_a);
		wakeupall();
		return NULL;
	}
	else if (is_south) {
		flag = true;
		pthread_cond_wait(&cond_west, &mutex_block_west);
		if (is_deadlock) {
			usleep(2000);
			pthread_mutex_lock(&mutex_a);
			pthread_mutex_unlock(&mutex_d);
			printf("car %d from West leaves at crossing\n", queueWest.current);
			if (dir == north)pthread_cond_signal(&cond_right_lock);
			else pthread_cond_signal(&cond_north);
			is_west = false;
			sem_post(&empty);
			usleep(2000);
			pthread_mutex_unlock(&mutex_a);
			return NULL;
		}
	}
	usleep(2000);
	pthread_mutex_lock(&mutex_a);
	if (!is_north && queueNorth.count > 0 && flag) {
		queueNorth.pop();
		pthread_cond_signal(&firstNorth);
	}

	sem_post(&empty); 
	pthread_mutex_unlock(&mutex_d);
	is_west = false;

	printf("car %d from West leaves at crossing\n", queueWest.current);
	if (is_north)pthread_cond_signal(&cond_north);
	else if (!is_west && queueWest.count > 0) {
		 queueWest.pop();
		pthread_cond_signal(&firstWest);
	}

	usleep(2000);
	pthread_mutex_unlock(&mutex_a);
}

void* check_dead_lock(void* arg) {
	//wait....
	usleep(4000);
	//at first wake up all the car;
	wakeupall();

	while(1){
		pthread_mutex_lock(&mutex_deadlock);
		//wait for deadlock
		pthread_cond_wait(&cond_wake_deadlock, &mutex_deadlock);
		//deadlock happen,set is_deadlock true
		is_deadlock = true;
		printf("DEADLOCK: car jam detected, signalling");
		//ask a car to go first,according to the latest car direction.
		switch (dir) {
		case north: {printf(" East "); pthread_cond_signal(&cond_east); break; }
		case east: {printf(" South "); pthread_cond_signal(&cond_south); break; }
		case west: {printf(" North "); pthread_cond_signal(&cond_north); break; }
		case south: {printf(" West "); pthread_cond_signal(&cond_west); break; }
		}
		printf("to go\n");

		pthread_mutex_unlock(&mutex_deadlock);
	}
}

int main(int argc, char** argv) {

	int num[100];
	pthread_t check;
	//init_threads();
	pthread_cond_init(&cond_right_lock, NULL);
	pthread_cond_init(&cond_wake_deadlock, NULL);
	pthread_cond_init(&cond_west, NULL);
	pthread_cond_init(&cond_south, NULL);
	pthread_cond_init(&cond_east, NULL);
	pthread_cond_init(&cond_north, NULL);
	pthread_cond_init(&firstEast, NULL);
	pthread_cond_init(&firstWest, NULL);
	pthread_cond_init(&firstSouth, NULL);
	pthread_cond_init(&firstNorth, NULL);

	sem_init(&empty, 0, 4);

	pthread_mutex_init(&mutex_a, NULL);
	pthread_mutex_init(&mutex_b, NULL);
	pthread_mutex_init(&mutex_c, NULL);
	pthread_mutex_init(&mutex_d, NULL);
	pthread_mutex_init(&mutex_deadlock, NULL);
	pthread_mutex_init(&mutex_wait_north, NULL);
	pthread_mutex_init(&mutex_wait_south, NULL);
	pthread_mutex_init(&mutex_wait_east, NULL);
	pthread_mutex_init(&mutex_wait_west, NULL);
	pthread_mutex_init(&mutex_block_north, NULL);
	pthread_mutex_init(&mutex_block_east, NULL);
	pthread_mutex_init(&mutex_block_west, NULL);
	pthread_mutex_init(&mutex_block_south, NULL);
	
	char s[100];
	scanf("%s", s);
	int len = strlen(s);

	//create the thread
	for (int i = 0; i < len; i++)num[i] = i + 1;
	for (int i = 0; i < len; i++) {
		switch (s[i]) {
		case 'w': {
			is_west = true;
			queueWest.push(num[i]);
			car[tot_car++] = queueWest.thread[queueWest.front];
			pthread_create(&queueWest.thread[queueWest.front],
				NULL, car_from_west, NULL);
			break;
		}
		case 'e': {
			is_east = true;
			queueEast.push(num[i]);
			car[tot_car++] = queueEast.thread[queueEast.front];
			pthread_create(&queueEast.thread[queueEast.rear],
				NULL, car_from_east, NULL);
			break;
		}
		case 's': {
			is_south = true;
			queueSouth.push(num[i]);
			car[tot_car++] = queueSouth.thread[queueSouth.rear];
			pthread_create(&queueSouth.thread[queueSouth.rear],
				NULL, car_from_south, NULL);
			break;
		}
		case 'n': {
			is_north = true;
			queueNorth.push(num[i]);
			car[tot_car++] = queueNorth.thread[queueNorth.rear];
			pthread_create(&queueNorth.thread[queueNorth.rear],
				NULL, car_from_north, NULL);
			break;
		}
		}
	}

	pthread_create(&check, NULL, check_dead_lock, NULL);
	//join the thread
	for (int i = 0; i < tot_car; i++) {
		pthread_join(car[i], NULL);
	}
}