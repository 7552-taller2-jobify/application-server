// "Copyright 2016 <Jobify>"
#include <string>
#include "FirebaseService.h"

FirebaseService::FirebaseService() {
}

Response* FirebaseService::SendNotification(std::string toToken, std::string notification) {
    std::string url = "https://fcm.googleapis.com/fcm/send";
    //  estructura de notificacion {\n\t\t\"body\": \"Hola!!\",\n\t\t\"title\": \"Jobify\"\n\t}
    std::string body = "{\n\t\"to\": \""+ toToken + "\",\n\t\"notification\": " + notification + "\n}";
    std::cout<< body << std::endl;
    Request* request = new Request();

    //  Firebase->mensajeria en la nube -> Clave del servidor
    //  key=AIzaSyAkvyuxBXTzeLiz9wkE2WrAsfyk0ylkQOk
    request->SetAuthorization("key=AIzaSyAkvyuxBXTzeLiz9wkE2WrAsfyk0ylkQOk");
    request->SetContentJson();

    Response* firebaseResponse = request->ExecutePost(url, body);

    Response* response = new Response();
        response->setContent(firebaseResponse->getContent());
        response->setStatus(200);
    return response;
}

FirebaseService::~FirebaseService() {
}
