#pragma once

//#include <Windows.h>
#include <semaphore.h>

class CCritical
{
public:
    CCritical()
    {
    }
    ~CCritical()
    {
    }
    void Init()
    {
        sem_init(&m_hSem, 0, 1);
    }
    void Delete()
    {
        sem_destroy(&m_hSem);
    }
    void Enter()
    {
        sem_wait(&m_hSem);
    }
    void Leave()
    {
        sem_post(&m_hSem);
    }
private:
    sem_t	m_hSem;
};
