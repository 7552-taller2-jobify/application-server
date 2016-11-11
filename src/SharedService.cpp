#include "SharedService.h"

using namespace std;

SharedService::SharedService(){
    
}
Response* SharedService::CreateCategory(string name, string description){

    Request* request = new Request();
    Response* responsePost = request->ExecutePost("https://jobify-7552-taller2.herokuapp.com/categories","{ \"name\": \"" + name +"\",\"description\": \"" + description + "\"}");
    

    Response* response = new Response();
        response->setContent(responsePost->getContent());
        response->setStatus(200);  
    return response;
}

SharedService::~SharedService(){
	
}
