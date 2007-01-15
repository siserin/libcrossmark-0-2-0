
# TODO install generated docs

doxfiles = \
	mainpage.dox \
	release-notes.dox \
	hacking.dox \
	authors.dox \
	copying.dox

EXTRA_DIST = \
	$(doxfiles) \
	Doxyfile.in \
	dox-stamp \
	footer.html.in \
	header.html.in

DISTCLEANFILES = \
	mainpage.dox \
	release-notes.dox \
	hacking.dox \
	authors.dox \
	copying.dox \
	copy-stamp

all-local:
	test -f header.html.in || $(LN_S) $(srcdir)/header.html.in
	test -f footer.html.in || $(LN_S) $(srcdir)/footer.html.in
	doxygen 

PYTHONPATH := "$(top_builddir):$(top_builddir)/python/.libs:$(top_builddir)/python/block/.libs:$(top_builddir)/python/link/.libs:$(top_builddir)/python/style/.libs"
XM2DOX := $(top_srcdir)/tools/xm2dox.py

dox-stamp: $(doxfiles)
	touch $@

mainpage.dox: $(top_srcdir)/docs/mainpage.dox.in $(top_srcdir)/README copy-stamp
	echo "/*!" > $@
	cat $< >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $(top_srcdir)/README >> $@
	echo "*/" >> $@

release-notes.dox: $(top_srcdir)/docs/release-0.1.0.txt $(top_srcdir)/docs/release-0.2.0.txt copy-stamp
	echo "/*!" > $@
	echo '\page release-notes Release Notes' >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $(top_srcdir)/docs/release-0.2.0.txt >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $(top_srcdir)/docs/release-0.1.0.txt >> $@
	echo "*/" >> $@

hacking.dox: $(top_srcdir)/HACKING copy-stamp
	echo "/*!" > $@
	echo '\page hacking Contributing' >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $< >> $@
	echo "*/" >> $@

authors.dox: $(top_srcdir)/AUTHORS copy-stamp
	echo "/*!" > $@
	echo '\page authors Credits' >> $@
	PYTHONPATH=$(PYTHONPATH) $(XM2DOX) $< >> $@
	echo "*/" >> $@

copying.dox: $(top_srcdir)/COPYING copy-stamp
	echo "/*!" > $@
	echo '\page copying License' >> $@
	echo '\include COPYING' >> $@
	echo "*/" >> $@

copy-stamp:
	test -f $(top_builddir)/crossmark.py || cp $(top_srcdir)/crossmark.py $(top_builddir)/
	touch $@

clean-local:
	rm -rf html
