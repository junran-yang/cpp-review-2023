CC=/opt/bb/bin/g++
COMPILE_OPTS=-Wall -Werror -m32 --std=gnu++14 -isystem /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/include -I.
LINK_OPTS=-m32 -L /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/lib -lbsl -lbde

revenue.tsk: licensing.o sales.o sumof.o main.o
	${CC} -o revenue.tsk main.o licensing.o sales.o sumof.o ${LINK_OPTS}

licensing.o: licensing.cpp licensing.h
	${CC} ${COMPILE_OPTS} -c licensing.cpp

sales.o: sales.cpp sales.h
	${CC} ${COMPILE_OPTS} -c sales.cpp

sumof.o: sumof.cpp sumof.h
	${CC} ${COMPILE_OPTS} -c sumof.cpp

main.o: main.cpp licensing.h sales.h
	${CC} ${COMPILE_OPTS} -c main.cpp
