PREFIX = /home/dyllonenos

CC = gcc

CSRCS = $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

LIBS = -lusloss4.7 -lphase1 -lphase2

LIB_DIR     = ${PREFIX}/lib
INCLUDE_DIR = ${PREFIX}/include

CFLAGS = -Wall -g -I${INCLUDE_DIR} -I.
LDFLAGS = -Wl,--start-group -L${LIB_DIR} -L. ${LIBS}



VPATH = testcases
TESTS = test00 test01 test02 test03 test04 test05 test06 test07 test08 test09 \
        test10 test11 test12 test13 test14 test15 test16 test17 test18 test19 \
        test20 test21 test22 test23 test24 test25



all: ${TESTS}

${TESTS}: phase3_common_testcase_code.o $(COBJS)

clean:
	-rm *.o ${TESTS} term[0-3].out

