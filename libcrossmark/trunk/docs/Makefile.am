
DIST_SUBDIRS = doxygen examples

SUBDIRS = doxygen

if BUILD_EXAMPLES
SUBDIRS += examples
endif

EXTRA_DIST = \
	mainpage.dox.in \
	release-0.1.0.txt \
	release-0.2.0.txt

if BUILD_DOCS

install-data-am:
	mkdir -p $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@

endif

dist-hook:
	rm -rf `find $(distdir)/examples -name '\.svn'`
	rm -rf `find $(distdir)/examples -name '*~'`
	rm -rf `find $(distdir)/examples -name '*_'`
