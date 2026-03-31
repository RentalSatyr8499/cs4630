#define _XOPEN_SOURCE 600 /* for strdup */

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <type_traits>
#include <cctype>

class Grader {
public:
    virtual void print_grade_for(const char *name) = 0;
    virtual void set_assignment(const char *assignment) = 0;
    virtual ~Grader() {}
};


class GraderImpl : public Grader {
public:
    GraderImpl() {
        assignment_ = "[unknown]";
        default_grade_ = "F";
    }

    void print_grade_for(const char *name) {
        const char *grade = default_grade_;
        if (0 == strcmp(name, "Draco Malfoy")) {
            grade = "A";
        }
        std::cout << "\n";
        std::cout << "I recommend " << name << " get a grade of " << grade 
                  << " for the " << assignment_ << " assignment." << std::endl;
    }

    void set_assignment(const char *assignment) {
        assignment_ = strdup(assignment);
    }

    void set_default_grade(const char *default_grade_) {
        default_grade_ = strdup(default_grade_);
    }
private:
    const char *assignment_;
    const char *default_grade_;
};


void print_escaped(const char *s) {
    size_t len = strlen(s);
    std::cout << '"';
    for (size_t i = 0; i < len; ++i) {
        if (std::isprint(s[i])) {
            std::cout << s[i];
        } else {
            char temp[8];
            sprintf(temp, "\\x%02x", (unsigned char) s[i]);
            std::cout << temp;
        }
    }
    std::cout << '"';
}

const int NUM_INFO = 3;

struct InfoTracker {
    char *data[NUM_INFO];

    InfoTracker() {
        for (int i = 0; i < NUM_INFO; ++i) {
            data[i] = (char*)"";
        }
    }

    void print(int i) {
        std::cout << "info[" << i << "]: ";
        print_escaped(data[i]);
        std::cout << "\n";
    }
};

const static int BUFFER_SIZE = 4096;

int read_argument(int last, char *buffer) {
    int i = 0, c = EOF;
    for (;;) {
        c = fgetc(stdin);
        if ((c == ' ' && !last) || c == '\n' || i == BUFFER_SIZE - 1 || c == EOF)
            break;
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    return c == '\n' || c == EOF;
}

InfoTracker *info_tracker = nullptr;
Grader *grader = nullptr;

int main() {
    static_assert(sizeof(GraderImpl) == sizeof(InfoTracker));
    for (;;) {
        printf("> ");
        char command[BUFFER_SIZE];
        char argument[BUFFER_SIZE];
        if (!read_argument(0, command)) {
            read_argument(1, argument);
        } else {
            argument[0] = '\0';
        }
        if (0 == strcmp(command, "setup-info")) {
            info_tracker = new InfoTracker;
            std::cout << "(info-tracker address " << (void*) info_tracker << ")\n";
        } else if (0 == strncmp(command, "set-info-", 9)) {
            int index = command[9] - '0';
            info_tracker->data[index] = new char[strlen(argument)+1];
            memcpy(info_tracker->data[index], argument, strlen(argument)+1);
            info_tracker->print(index);
        } else if (0 == strcmp(command, "free-info")) {
            delete info_tracker;
        } else if (0 == strcmp(command, "setup-grader")) {
            grader = new GraderImpl;
            grader->set_assignment(argument);
            std::cout << "(grader address " << (void*) grader << ")\n";
        } else if (0 == strcmp(command, "grade")) {
            grader->print_grade_for(argument);
        } else if (0 == strcmp(command, "exit")) {
            std::cout << "Exiting.\n";
            return EXIT_SUCCESS;
        } else if (0 == strcmp(command, "help")) {
            std::cout << "Available commands:\n"
                      << "  setup-info\n"
                      << "  set-info-0 STRING\n"
                      << "  set-info-1 STRING\n"
                      << "  set-info-2 STRING\n"
                      << "  free-info \n"
                      << "  setup-grader ASSIGNMENT\n"
                      << "  grade STUDENT\n"
                      << "  free-grader\n"
                      << "  exit\n";
        } else {
            std::cout << "Unrecognized command\n";
        }
        if (feof(stdin)) {
            std::cout << "End of input\n";
            return EXIT_FAILURE;
        }
    }
    return 0;
}
