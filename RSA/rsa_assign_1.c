#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void generateRSAKey(int key_length, mpz_t n, mpz_t e, mpz_t d) {
    mpz_t p, q, res1, res2, lambda;
    mpz_inits(p, q, res1, res2, lambda, NULL);
    gmp_randstate_t state; //initializing for using Random Number Functions
    gmp_randinit_mt(state); //Initialize state with a default algorithm. This will be a compromise between speed and randomness, and is recommended for applications with no special requirements. Currently this is gmp_randinit_mt.
    
    //STEP_1,STEP_2 give two random numbers p, q both length = length/2 and check if they are prime.
    while(1){
        mpz_urandomb(p, state, key_length/2);
        
        if(mpz_probab_prime_p(p,45) > 0){
            break;
        }
    }

    mpz_nextprime(q, p);
    
    //STEP_3
    mpz_mul(n, p, q); //n = p*q n 
   
    //STEP_4 lambda(n) = (p - 1) * (q - 1).
    mpz_sub_ui(res1 , p, 1);
    mpz_sub_ui(res2 , q, 1);
    mpz_mul(lambda, res1, res2);

    //STEP_5 Choose a prime e where (e % lambda(n) != 0) AND (gcd(e, lambda) == 1) where gcd() is the Greatest Common Denominator
     do{
        mpz_urandomb(e, state, key_length);
        mpz_mod(res1, e, lambda);
        mpz_gcd(res2, e, lambda);
        if(res1 != 0 && mpz_cmp_ui(res2,1) == 0 && mpz_probab_prime_p(e, 45) > 0 ){
            break;
        }
    }while(1);

    //STEP_6 Choose d where d is the modular inverse of (e, lambda).    
    mpz_invert(d, e, lambda);
    
    //7. The public key consists of n and d, in this order.
    //8. The private key consists of n and e, in this order.

    mpz_clears(p, q, NULL);
    gmp_randclear(state);
}

void storeToFile(char *type, char *length, mpz_t a, mpz_t b){
    char result[30]; 
    strcpy(result, type);
    strcat(result, length); 
    strcat(result, ".key");  

    FILE *pF = fopen(result,"w");
    
    char *str_a = mpz_get_str(NULL, 10, a); // Convert a to a base-10 string
    char *str_b = mpz_get_str(NULL, 10, b); // Convert b to a base-10 string

    fprintf(pF, "%s,%s,", str_a, str_b);

    fclose(pF);
}

int encryptFile(char *inputFilePath, char *outputFilePath, char *keyfilePath){
    mpz_t ec, n, e, en_txt;
    mpz_inits(ec, n, e, en_txt, NULL);

    FILE *input_fl = fopen(inputFilePath, "r");
    FILE *output_fl = fopen(outputFilePath, "w");
    FILE *key_fl = fopen(keyfilePath, "r");

    int ch = 0;   
    char buff_n[4096];
    char buff_e[4096];
    int index = 0;
    memset(buff_n, 0, sizeof(buff_n));
    memset(buff_e, 0, sizeof(buff_e));

    // Read characters from the .key file until a comma is encountered
    while ((ch = fgetc(key_fl)) != EOF) {
        if (ch == ',') {
            break;
        }

        buff_n[index] = (char)ch;
        index++;

        if (index >= sizeof(buff_n) - 1) {
            printf("Error with buffer.");
            break; 
        }
    }
    index = 0;

    // Read characters from the .key file until a comma is encountered
    while ((ch = fgetc(key_fl)) != EOF) {
        if (ch == ',') {
            break; 
        }

        buff_e[index] = (char)ch;
        index++;

        if (index >= sizeof(buff_e) - 1) {
            printf("Error with buffer.");
            break; 
        }
    }

    fseek(input_fl, 0, SEEK_END); // seek to end of file
    
    long fileSize = ftell(input_fl);

    if (fileSize == -1) {
        fprintf(stderr, "Error getting file size.\n");
        fclose(input_fl);
        
        return -1; 
    }
    fseek(input_fl, 0, SEEK_SET); // seek back to beginning of file
  
    char cur_txt[1000000];
    memset(cur_txt, 0, sizeof(cur_txt));

    //key n (mpz format)
    mpz_set_str(n, buff_n, 10);

    //key e (mpz format)
    mpz_set_str(e, buff_e, 10);

    fscanf(input_fl,"%s\n",cur_txt);

    mpz_set_str(en_txt, cur_txt, 62);

    mpz_powm (ec, en_txt, e, n);// encryption

    mpz_get_str(cur_txt, 62, ec);//encrypted message as a string

    fprintf(output_fl, "%s", cur_txt);

    fclose(input_fl);
    fclose(output_fl);
    fclose(key_fl);

    mpz_clears(en_txt, ec, n, e, NULL);
    return 1;
}

