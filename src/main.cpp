#include <string>
#include "daqtypes.h"
#include "Markup.h"
#include "system.h"
#include "dev.h"
#include "dev_pool.h"
#include "acqer_modbus.h"
#include "acqer_modbus_serialport.h"

using namespace std;

int main()
{
	CMarkup cfg;
	if(!cfg.Load("daq.conf"))
		err_exit("load config file failed");
	while(cfg.FindElem("device"))
	{
		string devid;
		devid = cfg.GetAttrib("	id");
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
				ip = cfg.GetAttrib("ip");
				if(!ip.empty())
				{
//					acqer_modbus_tcp *amcp = new acqer_modbus_tcp;
//					acqer_modbus = acqer_modbus_tcp;
				}
				else if(cfg.HasAttrib("serialport"))
				{
					uint16 port;
					uint32 baudrate;
					uint8 databits;
					uint8 stopbits;
					uint8 flowctl;
					uint8 parity;
					port = str2uint(cfg.GetAttrib("serialport"));
					// baudrate
					if(cfg.HasAttrib("baudrate"))
						baudrate = str2uint(cfg.GetAttrib("baudrate"));
					else
						err_exit("serialport must hav baudrate");
					// databits
					if(cfg.HasAttrib("databits"))
						databits = str2uint(cfg.GetAttrib());
					else
						err_exit("serialport must have databits");
					// stopbits
					if(cfg.HasAttrib("stopbits"))
						stopbits = str2uint(cfg.GetAttr("stopbits"));
					else
						err_exit("serialport must have stopbits");
					// flowctl
					if(cfg.HasAttrib("flowctl"))
						flowctl = str2uint(cfg.GetAttr("flowctl"));
					else
						err_exit("serialport must have flowctl");

					//create acqer
					acqer_modbus_serialport *amsp = new acqer_modbus_serialport(port, baudrate, databits, stopbits, flowctl, parity);
					acqer_modbus = acqer_modbus_serialport;
					devp->add_acqer(acqer_modbus);
					// items
					cfg.IntoElem();
					while(cfg.FindElem("item"))
					{
						string itemid;
						string itemname;
						unsigned int itemcycle;
						uint8 funcode;
						uint8 startaddr;
						uint8 count;
						uint8 offset;
						string expr;
						// create item
						item_modbus *imp = NULL;
						if(cfg.HasAttrib("id"))
							itemid = cfg.GetAttrib("id");
						else
							err_exit("item must have id");
						/* name */
						if(cfg.HasAttrib("name"))
							itemname = cfg.GetAttrib("name");
						/* cycle */
						if(cfg.HasAttrib("cycle"))
							itemcycle = str2uint(cfg.GetAttrib("cycle"));
						else 
							err_exit("item must have cycle");
						/* expression */
						if(cfg.HasAttrib("expr"))
							expr = cfg.GetAttrib("expr");
						/* modbus items */
						if(cfg.HasAttrib("funcode"))
						{
							funcode = str2uint(cfg.GetAttrib("funcode"));
							/* startaddr */
							if(cfg.HasAttrib("startaddr"))
								startaddr = str2uint(cfg.GetAttrib("startaddr"));
							else
								err_exit("modbus item must have startaddr");
							// count
							if(cfg.HasAttrib("count"))
								count = str2uint(cfg.GetAttrib("count"));
							else
								err_exit("modbus item must have count");
							// offset
							if(cfg.HasAttrib("offset"))
								offset = str2uint(cfg.GetAttrib("offset"));
							else 
								offset = 0;
							imp = new item_modbus(itemid, itemname, itemcycle, expr, false, funcode, startaddr, offset, count);
						}
						else
						{
							imp = new item_modbus(itemid, itemname, itemcycle, expr);
						}
						// add an item to acqer
						amp->add_item(imp);
					}
					cfg.OutOfElem();
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

	return 0;
}

