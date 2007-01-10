
SUBDIRS = crossmark

python_PYTHON = crossmark.py

if BUILD_DOCS
all-local: copy-stamp
endif

copy-stamp: $(srcdir)/crossmark.py
	cp $< .
	touch $@

CLEANFILES = \
	copy-stamp
