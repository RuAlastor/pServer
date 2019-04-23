
#include "logsqueue.h"

/* Check for errors later */

Locker::Locker(pthread_mutex_t& _mutex_to_lock) : mutex(_mutex_to_lock) {
    pthread_mutex_lock(&mutex);
}

Locker::~Locker() {
    pthread_mutex_unlock(&mutex);
}

LogsQueue::LogsQueue() : finished(false) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
}

void LogsQueue::addJob( std::unique_ptr<std::string> _new_job ) {
    {
        Locker lock(mutex);
        // std::cout << *_new_job << '\n';
        jobs.push( std::move(_new_job) );
    }
    pthread_cond_signal(&condition);
}

std::unique_ptr<std::string> LogsQueue::getJob() {
    Locker lock(mutex);
    while ( jobs.empty() ) {
        pthread_cond_wait( &condition, &mutex );
        if (finished) {
            return std::unique_ptr<std::string>(nullptr);
        }
        std::unique_ptr<std::string> nextJob = std::move(jobs.front());
        jobs.pop();
        return nextJob;
    }
}

LogsQueue::~LogsQueue() {
    finished = true;
    pthread_cond_broadcast(&condition);
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&mutex);
}
