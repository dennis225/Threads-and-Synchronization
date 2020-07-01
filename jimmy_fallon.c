#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <unistd.h> 


//Global variables
int next_id = 1;
sem_t connected_lock;  //binary semaphores 
sem_t operators;       //counting semaphore 


//the following function is executed by all threads (iteratively). In order to do this, binary semaphores are used to protect critical regions of code. We iteratively check if the connected number of lines is less than the number of lines available. If so, the call will ring and a counting semaphore is used properly to restrict the use of resources (operators). Furthermore, people can speak to the operator, buy a ticket and hang up the call. The connected number of lines and next_id is updated accordingly. Otherwise, the call will receive a busy signal (and binary semaphore is unlocked). We simulate the ticket order by sleeping both the conditions for a few seconds.

void *phonecall(void *threadId){
  static int NUM_OPERATORS = 3;
  static int NUM_LINES = 5;
  static int connected = 0;
  int id = next_id++; // updates next_id
  while(1){
    sem_wait(&connected_lock); //lock binary semaphore 
    if(connected < NUM_LINES){
      connected++; // incrementing connected number of lines
      sem_post(&connected_lock); //unlock binary semaphore
      printf(" Thread [  %i  ] has available line, call ringing \n", id);
      sem_wait(&operators); //lock counting semaphore
      printf(" Thread [  %i  ] is speaking to the operator \n", id);
      sleep(3); // sleeping program for 3 seconds
      printf(" Thread [  %i  ] has bought a ticket! \n", id); 
      printf(" Thread [  %i  ] has hung up! \n", id);  
      sem_post(&operators); // unlock counting semaphore
      sem_wait(&connected_lock); //lock binary semaphore 
      connected--; // decrementing connected number of lines
      sem_post(&connected_lock); //unlock binary semaphore
      pthread_exit(NULL); // terminates calling thread
    }
    else{
      sem_post(&connected_lock); //unlock binary semaphore
      sleep(3); // sleeping program for 3 seconds
      printf(" Thread [  %i  ] is calling line, busy signal \n",id);
    }
  }    
}

//the following function executes the program by initializing threads and handling errors. Furthermore, it waits for these threads to join and destroys both the binary and counting semaphores.

int main(int argc, char* argv[]){
  int rc;    //error handling
  pthread_t threads[atoi(argv[1])]; 
  int i;
  sem_init(&operators,0,3); //counting semaphore initiated
  sem_init(&connected_lock,0,1); //binary semaphore initiated
  for(i=0;i<atoi(argv[1]);++i){
    rc = pthread_create(&threads[i],NULL,phonecall, (void *)&i);
    if(rc){
      printf(" ERROR CREATING A THREAD!!! ");
      exit(-1);
    }
  }
  
  /* waiting for the threads*/
  for(int j=0;j<atoi(argv[1]);j++){
    pthread_join(threads[j], NULL);
  } 

  //Binary and counting semaphores are destroyed
  sem_destroy(&operators);
  sem_destroy(&connected_lock);
  
  return 0;
}