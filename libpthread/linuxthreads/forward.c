/* Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <features.h>
#include <stdlib.h>
#include <dlfcn.h>

/* psm: keep this before internals.h */
#if 0
vda: here is why:
headers contain libc_hidden_proto(foo).
In libpthread/linuxthreads/sysdeps/pthread/bits/libc-lock.h
adding libc_hidden_proto(foo) just before weak_extern (__pthread_initialize)
will not warn:
    /* libc_hidden_proto(foo) */
    weak_extern (__pthread_initialize)
    /* libc_hidden_proto(foo) */
but adding after will! Which is extremely strange -
weak_extern expands into just "#pragma weak __pthread_initialize".
TODO: determine whether it is a gcc bug or what
(see gcc.gnu.org/PR36282).
For now, just include all headers before internals.h
(they are again included in internals.h - maybe remove them there later)
#endif

#include <string.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "internals.h"

/* Pointers to the libc functions.  */
struct pthread_functions __libc_pthread_functions attribute_hidden;


# define FORWARD2(name, rettype, decl, params, defaction) \
rettype									      \
name decl								      \
{									      \
  if (__libc_pthread_functions.ptr_##name == NULL)			      \
    defaction;								      \
									      \
  return __libc_pthread_functions.ptr_##name params;			      \
}

# define FORWARD(name, decl, params, defretval) \
  FORWARD2 (name, int, decl, params, return defretval)

FORWARD (pthread_attr_destroy, (pthread_attr_t *attr), (attr), 0)

FORWARD (pthread_attr_init, (pthread_attr_t *attr), (attr), 0)

FORWARD (pthread_attr_getdetachstate,
	 (const pthread_attr_t *attr, int *detachstate), (attr, detachstate),
	 0)
FORWARD (pthread_attr_setdetachstate, (pthread_attr_t *attr, int detachstate),
	 (attr, detachstate), 0)

FORWARD (pthread_attr_getinheritsched,
	 (const pthread_attr_t *attr, int *inherit), (attr, inherit), 0)
FORWARD (pthread_attr_setinheritsched, (pthread_attr_t *attr, int inherit),
	 (attr, inherit), 0)

FORWARD (pthread_attr_getschedparam,
	 (const pthread_attr_t *attr, struct sched_param *param),
	 (attr, param), 0)
FORWARD (pthread_attr_setschedparam,
	 (pthread_attr_t *attr, const struct sched_param *param),
	 (attr, param), 0)

FORWARD (pthread_attr_getschedpolicy,
	 (const pthread_attr_t *attr, int *policy), (attr, policy), 0)
FORWARD (pthread_attr_setschedpolicy, (pthread_attr_t *attr, int policy),
	 (attr, policy), 0)

FORWARD (pthread_attr_getscope,
	 (const pthread_attr_t *attr, int *scope), (attr, scope), 0)
FORWARD (pthread_attr_setscope, (pthread_attr_t *attr, int scope),
	 (attr, scope), 0)


FORWARD (pthread_condattr_destroy, (pthread_condattr_t *attr), (attr), 0)
FORWARD (pthread_condattr_init, (pthread_condattr_t *attr), (attr), 0)


FORWARD (pthread_cond_broadcast, (pthread_cond_t *cond), (cond), 0)

FORWARD (pthread_cond_destroy, (pthread_cond_t *cond), (cond), 0)

FORWARD (pthread_cond_init,
	 (pthread_cond_t *cond, const pthread_condattr_t *cond_attr),
	 (cond, cond_attr), 0)

FORWARD (pthread_cond_signal, (pthread_cond_t *cond), (cond), 0)

FORWARD (pthread_cond_wait, (pthread_cond_t *cond, pthread_mutex_t *mutex),
	 (cond, mutex), 0)

FORWARD (pthread_cond_timedwait,
	 (pthread_cond_t *cond, pthread_mutex_t *mutex,
	  const struct timespec *abstime), (cond, mutex, abstime), 0)


FORWARD (pthread_equal, (pthread_t thread1, pthread_t thread2),
	 (thread1, thread2), 1)


/* Use an alias to avoid warning, as pthread_exit is declared noreturn.  */
FORWARD2 (__pthread_exit, void, (void *retval), (retval), exit (EXIT_SUCCESS))
strong_alias (__pthread_exit, pthread_exit)


FORWARD (pthread_getschedparam,
	 (pthread_t target_thread, int *policy, struct sched_param *param),
	 (target_thread, policy, param), 0)
FORWARD (pthread_setschedparam,
	 (pthread_t target_thread, int policy,
	  const struct sched_param *param), (target_thread, policy, param), 0)


FORWARD (pthread_mutex_destroy, (pthread_mutex_t *mutex), (mutex), 0)

FORWARD (pthread_mutex_init,
	 (pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr),
	 (mutex, mutexattr), 0)
strong_alias(pthread_mutex_init, __pthread_mutex_init)

FORWARD (pthread_mutex_lock, (pthread_mutex_t *mutex), (mutex), 0)
strong_alias(pthread_mutex_lock, __pthread_mutex_lock)

FORWARD (pthread_mutex_trylock, (pthread_mutex_t *mutex), (mutex), 0)
strong_alias(pthread_mutex_trylock, __pthread_mutex_trylock)

FORWARD (pthread_mutex_unlock, (pthread_mutex_t *mutex), (mutex), 0)
strong_alias(pthread_mutex_unlock, __pthread_mutex_unlock)

FORWARD2 (pthread_self, pthread_t, (void), (), return 0)


FORWARD (pthread_setcancelstate, (int state, int *oldstate), (state, oldstate),
	 0)

FORWARD (pthread_setcanceltype, (int type, int *oldtype), (type, oldtype), 0)

#if 0
FORWARD2 (_pthread_cleanup_push, void, (struct _pthread_cleanup_buffer * buffer, void (*routine)(void *), void * arg), (buffer, routine, arg), return)
#endif
FORWARD2 (_pthread_cleanup_push_defer, void, (struct _pthread_cleanup_buffer * buffer, void (*routine)(void *), void * arg), (buffer, routine, arg), return)

#if 0
FORWARD2 (_pthread_cleanup_pop, void, (struct _pthread_cleanup_buffer * buffer, int execute), (buffer, execute), return)
#endif
FORWARD2 (_pthread_cleanup_pop_restore, void, (struct _pthread_cleanup_buffer * buffer, int execute), (buffer, execute), return)
