#ifndef _COMM_H_
#define _COMM_H_

#include <string>
/* base communication class,  */
class comm
{
public:
	virtual ~comm(){}
	virtual bool clear() = 0;
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual size_t write(const void *buf, size_t size) = 0;
	virtual size_t read(void *buf, size_t size) = 0;
	/**/
	bool isopen() { return isopen_; }
	std::string name(){ return name_; }
	void settimeout(unsigned int timeout) { timeout_ = timeout; }
protected:
	bool isopen_;
	std::string name_;
	unsigned int timeout_;
private:


};

#endif

