// "Copyright 2016 <Jobify>"
#include <string>
#include "FirebaseService.h"

FirebaseService::FirebaseService() {
}

Response* FirebaseService::SendNotification(std::string toToken, std::string title, std::string message) {
    std::string url = "https://fcm.googleapis.com/fcm/send";
    std::string body = "{\n\t\"to\" : \""+ toToken + "\",\n\t\"notification\" : {\n\t\t\"body\" : \""
                + message + "\",\n\t\t\"title\" : \"" + title + "\",\n\t\t\"sound\" : \"mySound\"\n\t}\n}";
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

FirebaseService::~FirebaseService() {}
