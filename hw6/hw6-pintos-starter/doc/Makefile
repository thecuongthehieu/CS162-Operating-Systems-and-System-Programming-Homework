TEXIS = pintos.texi intro.texi threads.texi userprog.texi vm.texi	\
filesys.texi license.texi reference.texi 44bsd.texi standards.texi	\
doc.texi sample.tmpl.texi devel.texi debug.texi installation.texi	\
bibliography.texi localsettings.texi

all: pintos.html pintos.info pintos.pdf

pintos.html: $(TEXIS) texi2html
	./texi2html -toc_file=$@ -split=chapter -nosec_nav -nomenu -init_file ./pintos-t2h.init $<

pintos.info: $(TEXIS)
	makeinfo $<

pintos.text: $(TEXIS)
	makeinfo --plaintext -o $@ $<

pintos.dvi: $(TEXIS)
	texi2dvi $< -o $@

pintos.ps: pintos.dvi
	dvips $< -o $@

pintos.pdf: $(TEXIS)
	texi2pdf $< -o $@

%.texi: %
	sed < $< > $@ 's/\([{}@]\)/\@\1/g;'

clean:
	rm -f *.info* *.html
	rm -f *.dvi *.pdf *.ps *.log *~
	rm -rf WWW
	rm -f sample.tmpl.texi

dist: pintos.html pintos.pdf
	rm -rf WWW
	mkdir WWW WWW/specs
	cp *.html *.pdf *.css *.tmpl WWW
	(cd ../specs && cp -r *.pdf freevga kbd sysv-abi-update.html ../doc/WWW/specs)
