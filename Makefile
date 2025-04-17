TARGETS=$(HOME)/bin/unixcon $(HOME)/.local/lib/unixcon.so

unixcon.so: unixcon.c
	$(CC) -Os -fPIC -shared -o $@ $<

.PHONY: install uninstall

install: $(TARGETS)

uninstall:
	rm -f $(TARGETS)

$(HOME)/bin/unixcon: unixcon
	install -D -m 755 $< $@

$(HOME)/.local/lib/unixcon.so: unixcon.so
	install -D -m 755 $< $@