int decryptFile(char *inputFilePath, char *outputFilePath, char *keyfilePath){
    mpz_t dc_txt, dc, n, d;
    mpz_inits(dc_txt, dc, n, d, NULL);

    FILE *input_fl = fopen(inputFilePath, "r");
    FILE *output_fl = fopen(outputFilePath, "w");
    FILE *key_fl = fopen(keyfilePath, "r");

    int ch = 0;
    char buff_n[4096];
    char buff_d[4096];
    memset(buff_n, 0, sizeof(buff_n));
    memset(buff_d, 0, sizeof(buff_d));

    int index = 0;

    // Read characters from the .key file until a comma is encountered
    while ((ch = fgetc(key_fl)) != EOF) {
        if (ch == ',') {
            break; 
        }
        
        buff_n[index] = (char)ch;
        index++;

        if (index >= sizeof(buff_n) - 1) {
            printf("Error with buffer.");
            break; 
        }
    }

    index = 0;

    // Read characters from the .key file until a comma is encountered
    while ((ch = fgetc(key_fl)) != EOF) {
        if (ch == ',') {
            break; // Stop when a comma is found
        }

        // Store the character in the buffer
        buff_d[index] = (char)ch;
        index++;

        if (index >= sizeof(buff_d) - 1) {
            printf("Error with buffer.");
            break; // Buffer is full
        }
    }

    fseek(input_fl, 0, SEEK_END); // seek to end of file
    
    long fileSize = ftell(input_fl);

    if (fileSize == -1) {
        // Handle the error
        fprintf(stderr, "Error getting file size.\n");
        // Close the file and free any allocated memory
        fclose(input_fl);
        
        return -1; // Return an error code
    }
    fseek(input_fl, 0, SEEK_SET); // seek back to beginning of file

    char cur_txt[1000000];
    memset(cur_txt, 0, sizeof(cur_txt));

    mpz_set_str(n, buff_n, 10);

    mpz_set_str(d, buff_d, 10);
    
    fscanf(input_fl,"%s\n",cur_txt);

    mpz_set_str(dc_txt, cur_txt, 62);

    mpz_powm (dc, dc_txt, d, n);//decryption

    mpz_get_str(cur_txt, 62, dc);//decrypted message as a string

    fprintf(output_fl, "%s", cur_txt);

    fclose(input_fl);
    fclose(output_fl);
    fclose(key_fl);

    mpz_clears(dc_txt, dc, n, d, NULL);
    return 1;
}

