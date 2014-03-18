
/* HEADER FILES */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>


/* THREAD METHODS */
void *geekArrives(void *arg1);
void *nonGeekArrives(void *arg2);
void *singerArrives(void *arg3);

int boardBridge();
int goBridge();

int *geekCount;
int *ngeekCount;
int *singCount;

int geeks_curr = 0;
int nongeeks_curr = 0;
int singers_curr = 0;

int bridgeload = 0;
int count = 0;

sem_t geeks;
sem_t ngeeks;
sem_t mutex;
sem_t singers;

int main(int argc, char*argv[]){
	//Improper Number of Arguments
        if(argc != 4){
                printf("Usage: <Executable> <No. of Geeks> <No. of Non-Geeks> <No. of Singers>\n");
                exit(1);
        }
	
	int res, i, j, k;
	int gCount = atoi(argv[1]);
        int ngCount = atoi(argv[2]);
        int sCount = atoi(argv[3]);
	/* GLOBAL VARIABLE INITIALIZATIONS */
	geekCount = &gCount;
	ngeekCount = &ngCount;
	singCount = &sCount;
	
	res = sem_init(&geeks, 0, gCount);
	 if(res != 0){
                perror("semaphore initialization failed");
                exit(1);
        }

	res = sem_init(&ngeeks, 0, ngCount);
	 if(res != 0){
                perror("semaphore initialization failed");
                exit(1);
        }

	res = sem_init(&mutex, 0, 1);
	 if(res != 0){
                perror("semaphore initialization failed");
                exit(1);
        }

	res = sem_init(&singers, 0, sCount);
	 if(res != 0){
                perror("semaphore initialization failed");
                exit(1);
        }
	/* THREADS */
	pthread_t g_threads[gCount];
	pthread_t ng_threads[ngCount];
	pthread_t s_threads[sCount];

	int ret;
	srand (time(NULL));	

	/* GENERATING OF THE RANDOM THREADS FOR THE DIFFERENT PEOPLE */
	while(1){
		int ra = rand() % 3;
			if(ra == 0 ){	
				if(*geekCount > 0){
       	 				res = pthread_create(&(g_threads[count]), NULL, geekArrives, (void *) ra);
					count++;
				}
			}else if(ra == 1){
				if(*ngeekCount > 0){
					res = pthread_create(&(ng_threads[count]), NULL, nonGeekArrives, (void *) ra);
					count++;
				}
			}else if(ra == 2){
				if(*singCount > 0) { 	
					res = pthread_create(&(s_threads[count]), NULL, singerArrives, (void *) ra);
					count++;
				}
			}

			if(*geekCount > 0 || *ngeekCount == 0 || *singCount == 0){	
				int ret = goBridge();
			}
		
			sem_wait(&mutex);
                	
			if(*geekCount == 0 && *ngeekCount == 0 && *singCount == 0){
                        	break;
        	       	}
	
                	sem_post(&mutex);	
	}		
	
	printf("LEFT GEEKS : %d\t LEFT NON_GEEKS : %d\t LEFT SINGERS : %d\t\n",geeks_curr, nongeeks_curr , singers_curr);
	printf("ALL DONE\n");	
return 0;
}

int boardBridge(){
	int flag = 0;
	sem_wait(&mutex);	

	if( geeks_curr  >= 4 && nongeeks_curr >= 0 && singers_curr >= 0 ) {
		geeks_curr -= 4;
		printf("GEEKS : 4\t NON_GEEKS : 0\t SINGERS : 0\t\n");
                flag =  1;
        }else if( geeks_curr  >= 0 && nongeeks_curr >= 4 && singers_curr >= 0  ) {
		nongeeks_curr -= 4;
		printf("GEEKS : 0\t NON_GEEKS : 4\t SINGERS : 0\t\n");
                flag = 1;
	}else if( geeks_curr  >= 2 && nongeeks_curr >= 2 && singers_curr >= 0 ) {
                geeks_curr -= 2;
		nongeeks_curr -= 2;
		printf("GEEKS : 2\t NON_GEEKS : 2\t SINGERS : 0\t\n");
                flag = 1;
        }else if( geeks_curr  >= 2 && nongeeks_curr >= 1 && singers_curr >= 1 ) {
                geeks_curr -= 2;
		nongeeks_curr -= 1;
		singers_curr -= 1;
		printf("GEEKS : 2\t NON_GEEKS : 1\t SINGERS : 1\t\n");
                flag = 1;
        }else if( geeks_curr  >= 1 && nongeeks_curr >= 2 && singers_curr >= 1 ) {
                geeks_curr -= 1;
                nongeeks_curr -= 2;
                singers_curr -= 1;
		printf("GEEKS : 1\t NON_GEEKS : 2\t SINGERS : 1\t\n");
                flag = 1;
        }else if( geeks_curr  >= 3 && nongeeks_curr >= 0 && singers_curr >= 1 ) {
                geeks_curr -= 3;
                singers_curr -= 1;
                printf("GEEKS : 3\t NON_GEEKS : 0\t SINGERS : 1\t\n");
		flag = 1;
        }else if( geeks_curr  >= 0 && nongeeks_curr >= 3 && singers_curr >= 1 ) {
                nongeeks_curr -= 3;
                singers_curr -= 1;
		printf("GEEKS : 0\t NON_GEEKS : 3\t SINGERS : 1\t\n");
                flag = 1;
        }
	sem_post(&mutex);
	
return flag;
}

int goBridge(){
	 int value = boardBridge();
	 /* GO BRIDGE 1 SEC SLEEP FOR GOING*/
	 if(value == 1 )	
	 	sleep(1);	
	return value;
}

void *geekArrives(void *arg1){
	int geekid  = (int *)arg1;	
	sem_wait(&mutex);
	if(*geekCount > 0){
		geeks_curr += 1;
		*geekCount -= 1;
	}
	sem_post(&mutex);
	
}

void *nonGeekArrives(void *arg2){
	int nongeekid = (int *)arg2;
	sem_wait(&mutex);
	
	if(*ngeekCount > 0){
		nongeeks_curr += 1;
		*ngeekCount -= 1;
	}
	sem_post(&mutex);		
}

void *singerArrives(void *arg3){
	int singid = (int *)arg3;
	sem_wait(&mutex);

	if(*singCount > 0){
		singers_curr += 1;
		*singCount -= 1;
	}
	
	sem_post(&mutex);	
}

