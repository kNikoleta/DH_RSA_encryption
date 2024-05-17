#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char *outputFile = NULL;

    mpz_t p, g, a, b;
    mpz_t alice_k, bob_k, alice_secret_k, bob_secret_k;
    mpz_inits(p, g, a, b, alice_k, bob_k, alice_secret_k, bob_secret_k, NULL);
    int base = 10;

    // Check if the correct number of arguments is provided
    if (argc != 11 && argc != 2) {
        printf("Expecting input like: ./dh_assign_1 -o output.txt -p prime -g generator -a private_a -b private_b\n"
               "or like : ./dh_assign_1 -h\n");
        return -1;
    }

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            // Next argument should be the output file
            outputFile = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0) {
            // Next argument should be the prime
            mpz_set_str(p, argv[++i], base); //convert to gmp number
             if(mpz_probab_prime_p(p,45) == 0){
                 printf("For -p give a prime number!");
                 return -1;
            }
        } else if (strcmp(argv[i], "-g") == 0) {
            // Next argument should be the generator
            mpz_set_str(g, (argv[++i]), base); //convert to gmp number
        } else if (strcmp(argv[i], "-a") == 0) {
            // Next argument should be private key 'a'

            mpz_set_str(a, (argv[++i]), base); //convert to gmp number
            if (mpz_cmp (a, p) >= 0){
               printf("a should be less than p!");
               return -1;
            }
        } else if (strcmp(argv[i], "-b") == 0) {
            // Next argument should be private key 'b'
            mpz_set_str(b, (argv[++i]), base); //convert to gmp number
            if (mpz_cmp (b, p) >= 0){
               printf("b should be less than p!");
               return -1;
            }
        } else if (strcmp(argv[i], "-h") == 0) {
            // Help message option

            printf("\n-----------------------------------------------------------------------------------------------------------------\n"
                   "Hello this is the help message! Don't worry we got you!!!!" 
                   "\nThis program generates a secret key by using diffie-Hellman algorithm\n" 
                   "The input should be like: ./dh_assign_1 -o output.txt -p your_number -g your_number -a your_number -b your_number\n"
                   "-----------------------------------------------------------------------------------------------------------------\n"
                   "Lets explain a bit further:\n -o is the output file name in which you will see the results\n -p is the prime number "
                   "for the modulo\n -g is the base of the exponentiation\n -a is the secret number of person a\n -b is the secret number of person b\n\n");
            
            return 0; 
        }
    }

    mpz_powm (alice_k, g, a, p);

    mpz_powm (bob_k, g, b, p); 

    mpz_powm (bob_secret_k, alice_k, b, p);

    mpz_powm (alice_secret_k, bob_k, a, p);

    if (mpz_cmp (bob_secret_k, alice_secret_k) != 0){
      printf("Failed :(");
      return -1;
    }

   int secret_key = mpz_get_si(alice_secret_k); 
   int public_key_A = mpz_get_si(alice_k);
   int public_key_B = mpz_get_si(bob_k);

   FILE *pF = fopen(outputFile,"w");

   fprintf(pF, "< %d >,< %d >,< %d >",public_key_A,public_key_B,secret_key);

   fclose(pF);

   mpz_clears(p, g, a, b, alice_k, bob_k, alice_secret_k, bob_secret_k, NULL);
    
   printf("Key was created successfully. Check the txt file.\n");

   return 0;
}