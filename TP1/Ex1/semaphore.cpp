/*
  semaphore.cpp

  Semaphore implementation using C++11 Standard Thread Library.
  See "Cooperating sequential processes". E. W. Dijkstra, 1965.

  Frédéric Goualard <Frederic.Goualard@univ-nantes.fr>
  V. 1, 2012-01-21
 */
#include "semaphore.h"

using namespace std;

void Semaphore::P(void)
{
  unique_lock<mutex> lock(mut);
  queue.wait(lock,[=]{return tokens > 0;});
  --tokens;
}

void Semaphore::V(void)
{
  lock_guard<mutex> lock(mut);
  ++tokens;
  queue.notify_all(); // We give a chance to all threads waiting to awake
}

void P(Semaphore& sem)
{
  sem.P();
}

void V(Semaphore& sem)
{
  sem.V();
}

