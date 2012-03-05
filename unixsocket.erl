-module(unixsocket).

-export([init/0, open/1, close/1, read/2, write/2 ]).

-spec init() -> term().
init() ->
	erlang:load_nif("./unixsocket", 0).

-spec open(string()) -> integer().
open(_FileName) ->
	-1.	

-spec close(integer()) -> integer().
close(_FileDescriptor) ->
	-1.

-spec read(integer(),integer()) -> { integer(), binary() }.
read(_FileDescriptor,_Length) ->
	{ 0, <<"">> }.

-spec write(integer(),binary()) -> integer().
write(_FileDescriptor,_Binary) ->
	0.


