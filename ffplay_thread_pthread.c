/*
 * Copyright (c) 2016
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <pthread.h>
#include <sys/time.h>
#include <libavutil/mem.h>
#include <ffplay_thread.h>

struct ff_mutex_ {
    pthread_mutex_t mutex;
};

struct ff_cond_ {
    pthread_cond_t cond;
};

struct ff_thread_ {
    pthread_t thread;
    ff_thread_fun fun;
    void *arg;
    int ret;
};

static void* thread_fun_wrapper(void *arg)
{
    ff_thread *thread = arg;
    thread->ret = thread->fun(thread->arg);
    return NULL;
}

ff_thread* ff_thread_create(ff_thread_fun fun, void *arg)
{
    ff_thread *thread = av_malloc(sizeof(ff_thread));
    if (!thread)
        return NULL;
    thread->fun = fun;
    thread->arg = arg;
    if (pthread_create(&thread->thread, NULL, thread_fun_wrapper, thread) != 0)
        av_freep(&thread);
    return thread;
}

void ff_thread_wait(ff_thread *thread, int *ret)
{
    pthread_join(thread->thread, NULL);
    if (ret)
        *ret = thread->ret;
    av_free(thread);
}

ff_cond* ff_cond_create(void)
{
    ff_cond *cond = av_malloc(sizeof(ff_cond));
    if (!cond)
        return NULL;
    if (pthread_cond_init(&cond->cond, NULL) != 0)
        av_freep(&cond);
    return cond;
}

void ff_cond_destroy(ff_cond *cond)
{
    pthread_cond_destroy(&cond->cond);
    av_free(cond);
}

ff_mutex* ff_mutex_create(void)
{
    ff_mutex *mutex = av_malloc(sizeof(ff_mutex));
    if (!mutex)
        return NULL;
    if (pthread_mutex_init(&mutex->mutex, NULL) != 0)
        av_freep(&mutex);
    return mutex;
}

void ff_mutex_destroy(ff_mutex *mutex)
{
    pthread_mutex_destroy(&mutex->mutex);
    av_free(mutex);
}

int ff_mutex_lock(ff_mutex *mutex)
{
    return pthread_mutex_lock(&mutex->mutex);
}

int ff_mutex_unlock(ff_mutex *mutex)
{
    return pthread_mutex_unlock(&mutex->mutex);
}

int ff_cond_signal(ff_cond *cond)
{
    return pthread_cond_signal(&cond->cond);
}

int ff_cond_wait(ff_cond *cond, ff_mutex *mutex)
{
    return pthread_cond_wait(&cond->cond, &mutex->mutex);
}

int ff_cond_wait_timeout(ff_cond *cond, ff_mutex *mutex, int timeout)
{
    struct timeval delta;
    struct timespec abstime;

    gettimeofday(&delta, NULL);
    abstime.tv_sec = delta.tv_sec + (timeout / 1000);
    abstime.tv_nsec = (delta.tv_usec + (timeout % 1000) * 1000) * 1000;
    if ( abstime.tv_nsec > 1000000000 ) {
      abstime.tv_sec += 1;
      abstime.tv_nsec -= 1000000000;
    }

    return pthread_cond_timedwait(&cond->cond, &mutex->mutex, &abstime);
}
