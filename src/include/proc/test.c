#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_process_info(pid_t pid) {
    char command[256];
    snprintf(command, sizeof(command), "ps aux | grep %d | grep -v grep", pid);

    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        char user[32], command[256];
        double cpu, mem;
        unsigned long vsz, rss;
        int scanned = sscanf(buffer, 
                     "%31s %*d %lf %lf %lu %lu %*s %*s %*s %*s %255[^\n]",
                     user, &cpu, &mem, &vsz, &rss, command);


        if (scanned == 6) {
            printf("User: %s\n", user);
            printf("CPU usage: %.2f%%\n", cpu);
            printf("Memory usage: %.2f%%\n", mem);
            printf("VSZ (virtual size): %lu\n", vsz);
            printf("RSS (resident size): %lu\n", rss);
            printf("Command: %s\n", command);
        } else {
            fprintf(stderr, "Failed to parse process information.\n");
        }
    }

    pclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = atoi(argv[1]);
    if (pid <= 0) {
        fprintf(stderr, "Invalid PID: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    get_process_info(pid);

    return EXIT_SUCCESS;
}

