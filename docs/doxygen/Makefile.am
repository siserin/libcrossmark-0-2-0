
EXTRA_DIST = \
	Doxyfile.in \
	footer.html.in \
	header.html.in

if BUILD_DOCS

CLEANFILES = \
	mainpage.dox \
	release-notes.dox \
	hacking.dox \
	authors.dox \
	copying.dox

install-data-am:
	mkdir -p $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html
	$(INSTALL) html/*.html $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html/
	$(INSTALL) html/*.png $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html/
	$(INSTALL) html/*.css $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html/

uninstall-am:
	-rm -rf $(DESTDIR)$(datadir)/doc/libcrossmark-@CROSSMARK_VERSION@/html

all-local: mainpage.dox release-notes.dox hacking.dox authors.dox copying.dox
	test -f header.html.in || $(LN_S) $(srcdir)/header.html.in ./
	test -f footer.html.in || $(LN_S) $(srcdir)/footer.html.in ./
	doxygen 
endif

PYTHONPATH := "$(top_builddir)/python:$(top_srcdir)/python"
XM2DOX := $(top_srcdir)/tools/xm2dox.py

mainpage.dox: $(top_srcdir)/docs/mainpage.dox.in $(top_srcdir)/README
	echo "/*!" > $@
	cat $< >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $(top_srcdir)/README >> $@
	echo "*/" >> $@

release-notes.dox: $(top_srcdir)/docs/release-0.1.0.txt $(top_srcdir)/docs/release-0.2.0.txt
	echo "/*!" > $@
	echo '\page release-notes Release Notes' >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $(top_srcdir)/docs/release-0.2.0.txt >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $(top_srcdir)/docs/release-0.1.0.txt >> $@
	echo "*/" >> $@

hacking.dox: $(top_srcdir)/HACKING
	echo "/*!" > $@
	echo '\page hacking Contributing' >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $^ >> $@
	echo "*/" >> $@

authors.dox: $(top_srcdir)/AUTHORS
	echo "/*!" > $@
	echo '\page authors Credits' >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $^ >> $@
	echo "*/" >> $@

copying.dox: $(top_srcdir)/COPYING
	echo "/*!" > $@
	echo '\page copying License' >> $@
	echo '\include COPYING' >> $@
	echo "*/" >> $@

distclean-local: clean-local

clean-local:
	rm -rf html
