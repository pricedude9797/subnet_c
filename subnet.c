#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Prototypes
int match(const char *string, const char *pattern);
void parseIPCIDR(char *str, int *ip, int *cidr);
void cidr_to_binMask(int cidr, char *bin_mask);
void binMask_to_Mask(char *bin_mask, int *mask);
int bin_to_dec(char *octet);

int main(int argc, char *argv[]) {
    argc = 2; argv[0]="./subnet"; argv[1]="192.168.0.1/24";
    int ip[4] = {0};
    int mask[4] = {0};
    char bin_mask[33];
    int cidr = 0;

    const char *cidrValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";
    const char *ddnValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    if ((argc == 2) && (match(argv[1], cidrValidation))) {

        parseIPCIDR(argv[1], ip, &cidr);
        cidr_to_binMask(cidr, bin_mask);
        binMask_to_Mask(bin_mask, mask);

    } else if ( (argc == 3) && (match(argv[1], ddnValidation)) && (match(argv[2], ddnValidation)) ) {
        printf("IP MASK passed regex validation\n");
    } else {
        printf("Syntax error\n");
        exit(0);
    }

    printf("%d.%d.%d.%d/%d\n", ip[0],ip[1],ip[2],ip[3],cidr);
    printf("%d.%d.%d.%d\n", mask[0],mask[1],mask[2],mask[3]);

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

void cidr_to_binMask(int cidr, char *bin_mask) {
    // takes the cidr (int) and datafiles the binary representation
    // of the 32-bit mask, resulting in bin_mask (char)

    for (int i=0; i<32; i++) {
        if (i<cidr) {
            bin_mask[i] = '1';
        } else {
            bin_mask[i] = '0';
        }
    }
    bin_mask[32] = '\0';
}

void binMask_to_Mask(char *bin_mask, int *mask) {
    // char arrays to hold the individual octet strings
    char octet1[9]; octet1[8] = '\0';
    char octet2[9]; octet2[8] = '\0';
    char octet3[9]; octet3[8] = '\0';
    char octet4[9]; octet4[8] = '\0';

    // datafill the individual octet string arrays
    for(int i=0; i<8; i++) {
        octet1[i] = bin_mask[i];
        octet2[i] = bin_mask[i+8];
        octet3[i] = bin_mask[i+16];
        octet4[i] = bin_mask[i+24];
    }

    // use bin_to_dec to convert the octet string arrays to integers
    mask[0] = bin_to_dec(octet1);
    mask[1] = bin_to_dec(octet2);
    mask[2] = bin_to_dec(octet3);
    mask[3] = bin_to_dec(octet4);
}

int bin_to_dec(char *octet) {
    // Takes an 8-bit binary character array and converts it to int,
    // then return the result

    int result = 0;

    for(int i=0; i<8; i++) {
        if ((i==0) && (octet[i]=='1')) {result+=128;}
        if ((i==1) && (octet[i]=='1')) {result+=64;}
        if ((i==2) && (octet[i]=='1')) {result+=32;}
        if ((i==3) && (octet[i]=='1')) {result+=16;}
        if ((i==4) && (octet[i]=='1')) {result+=8;}
        if ((i==5) && (octet[i]=='1')) {result+=4;}
        if ((i==6) && (octet[i]=='1')) {result+=2;}
        if ((i==7) && (octet[i]=='1')) {result+=1;}
    }

    return result;
}