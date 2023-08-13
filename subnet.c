#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#include <string.h>

// Prototypes
int match(const char *string, const char *pattern);
void parseCIDR(char *string, int *ip, int *cidr);
void parseDDN(char *string, int *ip);
int validateIP(int *ip);
int validateMask(int *mask);
void cidr_to_mask(int cidr, int *mask);

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
            parseCIDR(argv[1], ip, &cidr);
            if (!validateIP(ip)) { inputValidation = false; }
            if ( (cidr < 1) || (cidr > 32) ) { inputValidation = false; }
            if (inputValidation == true) { cidr_to_mask(cidr, mask); }
        }
    } else if (argc == 3) {
        if ( (match(argv[1], ddnValidation)) && (match(argv[2], ddnValidation))) {
            inputValidation = true;
            parseDDN(argv[1], ip);
            parseDDN(argv[2], mask);
            if (!validateIP(ip)) { inputValidation = false; }
            if (!validateMask(mask)) { inputValidation = false; }
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
    // takes a string and compares it with a passed regular expression.
    // if there is a match, it returns 1, and if there is no match it
    // returns 0.

    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

void parseCIDR(char *string, int *ip, int *cidr) {
    // Takes in a string representation of IP/CIDR and first splits it
    // into a string representation of an IP address, and a string
    // representation of a CIDR notation. It converts the CIDR notation
    // into the cidr int, then converts the string representation of the
    // IP into an ip int array.

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
    parseDDN(string, ip);
}

void parseDDN(char *string, int *ip) {
    // Takes in a string representation of an IP address, splts it into
    // 4 octets using the . as a delimeter, then casts each string represented
    // octet into an IP int array.

    char a[4], b[4], c[4], d[4], delim[] = ".";
    char *ptr = strtok(string, delim);
    int i = 0;
    while(ptr != NULL) {
        if (i==0) { ip[0] = atoi(ptr); }
        if (i==1) { ip[1] = atoi(ptr); }
        if (i==2) { ip[2] = atoi(ptr); }
        if (i==3) { ip[3] = atoi(ptr); }
        i++;
		ptr = strtok(NULL, delim);
	}
}

int validateIP(int *ip) {
    // Validates an IPv4 address from 1.0.0.0 thru 223.255.255.255

    if (  (ip[0] >= 1) && (ip[0] <= 223) &&
        ( (ip[1] >= 0) && (ip[1] <= 255) ) &&
        ( (ip[2] >= 0) && (ip[2] <= 255) ) &&
        ( (ip[3] >= 0) && (ip[3] <= 255) ) )
    {
        return 1;
    } else {
        return 0;
    }
}

int validateMask(int *mask) {
    // Validates IPv4 maskbinMask[i]

    if (  (mask[0] >= 0) && (mask[0] <= 255) &&
        ( (mask[1] >= 0) && (mask[1] <= 255) ) &&
        ( (mask[2] >= 0) && (mask[2] <= 255) ) &&
        ( (mask[3] >= 0) && (mask[3] <= 255) ) )
    {
        return 1;
    } else {
        return 0;
    }
}

void cidr_to_mask(int cidr, int *mask) {

}