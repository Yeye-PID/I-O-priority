#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/wait.h>

void NFC(const char *message) {
    char notifie[512];
    snprintf(notifie, sizeof(notifie),
             "am start -a android.intent.action.MAIN "
             "-e toasttext \"%s\" -n bellavita.toast/.MainActivity > /dev/null 2>&1",
             message);
    system(notifie);
}

void adjust_ionice_high(const char *pid) {
    char command[256];
    snprintf(command, sizeof(command), "ionice -c 2 -n 0 -p %s", pid);
    system(command);
}

void adjust_ionice_low(const char *pid) {
    char command[256];
    snprintf(command, sizeof(command), "ionice -c 1 -n 3 -p %s", pid);
    system(command);
}

int check_app_running(const char *app_name) {
    char command[512];
    snprintf(command, sizeof(command),
             "dumpsys SurfaceFlinger | grep Output | head -n 1 | cut -f1 -d/ | awk -F '(' '{print $2}' | grep -w \"%s\"",
             app_name);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        return 0;
    }

    char buffer[256];
    int is_running = 0;

    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        is_running = 1;
    }

    pclose(fp);
    return is_running;
}

int main() {
          // make to notification script running
     NFC("♨️ I/O Priority running program ");                    
    const char *primarypeth = "/sdcard/Apple/gamelist.txt";
    char line[1024];
    char app_in_running[1024] = "";

    while (1) {
        FILE *file = fopen(primarypeth, "r");

        if (file == NULL) {
            static int error_count = 0;
            if (error_count % 10 == 0) {
                printf("File gamelist.txt tidak ditemukan!\n");
            }
            error_count++;
            sleep(1);
            continue;
        }

        int app_found = 0;

        while (fgets(line, sizeof(line), file) != NULL) {
            line[strcspn(line, "\n")] = '\0';

            if (check_app_running(line)) {
                app_found = 1;

                if (strcmp(line, app_in_running) != 0) {
                    char message[512];
                    snprintf(message, sizeof(message),
                             "♨️ I/O Priority High : game %s", line);
                    NFC(message);
                    strcpy(app_in_running, line);

                    char cmd[512];
                    snprintf(cmd, sizeof(cmd), "pgrep -f %s", line);

                    FILE *pid_fp = popen(cmd, "r");
                    if (pid_fp != NULL) {
                        char pid[16];
                        if (fgets(pid, sizeof(pid), pid_fp) != NULL) {
                            pid[strcspn(pid, "\n")] = '\0';
                            adjust_ionice_high(pid);
                        }
                        pclose(pid_fp);
                    }
                }
                break;
            }
        }
        fclose(file);

        if (!app_found) {
            if (strlen(app_in_running) > 0) {
                NFC("♨️ I/O Priority Low : game close");
                strcpy(app_in_running, "");

                char cmd[512];
                snprintf(cmd, sizeof(cmd), "pgrep -f %s", app_in_running);

                FILE *pid_fp = popen(cmd, "r");
                if (pid_fp != NULL) {
                    char pid[16];
                    if (fgets(pid, sizeof(pid), pid_fp) != NULL) {
                        pid[strcspn(pid, "\n")] = '\0';
                        adjust_ionice_low(pid);
                    }
                    pclose(pid_fp);
                }
            }
        }

        sleep(5);
    }

    return 0;
}