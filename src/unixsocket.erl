-module(unixsocket).

-export([init/0, open/1, close/1, read/2, write/2 ]).

-ifdef(TEST).
-include_lib("eunit/include/eunit.hrl").
-endif.

-spec init() -> term().
init() ->
	SoFile = filename:join(code:priv_dir(?MODULE),"unixsocket_drv"),
	erlang:load_nif(SoFile, 0).

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

-ifdef(TEST).
socket_test() ->
	?assertMatch(ok,unixsocket:init()),
	F = unixsocket:open("/tmp/haproxy"),
	?assert(is_integer(F)),
	?assert(F > 0),
	C = unixsocket:write(F,<<"show info",10,13,0>>),
	?assert(C =:= 12),
	{B, R} = unixsocket:read(F,4096),
	?assert(B > 0),
	?assertMatch(["Name: HAProxy" | _ ],string:tokens(binary:bin_to_list(R),"\n")),
	?assert(0 =:= unixsocket:close(F)).
-endif.
	
