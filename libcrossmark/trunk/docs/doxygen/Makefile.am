EXTRA_DIST = \
	Doxyfile.in \
	footer.html.in \
	header.html.in

if BUILD_DOCS

install-data-am:
	mkdir -p $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html
	$(INSTALL) html/*.html $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html/
	$(INSTALL) html/*.png $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html/
	$(INSTALL) html/*.css $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html/

uninstall-am:
	-rm -rf $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html

all: mainpage.dox release-notes.dox hacking.dox authors.dox copying.dox
	test -f header.html.in || $(LN_S) $(srcdir)/header.html.in ./
	test -f footer.html.in || $(LN_S) $(srcdir)/footer.html.in ./
	doxygen 

else

all:

endif

mainpage.dox: $(top_srcdir)/docs/mainpage.dox.in
	sleep 1
	echo "/*!" > $@
	cat $^ >> $@
	$(top_builddir)/tools/cm2dox $(top_srcdir)/README >> $@
	echo "*/" >> $@

release-notes.dox:
	sleep 1
	echo "/*!" > $@
	echo '\page release-notes Release Notes' >> $@
	$(top_builddir)/tools/cm2dox $(top_srcdir)/docs/release-0.1.0.txt >> $@
	echo "*/" >> $@

hacking.dox: $(top_srcdir)/HACKING
	sleep 1
	echo "/*!" > $@
	echo '\page hacking Contributing' >> $@
	$(top_builddir)/tools/cm2dox $^ >> $@
	echo "*/" >> $@

authors.dox: $(top_srcdir)/AUTHORS
	sleep 1
	echo "/*!" > $@
	echo '\page authors Credits' >> $@
	$(top_builddir)/tools/cm2dox $^ >> $@
	echo "*/" >> $@

copying.dox: $(top_srcdir)/COPYING
	sleep 1
	echo "/*!" > $@
	echo '\page copying License' >> $@
	echo '\include COPYING' >> $@
	echo "*/" >> $@

distclean-local: clean-local

clean-local:
	rm -rf html
