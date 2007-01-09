
pkgconfdir = $(libdir)/pkgconfig
pkgconf_DATA = libcrossmark-1.pc

SUBDIRS = crossmark tools tests
if HAVE_PYTHON
SUBDIRS += python
endif
if BUILD_DOCS
SUBDIRS += docs
endif

EXTRA_DIST = \
	AUTHORS			\
	COPYING			\
	ChangeLog		\
	HACKING			\
	INSTALL			\
	README			\
	autogen.sh		\
	libcrossmark-1.pc.in	\
	python.m4

CLEANFILES = \
	cl-stamp

distclean-local:
	rm -rf *.cache *~

dist-hook: ChangeLog
	rm -rf `find $(distdir)/tests -name '\.svn'`
	rm -rf `find $(distdir)/tests -name '*~'`
	rm -rf `find $(distdir)/tests -name '*_'`

ChangeLog: cl-stamp
	if [ -d "$(srcdir)/.svn" ]; then \
		pwd=`pwd` && cd $(srcdir) && svn log -v > "$$pwd/$@"; \
	fi
	touch $<

cl-stamp:

if BUILD_DOCS
install-data-local:
	mkdir -p $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@
	$(INSTALL) $(srcdir)/ChangeLog $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
	$(INSTALL) $(srcdir)/COPYING $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
	$(INSTALL) $(srcdir)/AUTHORS $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
	$(INSTALL) $(srcdir)/HACKING $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
	$(INSTALL) $(srcdir)/INSTALL $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
uninstall-local:
	-rm -rf $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@
endif