void performance_comp(char *performance_txt, mpz_t n, mpz_t e, mpz_t d){
    
    FILE *perf_fl = fopen(performance_txt, "w");

    clock_t start, end;
    double cpu_time_used_1024, cpu_time_used_2048, cpu_time_used_4096;
    
    start = clock();
    generateRSAKey(1024, n, e, d);
    storeToFile("public_","1024", n, e);
    storeToFile("private_","1024", n, d);
    encryptFile("plain.txt", "encrypted1024.txt", "public_1024.key");
    decryptFile("encrypted1024.txt", "decrypted1024.txt", "private_1024.key");
    end = clock(); 
    cpu_time_used_1024 = ((double)(end - start)) / CLOCKS_PER_SEC;

    start = clock();
    generateRSAKey(2048, n, e, d);
    storeToFile("public_","2048", n, e);
    storeToFile("private_","2048", n, d);
    encryptFile("plain.txt", "encrypted2048.txt", "public_2048.key");
    decryptFile("encrypted2048.txt", "decrypted2048.txt", "private_2048.key");
    end = clock();
    cpu_time_used_2048 = ((double)(end - start)) / CLOCKS_PER_SEC;

    start = clock();
    generateRSAKey(4096, n, e, d);
    storeToFile("public_","4096", n, e);
    storeToFile("private_","4096", n, d);
    encryptFile("plain.txt", "encrypted4096.txt", "public_4096.key");
    decryptFile("encrypted4096.txt", "decrypted4096.txt", "private_4096.key");
    end = clock();
    cpu_time_used_4096 = ((double)(end - start)) / CLOCKS_PER_SEC;    

    fprintf(perf_fl, "Time used to generate keys with length 1024, encrypt and decrypt the text is: %f seconds\n"
    "Time used to generate keys with length 2048, encrypt and decrypt the text is: %f seconds\n"
    "Time used to generate keys with length 4096, encrypt and decrypt the text is: %f seconds\n", cpu_time_used_1024, cpu_time_used_2048, cpu_time_used_4096);
    
    fclose(perf_fl);
}

int main(int argc, char *argv[]) {
    char *inputFilePath = NULL;
    char *outputFilePath = NULL;
    char *keyfilePath = NULL;
    char *perfPath = NULL;
    
    mpz_t n, e, d;
    mpz_inits(n, e, d, NULL);
    int base = 62;
    int g;
    // Check if the correct number of arguments is provided
    if (argc != 8 && argc != 3 && argc!= 2) {
        printf("wrong input");
        return -1;
    }

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {             
            inputFilePath = argv[++i];            
        } else if (strcmp(argv[i], "-o") == 0) {           
            outputFilePath = argv[++i];                   
        } else if (strcmp(argv[i], "-k") == 0) {           
            keyfilePath = argv[++i];       
        } else if (strcmp(argv[i], "-g") == 0) {  
            g = atoi(argv[++i]);         
            //mpz_set_str(g, (argv[++i]), base);-g length Perform RSA key-pair generation given a key length “length”
            generateRSAKey(g, n, e, d);

            storeToFile("public_",argv[i], n, e);
            storeToFile("private_",argv[i], n, d);

        } else if (strcmp(argv[i], "-d") == 0) {
            decryptFile(inputFilePath, outputFilePath, keyfilePath);
            //Decrypt input and store results to output.
            
        } else if (strcmp(argv[i], "-e") == 0) {
            encryptFile(inputFilePath, outputFilePath, keyfilePath);           
            //Encrypt input and store results to output
            
        } else if (strcmp(argv[i], "-a") == 0) {
             perfPath = argv[++i];
             performance_comp(perfPath, n, e, d);
            //Compare the performance of RSA encryption and decryption with three
            //different key lengths (1024, 2048, 4096 key lengths) in terms of computational time.
        
        }else if (strcmp(argv[i], "-h") == 0) {
            // Help message option
            printf("\n----------------------------------------------------------------------------------------------------------------------------\n"
                   "Hello this is the help message! Don't worry we got you!!!!"
                   "\nThis code is an implementation of RSA algorithm.\n" 
                   "It allows you to generate RSA key pairs and then encrypt and decrypt text using these keys.\n"
                   "You can also evaluate the performance of the encryption and decryption processes for the different key lengths(1024,2048,4096)\n"
                   "------------------------------------------------------------------------------------------------------------------------------\n"
                   "Before run read the README file\n"
                   "------------------------------------------------------------------------------------------------------------------------------\n\n");

            return 0;
        }
    }

    
    mpz_clears(n, d, e, NULL);

   return 0;
}