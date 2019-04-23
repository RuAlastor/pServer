#ifndef LOGSQUEUE_H
#define LOGSQUEUE_H

#include "headers.h"

class Locker {
private:
    pthread_mutex_t& mutex;

public:
    Locker( pthread_mutex_t& _mutex_to_lock );

    ~Locker();
};

class LogsQueue {
    std::queue<std::unique_ptr<std::string>> jobs;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    bool finished;

public:
    LogsQueue();

    void addJob( std::unique_ptr<std::string> _new_job );
    std::unique_ptr<std::string> getJob();

    ~LogsQueue();

};


#endif // LOGSQUEUE_H
