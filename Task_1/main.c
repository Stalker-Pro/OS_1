
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Мьютекс для синхронизации доступа к общему ресурсу.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // Условная переменная для уведомления о событии.
int event_count = 0;  // Счётчик событий

void* producer(void* arg) {
    while (1) {
        sleep(1);  // Задержка в 1 секунду
        pthread_mutex_lock(&mutex);
        event_count++;  // Увеличиваем счётчик событий
        printf("Producer: Event initiated. Current event count: %d\n", event_count);
        pthread_cond_signal(&cond);  // Уведомление потребителя
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        // Ждёт, пока не произойдет хотя бы одно событие
        while (event_count == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        // Обработка всех доступных событий
        while (event_count > 0) {
            printf("Consumer: Event processed. Remaining events: %d\n", event_count - 1);
            event_count--;  
        }
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
