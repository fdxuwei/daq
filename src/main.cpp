#include <string>
#include <stdio.h>
#include "daqtypes.h"
#include "Markup.h"
#include "system.h"
#include "dev.h"
#include "dev_pool.h"
#include "acqer_modbus.h"
#include "acqer_modbus_serialport.h"
#include "serialport_pool.h"

using namespace std;

bool run = true;

int main()
{
	CMarkup cfg;
	if(!cfg.Load("daq.conf"))
		err_exit("load config file failed");
	while(cfg.FindElem("device"))
	{
		string devid;
		devid = cfg.GetAttrib("id");
		if(devid.empty())
			err_exit("device must have an id");
		// add device
		dev *devp = new dev(devid);
		dev_pool::instance().add_dev(devp);
		//
		cfg.IntoElem();
		while(cfg.FindElem("acqer"))
		{
			string proto;
			proto = cfg.GetAttrib("proto");
			if(proto.empty())
				err_exit("acqer must have a proto");

			if(proto == "modbus")
			{
				acqer_modbus *amp = NULL;
				string ip;
				string serialportstr;
				string devaddrstr;
				uint8 devaddr;
				devaddrstr = cfg.GetAttrib("devaddr");
				if(devaddrstr.empty())
					err_exit("modbus acqer must have devaddr");
				devaddr = str2uint(devaddrstr);
				ip = cfg.GetAttrib("ip");
				if(!ip.empty())
				{
//					acqer_modbus_tcp *amcp = new acqer_modbus_tcp;
//					acqer_modbus = acqer_modbus_tcp;
				}
				else if(!((serialportstr = cfg.GetAttrib("serialport")).empty()))
				{
					uint16 port;
					uint32 baudrate;
					uint8 databits;
					uint8 stopbits;
					uint8 flowctl;
					uint8 parity;
					string stemp;
					port = str2uint(serialportstr);
					// baudrate
					stemp = cfg.GetAttrib("baudrate");
					if(stemp.empty())
						err_exit("serialport must hav baudrate");
					baudrate = str2uint(stemp);
					// databits
					stemp = cfg.GetAttrib("databits");
					if(stemp.empty())
						err_exit("serialport must have databits");
					databits = str2uint(stemp);
					// stopbits
					stemp = cfg.GetAttrib("stopbits");
					if(stemp.empty())
						err_exit("serialport must have stopbits");
					stopbits = str2uint(stemp);
					// flowctl
					stemp = cfg.GetAttrib("flowctl");
					if(stemp.empty())
						err_exit("serialport must have flowctl");
					flowctl = str2uint(stemp);

					//create acqer
					acqer_modbus_serialport *amsp = new acqer_modbus_serialport(devaddr, port, baudrate, databits, stopbits, flowctl, parity);
					amp = amsp;
					devp->add_acqer(amsp);
					serialport_pool::instance().add_serialport(port);
					// items
					cfg.IntoElem();
					while(cfg.FindElem("item"))
					{
						int itemid;
						string itemname;
						unsigned int itemcycle;
						uint8 funcode;
						uint8 startaddr;
						uint8 count;
						uint8 offset;
						string expr;
						string stemp;
						string valtypestr;
						ITEM_VALUE_TYPE ivt;
						// create item
						item_modbus *imp = NULL;
						stemp = cfg.GetAttrib("id");
						if(stemp.empty())
							err_exit("item must have id");
						itemid = str2int(stemp);
						/* name */
						itemname = cfg.GetAttrib("name");
						/* cycle */
						stemp = cfg.GetAttrib("cycle");
						if(stemp.empty())
							err_exit("item must have cycle");
						itemcycle = str2uint(stemp);
						/* expression */
						expr = cfg.GetAttrib("expr");
						/* valuetype */
						valtypestr = cfg.GetAttrib("valtype");
						if(valtypestr.empty())
							valtypestr = "integer";
						if(valtypestr == "float")	
							ivt = IVT_FLOAT;
						else if(valtypestr == "string")	
							ivt = IVT_STRING;
						else
							ivt = IVT_INTEGER;
						/* modbus items */
						stemp = cfg.GetAttrib("funcode");
						if(!stemp.empty())
						{
							funcode = str2uint(cfg.GetAttrib("funcode"));
							/* startaddr */
							stemp = cfg.GetAttrib("startaddr");
							if(stemp.empty())
								err_exit("modbus item must have startaddr");
							startaddr = str2uint(stemp);
							// count
							stemp = cfg.GetAttrib("count");
							if(stemp.empty())
								err_exit("modbus item must have count");
							count = str2uint(stemp);
							// offset
							stemp = cfg.GetAttrib("offset");
							offset = str2uint(stemp);
							imp = new item_modbus(itemid, itemname, itemcycle, expr, ivt, false, funcode, startaddr, offset, count);
						}
						else
						{
							imp = new item_modbus(itemid, itemname, itemcycle, expr, ivt);
						}
						// add an item to acqer
						amp->add_item(imp);
					} // cfg.Find("item")
					cfg.OutOfElem();

					//test expression configuration
					if(!amp->calc_items())
						err_exit("wrong expression config");
				}
				else
				{
					err_exit("modbus acqer must have an ip or com");
				}
			}// modbus
			else
			{
				err_exit("protocol %s not supported",  proto.c_str());
			}
		}
		cfg.OutOfElem();
	}
	// acq loop
	while(run)
	{
		dev_pool::instance().acq_once();
		dev_pool::instance().handle_item();
		sleep(1);
	}

	err_exit("exited");
	return 0;
}

