all: docu_auto

docu_auto:
	ls *.md *py doc/*  | entr -s 'python make_doc.py'

docu: make_doc.py
	python make_doc.py

.PHONY: docu
