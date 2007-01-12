
AM_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(DEPS_CFLAGS) \
	-I$(srcdir) \
	-I$(top_srcdir)

pkgpyexecdir = $(pyexecdir)/crossmark/block
pkgpyexec_PYTHON = __init__.py block.py
pkgpyexec_LTLIBRARIES = _block.la

_block_la_LDFLAGS = \
	-avoid-version \
	-module \
	-no-undefined

_block_la_LIBADD = \
	$(DEPS_LIBS) \
	$(top_builddir)/crossmark/libcrossmark-1.la

_block_la_SOURCES = \
	$(BUILT_SOURCES)

# swig-stamp is EXTRA_DIST'd so tarball builds don't require swig
swig-stamp: block.swig block.hh Makefile.am
	$(SWIG) -threads -Wall -c++ -python -directors -o block-wrap.cc $(srcdir)/$<
	touch $@

block-wrap.cc: swig-stamp

block-wrap.h: swig-stamp

block.py: swig-stamp

# when generating docs we run python tools in the build tree
all-local:
	test -f __init__.py || $(LN_S) $(srcdir)/__init__.py
	test -f block.py || $(LN_S) $(srcdir)/block.py

noinst_HEADERS = \
	block.hh

BUILT_SOURCES = \
	block-wrap.cc \
	block-wrap.h \
	block.py

EXTRA_DIST = \
	block.swig \
	$(BUILT_SOURCES) \
	swig-stamp

DISTCLEANFILES = \
	$(BUILT_SOURCES) \
	__init__.pyc \
	block.pyc \
	swig-stamp
