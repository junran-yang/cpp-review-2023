CC=/usr/bin/g++
COMPILE_OPTS=-Wall -Werror --std=gnu++14 -I.
LINK_OPTS=

revenue: licensing.o sales.o sumof.o main.o
	${CC} -o revenue main.o licensing.o sales.o sumof.o ${LINK_OPTS}

licensing.o: licensing.cpp licensing.h
	${CC} ${COMPILE_OPTS} -c licensing.cpp

sales.o: sales.cpp sales.h
	${CC} ${COMPILE_OPTS} -c sales.cpp

sumof.o: sumof.cpp sumof.h
	${CC} ${COMPILE_OPTS} -c sumof.cpp

main.o: main.cpp licensing.h sales.h
	${CC} ${COMPILE_OPTS} -c main.cpp
