include ../../../moriiism/mitool/Makefile.in
include ../../../moriiism/mitool/milib/Makefile.in
include ../../../moriiism/mitool/miblaslib/Makefile.in
include ../../../moriiism/mitool/mifitslib/Makefile.in
include ../../../moriiism/mitool/mirootlib/Makefile.in
include ../../../moriiism/mitool/mifunclib/Makefile.in

INCLUDE := $(MIFUNCLIB_INC) $(MIROOTLIB_INC) $(MIFITSLIB_INC) $(MIBLASLIB_INC) $(MILIB_INC) \
	$(CFITSIO_INC) $(ROOT_INC) $(MT_INC)

TARGET := libhsclib.a

OBJ := aucboost.o  hscdata.o  \
	sub_step.o  sub_step0.o  sub_step1.o  sub_step2.o  sub_step3.o  sub_step4.o

all: $(TARGET)

libhsclib.a: ${OBJ}
	$(AR) -r $@ $(OBJ)

libhsclib.so: ${OBJ}
	$(CC) -shared -g -O2 -o $@ ${OBJ}

.SUFFIXES:
.SUFFIXES: .o .c .f .F .cc

.c.o:
	$(CCN) -c $<

.F.o : 
	$(FCN) -c $<

.f.o:
	$(FCN) -c $<

.cc.o:
	${CXXN} -c $<

clean:
	-rm -f *.o *.a *.so

cleaner: clean
	-rm -f ${TARGET} *% *~ core
