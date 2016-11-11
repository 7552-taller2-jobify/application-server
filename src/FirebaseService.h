#ifndef FIREBASESERVICE_H_
#define FIREBASESERVICE_H_
#include "Request.h"
#include "Response.h"

using namespace std;

class FirebaseService{
private:
	
public:
		FirebaseService();
		Response* SendNotification(string toToken, string body);
		~FirebaseService();
};
#endif
