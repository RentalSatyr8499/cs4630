I put the Student object behind Rc. Every time a new structure needs to store a Student, it uses Rc.clone().

In theory, using raw pointers requires close to zero overhead, while smart pointers add overhead. But because the program is so small, and the original unsafe{} approach also required bookkeeping operations that the Rc approach did not, there is likely not a noticeable difference in the performance of the program. 

If the program became more robust and required more operations, especially on the Student object, then my version of the program (uses Rc) might be slightly more costly than the original version (uses raw pointers). Although if enough manual bookkeeping and safety checks are added to the original raw pointer approach, the Rc approach might end up being faster.

But if you think safety is also an aspect of performance, my version is superior, because it avoids unsafe operations.