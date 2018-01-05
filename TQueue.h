// class TQueue
#include <pthread.h>
#include <list>
using namespace std;

template <typename T> class TQueue
{
  list<T> queue;
  pthread_mutex_t mutex;
  pthread_cond_t conditionalVar;
  
  public:
  
  TQueue(){
    //initialize the mutex and conditional variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conditionalVar, NULL);
  }
  
  void additem(T item){ //should be called by the producer thread
    //lock the queue to prevent competition between threads
    pthread_mutex_lock(&mutex);
    queue.push_back(item); //add item to the list
    
    //use signal to wake up only one item
    pthread_cond_signal(&conditionalVar);
    pthread_mutex_unlock(&mutex);
  }
  
  T removeItem(){ //should be called by the consumer thread
    pthread_mutex_lock(&mutex);
    while(queue.size() == 0){
      //tell the consumer thread to sleep and wait
      pthread_cond_wait(&conditionalVariable, &mutex);
    }
    
    //get the item from the list and pop it out
    T item = queue.front();
    queue.pop_front();
    
    pthread_mutex_unlock(&mutex);
    
    return item;
    
  }
  
  int queue_size(){
    pthread_mutex_lock(&mutex);
    int size = queue.size();
    pthread_mutex_unlock(&mutex);
    return size;
  }
  
}
