#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Prototypes
int match(const char *string, const char *pattern);
void parseIPCIDR(char *str, int *ip, int *cidr);
void decimalToBinary(int n, int *binary);
int binaryToInt(int *binary);
void ipToBinary(int *ip, int *ip_b);
void cidrToBinary(int cidr, int *mask_b);
void binaryToDottedDecimal(int *binary, int *ddn);





int main(int argc, char *argv[]) {
    //argc = 2; argv[0]="./subnet"; argv[1]="192.168.0.1/24";

    int ip[4] = {0}, ip_b[32] = {0};
    int cidr = 0, mask_b[32] = {0}, mask[4] = {0};

    const char *cidrValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";
    const char *ddnValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    if ((argc == 2) && (match(argv[1], cidrValidation))) {

        parseIPCIDR(argv[1], ip, &cidr);
        ipToBinary(ip, ip_b);
        cidrToBinary(cidr, mask_b);
        binaryToDottedDecimal(mask_b, mask);

    } else if ( (argc == 3) && (match(argv[1], ddnValidation)) && (match(argv[2], ddnValidation)) ) {
        printf("IP MASK passed regex validation\n");
    } else {
        printf("Syntax error\n");
        exit(0);
    }

    return 0;
}





int match(const char *string, const char *pattern) {
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

void parseIPCIDR(char *str, int *ip, int *cidr) {
    // the input string (*str here), would have already been
    // checked to conform to ip/cidr notation, so at this point
    // we already knw the basic format is valid.

    // make a copy of the input string
    char string[strlen(str)];
    strcpy(string, str);
    
    // split the string with '/' to get the ip_str (string), and
    // the cidr (int)
    char *ip_str = strtok(string, "/");
    *cidr = atoi(strtok(NULL, " "));

    // error checking the cidr input
    if ((*cidr < 8) || (*cidr > 30)) {
        printf("Invalid CIDR entry...exiting\n");
        exit(0);
    }

    // take the ip_str (string), and parse it using '.', then
    // convert each octet into an integer.
    ip[0] = atoi(strtok(ip_str, "."));
    ip[1] = atoi(strtok(NULL, "."));
    ip[2] = atoi(strtok(NULL, "."));
    ip[3] = atoi(strtok(NULL, ""));

    // error checking the ip input
    if ((ip[0] < 1) || (ip[0] > 223) ||
        (ip[1] < 0) || (ip[1] > 255) ||
        (ip[2] < 0) || (ip[2] > 255) ||
        (ip[3] < 0) || (ip[3] > 255) ) {
        printf("Invalid IP entry...exiting\n");
        exit(0);
    }
}

void decimalToBinary(int n, int *binary) {
    // Takes a number from 0 to 255 and converts it to an
    // 8 bit binary digit. It is then placed in the passed
    // binary array

    int index = 7;
    if (n > 0) {
        while (n > 0) {
            binary[index] = n % 2;
            n /= 2;
            index--;
        }
    }

}

int binaryToInt(int *binary) {
    int decimal = 0;

    for (int i = 7; i >= 0; i--) {
        if (binary[i] == 1) {
            decimal += 1 << (7 - i);
        }
    }

    return decimal;
}

void ipToBinary(int *ip, int *ip_b) {
    // Takes in the ip array, then converts it to a 32-bit
    // binary array. The output is stored in the ip_b int array

    // set up the temporary 8 bit arrays (one for each octet)
    int oct0[8] = {0};
    int oct1[8] = {0};
    int oct2[8] = {0};
    int oct3[8] = {0};

    // convert each octet into its 8-bit binary equivilent
    decimalToBinary(ip[0], oct0);
    decimalToBinary(ip[1], oct1);
    decimalToBinary(ip[2], oct2);
    decimalToBinary(ip[3], oct3);

    // datafill the ip_b 32-bit array
    for (int i=0; i<8; i++) { ip_b[i] = oct0[i]; }
    for (int i=8; i<16; i++) { ip_b[i] = oct1[i-8]; }
    for (int i=16; i<24; i++) { ip_b[i] = oct2[i-16]; }
    for (int i=24; i<32; i++) { ip_b[i] = oct3[i-24]; }
}

void cidrToBinary(int cidr, int *mask_b) {
    // convert the cidr notation to a 32-bit binary number
    // place the results in the mask_b array

    for (int i=0; i<32; i++) {
        if (i<cidr) {
            mask_b[i] = 1;
        } else {
            mask_b[i] = 0;
        }
    }
}

void binaryToDottedDecimal(int *binary, int *ddn) {
    // Take in a 32-bit binary int array, and convert it to dotted decimal

    // create and initialize the 8-bit int arrays to hold the octets
    int octet1[8] = {0};
    int octet2[8] = {0};
    int octet3[8] = {0};
    int octet4[8] = {0};

    // break up the 32-bit binary number into octets
    for (int i=0; i<32; i++) {
        if ((i>=0) && (i<=7)) { octet1[i] = binary[i]; }
        if ((i>=8) && (i<16)) { octet2[i-8] = binary[i]; }
        if ((i>=16) && (i<24)) { octet3[i-16] = binary[i]; }
        if ((i>=24) && (i<32)) { octet4[i-24] = binary[i]; }
    }

    // convert each octet to decimal
    ddn[0] = binaryToInt(octet1);
    ddn[1] = binaryToInt(octet2);
    ddn[2] = binaryToInt(octet3);
    ddn[3] = binaryToInt(octet4);
}
