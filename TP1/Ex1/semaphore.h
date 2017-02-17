/*
  semaphore.h

  Semaphore implementation using C++11 Standard Thread Library.
  See "Cooperating sequential processes". E. W. Dijkstra, 1965.

  Frédéric Goualard <Frederic.Goualard@univ-nantes.fr>
  V. 1, 2012-01-21
 */
#include <mutex>
#include <condition_variable>
#include <thread>

class Semaphore {
 public:
  Semaphore(int v) : tokens(v) {}
  friend void P(Semaphore& sem);
  friend void V(Semaphore& sem);
 private:
  void P(void);
  void V(void);
  int tokens;
  std::mutex mut;
  std::condition_variable queue;
};

// wait operation on a semaphore
void P(Semaphore& sem);
// signal operation on a semaphore
void V(Semaphore& sem);
