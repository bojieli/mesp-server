CC=gcc -g -O2 -std=c99 -D_GNU_SOURCE -Wall -Iinclude

all: mesp datiqi

mesp: cloudsto http
	${CC} -o bin/mesp src/mesp_demo.c src/mesp_api.c bin/cloudsto.o bin/http.o bin/base64.o

datiqi: cloudsto http
	${CC} -o bin/datiqi src/datiqi_demo.c src/datiqi_api.c bin/cloudsto.o bin/http.o bin/base64.o

cloudsto: base64
	${CC} -c -o bin/cloudsto.o src/cloudsto.c

http:
	${CC} -c -o bin/http.o src/http.c

http_win:
	${CC} -c -o bin/http.o src/http_win.c

base64:
	${CC} -c -o bin/base64.o src/base64.c

clean:
	-rm bin/*
