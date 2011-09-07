/*
 * document.h
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

#ifndef GEANYPY_DOCUMENT_H__
#define GEANYPY_DOCUMENT_H__
#ifdef __cplusplus
extern "C" {
#endif


#include <Python.h>
#include <geanyplugin.h>
#include "common.h"


typedef struct
{
	PyObject_HEAD
	GeanyDocument *doc;
} Document;


PyMODINIT_FUNC init_geany_document(void);
Document *Document_create_new_from_geany_document(GeanyDocument *doc);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* GEANYPY_DOCUMENT_H__ */
