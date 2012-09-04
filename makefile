cxx=g++
lver=0.0.1
name=stackonfigure
lname=lib$(name).so.$(lver)
incdir=/usr/include/stackonfigure
cflags=-c -fPIC -Wall -Wextra -Fall -Fextra -Iheaders -Iheaders/stackonfigure -std=c++0x
lflags=-shared -Wl,-soname,$(lname) -o$(lname)

dist: build/value.o build/config.o
	$(cxx) $(lflags) $<

install:
	if [ ! -d $(incdir) ] ; then mkdir $(incdir) ; fi
	install headers/stackonfigure/*.hh -C $(incdir)

install_x32: $(lname) install
	install $(lname) -C /usr/lib
	ln -s /usr/lib/$(lname) /usr/lib/lib$(name).so

install_x64: $(lname) install
	install $(lname) -C /usr/lib64
	ln -s /usr/lib64/$(lname) /usr/lib64/lib$(name).so

uninstall:
	rm -rf $(incdir)

uninstall_x32: uninstall
	rm /usr/lib/lib$(name)*

uninstall_x64:
	rm /usr/lib64/lib$(name)*
	
build/value.o: src/value.cc
	$(cxx) $(cflags) -o$@ $<

build/config.o: src/config.cc
	$(cxx) $(cflags) -o$@ $<

