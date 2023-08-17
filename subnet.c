#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Prototypes
int match(const char *string, const char *pattern);
void parseIPCIDR(char *str, int *ip, int *cidr);
void decimalToBinary(int *ip, int *result);

int main(int argc, char *argv[]) {
    argc = 2; argv[0]="./subnet"; argv[1]="192.168.0.1/24";

    int ip[4] = {0}, ip_b[32] = {0};
    int cidr = 0;

    const char *cidrValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";
    const char *ddnValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    if ((argc == 2) && (match(argv[1], cidrValidation))) {

        parseIPCIDR(argv[1], ip, &cidr);
        decimalToBinary(ip, ip_b);

        printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
        printf("%d\n", cidr);

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

void decimalToBinary(int *ip, int *result) {
    int decimalNumber = 192;
    
    int binary[32];

    int index = 0;
    while (decimalNumber > 0) {
        binary[index] = decimalNumber % 2;
        decimalNumber /= 2;
        index++;
    }

    for (int i = index - 1; i >= 0; i--) {
        printf("%d", binary[i]);
    }
    printf("\n");
}