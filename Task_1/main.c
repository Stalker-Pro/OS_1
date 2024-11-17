#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // ������� ��� ������������� ������� � ������ �������.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // �������� ���������� ��� ����������� � �������.
int event_occurred = 0;  // ���� �������

void* producer(void* arg) {
    while (1) {
        sleep(1);  // �������� � 1 �������
        pthread_mutex_lock(&mutex);
        event_occurred = 1;  // ������������� ���� �������
        printf("Producer: Event initiated.\n");
        pthread_cond_signal(&cond);  // ����������� �����������
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        // �����, ���� �� ���������� �������
        while (!event_occurred) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Consumer: Event processed.\n");
        event_occurred = 0;  // ���������� ���� �������
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread; //������ ������

    // �������� �������
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // �������� ���������� �������
    pthread_join(producer_thread, NULL);  //������������� �������� ��������� �� ���������
    pthread_join(consumer_thread, NULL); //������� ���������� �������

    // ������������ �������� (���� � ����� ������ ��� ������� �� ����������)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}




//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
//int pthread_mutex_destroy(pthread_mutex_t *LOCK) - ������� ���������� LOCK.
//int pthread_mutex_lock(pthread_mutex_t *LOCK) - ��������� ��������� ������.
//int pthread_mutex_unlock(pthread_mutex_t *LOCK) - ������������ ��������� ������.
