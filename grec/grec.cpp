#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/syscall.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/uio.h>


void file_decrypt( unsigned char *EncK,unsigned char *initv, FILE *ifp, FILE *ofp)
{
    //Creating the buffer of sufficient length to accomodate the output
    unsigned char ibuf[1024], obuf[2048];

    int bytes, olen;
    EVP_CIPHER_CTX *ctx;

    ctx = EVP_CIPHER_CTX_new();
    
    EVP_CipherInit_ex(ctx,EVP_aes_256_cbc(), NULL, NULL, NULL,0);


    // pass the encryption key and initialization vector.
    EVP_CipherInit_ex(ctx,NULL, NULL, EncK, initv,0);
    

    while(1){
      //Read till end of file is reached. 
 
        bytes = fread(ibuf, sizeof(unsigned char), 1024, ifp);
        
        EVP_CipherUpdate(ctx, obuf, &olen, ibuf, bytes);
        fwrite(obuf, sizeof(unsigned char), olen, ofp);
        
        if (bytes < 1024) {
            //End of the file has been reached.
            break;
        }
    }

    //final block is deciphered and written to the block
    EVP_CipherFinal_ex(ctx, obuf, &olen);
    fwrite(obuf, sizeof(unsigned char), olen, ofp);
    
}

int main(int argc, char **argv) 
{
// initializating the variables    
int sock, nsoc,port,n;
struct sockaddr_in serva,clia;
socklen_t clength;

char buf[255];

sock=socket(AF_INET,SOCK_STREAM,0);

if(sock<0)
{
printf("Socket cannot be Created \n");
}

bzero((char *)&serva,sizeof(serva));
port=atoi(argv[1]);
serva.sin_family=AF_INET;
serva.sin_port=htons(port);
serva.sin_addr.s_addr=INADDR_ANY;

//binding the socket
if(bind(sock,(struct sockaddr *)&serva,sizeof(serva)) < 0)
printf("Binding failed");
listen(sock,5);
clength=sizeof(clia);

nsoc=accept(sock,(struct sockaddr *)&clia, &clength);
if(nsoc<0)
printf("Error on accept");

FILE *fi;
int wl=0;
//opening the output file
fi=fopen("hello_rec.txt","a");
int words;
read(nsoc, &words,sizeof(int));

while(wl != words)
{ 
 read(nsoc,buf,255);
 fprintf(fi,"%s ",buf);
 wl++;
}

printf("File has been received Succesfully");
close(sock);
close(nsoc);

//Starting the decryption
FILE *f_input , *f_dec;

    unsigned char *EncK=(unsigned char*)"12345678900987654321123456789009";

    unsigned char *initv=(unsigned char*)"1324354657687980";

    
 
    //opening the input file to read in binary mode
    f_input = fopen(buf, "rb");

    //create new file for keeping the decrypted data 
    f_dec = fopen(strcat(buf,".uf"), "wb");
 
    //decrypt the given file
    file_decrypt(EncK,initv, f_input, f_dec);

    //close the file descriptors 
    fclose(f_input);
    fclose(f_dec);

    return 0;

}

