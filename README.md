<h1 align="center">
	📖 Pipex
</h1>
<br>
<p align="center">
<img width="500" src="https://github.com/user-attachments/assets/86d11c1e-1f62-4743-bb2f-9928eb6e3a87"
/>
</p>

Development repo for 42cursus Pipex project

About

This is a project in daily use. If you want to see the project that i have been evaluated in 42school click in releases at the right side!
🗣️ About

    Pipex is a project that re-creates in C the way two commands are piped together via | in the shell


Reproduction of shell pipes
The beginning

To understand how to re-create the pipe used in shell we must understand the usage of those functions

    fork( )
    pipe( )
    dup2( )
    execve( )

i will give you a small starting point of each of them them at the very end of this file you find resources not just for those functions but for the whole project.
The fork( ) function

Understanding fork( )

The fork( ) function in C is a system call that creates a new process, which is a copy of the current process. This new process is called a child process, and the original process is the parent process.
How it works

    When fork() is called, two things happen:

    A new process is created: The operating system creates a copy of the parent process, including its memory space, open files, and other resources.
    Return value: The fork() function returns a value to both the parent and child processes:

    - In the parent process, fork() returns the process ID (PID) of the child process.
    - In the child process, fork() returns 0.

Here is an example.

#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork");
        return (1);
    } 
    else if (pid == 0)
    {
        // Child process
        printf("I am the child process\n");
    } 
    else
    {
        // Parent process
        printf("I am the parent process\n");
    }

    return 0;
}

Don not forget to test every example so you can understand what is going on
Understanding the pipe ( ) function

The pipe() function in C is a system call used for inter-process communication (IPC). It creates a unidirectional communication channel between two processes. Data written to one end of the pipe can be read from the other end. That mean you can create a connection between those two processes using pipe(here is where the magic happens).
How it works

    Creation:

    pipe() takes an array of two integers as input.
    It creates a pipe and stores file descriptors for the read and write ends of the pipe in the first and second elements of the array respectively.
    Returns 0 on success, -1 on failure.

    Reading and Writing:

    The process that holds the write end can write data to the pipe using write().
    The process that holds the read end can read data from the pipe using read().
    The pipe has a limited buffer size. If a process writes more data than the buffer can hold, the write operation blocks until space is available.
    If a process tries to read from an empty pipe, the read operation blocks until data is available.

Heres a example code. If you do not understand yet, dont worry. The next snippet will make more sense, but yet, test this one.

#include <stdio.h>
#include <unistd.h>

int main() {
    int pipefd[2];
    char buffer[1024];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0) // Child process
    {
        close(pipefd[0]); // Close the read end
        write(pipefd[1], "Hello from child\n", 14);
        close(pipefd[1]);
    }
    else // Parent process
    {
        close(pipefd[1]); // Close the write end
        read(pipefd[0], buffer, 1024);
        printf("Parent received: %s", buffer);
        close(pipefd[0]);
    }

    return 0;
}

Pipes and Forks: A Collaborative Duo
Understanding the Basics

    Before diving into how pipe() and fork() work together, let's recap their individual functions:

    fork(): Creates a child process, which is a copy of the parent process. Both processes share the same memory space initially (copy-on-write).
    pipe(): Creates a unidirectional communication channel between two processes. Data written to one end can be read from the other.

Combining pipe() and fork()

A common use case for these functions is to create a parent-child relationship where data is passed from the parent to the child or vice versa through a pipe.

Here's a breakdown of the steps involved:

    Create a pipe: Use pipe() to create a pipe, obtaining two file descriptors: one for reading (read end) and one for writing (write end).

    Fork a child process: Use fork() to create a child process. Both the parent and child processes will have copies of the file descriptors.

    Close unnecessary file descriptors: In the parent process, close the read end of the pipe as it will be used for writing. In the child process, close the write end of the pipe as it will be used for reading.

    Inter-process communication: The parent process can write data to the write end of the pipe. The child process can read data from the read end of the pipe.

