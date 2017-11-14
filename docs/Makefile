all: docu_auto

docu_auto:
	ls generation/*  | entr python generation/make_doc.py

docu: make_doc.py
	python generation/make_doc.py

.PHONY: docu
