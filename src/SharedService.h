#ifndef SHAREDSERVICE_H_
#define SHAREDSERVICE_H_
#include "Request.h"
#include "Response.h"

using namespace std;

class SharedService{
private:
	
public:
		SharedService();
		Response* CreateCategory(string name, string descripcion);
		~SharedService();
};
#endif
