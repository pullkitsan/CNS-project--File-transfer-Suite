#include<iostream>
#include<ctype.h>
#include<fstream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/uio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/syscall.h>
#include<string.h>
#include<stdio.h>
#include<openssl/evp.h>
#include<bits/stdc++.h>
#include<iomanip>
#include<stdlib.h>
#include<errno.h>
#include<openssl/err.h>
#include<openssl/aes.h>
#include<openssl/rand.h>
#include<arpa/inet.h>

using namespace std;

void file_encrypt( unsigned char *EncK,unsigned char *initv, FILE *ifp, FILE *ofp)
{
    // creating an output buffer of sufficient length
    unsigned char ibuf[1024], obuf[2048];

    int bytes, olen;
    EVP_CIPHER_CTX *ctx;

    ctx = EVP_CIPHER_CTX_new();
    

    EVP_CipherInit_ex(ctx,EVP_aes_256_cbc(), NULL, NULL, NULL, 1);


    //Passing the value of IV and Key
    EVP_CipherInit_ex(ctx,NULL, NULL, EncK, initv,1);
    

    while(1){
        // Data is read till EOF is reached .
        bytes = fread(ibuf, sizeof(unsigned char), 1024, ifp);
        
        EVP_CipherUpdate(ctx, obuf, &olen, ibuf, bytes);
        fwrite(obuf, sizeof(unsigned char), olen, ofp);
        
        if (bytes < 1024) {
            /* Reached End of file */
            break;
        }
    }

    /* Now cipher the final block and write it out to file */
    EVP_CipherFinal_ex(ctx, obuf, &olen);
    fwrite(obuf, sizeof(unsigned char), olen, ofp);
    
}

int main(int argc, char **argv)
{
 //PBKDF2 implementation START
 
    size_t i;
    unsigned char *KEY;
    printf("Enter the password :");
    char pass[100];
    scanf("%s", pass);
    unsigned char salt[] = {'k','c','l'};

    KEY = (unsigned char *) malloc(sizeof(unsigned char) * 32);

    if( PKCS5_PBKDF2_HMAC_SHA1(pass, strlen(pass), salt, sizeof(salt), 4096, 32, KEY) != 0 )
    {
      printf("KEY: "); 
      for(i=0;i<32;i++)
        { printf("%02x",toupper(KEY[i]));
          printf(" "); 
     
        } 
    printf("\n");
    }

 //PBKDF2 implementation END

 //Encryption Starts

FILE *f_input, *f_enc, *f_dec;


    unsigned char *EncK=(unsigned char*)"12345678900987654321123456789009";

    unsigned char *initv=(unsigned char*)"1324354657687980";
 
     
    /* Open the input file for reading in binary ("rb" mode) */
    f_input = fopen(argv[1], "rb");

    /* Open and truncate file to zero length or create ciphertext file for writing */
    f_enc = fopen(strcat(argv[1],".uf"), "wb");
    //char buf[255];
    //fread(buf, sizeof(buf),1,f_enc);// read the data to buffer
    /* Encrypt the given file */
    file_encrypt(EncK,initv, f_input, f_enc);

    /* Encryption done, close the file descriptors */
    fclose(f_input);
    

// Socket programming 
    
int sock,pno,n;
struct sockaddr_in serva;
char bfr[255];

sock=socket(AF_INET,SOCK_STREAM,0);

if(sock<0)
printf("Socket cannot be opened");
else
printf("Socket is opened \n");

pno=atoi(argv[3]);

inet_pton(AF_INET,argv[2],&serva.sin_addr);
serva.sin_port=htons(pno);

 serva.sin_family=AF_INET;

if(connect(sock,(struct sockaddr *)&serva,sizeof(serva))<0)

{printf("connection has failed");
 }

else
printf("connection is succesful");
 
FILE *sip;
int count=0 ;
char cnt;

//sip=fopen(buf,"rb");
while((cnt=getc(f_enc)!= EOF))
{ 
 fscanf(f_enc,"%s",bfr);
 count++;
}

write(sock,&count,sizeof(int));
rewind(sip);

char file_w;
while(file_w != EOF)
{
 fscanf(sip,"%s",bfr);
 write(sock , bfr,255);
 file_w=fgetc(sip);
}

printf("Sending the file to : %s %s",argv[2],argv[3]);
printf("\n");
return 0;

}




  