Example:

#include <stdio.h>
#include <unistd.h>

int main() 
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) // Child process
    {
        close(pipefd[1]); // Close write end
        char buffer[100];
        read(pipefd[0], buffer, 100);
        printf("Child process received: %s\n", buffer);
        close(pipefd[0]);
    }
    else  // Parent process
    {
        close(pipefd[0]); // Close read end
        write(pipefd[1], "Hello from parent\n", 16);
        close(pipefd[1]);
    }

    return 0;
}

All of this should give you a starting point. But dont forget to check the resources at the end of this file.
FDs

pipex is run like this ./pipex infile cmd1 cmd2 outfile
FDs 0, 1 and 2 are by default assigned to stdin, stdout and stderr
infile, outfile, the pipe, the stdin and stdout are all FDs
On linux, you can check your fds currently open with the command ls -la /proc/$$/fd

Our fd table right now looks like this:

                           -----------------    
                 0         |     stdin     |  
                           -----------------    
                 1         |     stdout    |    
                           -----------------    
                 2         |     stderr    |  
                           -----------------
                 3         |     infile    |  // open()
                           -----------------
                 4         |     outfile   |  // open()
                           -----------------
                 5         |     end[0]    | 
                           -----------------
                 6         |     end[1]    |  
                           -----------------

Swapping fds with dup2()

For the child process, we want infile to be our stdin (as input), and end[1] to be our stdout (we write to end[1] the output of cmd1)
In the parent process, we want end[0] to be our stdin (end[0] reads from end[1] the output of cmd1), and outfile to be our stdout (we write to it the output of cmd2)
Visually,

// each cmd needs a stdin (input) and returns an output (to stdout)
   
    infile                                             outfile
as stdin for cmd1                                 as stdout for cmd2            
       |                        PIPE                        ↑
       |           |---------------------------|            |
       ↓             |                       |              |
      cmd1   -->    end[1]       ↔       end[0]   -->     cmd2           
                     |                       |
            cmd1   |---------------------------|  end[0]
           output                             reads end[1]
         is written                          and sends cmd1
          to end[1]                          output to cmd2
       (end[1] becomes                      (end[0] becomes 
        cmd1 stdout)                           cmd2 stdin)

We swap fds to stdin/stdout with dup2()
From the MAN,

int dup2(int fd1, int fd2) : it will close fd2 and duplicate the value of fd2 to fd1
else said, it will redirect fd1 to fd2

In pseudo code:

# child_process(f1, cmd1); // add protection if dup2() < 0
// dup2 close stdin, f1 becomes the new stdin
dup2(f1, STDIN_FILENO); // we want f1 to be execve() input
dup2(end[1], STDOUT_FILENO); // we want end[1] to be execve() stdout
close(end[0]) # --> always close the end of the pipe you don't use,
                    as long as the pipe is open, the other end will 
                    be waiting for some kind of input and will not
                    be able to finish its process
close(f1)
// execve function for each possible path (see below)
exit(EXIT_FAILURE);

Parent process in pseudo code will be similar
It needs a waitpid() at the very beginning to wait for the child to finish her process

# parent_process(f2, cmd2);
int status;
waitpid(-1, &status, 0);
dup2(f2, ...); // f2 is the stdout
dup2(end[0], ...); // end[0] is the stdin
close(end[1])
close(f2);
// execve function for each possible path (see below)
exit(EXIT_FAILURE);

Executing with execve()

From the MAN,

