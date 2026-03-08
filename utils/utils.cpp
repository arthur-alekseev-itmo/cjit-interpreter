

#define MAX_BUFFER_SIZE 128
#include <cstdio>
#include <cstdlib>

int exec_print(const char* command) {
    char buffer[MAX_BUFFER_SIZE];

    FILE* fp = popen(command, "r");
    if (fp == nullptr) {
        printf("Failed to run command\n");
        exit(1);
    }

    printf("Output of command '%s':\n", command);
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        printf("%s", buffer);
    }

    const int status = pclose(fp);
    return status;
}

int exec_silent(const char* command) {
    FILE* fp = popen(command, "r");

    if (fp == nullptr) {
        printf("Failed to run command\n");
        exit(1);
    }

    const int status = pclose(fp);
    return status;
}
