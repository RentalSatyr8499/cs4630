use std::collections::{BTreeMap, VecDeque};
use std::io::{self, Write};
use std::rc::Rc;


#[derive(Debug)]
pub struct Student {
    id: u32,
    sis_id: u32,
    dropped: bool
}

impl Student {
    fn new(id: u32, sis_id: u32) -> Rc<Student> {
        Rc::new(Student{
            id,
            sis_id,
            false
        })
    }

    fn show(&self) {
        print!("Student#{} [SIS ID#{}]", self.id, self.sis_id);
        if self.dropped{
            print!(" (dropped)");
        }
    }
}

#[derive(Debug)]
pub struct Submission {
    grade: Option<u32>,
    student1: Rc<Student>,
    student2: Rc<Student>,
}

impl Submission {
    pub fn new(student1: Rc<Student>, student2: Rc<Student>) -> Rc<Submission> {
        Rc::new(Submission{
            grade: None,
            student1: Rc::clone(&student1),
            student2: Rc::clone(&student2)
        })
    }

    pub fn show_grade(&self) {
        match self.grade {
            None => print!("[ungraded]"),
            Some(grade) => print!("grade {}", grade),
        }
    }

    pub fn show(&self) {
        print!("Submission for ");

        if Rc::strong_count(&self.student1) == 1{
            print!("STUDENT_NOT_IN_CLASS_REGISTRY")
        } else {
            self.student1.show();
        }

        print!(" and ");

        if Rc::strong_count(&self.student2) == 1{
            print!("STUDENT_NOT_IN_CLASS_REGISTRY")
        } else {
            self.student2.show();
        }

        print!(" ");
        self.show_grade();
        print!("(Rc count for student1 is {}, for student 2 it's {})", Rc::strong_count(&self.student1), Rc::strong_count(&self.student1));
    }
}

#[derive(Debug)]
pub struct Class {
    students: BTreeMap<u32, Rc<Student>>,
    graded_submissions: VecDeque<Rc<Submission>>,
    ungraded_submissions: VecDeque<Rc<Submission>>,
}

fn show_submission_deque(submissions: &VecDeque<Rc<Submission>>) {
    for submission in submissions.iter() {
        submission.show();
        print!("\n");
    }
}

impl Class {
    pub fn new() -> Class {
        Class {
            students: BTreeMap::new(),
            graded_submissions: VecDeque::new(),
            ungraded_submissions: VecDeque::new(),
        }
    }

    pub fn show(&self) {
        for (_k, v_ref) in self.students.iter() {
            // (k, v_ref) is a (&u32, &Rc<Student>)
            Rc::clone(v_ref).show();
            print!("\n");
        }
        println!("Graded submissions:");
        show_submission_deque(&self.graded_submissions);
        println!("Ungraded submission [next-to-grade last]:");
        show_submission_deque(&self.ungraded_submissions);
    }
}

pub fn show_help() {
    print!(concat!(
           "Usage:\n",
           "  show\n",
           "  new-student ID SIS-ID\n",
           "  new-submission STUDENT-ID1 STUDENT-ID2\n",
           "  show-next\n",
           "  grade-next GRADE\n",
           "  drop-student ID\n",
           "  exit\n"));
}

pub fn run_command(class: &mut Class, command: &str, args: Vec<u32>) {
    match command {
        "help" => show_help(),
        "show" => {
            if args.len() != 0 {
                show_help();
            } else {
                class.show();
            }
        }
        "new-student" => {
            if args.len() != 2 {
                show_help();
            } else {
                class.students.insert(
                    args[0],
                    Student::new(args[0], args[1])
                );
                println!("Added student #{}", class.students.len());
            }
        }
        "new-submission" => {
            if args.len() != 2 {
                show_help();
            } else {
                match (class.students.get(&args[0]), class.students.get(&args[1])) {
                    (Some(student_ptr_ref1), Some(student_ptr_ref2)) => {
                        let submission = Submission::new(
                            student_ptr_ref1.clone(), 
                            student_ptr_ref2.clone()
                        );
                        class.ungraded_submissions.push_front(submission);
                        println!("Created submission");
                    },
                    _ => {
                        eprintln!("Could not find students")
                    }
                }
            }
        },
        "show-next" => {
            if args.len() != 0 {
                show_help();
            } else {
                match class.ungraded_submissions.back() {
                    Some(submission_ptr_ref) => {
                        Rc::clone(&submission_ptr_ref).show();
                        print!("\n");
                    },
                    None => {
                        eprintln!("No submissions to grade.");
                    },
                }
            }
        },
        "grade-next" => {
            if args.len() != 1 {
                show_help();
            } else {
                match class.ungraded_submissions.pop_back() {
                    Some(submission_ptr) => {
                        let graded_submission = Rc::new(
                            Submission{
                                grade: Some(args[0]),
                                student1: Rc::clone(&submission_ptr.student1),
                                student2: Rc::clone(&submission_ptr.student2)
                            }
                        );
                        class.graded_submissions.push_back(graded_submission);
                    },
                    None => {
                        eprintln!("No submissions to grade.");
                    },
                }
            }
        },
        "drop-student" => {
            if args.len() != 1 {
                show_help();
            } else {
                match class.students.remove(&args[0]) {
                    Some(_student_ptr) => {
                        println!("Removed student '{}'", args[0]);
                    },
                    None => {
                        eprintln!("No such student '{}'", args[0])
                    }
                }
            }
        }
        _ => {
            show_help();
        }
    }
}

fn main() {
    let mut class = Class::new();
    loop {
        let mut line = String::new();
        print!("> ");
        io::stdout().flush().expect("I/O error");
        io::stdin().read_line(&mut line).expect("error reading input");
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.len() == 0 {
            show_help();
            continue
        }
        if parts[0] == "exit" {
            break
        }
        let mut args: Vec<u32> = Vec::new();
        for arg_str in &parts[1..] {
            match arg_str.parse::<u32>() {
                Ok(value) => args.push(value),
                _ => {
                    eprintln!("'{}' is not a non-negative number.", arg_str);
                    break;
                }
            }
        }
        run_command(&mut class, parts[0], args);
    }
}
