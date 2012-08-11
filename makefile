cxx=g++
lver=0.0.1
lname=libstackonfigure.so.$(lver)
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
	install stackonfigure.pc -C /usr/lib/pkgconfig/

install_x64: $(lname) install
	install $(lname) -C /usr/lib64
	install stackinfigure.pc -C /usr/lib64/pkgconfig/
	
build/value.o: src/value.cc
	$(cxx) $(cflags) -o$@ $<

build/config.o: src/config.cc
	$(cxx) $(cflags) -o$@ $<

