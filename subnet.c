#include <stdio.h>
#include <string.h>
#include <regex.h>

// Prototypes
int match(const char *string, const char *pattern);

int main(int argc, char *argv[]) {
    argc = 2; argv[0]="./subnet"; argv[1]="192.168.0.1/24";

    const char *cidrValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";
    const char *ddnValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    if ((argc == 2) && (match(argv[1], cidrValidation))) {
        printf("IP/CIDR passed regex validation\n");
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