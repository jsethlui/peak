
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <getopt.h>
#include <csignal>
#include <vector>
#include <string>
#include <sstream>

const int SIZE = 256;

// ([^CPU die temperature:]*[0-9]{1,2}.{2,3})

void signal_handler(int signum) {
    std::cout << "Caught SIGINT" << "\n";
    exit(signum);
}

void print_vector(const std::vector<std::string> a_vector) {
    for (int i = 0; i < a_vector.size(); i++) {
        std::cout << "[" << i << "]: " << a_vector[i] << "\n";
    }
}

std::vector<std::string> split_string(const std::string &str, char delimiter) {
    std::vector<std::string> result_vector;
    std::stringstream ss(str);
    std::string current_str;
    while (std::getline(ss, current_str, delimiter)) {
        result_vector.push_back(current_str);
    }
    return result_vector;
}

void get_pid() {
    char buffer[SIZE];
    std::vector<std::string> all_pid;
    std::string result;
    int current_row = 1;
    // getting all PID's as one contained string
    while (current_row != 10) {
        std::string current_command = "ps -A | awk \'NR==" + std::to_string(current_row) + "{print $1}\' | grep [0-9]";
        FILE* pipe = popen(current_command.c_str(), "r"); // getting all PID's
        if (!pipe) {
            std::cout << "Error: popen failed" << "\n";
            exit(EXIT_FAILURE);
        }
        while (!feof(pipe)) {
            if(fgets(buffer, SIZE, pipe) != NULL) {
                result += buffer;
            }
        }  
        
        current_row++;
    }
    all_pid = split_string(result, '\n');   // splitting PID string by newline characters 
    print_vector(all_pid);
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

//    signal(SIGINT, signal_handler);
    get_pid();
    std::cout << "\n";
    exit(EXIT_SUCCESS);
}