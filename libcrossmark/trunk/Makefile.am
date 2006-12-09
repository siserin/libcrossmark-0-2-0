
pkgconfdir = $(libdir)/pkgconfig
pkgconf_DATA = libcrossmark-1.pc

SUBDIRS = crossmark docs

EXTRA_DIST = \
	AUTHORS		\
	COPYING		\
	HACKING		\
	INSTALL		\
	autogen.sh	\
	libcrossmark-1.pc.in

distclean-local:
	rm -rf *.cache *~

#if WITH_LIBCROSSMARK_DOCS
#install-data-local:
#	mkdir -p $(DESTDIR)$(datadir)/doc/libwpd-@CROSSMARK_VERSION@
#	$(INSTALL) $(srcdir)/CHANGES $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
#	$(INSTALL) $(srcdir)/COPYING $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
#	$(INSTALL) $(srcdir)/CREDITS $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
#	$(INSTALL) $(srcdir)/HACKING $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
#	$(INSTALL) $(srcdir)/INSTALL $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
#	$(INSTALL) $(srcdir)/README $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
#	$(INSTALL) $(srcdir)/TODO $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/
#uninstall-local:
#	-rm -rf $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@
#endif
