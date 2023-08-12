#include <stdio.h>
#include <regex.h>

int match(const char *string, const char *pattern) {
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

int main(int argc, char *argv[]) {

    // Regular expression for IP/CIDR notation
    const char *cidr = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$";

    // Regular expression for dotted decimal notation
    const char *ddn = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$";

    // If a single parameter is passed (argc == 2), it will be tested to see if
    // it conforms to the correct IP/CIDR notation.
    // If two parameters are passed (argc == 3), it will be tested to see if
    // they conform to the correct dotted decimal (IP MASK) notation.
    if (argc == 2) {
        if (match(argv[1], cidr)) {
            printf("yes, cidr\n");
        } else {
            printf("no, cidr\n");
        }
    } else if (argc == 3) {
        if ( (match(argv[1], ddn)) && (match(argv[2], ddn))) {
            printf("yes, ddn\n");
        } else {
            printf("no, ddn\n");
        }
    } else {
        printf("nope\n");
    }

    return 0;
}