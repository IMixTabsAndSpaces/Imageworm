TESTS = loadtest.py

check_SCRIPTS = loadtest.py
check_DATA = testenv.py

CLEANFILES = $(check_SCRIPTS)
EXTRA_DIST = loadtest.in.py testenv.in.py

do_subst = sed -e 's,[@]top_builddir[@],$(top_builddir),g' \
    -e 's,[@]PYTHON[@],$(PYTHON),g'

loadtest.py: loadtest.in.py Makefile
	$(do_subst) < $< > $@
	chmod +x $@

testenv.py: testenv.in.py Makefile
	$(do_subst) < $< > $@
	chmod +x $@