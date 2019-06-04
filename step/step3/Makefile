include ../../../../moriiism/mitool/Makefile.in
include ../../../../moriiism/mitool/milib/Makefile.in
include ../../../../moriiism/mitool/miblaslib/Makefile.in
include ../../../../moriiism/mitool/mirootlib/Makefile.in
include ../../../../moriiism/mitool/mifunclib/Makefile.in

include ../../Makefile.in

INCLUDE = $(HSC_INC) $(MIFUNCLIB_INC) $(MIROOTLIB_INC) $(MIFITSLIB_INC) $(MIBLASLIB_INC) $(MILIB_INC) $(ROOT_INC) $(MT_INC)
LDLIBS  = $(HSC_LIB) $(MIFUNCLIB_LIB) $(MIROOTLIB_LIB) $(MIFITSLIB_LIB) $(MIBLASLIB_LIB) $(MILIB_LIB) $(ROOT_ALLLIBS) \
	$(LAPACK_LIB) $(OPENBLAS_LIB) $(MT_LIB) $(LIB) -lgfortran

#	$(LAPACK_LIB) $(BLAS_LIB) $(MT_LIB) $(LIB) -lgfortran



OBJ = 

TARGET = step3

all: $(TARGET) 

step3: step3.o  arg_step3.o
	$(CXXN) $@.o arg_$@.o ${LDLIBS} -o $@

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

install: all
	if [ ! -d $(INST_BIN_DIR) ]; then mkdir $(INST_BIN_DIR); fi
	install ${TARGET} ${INST_BIN_DIR}

clean:
	-rm -f *.o

cleaner: clean
	-rm -f ${TARGET} *% *~ core
