# src/Makefile.am


BUILT_SOURCES = \
	tiger_grm.h

AM_YFLAGS = -d

lib_LTLIBRARIES = \
	libtiger.la

libtiger_la_SOURCES = \
	errormsg.c \
	symbol.c \
	table.c \
	temp.c \
	util.c \
	absyn.c \
	env.c \
	escape.c \
	semant.c \
	tiger_grm.y \
	tiger_lex.l \
	translate.c \
	types.c \
	x86frame.c \
	tree.c \
	canon.c \
	assem.c \
	x86codegen.c \
	graph.c \
	flowgraph.c \
	liveness.c \
	color.c \
	regalloc.c \
	prtree.c \
	prabsyn.c

EXTRA_DIST = \
	include/errormsg.h \
	include/symbol.h \
	include/table.h \
	include/temp.h \
	include/util.h \
	include/tokens.h \
	include/absyn.h \
	include/env.h \
	include/escape.h \
	include/semant.h \
	include/translate.h \
	include/types.h \
	include/frame.h \
	include/tree.h \
	include/canon.h \
	include/assem.h \
	include/codegen.h \
	include/graph.h \
	include/flowgraph.h \
	include/liveness.h \
	include/color.h \
	include/regalloc.h \
	include/prtree.h \
	include/prabsyn.h

bin_PROGRAMS = \
	tc

tc_SOURCES = \
	main.c

tc_LDADD = \
	libtiger.la
