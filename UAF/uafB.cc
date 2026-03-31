#define _XOPEN_SOURCE 600 /* for strdup */

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include <cctype>

class Grader {
public:
    virtual void print_grade_for(const char *name) = 0;
    virtual void set_assignment(const char *assignment) = 0;
    virtual ~Grader() {}
};

char grader_impl_default_grade = 'F';


class GraderImpl : public Grader {
public:
    GraderImpl() { num_graded_ = 0; }


    void print_grade_for(const char *name) {
        char grade = grader_impl_default_grade;
        if (0 == strcmp(name, "Draco Malfoy")) {
            grade = 'A';
        }
        if (grade == 'A') {
            print_A_grade(name);
        } else {
            print_non_A_grade(name, grade);
        }
        ++num_graded_;
    }

    void set_assignment(const char *assignment) {
        assignment_ = strdup(assignment);
    }

protected:
    /* virtual method so subclasses can customize this message! */
    virtual void print_non_A_grade(const char *name, char grade) {
        std::cout << "\n";
        std::cout << "Sorry, I recommend " << name << " get a grade of "
                  << grade << " for the " << assignment_ << " assignment." << std::endl;
    }

    /* virtual method so subclasses can customize this message! */
    virtual void print_A_grade(const char *name) {
        std::cout << "Congratulations!\n";
        std::cout << "I recommend " << name << " get a grade of A for the " << assignment_ << " assignment." << std::endl;
    }
    const char *assignment_;
    int num_graded_;
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

struct Student {
    char *name;
    char *id;
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
        std::cout << "> " << std::flush;
        char command[BUFFER_SIZE];
        char argument[BUFFER_SIZE];
        if (!read_argument(0, command)) {
            read_argument(1, argument);
        } else {
            argument[0] = '\0';
        }
        if (0 == strcmp(command, "setup-info")) {
            info_tracker = new InfoTracker;
            std::cout << "(info address " << (void*) info_tracker << ")\n";
        } else if (0 == strncmp(command, "set-info-", 9)) {
            int index = command[9] - '0';
            if (index >= NUM_INFO) {
                std::cout << "index " << index << " out of range";
            } else {
                info_tracker->data[index] = new char[BUFFER_SIZE];
                memcpy(info_tracker->data[index], argument, BUFFER_SIZE); // line 136
                info_tracker->print(index); 
            }
        } else if (0 == strcmp(command, "free-info")) {
            delete info_tracker;
        } else if (0 == strcmp(command, "setup-grader")) {
            grader = new GraderImpl;
            grader->set_assignment(argument);
            std::cout << "(grader address " << (void*) grader << ")\n";
        } else if (0 == strcmp(command, "grade")) {
            grader->print_grade_for(argument);
        } else if (0 == strcmp(command, "free-grader")) {
            delete grader;
        } else if (0 == strcmp(command, "exit")) {
            std::cout << "Exiting.\n";
            return EXIT_SUCCESS;
        } else if (0 == strcmp(command, "help")) {
            std::cout << "Available commands:\n"
                      << "  setup-info\n"
                      << "  set-info-0 STRING\n"
                      << "  set-info-1 STRING\n"
                      << "  set-info-2 STRING\n"
                      << "  free-info\n"
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
