
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <getopt.h>
#include <csignal>

const int SIZE = 256;

// ([^CPU die temperature:]*[0-9]{1,2}.{2,3})

void signal_handler(int signum) {
    std::cout << "Caught SIGINT" << "\n";
    exit(signum);
}

std::string get_all_pids() {
    char buffer[SIZE];
    std::string result = "";
    FILE* pipe = popen("ps -A | awk \'NR==2{print $1}\' | grep [0-9]", "r"); // getting all PID's
    if (!pipe) {
        std::cout << "Error: popen failed" << "\n";
        exit(EXIT_FAILURE);
    }

    while (!feof(pipe)) {
        if(fgets(buffer, SIZE, pipe) != NULL) {
            result += buffer;
        }
    }  
    pclose(pipe);
    return result; 
}

std::vector<std::string> get_pid() {
    char buffer[SIZE];
    std::vector<std::string> all_pid;
    std::string result;
    std::string current_row = "";
    int current_row_num = 1;
    while (current_row_num != 10) {
        current_row += current_row_num;
        FILE* pipe = popen("ps -A | awk \'NR==" + current_row + "{print $1}\' | grep [0-9]", "r"); // getting all PID's
        if (!pipe) {
            std::cout << "Error: popen failed" << "\n";
            exit(EXIT_FAILURE);
        }
        while (!feof(pipe)) {
            if(fgets(buffer, SIZE, pipe) != NULL) {
                result += buffer;
            }
        }  
        all_pid.push_back(current_row);
        current_row_num++;
    }
}

int main(int argc, char* argv[]) {
    int c;
    int help_flag;
    while (1) {
        int option_index;
        option_index = 0;
        static struct option long_options[] = {
            {"help", no_argument, NULL, 'h' },
            {0, 0, 0, 0}
        };

        c = getopt_long (argc, argv, "", long_options, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) { 
            case 'h':
                help_flag = 1;
                break;
            case '?':
                fprintf(stderr, "Error: incorrect usage\n");
                exit(EXIT_FAILURE);
            default:
                abort();
        }
    }

    // change indentation
    if (help_flag) {
        std::string correct_usage = "\nbetter_top [--help]";
        correct_usage += "\nhelp\toutputs correct usage of better top";
        std::cout << correct_usage;
    }   

    // ([^CPU die temperature:]*[0-9]{1,2}.{2,3})

//    int get_CPU_temp = system("sudo powermetrics --samplers smc | grep -i \"CPU die temperature: \"");
    //int get_CPU_temp = system("sudo powermetrics --samplers smc |grep -i \"CPU die temperature: [0-9]\"");
    //int get_CPU_temp = system("sudo powermetrics --samplers smc | grep -i ([^\"CPU die temperature:\"]*[0-9]{1,2}.{2,3})");
         //std::cout << "\n" << get_CPU_temp << "\n";

    signal(SIGINT, signal_handler);
    std::cout << get_all_pids() << "\n";
    exit(EXIT_SUCCESS);
}