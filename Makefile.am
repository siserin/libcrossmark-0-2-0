pkgconfdir = $(libdir)/pkgconfig

pkgconf_DATA = libcrossmark-1.pc

SUBDIRS = src docs

EXTRA_DIST = \
	autogen.sh	\
	libcrossmark-*.pc.in	\
	CHANGES		\
	COPYING		\
	CREDITS		\
	HACKING		\
	INSTALL		\
	README		\
	TODO

distclean-local:
	rm -rf *.cache *~

#if WITH_LIBWPD_DOCS
#install-data-local:
#	mkdir -p $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@
#	$(INSTALL) $(srcdir)/CHANGES $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
#	$(INSTALL) $(srcdir)/COPYING $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
#	$(INSTALL) $(srcdir)/CREDITS $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
#	$(INSTALL) $(srcdir)/HACKING $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
#	$(INSTALL) $(srcdir)/INSTALL $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
#	$(INSTALL) $(srcdir)/README $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
#	$(INSTALL) $(srcdir)/TODO $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@/
#uninstall-local:
#	-rm -rf $(DESTDIR)$(datadir)/doc/libwpd-@WPD_VERSION@
#endif