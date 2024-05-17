Diffie-Hellman Key Exchange Implementation

This code is an implementation of the Diffie-Hellman key exchange algorithm. 
It allows two parties to securely exchange a secret key over an insecure communication channel. 

--Prerequisites
To compile and run the code, you will need:
 C compiler (e.g., gcc)
 GMP (GNU Multiple Precision Arithmetic Library)

--Compilation
A Makefile is provided to simplify the compilation process. 
Execute the following commands in your terminal to compile the code:
 make all

--Usage
To use the program, you can run it with the following command-line options:

--Command-Line Options
  -o output.txt: Specifies the output file where the results will be written.
  -p prime: Specifies the prime number for modulo operations.
  -g generator: Specifies the base of the exponentiation.
  -a private_a: Specifies the private key of person A.
  -b private_b: Specifies the private key of person B.
  -h: Displays a help message explaining the usage.

--Code Explanation
The code performs the following steps:

1-Parses the command-line arguments to obtain the prime number, generator, and private keys of person A and person B.
2-Calculates the public keys of person A and person B.
3-Computes the shared secret keys for both parties.
4-Verifies that the shared secret keys match, ensuring a successful key exchange.
5-Writes the public keys and shared secret key to the output file in the format < public_key_A >,< public_key_B >,< secret_key >.

--Sample Run
Here's an example of how to run the code:

 ./dh_assign_1 -o output.txt -p 23 -g 9 -a 15 -b 2

This command will generate the public keys and shared secret key and save them in the output.txt file.
