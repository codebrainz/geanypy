#!/bin/sh

_mkdir()
{
	test -d $@ || mkdir $@
}

_mkdir build-aux	|| exit 1
autoreconf -vfi		|| exit 1
