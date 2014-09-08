CXX = g++
CXXFLAGS = -I. -I./modbus/ -DMARKUP_STL -g
LDLIBS = -leval
HEADERS = dev.h dev_pool.h acqer.h item.h Markup.h system.h \
	   modbus/acqer_modbus.h \
	   modbus/acqer_modbus_serialport.h \
	   modbus/comm.h \
	   modbus/comm_serialport.h \
	   modbus/comm_serialport_linux.h \
	   modbus/item_modbus.h \
	   modbus/serialport_pool.h \
	   modbus/ProtoModbus.h 
OBJS = main.o dev.o dev_pool.o acqer.o item.o Markup.o system.o item_handler.o\
	   modbus/acqer_modbus.o \
	   modbus/acqer_modbus_serialport.o \
	   modbus/comm_serialport_linux.o \
	   modbus/item_modbus.o \
	   modbus/serialport_pool.o \
	   modbus/ProtoModbus.o
EXEC = main

all: ${OBJS} 
	$(CXX) $(CXXFLAGS) $(LDLIBS) $(OBJS) -o $(EXEC)			      
clean:
	rm -f ${EXEC} $(OBJS)      
