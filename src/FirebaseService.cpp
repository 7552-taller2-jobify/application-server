#include "FirebaseService.h"

using namespace std;

FirebaseService::FirebaseService(){
    
}
Response* FirebaseService::SendNotification(string toToken, string notification){
	std::string url = "https://fcm.googleapis.com/fcm/send";
	//estructura de notificacion {\n\t\t\"body\": \"Hola!!\",\n\t\t\"title\": \"Jobify\"\n\t}
    std::string body = "{\n\t\"to\": \""+ toToken + "\",\n\t\"notification\": " + notification + "\n}";
    std::cout<< body <<std::endl;
    Request* request = new Request();

    //Firebase->mensajeria en la nube -> Clave del servidor
    //key=AIzaSyAkvyuxBXTzeLiz9wkE2WrAsfyk0ylkQOk
    request->SetAuthorization("key=AIzaSyAkvyuxBXTzeLiz9wkE2WrAsfyk0ylkQOk");
    request->SetContentJson();


    Response* firebaseResponse = request->ExecutePost( url , body);

    Response* response = new Response();
        response->setContent(firebaseResponse->getContent());
        response->setStatus(200);  
    return response;
}

FirebaseService::~FirebaseService(){
	
}
