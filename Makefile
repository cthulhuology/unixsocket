all : release

.PHONY: compile release clean test
compile:
	./rebar compile

release: compile
	./rebar generate -f

test:
	./rebar eunit

clean:
	./rebar clean



