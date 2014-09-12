CXX = g++
CXXFLAGS = -I. -I./modbus/ -DMARKUP_STL -g
LDLIBS = -leval
OBJS = main.o dev.o dev_pool.o acqer.o item.o Markup.o system.o item_handler.o\
	   modbus/acqer_modbus.o \
	   modbus/acqer_modbus_serialport.o \
	   modbus/comm_serialport_linux.o \
	   modbus/item_modbus.o \
	   modbus/serialport_pool.o \
	   modbus/ProtoModbus.o
EXEC = main

all: ${OBJS} 
	$(CXX) $(OBJS)  $(CXXFLAGS) $(LDLIBS) -o $(EXEC)			      

modbus/acqer_modbus.o: acqer.h modbus/acqer_modbus.h
modbus/acqer_modbus_serialport.o: acqer.h modbus/acqer_modbus.h modbus/acqer_modbus_serialport.h
modbus/item_modbus.o: item.h modbus/item_modbus.h
modbus/comm_serialport_linux.o: comm.h modbus/comm_serialport.h modbus/comm_serialport_linux.h

clean:
	rm -f ${EXEC} $(OBJS)      
