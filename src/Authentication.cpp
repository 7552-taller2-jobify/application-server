#include "Authentication.h"
#include <iostream>

/* Constant time to make tests consistent. */
#define TS_CONST	1475980545L

Authentication::Authentication() {
/*	FILE *out;
	jwt_t *jwt = NULL;
	int ret = 0;

	ret = jwt_new(&jwt);
//	ck_assert_int_eq(ret, 0);
    std::cout<<ret<<std::endl;
    std::cout<<jwt<<std::endl;
//	ck_assert(jwt != NULL);

	ret = jwt_add_grant(jwt, "email", body.email);
//	ck_assert_int_eq(ret, 0);
    std::cout<<ret<<std::endl;
	ret = jwt_add_grant(jwt, "sub", "user0");
//	ck_assert_int_eq(ret, 0);
    std::cout<<ret<<std::endl;
	ret = jwt_add_grant(jwt, "ref", "XXXX-YYYY-ZZZZ-AAAA-CCCC");
//	ck_assert_int_eq(ret, 0);
    std::cout<<ret<<std::endl;
	ret = jwt_add_grant_int(jwt, "iat", TS_CONST);
//	ck_assert_int_eq(ret, 0);
    std::cout<<ret<<std::endl;
	out = fopen("salida.txt", "w");
//	ck_assert(out != NULL);
    std::cout<<out<<std::endl;
	ret = jwt_dump_fp(jwt, out, 1);
    std::cout << jwt_dump_str(jwt, 1)<<std::endl;


//	ck_assert_int_eq(ret, 0);
    std::cout<<ret<<std::endl;
	ret = jwt_dump_fp(jwt, out, 0);
    std::cout << jwt_dump_str(jwt, 0)<<std::endl;

//	ck_assert_int_eq(ret, 0);
    std::cout<<ret<<std::endl;

    unsigned char key256[5] = "pass";
    jwt_set_alg(jwt, JWT_ALG_HS256, key256, sizeof(key256));

    std::cout<<jwt_encode_str(jwt)<<std::endl;
    std::cout<<"ALGORITMO : " << jwt_get_alg(jwt)<<std::endl;
//	fclose(out);

    jwt_free(jwt);
*/
}


std::string Authentication::encode(std::string email, std::string password) {
    // Seteo textEncode en grant de jwt, con metodo    jwt_add_grant(jwt, "unGrant", "valor_grant")
//    unsigned char key256[33] = "012345678901234567890123456789XY";
        
    jwt_new(&this->myJWT);
    jwt_set_alg(this->myJWT, JWT_ALG_HS256, (const unsigned char *) SECRET, SECRET_LEN);

    jwt_add_grant(this->myJWT, "mail", email.c_str());
    jwt_add_grant(this->myJWT, "password", password.c_str());
    
    std::string token = jwt_encode_str(this->myJWT);
    jwt_free(this->myJWT);

    return token;
}

bool Authentication::isDecode(std::string token) {
    
    
    int ret = jwt_decode(&this->myJWT, token.c_str(),(const unsigned char *)SECRET, SECRET_LEN);
    
    if (ret == 0){
        std::cout<<"TRUE"<< std::endl;
    } else {
        std::cout<<"FALSE"<< std::endl;
    }
    return ret == 0;
}


Authentication::~Authentication() {
 
}
