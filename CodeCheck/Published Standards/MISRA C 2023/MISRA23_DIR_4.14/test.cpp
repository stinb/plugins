#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/socket.h>
#else
// Declarations for Windows compatibility
typedef long ssize_t;
ssize_t read(int fd, void *buf, size_t count);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
#endif

char dst[128];
int fd;
int sock;

void process(char *buf) {}
int is_valid_env(char *p) { return 1; }
void use(char *p) {}
void sanitize(char *buf) {}

void f1( void ) {   
    char input [ 128 ];   
    (void) scanf ( "%127s", input );   
    (void) printf ( "%s", input ); // UndCC_Violation
}

void f2(void) {
    char buf[64];
    char dst[64];
    fgets(buf, sizeof(buf), stdin);
    strcpy(dst, buf); // UndCC_Violation
}
void f3(void) {
    char *p = getenv("FOO");
    puts(p); // UndCC_Violation 
}

void f4(void) {
    int n;
    char buf[128];
    n = read(fd, buf, sizeof buf);
    if (n > 0) {}
    printf("%d\n", n); // UndCC_FalseNeg
}

void f5(void) {
    char buf[32];
    recv(sock, buf, sizeof buf, 0);
    if (buf[0] == '\0') { /* some check on buf content */ }
    process(buf); /* MAY be flagged if check not clearly mentioning var */
}

void good1(void) {
    char input[128];
    if (scanf("%127s", input) == 1) {
        if (strlen(input) < sizeof(dst)) {
            strcpy(dst, input);
        }
    }
}

void good2(void) {
    char buf[16];
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return;
    }
    if (strnlen(buf, sizeof(buf)) >= sizeof(dst))
        return;
    strcpy(dst, buf);
}

void good3(void) {
    char *p = getenv("X");
    if (p == NULL) { /* handle error */ return; }
    if (is_valid_env(p)) { use(p); } /* if function named is_valid_env */
}

void good4(void) {
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    sanitize(buf); /* assume sanitize() is project validation */
    strcpy(dst, buf);
}
