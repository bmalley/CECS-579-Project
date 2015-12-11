//Brian Malley Server Authentication Library.




// OpenSSL headers
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <iostream>
#include <time.h>

using namespace std;

int VerifyAuthCode(char*, long);
void GenVerifCode(char*, long);




int main()
{
//Initialize Auth Code variables
long AcceptableID = 3816005;
long VerificationID = 5006183;

// Initialize OpenSSL
SSL_library_init();
SSL_load_error_strings();
ERR_load_BIO_strings();
OpenSSL_add_all_algorithms();


//initialize variables
string plaintext, ciphertext
BIO* bio;
BIO* acpt;
BIO* tempbio; 		//for responding
char buffer[9]; 	//for messages
int counter = 0;	//for connection number
SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_server_method());
if(!ctx) {
    ERR_print_errors_fp(stderr);
    //throw or return an error 
}
SSL* ssl;

//load trust store
if(! SSL_CTX_load_verify_locations(ctx, "/home/rick/Desktop/579/server.pem", NULL))
{
    cout<<"FAILED TO LOAD TRUST STORE \n";
}

//set up secure server
bio = BIO_new_ssl(ctx,0);
BIO_get_ssl(bio, &ssl);
SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

if (!ssl)
{
	cout<<"ERROR GETTING SSL POINTER\n";
	return 0;
}



//create buffer BIO
tempbio = BIO_new(BIO_f_buffer());

//add to chain
bio = BIO_push(tempbio, bio);



// While loop to respond to incoming connections

while (1)
{
	//Set up accept port
	acpt=BIO_new_accept("localhost:23962");
	BIO_set_accept_bios(acpt,bio);
	if (BIO_do_accept(acpt) <= 0)
	{
		cout<<"ERROR SETTING UP ACCEPT PORT \n";
		return 0;
	}
	counter++;

	//wait for connection
	if (BIO_do_accept(acpt) <= 0)
	{
		cout<<"ERROR ACCEPTING CONNECTION \n";
		return 0;
	}
	cout<<"Connection "<<counter<<" Established \n";

	bio = BIO_pop(acpt);
	BIO_free_all(acpt);


	//attempt SSL handshake
	if (BIO_do_handshake(bio) <= 0)
	{
		cout<<"ERROR WITH SSL HANDSHAKE\n";
  		ERR_print_errors_fp(stderr);
		return 0;
	}
	else
	{
		cout<<"SSL Connection "<<counter<<" Established \n";
		// read information from connection
		BIO_read(bio, buffer, 20);
		buffer[20] = '\0';

		//******** HMAC then DECRYPT *************
		//Check MAC integrity

		//Decrypt with AES 128 with key + IV (given)

		//check that the result is valid
		VerifyAuthCode(char*, AcceptableID);

		//Create verification Code if valid:
		//******** ENCRYPTION THEN HMAC *************
		//generate AES IV

		//Encrypt with AES 128 with key + IV

		//Concatenate IV with ciphertext

		//HMAC <SHA256> the concatenated ciphertext with key

		// concatenate the ciphertext and MAC

		//send the result
	}
	
}


return 0;
}


int VerifyAuthCode(char* bytes, long goodID)
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
	else if (bytes[8] != 0x01)
	{
		cout<<"wrong direction bit\n";
		return 0;
	}
	else
	{
		return 1;
	}

}

void GenVerifCode(char* bytes, long ID)
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
bytes[8] = 0x00;


}























