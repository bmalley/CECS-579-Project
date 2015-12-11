//Brian Malley User Authentication Library.




// OpenSSL headers
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <iostream>
#include <time.h>

using namespace std;

void GenAuthCode(char*, long);
int VerifyVerifCode(char*, long);


int main()
{
//Initialize Auth Code variables
long UniqueID = 3816005;
long VerificationID = 5006183;
byte aes_key[16];
SecByteBlock aes_IV(16);

// Initialize OpenSSL
SSL_library_init();
SSL_load_error_strings();
ERR_load_BIO_strings();
OpenSSL_add_all_algorithms();

//initialize variables
BIO* bio;
char buffer[9];
GenAuthCode(buffer, uniqueID);
SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_client_method());
SSL* ssl;

//load trust store
if(! SSL_CTX_load_verify_locations(ctx, "/home/rick/Desktop/579/server.pem", NULL))
{
    cout<<"FAILED TO LOAD TRUST STORE \n";
}

//set up secure connection
bio = BIO_new_ssl_connect(ctx);
BIO_get_ssl(bio, &ssl);
SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);


//open up secure connection
BIO_set_conn_hostname(bio, "localhost:23962");
if (bio == NULL)
{
	cout<<"CONNECTION ERROR: NULL POINTER \n";
}
else if (BIO_do_connect(bio) <= 0)
{
	ERR_print_errors_fp(stderr);
	cout<<"CONNECTION ERROR: CONNECTION FAILED \n";
}
//verify the SSL
if(SSL_get_verify_result(ssl) != X509_V_OK)
{
   	cout<<"SSL NOT VERIFIED\n";
}
cout<<"Connection Established. Sending data . . .\n";
//send data

//******** ENCRYPTION THEN HMAC *************
//generate AES IV

//Encrypt with AES 128 with key + IV

//Concatenate IV with ciphertext

//HMAC <SHA256> the concatenated ciphertext with key

// concatenate the ciphertext and MAC

//send the result


BIO_write(bio, buffer, 20);

cout<<"Data sent. \n";


//wait for a response

//******** HMAC then DECRYPT *************
//Check MAC integrity

//Decrypt with AES 128 with key + IV (given)

//check that the result is valid
VerifyVerifCode(char*, VerificationID);

//begin execution of program if valid




BIO_free(bio);

return 0;
}








void GenAuthCode(char* bytes, long ID)
{
//authentication code is composed of unique ID, timestamp, and direction bit concatenated into 9 chars.
//bytes must be an array of size 9

//ID
bytes[0] = (ID >> 24) & 0xFF;
bytes[1] = (ID >> 16) & 0xFF;
bytes[2] = (ID >> 8) & 0xFF;
bytes[3] = ID & 0xFF;


//TimeStamp
time_t T = time();

bytes[4] = (T >> 24) & 0xFF;
bytes[5] = (T >> 16) & 0xFF;
bytes[6] = (T >> 8) & 0xFF;
bytes[7] = T & 0xFF;

//Direction Bit
bytes[8] = 0x01;


}



int VerifyVerifCode(char* bytes, long goodID)
{
	//verify the ID code, timestamp, and direction bit
	long ID = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
	time_t T = (bytes[4] << 24) + (bytes[5] << 16) + (bytes[6] << 8) + bytes[7]

	if (goodID != ID)
	{	
		cout<<"wrong ID\n";
		return 0;
	}
	else if ( (T >= (time() + 5)) || (T <= (time() - 5)) )
	{
		cout<<"wrong time\n";
		return 0;
	}
	else if (bytes[8] != 0x00)
	{
		cout<<"wrong direction bit\n";
		return 0;
	}
	else
	{
		return 1;
	}

}


























