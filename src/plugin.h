/*
 * plugin.h
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

#ifndef PLUGIN_H
#define PLUGIN_H
#ifdef __cplusplus
extern "C" {
#endif


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif


/* Set by Geany when plugin is loaded */
extern GeanyPlugin		*geany_plugin;
extern GeanyData		*geany_data;
extern GeanyFunctions	*geany_functions;


#include "app.h"
#include "project.h"


PyMODINIT_FUNC initmsgwindow(void);
PyMODINIT_FUNC init_geany_navqueue(void);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* PLUGIN_H */
