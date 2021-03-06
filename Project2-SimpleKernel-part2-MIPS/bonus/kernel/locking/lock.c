#include "lock.h"
#include "sched.h"
#include "syscall.h"

int lock_queue_id = 1;

void spin_lock_init(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void spin_lock_acquire(spin_lock_t *lock)
{
    while (LOCKED == lock->status)
    {
    };
    lock->status = LOCKED;
}

void spin_lock_release(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void do_mutex_lock_init(mutex_lock_t *lock)
{
	lock->status = UNLOCKED;
    lock->lock_id = lock_queue_id++;
}

void do_mutex_lock_acquire(mutex_lock_t *lock)
{
	if(lock->status == LOCKED)
    {
        do_block(&block_queue[lock->lock_id]);
    }
    lock->status = LOCKED;

}

void do_mutex_lock_release(mutex_lock_t *lock) 
{
	if(!queue_is_empty(&block_queue[lock->lock_id]))
    {
        do_unblock_one(&block_queue[lock->lock_id]);
        lock->status = UNLOCKED;             //先把阻塞队列中的第一个进入准备队列，然后把锁解开，这样准备队列
		                                     //轮到它执行时才能把锁拿到
    }
    else
        lock->status = UNLOCKED;
}