int execve(const char *path, char *const argv[], char *envp[]);
/*
path: the path to our command
type `which ls` and `which wc` in your terminal
you'll see the exact path to the commands' binaries

argv[]: the args the command needs, for ex. `ls -la`
you can use your ft_split to obtain a char **
like this { "ls", "-la", NULL }
it must be null terminated

envp: environmental variable -> retrieved from main (see below)
in envp the line PATH contains all possible paths to the commands' binaries
type env in the terminal to have a look
split on : to retrieve all possible PATHs 
*/
int main(int ac, char **ag, char **envp)
{
     int f1;
     int f2;
     f1 = open(ag[1], O_RDONLY);
     f2 = open(ag[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
     if (f1 < 0 || f2 < 0)
          return (-1);
     pipex(f1, f2, ag, envp);
     return (0);
}

execve() will try every possible path to the cmd until it finds the good one
If the command does not exist, execve() will do nothing and return -1;
else, it will execute the cmd and delete all ongoing processes (so no leaks)

In pseudo code,

// parsing (somewhere in your code) char *PATH_from_envp;
char **mypaths;
char **mycmdargs; // retrieve the line PATH from envp

PATH_from_envp = ft_substr(envp ....);
mypaths = ft_split(PATH_from_envp, ":");
mycmdargs = ft_split(ag[2], " ");// in your child or parent process
int  i;
char *cmd;

i = -1;
while (mypaths[++i])
{
    cmd = ft_join(mypaths[i], ag[2]); // protect your ft_join
    execve(cmd, mycmdargs, envp); // if execve succeeds, it exits
    // perror("Error"); <- add perror to debug
    free(cmd) // if execve fails, we free and we try a new path
}
return (EXIT_FAILURE);

Creating a pipe with two child processes

void    pipex(int f1, int f2, char *cmd1, char *cmd 2)
{
    int   end[2];
    int   status;
    pid_t child1;
    pid_t child2;    pipe(end);
    child1 = fork();
    if (child1 < 0)
         return (perror("Fork: "));
    if (child1 == 0)
        child_one(f1, cmd1);
    child2 = fork();
    if (child2 < 0)
         return (perror("Fork: "));
    if (child2 == 0)
        child_two(f2, cmd2);
    close(end[0]);         // this is the parent
    close(end[1]);         // doing nothing
    waitpid(child1, &status, 0);  // supervising the children
    waitpid(child2, &status, 0);  // while they finish their tasks
}

Using access()

If the command that does not exist, execve() will execute nothing without error messages
You need to check if the command exists before its execution with access(), else send an error pipex: weirdcmd: weirdcmd not found
Important tips to keep in mind

    test your pipex with this cmd

./pipex infile "sleep 5" "sleep" outfile

this sould not wait 10s on screen, but only 5. Compare with the same cmd in bash. Might be the wait().

    You might need to change your split to handle special arguments such as:

~$ < infile tr pe ' X' |  tr pi ' P' > outfile

See how works on bash and if you can do the same in yout pipex. Your split at this point pobabily dont handle to separate CMD like this

./pipex infile "tr pe ' X'" "tr pi ' P'" outfile

nothing like a pain to change your ft_split to consider this special cases, but dont stop when they're done. Look for more special cases and see if your split handle them the way it should.
Debugging

[0] When splitting the env, print out the result of split. Add a / at the end for the path to work correctly.

[1] If the program gets stuck without executing anything, most probably the pipe ends are not closed correctly. Until one end is open, the other will be waiting for input and its process will not finish.

[2] Place perror("Error") in your code, especially right after fork() or execve() , to see what is going on in the pipe. Here's a good moment to use your _fd functions. Inside the pipe, everything we do will go to one of its ends. printf for ex. won’t print to the terminal, it will print to your outfile (because we swapped the stdout) perror("Error") will work because it prints to stderr. You can also use write(2, "example", 7) if you want. Remember you got _fd functions as well.

[3] Handle file rights when open()ing them. Return error if the file cannot be opened, read or written. access() comes handy here. Check how the shell treats infile and outfile when they do not exist, are not readable, writable etc. (chmod is your best friend).
Great Resources for Pipex

Unix processes - Youtube playlist

Compreensive guide to pipex - article

For detailed information, refer to the subject of the project
