#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

typedef struct IP {
  int dIPOct1,dIPOct2,dIPOct3,dIPOct4;
  int dMaskOct1,dMaskOct2,dMaskOct3,dMaskOct4;
  int cidr;
} IP;

// Prototypes
int match(const char *string, const char *pattern);

int main(int argc, char *argv[]) {
    argc = 2; argv[0]="./subnet"; argv[1]="192.168.0.1/24";

    IP ip;

    const char *cidrValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";
    const char *ddnValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    if ((argc == 2) && (match(argv[1], cidrValidation))) {

        char string[strlen(argv[1])];
        char ipstr[strlen(string)];

        strcpy(string, argv[1]);

        strcpy(string, argv[1]);

        char * token = strtok(string, "/");
        strcpy(ipstr, token);
        token = strtok(NULL, " ");
        ip.cidr = atoi(token);


        printf( "%s\t%d\n", ipstr, ip.cidr );

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