all: niftest.so niftest.beam unixsocket.so unixsocket.beam

ERL_ROOT=/opt/local/lib/erlang

%.so: %.c
	gcc -m32 -undefined dynamic_lookup -dynamiclib -o $@ -I $(ERL_ROOT)/usr/include $<

%.beam: %.erl
	erlc $<

.PHONY:clean
clean:
	rm *.beam *.so
