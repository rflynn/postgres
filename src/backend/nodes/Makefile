#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for backend/nodes
#
# IDENTIFICATION
#    src/backend/nodes/Makefile
#
#-------------------------------------------------------------------------

subdir = src/backend/nodes
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS = nodeFuncs.o nodes.o list.o bitmapset.o tidbitmap.o \
       copyfuncs.o equalfuncs.o makefuncs.o \
       outfuncs.o outfuncs_json.o readfuncs.o print.o read.o params.o value.o

outfuncs_json.o: outfuncs_shared_conds.c outfuncs_shared_defs.c

include $(top_srcdir)/src/backend/common.mk
