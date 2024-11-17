#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Мьютекс для синхронизации доступа к общему ресурсу.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // Условная переменная для уведомления о событии.
int event_occurred = 0;  // Флаг события

void* producer(void* arg) {
    while (1) {
        sleep(1);  // Задержка в 1 секунду
        pthread_mutex_lock(&mutex);
        event_occurred = 1;  // Устанавливаем флаг события
        printf("Producer: Event initiated.\n");
        pthread_cond_signal(&cond);  // Уведомление потребителя
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        // Ждать, пока не произойдет событие
        while (!event_occurred) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Consumer: Event processed.\n");
        event_occurred = 0;  // Сбрасываем флаг события
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread; //создаём потоки

    // Создание потоков
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Ожидание завершения потоков
    pthread_join(producer_thread, NULL);  //инициализация мьютекса значением по умолчанию
    pthread_join(consumer_thread, NULL); //Ожидает завершения потоков

    // Освобождение ресурсов (хотя в нашем случае код никогда не завершится)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}




//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
//int pthread_mutex_destroy(pthread_mutex_t *LOCK) - удаляет блокировку LOCK.
//int pthread_mutex_lock(pthread_mutex_t *LOCK) - блокирует остальные потоки.
//int pthread_mutex_unlock(pthread_mutex_t *LOCK) - разблокирует остальные потоки.
