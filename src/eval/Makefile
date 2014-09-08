CXX=g++
CXXFLAGS=-DNDEBUG
AR =ar
LIBMISC = lib/libeval.a
OBJS = eval.o operator_factory.o
     
all : $(OBJS)
	$(AR) rv $(LIBMISC) $?
		      
clean :
	rm -v $(LIBMISC) $(OBJS) 
