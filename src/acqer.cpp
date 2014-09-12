#include <assert.h>
#include <eval.h>
#include <stdio.h>
#include "acqer.h"
#include "system.h"

using namespace std;

acqer::acqer()
	: acqtime_ (0x7fffffff)
{
}

acqer::~acqer()
{
	for(item_list::iterator it = items_.begin(); it != items_.end(); ++it)
	{
		delete (*it);
	}
}


void acqer::add_item(item *ip)
{
	items_.push_back(ip);
	iditem_[ip->id_] = ip;
}

void acqer::acq_once()
{
	simple_acq();
	calc_items();
	update_item_acqtimes();
}

bool acqer::is_acqtime(item *ip)
{
	return (ip->latime_ + ip->cycle_ <= acqtime_);
}

void acqer::update_item_acqtimes()
{
	for(item_list::iterator iti = items_.begin(); iti != items_.end(); ++iti)
	{
		if(is_acqtime(*iti))
			(*iti)->latime_ = acqtime_;
	}
}

// tconfig==true: test configuration
bool acqer::calc_items()
{
	for(item_list::iterator iti = items_.begin(); iti != items_.end(); ++iti)
	{
		item *ip = *iti;
		itemval_t *iv = &(ip->value_);
		if(iv->ivt_ != IVT_STRING && (!ip->expr_.empty()) && is_acqtime(ip))
		{
			string rexpr = ip->expr_;
			double dval;
			bool deperr;
			if(!replace_var(rexpr, &deperr))
				return false;
			if(!eval(rexpr, &dval))
				return false;
			// the depend item acquisition failed, so this item is also failed
			if(deperr)
			{
				printf("deperr\n");
				ip->status_ = IS_FAILED;
				continue;
			}
			switch(iv->ivt_)
			{
				case IVT_INTEGER:
					iv->val_.ival = (int)dval;
					break;
				case IVT_FLOAT:
					iv->val_.fval = dval;
					break;
				default:
					assert(false);
					return false;
			}
			ip->status_ = IS_UPDATED;
		}
	}
	return true;
}

// return false  means wrong expression
bool acqer::replace_var(string &expr, bool *deperr)
{
	size_t varbeg, varend;
	int itemid;
	string valstr;
	itemval_t *valp;
	item_map::iterator itm;
	*deperr = false;
	while(1)
	{
		if(!find_var(expr, varbeg, varend, &itemid))
			return false;
		if(string::npos == varbeg) // find no var
			break;
		itm = iditem_.find(itemid);
		if(iditem_.end() == itm)
			return false;
		valp = &(itm->second->value_);
		// the depend item acquisition failed, so this item is also failed
		if(itm->second->status_ == IS_FAILED)
		{
			*deperr = true;
			return true;
		}
		if(IVT_STRING == valp->ivt_)
			return false;
		if(IVT_INTEGER == valp->ivt_)
			valstr = tostr(valp->val_.ival);
		if(IVT_FLOAT == valp->ivt_)
			valstr = tostr(valp->val_.fval);
		expr.replace(varbeg, varend-varbeg, valstr);
	}
	return true;
}

// return false  means wrong expression
bool acqer::find_var(const std::string &expr, size_t &varbeg, size_t &varend, int *id)
{
	varbeg = expr.find('$');
	if(string::npos == varbeg)
		return true; // only wrong expression, return false
	varend = varbeg+1;
	while(varend != string::npos && isdigit(expr[varend]))
	{
		++varend;	
	}

	//
	string idstr(expr, varbeg+1, varend-varbeg-1);
	if(idstr.empty())
		return false;// wrong expression
	*id = str2int(idstr);
	return true;
}
