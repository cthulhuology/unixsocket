// unixsocket.c 
//
// © 2012 David J. Goehrig
// 

#include "erl_nif.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef enif_is_number
#define enif_is_number(X,Y) Y
#endif

static ERL_NIF_TERM open_socket(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	struct sockaddr_un address;
	char path[sizeof(address.sun_path)];
	int fd = 0;
	
	if (argc != 1 || ! enif_is_list(env,argv[0])) return enif_make_badarg(env);

	memset(&address,0,sizeof(struct sockaddr_un));
	memset(path,0,sizeof(address.sun_path));
	enif_get_string(env,argv[0],path,sizeof(address.sun_path)-1,ERL_NIF_LATIN1);

 	fd = socket(PF_LOCAL,SOCK_STREAM,0);
	address.sun_family = AF_UNIX;
	snprintf(address.sun_path, sizeof(address.sun_path) -1 , "%s", path);
	if (connect(fd,(struct sockaddr *) &address,sizeof(struct sockaddr_un))) {
		fprintf(stderr,"Failed to connect to %s\n",path);
	}

	return enif_make_int(env,fd);
}

static ERL_NIF_TERM close_socket(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	int fd = 0;	

	if (argc != 1 || !enif_is_number(env,argv[0])) return enif_make_badarg(env);

	enif_get_int(env,argv[0],&fd);

	return enif_make_int(env,close(fd));
}

static ERL_NIF_TERM read_socket(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	int fd = 0;
	int length = 4096;
	ErlNifBinary bin;

	if (argc < 1 || argc > 2 || !enif_is_number(env,argv[0])) return enif_make_badarg(env);
	if (argc == 2) enif_get_int(env,argv[1],&length);
	
	enif_get_int(env,argv[0],&fd);
	enif_alloc_binary(length,&bin);
	memset(bin.data,0,bin.size);

	length = recv(fd,bin.data,bin.size,0);
	enif_realloc_binary(&bin,length);
	
	return enif_make_tuple2(env, enif_make_int(env,length), enif_make_binary(env,&bin));
}

static ERL_NIF_TERM write_socket(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	int fd = 0;
	size_t length = 0;
	ErlNifBinary bin;

	if (argc != 2 || !enif_is_binary(env,argv[1]) || !enif_is_number(env,argv[0])) return enif_make_badarg(env);
	enif_inspect_binary(env,argv[1],&bin);
	enif_get_int(env,argv[0],&fd);

	length = send(fd,bin.data,bin.size,0);

	return enif_make_int(env,length);
}

static ErlNifFunc nif_funcs[] = {
	{"open", 1, open_socket},
	{"close", 1, close_socket},
	{"read",2, read_socket},
	{"write",2, write_socket}
};

ERL_NIF_INIT(unixsocket,nif_funcs,NULL,NULL,NULL,NULL)

