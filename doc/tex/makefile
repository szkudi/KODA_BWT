PDFLATEXOPT=-synctex=1 #-output-directory=..
ROOT=report

all:
	pdflatex $(PDFLATEXOPT) $(ROOT)
#	bibtex $(ROOT)
	pdflatex $(PDFLATEXOPT) $(ROOT)
	pdflatex $(PDFLATEXOPT) $(ROOT)

clean:
	rm -f *.aux
	rm -f *.ind
	rm -f *.toc
	rm -f *.app
	rm -f *.bbl
	rm -f *.blg
	rm -f *.ilg
	rm -f *.idx
	rm -f *.lof
	rm -f *.log
	rm -f *.lol
	rm -f *.lot
	rm -f *.out
