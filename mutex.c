#include <stdio.h>
#include <pthread.h>
pthread_mutex_t mutex;
pthread_cond_t condc,condp;
int buffer = 0;
int consumed =0;
void* producer(void* i)
{
    int* id = (int*)i;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(consumed > 50)
            break;
        while(buffer >= 10)
        {
            printf("producer%d wait\n", *id);
            pthread_cond_wait(&condp,&mutex);
        }
        buffer += 1;
        printf("producer%d produce\n", *id);
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);
    }
}

void* consumer(void* i)
{
    int* id = (int*)i;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(consumed > 50)
            break;
        while(buffer ==0)
        {
            printf("consumer%d wait\n", *id);
            pthread_cond_wait(&condc,&mutex);
        }
        buffer -= 1;
        consumed +=1;
        printf("consumer%d consume\n", *id);
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char** argv)
{
    pthread_t pro1, pro2, pro3, con1, con2;
    int a1=1,a2=2,a3=3;
    pthread_mutex_init(&mutex,0);
    pthread_cond_init(&condc,0);
    pthread_cond_init(&condp,0);
    pthread_create(&con1,0,consumer, &a1);
    pthread_create(&con2,0,consumer, &a2);
    pthread_create(&pro1,0,producer, &a1);
    pthread_create(&pro2,0,producer, &a2);
    pthread_create(&pro3,0,producer, &a3);
    pthread_join(pro1,0);
    pthread_join(pro2,0);
    pthread_join(pro3,0);
    pthread_join(con1,0);
    pthread_join(con2,0);
    return 0;
}
