#ifndef __DAQ_TYPES_H__
#define __DAQ_TYPES_H__ 

#include <string>
#include <vector>

/* module command types. the master process exchange module command  data with child acq process . */
enum MODULECMD
{
	MCMD_ITEM_VALUE,
	MCMD_ACTIVE,
	MCMD_ACQ_RULE
};

enum DATATYPE
{
	DT_INTEGER,
	DT_FLOAT,
	DT_STRING
};

/* item value send to master process */
struct mc_item_value
{
	MODULECMD cmd_;
	std::string itemid_;
	std::string value_;

	template<class Archive>
	void serialize(Archive & ar)	
	{
		ar & cmd_;
		ar & itemid_;
		ar & value_;
	}
};

/* active data to identify dead process. */
struct mc_active
{
	MODULECMD cmd_;

	template<class Archive>
	void serialize(Archive & ar)	
	{
		ar & cmd_;
	}
};

struct acq_rule
{
	std::string itemid_;
	std::string max_;
	std::string min_;
	int cycle_;

	template<class Archive>
	void serialize(Archive & ar)	
	{
		ar & itemid_;
		ar & max_;
		ar & min_;
		ar & cycle_;
	}
}

struct mc_acq_rule
{
	MODULECMD cmd_;
	std::vector<acq_rule> rules_;

	template<class Archive>
	void serialize(Archive & ar)
	{
		ar & rules_;
	}
};



#endif

