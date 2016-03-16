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

#ifndef FFPLAY_THREAD_H
#define FFPLAY_THREAD_H

#include <config.h>

typedef struct ff_mutex_ ff_mutex;
typedef struct ff_cond_ ff_cond;
typedef struct ff_thread_ ff_thread;
typedef int (*ff_thread_fun)(void *);

ff_mutex* ff_mutex_create(void);
void ff_mutex_destroy(ff_mutex *mutex);
int ff_mutex_lock(ff_mutex *mutex);
int ff_mutex_unlock(ff_mutex *mutex);

ff_cond* ff_cond_create(void);
void ff_cond_destroy(ff_cond *cond);
int ff_cond_signal(ff_cond *cond);
int ff_cond_wait(ff_cond *cond, ff_mutex *mutex);
int ff_cond_wait_timeout(ff_cond *cond, ff_mutex *mutex, int timeout);

ff_thread* ff_thread_create(ff_thread_fun fun, void *arg);
void ff_thread_wait(ff_thread *thread, int *ret);

#endif /* FFPLAY_THREAD_H */
