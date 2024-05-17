RSA Encryption and Decryption Implementation

This code is an implementation of RSA encryption and decryption. 
It allows you to generate RSA key pairs, encrypt and decrypt text using these keys,
and evaluate the performance of the encryption and decryption processes

--Prerequisites
To compile and run the code, you will need:
 C compiler (e.g., gcc)
 GMP (GNU Multiple Precision Arithmetic Library)

--Compilation
A Makefile is provided to simplify the compilation process. Execute the following commands in your terminal to compile the code:
    make all
This command will compile the code using gcc and link it with the GMP library. 
Also, it is initialized to run the performance test ( ./rsa_assign_1 -a performance.txt )

--Our performance test output:
    Time used to generate keys with length 1024, encrypt and decrypt the text is: 0.018792 seconds
    Time used to generate keys with length 2048, encrypt and decrypt the text is: 0.840007 seconds
    Time used to generate keys with length 4096, encrypt and decrypt the text is: 1.849447 seconds

--Usage
To use the program, you can run it with various command-line options:

--Command-Line Options
  -i inputFilePath: The path to the input file for encryption.
  -o outputFilePath: The path to the output file where the results will be written.
  -k keyFilePath: The path to the key file for encryption or decryption.
  -g key_length: Generates an RSA key pair with the given key length.
  -e: Encrypts the input using the specified key file and stores the result in the output file.
  -d: Decrypts the input using the specified key file and stores the result in the output file.
  -a: Measures the performance of RSA encryption and decryption with key lengths (1024, 2048, 4096).

--Sample Runs
Here are some example commands for running the code:

Generate RSA key pairs with a key length of 1024 bits:
bash
        ./rsa_assign_1 -g 1024

Encrypt a text file using a public key:
bash
        ./rsa_assign_1 -i input.txt -o encrypted.txt -k public_2048.key -e

Decrypt the encrypted text using a private key:
bash
        ./rsa_assign_1 -i encrypted.txt -o decrypted.txt -k private_2048.key -d

Measure the performance of RSA encryption and decryption:
bash
        ./rsa_assign_1 -a performance.txt

--Note
For the performance option please be patient it works but it wants its time.
Also the plain text can not have white spaces and special characters! Please be kind with our sript :)
Thank you.