#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#include <string.h>

int match(const char *string, const char *pattern);
void parseCIDR(char *string, int *ip, int *cidr);

int main(int argc, char *argv[]) {
    bool inputValidation = false;
    int ip[4] = {0,0,0,0};
    int mask[4] = {0,0,0,0};
    int cidr = 0;

    // Regular expression for IP/CIDR notation
    const char *cidrValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";

    // Regular expression for dotted decimal notation
    const char *ddnValidation = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    // If a single parameter is passed (argc == 2), it will be tested to see if
    // it conforms to the correct IP/CIDR notation.
    // If two parameters are passed (argc == 3), it will be tested to see if
    // they conform to the correct dotted decimal (IP MASK) notation.
    if (argc == 2) {
        if (match(argv[1], cidrValidation)) {
            parseCIDR(argv[1], ip, &cidr);
            inputValidation = true;

            printf("%d.%d.%d.%d/%d\n", ip[0], ip[1], ip[2], ip[3], cidr);
        }
    } else if (argc == 3) {
        if ( (match(argv[1], ddnValidation)) && (match(argv[2], ddnValidation))) {
            printf("yes, ddn\n");
            inputValidation = true;
        }
    }

    // if the input syntax was not correct, print the syntax error message
    // giving the correct syntax.
    if (inputValidation == false) {
        printf("\nSyntax: \"subnet xxx.xxx.xxx.xxx/xx\" or"
            " \"subnet xxx.xxx.xxx.xxx xxx.xxx.xxx.xxx\"\n\n");
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

void parseCIDR(char *string, int *ip, int *cidr) {
    int i = 0;

    // First, seperate the IP from the CIDR parts of the string
    char delim1[] = "/";
    char *ptr1 = strtok(string, delim1);

    i = 0;
    while(ptr1 != NULL) {
        if (i==1) { *cidr = atoi(ptr1); }
        i++;
		ptr1 = strtok(NULL, delim1);
	}

    // Next, parse out the ip octets
    char a[4], b[4], c[4], d[4], delim2[] = ".";
    char *ptr2 = strtok(string, delim2);
    i = 0;
    while(ptr2 != NULL) {
        if (i==0) { ip[0] = atoi(ptr2); }
        if (i==1) { ip[1] = atoi(ptr2); }
        if (i==2) { ip[2] = atoi(ptr2); }
        if (i==3) { ip[3] = atoi(ptr2); }
        i++;
		ptr2 = strtok(NULL, delim2);
	}
}