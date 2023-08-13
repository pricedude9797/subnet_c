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
int validateMask(int *mask, int *cidr);
void cidr_to_mask(int cidr, int *mask);
int bin_to_dec(int *binArray);

int main(int argc, char *argv[]) {
    bool inputValidation = false;
    int ip[4] = {0,0,0,0};
    int mask[4] = {0,0,0,0};
    int cidr = 0;
    int network[4] = {0,0,0,0};
    int first[4] = {0,0,0,0};
    int last[4] = {0,0,0,0};
    int broadcast[4] = {0,0,0,0};
    int hosts = 0;

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
            if ( (cidr < 8) || (cidr > 30) ) { inputValidation = false; }
            if (inputValidation == true) { cidr_to_mask(cidr, mask); }
        }
    } else if (argc == 3) {
        if ( (match(argv[1], ddnValidation)) && (match(argv[2], ddnValidation))) {
            inputValidation = true;
            parseDDN(argv[1], ip);
            parseDDN(argv[2], mask);
            if (!validateIP(ip)) { inputValidation = false; }
            if (!validateMask(mask, &cidr)) { inputValidation = false; }         
        }
    }

    // If the input passes validation, IP, CID, and Mask are printed.
    // Or...a syntax message is displayed
    if (inputValidation == true) {
        printf("\nIP Address       : %d.%d.%d.%d/%d (%d.%d.%d.%d)\n",
            ip[0], ip[1], ip[2], ip[3], cidr, mask[0], mask[1], mask[2], mask[3]);
        printf("Number of hosts  : %d\n", hosts);
        printf("Network address  : %d.%d.%d.%d\n", network[0], network[1], network[2], network[3]);
        printf("First address    : %d.%d.%d.%d\n", first[0], first[1], first[2], first[3]);
        printf("Last address     : %d.%d.%d.%d\n", last[0], last[1], last[2], last[3]);
        printf("Broadcast address: %d.%d.%d.%d\n\n", broadcast[0], broadcast[1], broadcast[2], broadcast[3]);
    } else {
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

int validateMask(int *mask, int *cidr) {

    // Create a 32 bit integer array to hold the binary representation of the mask
    int binMask[32] = {0};
    int a = mask[0], b=mask[1], c=mask[2], d=mask[3];

    // Datafll the binary array of the mask
    if (a >= 128) { binMask[0] = 1; a-=128; }
    if (a >= 64) { binMask[1] = 1; a-=64; }
    if (a >= 32) { binMask[2] = 1; a-=32; }
    if (a >= 16) { binMask[3] = 1; a-=16; }
    if (a >= 8) { binMask[4] = 1; a-=8; }
    if (a >= 4) { binMask[5] = 1; a-=4; }
    if (a >= 2) { binMask[6] = 1; a-=2; }
    if (a >= 1) { binMask[7] = 1; a-=1; }

    if (b >= 128) { binMask[8] = 1; b-=128; }
    if (b >= 64) { binMask[9] = 1; b-=64; }
    if (b >= 32) { binMask[10] = 1; b-=32; }
    if (b >= 16) { binMask[11] = 1; b-=16; }
    if (b >= 8) { binMask[12] = 1; b-=8; }
    if (b >= 4) { binMask[13] = 1; b-=4; }
    if (b >= 2) { binMask[14] = 1; b-=2; }
    if (b >= 1) { binMask[15] = 1; b-=1; }

    if (c >= 128) { binMask[16] = 1; c-=128; }
    if (c >= 64) { binMask[17] = 1; c-=64; }
    if (c >= 32) { binMask[18] = 1; c-=32; }
    if (c >= 16) { binMask[19] = 1; c-=16; }
    if (c >= 8) { binMask[20] = 1; c-=8; }
    if (c >= 4) { binMask[21] = 1; c-=4; }
    if (c >= 2) { binMask[22] = 1; c-=2; }
    if (c >= 1) { binMask[23] = 1; c-=1; }

    if (d >= 128) { binMask[24] = 1; d-=128; }
    if (d >= 64) { binMask[25] = 1; d-=64; }
    if (d >= 32) { binMask[26] = 1; d-=32; }
    if (d >= 16) { binMask[27] = 1; d-=16; }
    if (d >= 8) { binMask[28] = 1; d-=8; }
    if (d >= 4) { binMask[29] = 1; d-=4; }
    if (d >= 2) { binMask[30] = 1; d-=2; }
    if (d >= 1) { binMask[31] = 1; d-=1; }

    // Test the validity of the mask. There can never be 1's to the right
    // of a 0 in a mask. Also, in this check, the CIDR bits are counted
    int tst=1, count=0;
    bool val=true;
    for (int i=0; i<32; i++) {
        if (binMask[i] == 0) { tst = 0; }
        if ((tst == 0) && (binMask[i] == 1)) { val = false; break; }
        if (binMask[i] == 1) { count++; }
    }

    if (val == true) { *cidr = count; } 

    return val;
}

void cidr_to_mask(int cidr, int *mask) {
    int binA[8]={0}, binB[8]={0}, binC[8]={0}, binD[8]={0};
    int a=0, b=0, c=0, d=0;

    // create the binary representation of the octets
    for (int i=0; i<cidr; i++) {
        if (i<8) { binA[a] = 1; a++; }
        if ( (i>=8) && (i<16) ) { binB[b] = 1; b++; }
        if ( (i>=16) && (i<24) ) { binC[c] = 1; c++; }
        if (i>=24) { binD[d] = 1; d++; }
    }

    // datafill the mask array
    mask[0] = bin_to_dec(binA);
    mask[1] = bin_to_dec(binB);
    mask[2] = bin_to_dec(binC);
    mask[3] = bin_to_dec(binD);
}

int bin_to_dec(int *binArray) {
    // Takes an integer arry representing 8 bits and calculates/returns
    // the decimal value

    int ph[] = {128,64,32,16,8,4,2,1};
    int result=0;
    for(int i=0; i<8; i++) {
        if (binArray[i] == 1) { result += ph[i]; }
    }
    return result;
}