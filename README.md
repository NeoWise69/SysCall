# SysCall
 This is a *tiny* (~300 lines of code) single-header command manager for handling and executing ***system*** commands, also by given priority
## Including
 Just include header and define implementation somewhere in '.cpp' file
 ```
 #define SYS_CALL_IMPLEMENTATION
 #include "SysCall.hpp"
 ```
 
 ## Example
 
 **Explicit Commands**
 ```
  // your code
  SysCall.RunExplicit("some command");
  // your code
 ```
  This will execute your command immediately. Just right now. All or nothing.
  If you declare several explicit tasks, it will be executed just in order.
  
  **Queued Commands(Tasks)** \
  Firstly, you need to push some commands(tasks):
  ```
  SysCall.AddTask({"pause", PriorityLow}); // surprisely, this task will execute last
  SysCall.AddTask({"print", PriorityMax}); // this one will execute first
  SysCall.AddTask({"pause", PriorityDefault}); // this one will be the second
  SysCall.AddTask({"pause"}); // this one is a task with the default constructor of 'PriorityDefault'
  SysCall.AddTask("pause"); // and this one also the same
  ```
  Right after all our tasks declared, we execute them just by:
  ```
  SysCall.Execute();
  ```
  
  **Inserting Commands(Tasks)**\
  You also may insert tasks by calling:
  ```
  SysCall.InsertTask("pause", "print");
  ```
  First parameter takes task that will be inserted, and second parameter is a 'key' to find task in queue.
  If queue do not have this task, it will do nothing (or may cause some problems, i'll fix this in near future).
  You can also do something like this:
  ```
  auto task = Syscall.AddTask("print");
  ```
  And then paste 'task' variable in:
  ```
  SysCall.InsertTask("pause", task);
  ```
  And it will works as well.
  
  ## Plans
  - Adding some more functionality (like, execute tasks in some range)
  - More code cleanup
  - Maybe, making it FULLY independent???
