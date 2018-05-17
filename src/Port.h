#ifndef PORT_H
#define PORT_H

#include <string>

#include "HidPort.h"
#include "Win32/Serial.h"

class Port
{
protected:
	Port() { };
public:
	virtual ~Port() { };
	virtual bool IsOpen() = 0;
	virtual bool CanRead() = 0;
	virtual int ReceiveBytes(void *data, size_t size) = 0;
	inline bool ReceiveByte(char &byte)
		{ return(ReceiveBytes(&byte, 1) > 0); }
	virtual bool Send(void *data, size_t size) = 0;
	inline bool Send(unsigned char byte)
		{ return(Send(&byte, 1)); }
};

class Hid : public Port
{
public:
	Hid(int vendorId, int productId, int serial = 0)
		{ HidPortOpen(&port, vendorId, productId, serial); }
	virtual ~Hid()
		{ HidPortClose(port); }
	virtual bool IsOpen()
		{ return(port != NULL); };
	virtual int ReceiveBytes(void *data, size_t size)
		{ return(HidPortReceiveBytes(port, (unsigned char *) data, size)); }	
	virtual bool Send(void *data, size_t size)
		{ return(HidPortSendBytes(port, (unsigned char *) data, size) == 0); }	
	virtual bool CanRead()
		{ return(HidPortCanRead(port) > 0); }
private:
	struct HidPort *port;
};

class Serial : public Port
{
public:
	Serial(const std::string &path, int baud)
		{ NativeSerialOpen(&port, path, baud); }
	virtual ~Serial()
		{ NativeSerialClose(&port); }
	virtual bool IsOpen()
		{ return(NativeSerialIsOpen(port)); }
	virtual int ReceiveBytes(void *data, size_t size)
		{ return(NativeSerialReceiveBytes(port, (unsigned char *) data, size)); }
	virtual bool Send(void *data, size_t size)
		{ return(NativeSerialSend(port, (unsigned char *) data, size)); }
	virtual bool CanRead()
		{ return(NativeSerialGetAvailableBytes(port) > 0); }

private:
	NativeSerial port;
};



#endif /* PORT_H */