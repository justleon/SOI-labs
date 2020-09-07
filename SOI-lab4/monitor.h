#ifndef MONITOR_H_INCLUDED
#define MONITOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

class Semaphore
{
private:
	sem_t sem;

public:
  Semaphore( int value )
  {
     if( sem_init( & sem, 0, value ) != 0 )
       throw "sem_init: failed";
  }
  ~Semaphore()
  {
	  sem_destroy( & sem );
  }

  void p()
  {
     if( sem_wait( & sem ) != 0 )
       throw "sem_wait: failed";
  }

  void v()
  {
     if( sem_post( & sem ) != 0 )
       throw "sem_post: failed";
  }

};

class Condition
{
  friend class Monitor;

private:
	Semaphore w;
	int waitingCount; //liczba oczekujacych watkow

public:
	Condition() : w( 0 )
	{
		waitingCount = 0;
	}

	void wait()
	{
		w.p();
	}

	bool signal()
	{
		if( waitingCount )
		{
			--waitingCount;
			w.v();
			return true;
		}//if
		else
			return false;
	}

};

class Monitor
{
private:
	Semaphore s;

public:
	Monitor() : s( 1 ) {}

	void enter()
	{
		s.p();
	}

	void leave()
	{
		s.v();
	}

	void wait( Condition & cond )
	{
		++cond.waitingCount;
		leave();
		cond.wait();
	}

	void signal( Condition & cond )
	{
		if( cond.signal() )
			enter();
	}

};

#endif // MONITOR_H_INCLUDED
