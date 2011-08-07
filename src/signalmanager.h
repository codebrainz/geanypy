/*
 * signalmanager.h
 *
 * Copyright 2011 Matthew Brush <mbrush@codebrainz.ca>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 *
 */

#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "geanyplugin.h"

typedef struct _SignalManager SignalManager;

struct _SignalManager
{
    GeanyPlugin *geany_plugin;
    PyObject *py_manager;
    PyObject *py_emit_func;
};


SignalManager *signal_manager_new(GeanyPlugin *geany_plugin);
void signal_manager_free(SignalManager *signal_manager);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SIGNALMANAGER_H */
