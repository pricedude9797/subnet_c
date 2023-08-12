#include <stdio.h>
#include <regex.h>
#include <stdbool.h>

int match(const char *string, const char *pattern);

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
            inputValidation = true;
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