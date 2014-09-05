CXX = g++
CXXFLAGS = -I. -I./modbus/ -DMARKUP_STL
LDLIBS = -lexpr
OBJS = dev.o dev_pool.o acqer.o item.o Markup.o system.o \
	   modbus/acqer_modbus.o\
	   modbus/acqer_modbus_serialport.o\
	   modbus/comm_serialport.o\
	   modbus/comm_serialport_linux.o\
	   modbus/item_modbus.o\
	   modbus/serialport_pool.o\
	   modbus/ProtoModbus.o
EXEC = main
all: ${EXEC}
			      
clean:
	rm -f ${EXEC} *.o      
