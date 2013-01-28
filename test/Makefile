all: mesp datiqi

mesp: mesp.c cloudsto
	gcc -g -O2 -o mesp mesp.c cloudsto.o

datiqi: datiqi.c datiqi_api.c cloudsto
	gcc -g -O2 -o datiqi datiqi.c datiqi_api.c cloudsto.o

cloudsto: cloudsto.c http.c
	gcc -g -O2 -c -o cloudsto.o cloudsto.c http.c

clean:
	-rm mesp datiqi cloudsto.o
