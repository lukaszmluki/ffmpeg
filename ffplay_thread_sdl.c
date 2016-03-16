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

#include <SDL_thread.h>
#include <libavutil/mem.h>
#include <ffplay_thread.h>

struct ff_mutex_ {
    SDL_mutex *mutex;
};

struct ff_cond_ {
    SDL_cond *cond;
};

struct ff_thread_ {
    SDL_Thread *thread;
};

ff_thread* ff_thread_create(ff_thread_fun fun, void *arg)
{
    ff_thread *thread = av_malloc(sizeof(ff_thread));
    if (!thread)
        return NULL;
    thread->thread = SDL_CreateThread(fun, arg);
    if (!thread->thread )
        av_freep(&thread);
    return thread;
}

void ff_thread_wait(ff_thread *thread, int *ret)
{
    SDL_WaitThread(thread->thread, ret);\
    av_free(thread);
}

ff_cond* ff_cond_create(void)
{
    ff_cond *cond = av_malloc(sizeof(ff_cond));
    if (!cond)
        return NULL;
    cond->cond = SDL_CreateCond();
    if (!cond->cond)
        av_freep(&cond);
    return cond;
}

void ff_cond_destroy(ff_cond *cond)
{
    SDL_DestroyCond(cond->cond);
    av_free(cond);
}

ff_mutex* ff_mutex_create(void)
{
    ff_mutex *mutex = av_malloc(sizeof(ff_mutex));
    if (!mutex)
        return NULL;
    mutex->mutex = SDL_CreateMutex();
    if (!mutex->mutex)
        av_freep(&mutex);
    return mutex;
}

void ff_mutex_destroy(ff_mutex *mutex)
{
    SDL_DestroyMutex(mutex->mutex);
    av_free(mutex);
}

int ff_mutex_lock(ff_mutex *mutex)
{
    return SDL_LockMutex(mutex->mutex);
}

int ff_mutex_unlock(ff_mutex *mutex)
{
    return SDL_UnlockMutex(mutex->mutex);
}

int ff_cond_signal(ff_cond *cond)
{
    return SDL_CondSignal(cond->cond);
}

int ff_cond_wait(ff_cond *cond, ff_mutex *mutex)
{
    return SDL_CondWait(cond->cond, mutex->mutex);
}

int ff_cond_wait_timeout(ff_cond *cond, ff_mutex *mutex, int timeout)
{
    return SDL_CondWaitTimeout(cond->cond, mutex->mutex, timeout);
}
