#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <math.h>

// Prototypes
int match(const char *string, const char *pattern);
void cidr_to_binmask(int cidr, char *binmask);

void binmask_to_decmask (char *binmask, int *decmask) {
    int j=0;

    for (int i=0; i<32; i++) {
        if (i<8) {
            printf("%c", binmask[i]);
        } else if ( (i>7) && (i<16) ) {
            printf("%c", binmask[i]);
        } else if ( (i>15) && (i<24) ) {
            printf("%c", binmask[i]);
        } else if (i>23) {
            printf("%c", binmask[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    argc = 2; argv[0]="./subnet"; argv[1]="192.168.0.1/24";
    int ip[4] = {0};
    int cidr = 0;
    char binmask[33] = {"0"};
    int decmask[4] = {0};

    const char *cidrValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";
    const char *ddnValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    if ((argc == 2) && (match(argv[1], cidrValidation))) {

        // split the string into 2 parts delimited by '/'
        char string[strlen(argv[1])];
        strcpy(string, argv[1]);
        char *ip_str = strtok(string, "/");
        cidr = atoi(strtok(NULL, " "));

        // split the ip into its 4 octets delimited by '.'
        ip[0] = atoi(strtok(ip_str, "."));
        ip[1] = atoi(strtok(NULL, "."));
        ip[2] = atoi(strtok(NULL, "."));
        ip[3] = atoi(strtok(NULL, ""));

        cidr_to_binmask(cidr, binmask);
        binmask_to_decmask(binmask, decmask);

        printf("%s\n", binmask);


    } else if ( (argc == 3) && (match(argv[1], ddnValidation)) && (match(argv[2], ddnValidation)) ) {
        printf("IP MASK passed regex validation\n");
    } else {
        printf("Syntax error\n");
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

void cidr_to_binmask(int cidr, char *binmask) {
    // convert a cidr (int) into a 32 bit binary representation (string)
    int i=0;
    for (i=0; i<32; i++) {
        if (i<cidr) {
            binmask[i] = '1';
        } else {
            binmask[i] = '0';
        }
    }
    binmask[i] = '\0';
}