// 
// tsh - A tiny shell program with job control
// 
// <Put your name and login ID here>
//

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"

//
// Needed global variable definitions
//

static char prompt[] = "tsh> ";
int verbose = 0;

//
// You need to implement the functions eval, builtin_cmd, do_bgfg,
// waitfg, sigchld_handler, sigstp_handler, sigint_handler
//
// The code below provides the "prototypes" for those functions
// so that earlier code can refer to them. You need to fill in the
// function bodies below.
// 

void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

//
// main - The shell's main routine 
//
int main(int argc, char *argv[]) 
{
  int emit_prompt = 1; // emit prompt (default)

  //
  // Redirect stderr to stdout (so that driver will get all output
  // on the pipe connected to stdout)
  //
  dup2(1, 2);

  /* Parse the command line */
  char c;
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
    case 'h':             // print help message
      usage();
      break;
    case 'v':             // emit additional diagnostic info
      verbose = 1;
      break;
    case 'p':             // don't print a prompt
      emit_prompt = 0;  // handy for automatic testing
      break;
    default:
      usage();
    }
  }

  //
  // Install the signal handlers
  //

  //
  // These are the ones you will need to implement
  //
  Signal(SIGINT,  sigint_handler);   // ctrl-c
  Signal(SIGTSTP, sigtstp_handler);  // ctrl-z
  Signal(SIGCHLD, sigchld_handler);  // Terminated or stopped child

  //
  // This one provides a clean way to kill the shell
  //
  Signal(SIGQUIT, sigquit_handler); 

  //
  // Initialize the job list
  //
  initjobs(jobs);

  //
  // Execute the shell's read/eval loop
  //
  for(;;) {
    //
    // Read command line
    //
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }

    char cmdline[MAXLINE];

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
      app_error("fgets error");
    }
    //
    // End of file? (did user type ctrl-d?)
    //
    if (feof(stdin)) {
      fflush(stdout);
      exit(0);
    }

    //
    // Evaluate command line
    //
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  } 

  exit(0); //control never reaches here
}
  
/////////////////////////////////////////////////////////////////////////////
//
// eval - Evaluate the command line that the user has just typed in
// 
// If the user has requested a built-in command (quit, jobs, bg or fg)
// then execute it immediately. Otherwise, fork a child process and
// run the job in the context of the child. If the job is running in
// the foreground, wait for it to terminate and then return.  Note:
// each child process must have a unique process group ID so that our
// background children don't receive SIGINT (SIGTSTP) from the kernel
// when we type ctrl-c (ctrl-z) at the keyboard.
//
void eval(char *cmdline) 
{
  /* Parse command line */
  //
  // The 'argv' vector is filled in by the parseline
  // routine below. It provides the arguments needed
  // for the execve() routine, which you'll need to
  // use below to launch a process.
  //
  char *argv[MAXARGS];
  //
  // The 'bg' variable is TRUE if the job should run
  // in background mode or FALSE if it should run in FG
  //
  int bg = parseline(cmdline, argv);
    
    sigset_t mask; // mask to block SIGCHLD
  
  if(!builtin_cmd(argv))
  {
      // Block SIGCHLD before forking to avoid a Race
      sigemptyset(&mask);
      sigaddset(&mask, SIGCHLD);
      sigprocmask(SIG_BLOCK, &mask, NULL);
      
      pid_t pid = fork();
      
      if(pid == 0) // if we are in the child...
      {
          sigprocmask(SIG_BLOCK, &mask, NULL); // Unblock SIGCHLD when in child
          // make child pid it's own unique group ID to avoid SIGINT/SIGSTP from ctrl-c/z
          setpgid(0,0);
          if(execv(argv[0], argv) < 0) // load the command and check for error
          {
              // if the command doesn't exist print a massage
              printf("%s: Command not Found \n", argv[0]);
              exit(0);
          }
      }
      else
      {
          // if the parent is the background add the job to background job list
          if (bg) 
          {
              addjob(jobs, pid, BG, cmdline);
          }
          // if the parent is in the foreground add job foreground job list
          else
          {
              addjob(jobs, pid, FG, cmdline); 
          }
          //unblock signals
          sigprocmask(SIG_UNBLOCK, &mask, NULL);
          // if the parent is the background print out job id, pid, and command
          if (bg)
          {
              printf("[%d] (%d) %s",pid2jid(pid), pid, cmdline);
          }
           // if the parent is in the foreground wait for foreground process to complete
          else
          {
              waitfg(pid);
          }
      }
  }
  if (argv[0] == NULL)  
  return;   /* ignore empty lines */
    
  return;
}


/////////////////////////////////////////////////////////////////////////////
//
// builtin_cmd - If the user has typed a built-in command then execute
// it immediately. The command name would be in argv[0] and
// is a C string. We've cast this to a C++ string type to simplify
// string comparisons; however, the do_bgfg routine will need 
// to use the argv array as well to look for a job number.
//
int builtin_cmd(char **argv) 
{
  string cmd(argv[0]);
    //built in command quit
    if(cmd == "quit")
    {
        exit(0);
    }
    //built in command jobs
    if(cmd == "jobs")
    {
        //list jobs
        listjobs(jobs);
        return 1;
    }
    //built in command fg and bg
    if(cmd == "fg" || cmd == "bg")
    {
        do_bgfg(argv);
        return 1;
    }
    //background process
    if(cmd == "&")
    {
        return 1;
    }
  return 0;     /* not a builtin command */
}

/////////////////////////////////////////////////////////////////////////////
//
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) 
{
  struct job_t *jobp=NULL;
    
  /* Ignore command if no argument */
  if (argv[1] == NULL) {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }
    
  /* Parse the required PID or %JID arg */
  if (isdigit(argv[1][0])) {
    pid_t pid = atoi(argv[1]);
    if (!(jobp = getjobpid(jobs, pid))) {
      printf("(%d): No such process\n", pid);
      return;
    }
  }
  else if (argv[1][0] == '%') {
    int jid = atoi(&argv[1][1]);
    if (!(jobp = getjobjid(jobs, jid))) {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }	    
  else {
    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

  //
  // You need to complete rest. At this point,
  // the variable 'jobp' is the job pointer
  // for the job ID specified as an argument.
  //
  // Your actions will depend on the specified command
  // so we've converted argv[0] to a string (cmd) for
  // your benefit.
  //
  string cmd(argv[0]);
    //restart job by sending SIGCONT signal first
    kill(-jobp->pid, SIGCONT);
    //now change the state of the job
    if(cmd == "bg")
    {
        //print job id, pidm and command line
        printf("[%d] (%d) %s", jobp->jid, jobp->pid, jobp->cmdline);
        //change the state of the job
        jobp->state = BG;
    }
    else if(cmd == "fg")
    {
        //change the job state to foreground before running
        jobp->state = FG;
        //wait until current foreground process terminates before running it
        waitfg(jobp->pid);
    }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// waitfg - Block until process pid is no longer the foreground process
//
void waitfg(pid_t pid)
{
    //loop until the state of the the job is no longer foreground 
    while (pid == fgpid(jobs))
    {
        //block process until signal is recieved 
        sleep(1);
    }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// Signal handlers
//


/////////////////////////////////////////////////////////////////////////////
//
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//     a child job terminates (becomes a zombie), or stops because it
//     received a SIGSTOP or SIGTSTP signal. The handler reaps all
//     available zombie children, but doesn't wait for any other
//     currently running children to terminate.  
//
void sigchld_handler(int sig) 
{
    pid_t pid;
    int status;
    //waitpid returns 0 or a value less than 0 when none of the children in the wait set have terminated/stopped or when there is an error if the calling process has no children
    //the pid/first argument being set to 1 means that the wait set consists of al the parent's child process
    //this loop goes through entire wait set and returns values above 0 that are process pid's
    while((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0)
    {
        //find job by process pid
        job_t *job = getjobpid(jobs, pid);
        //if process if stopped
        if(WIFSTOPPED(status))
        {
            //change job state to stopped
            job->state = ST;
            printf("Job [%d] (%d) stopped by signal %d\n", job->jid, pid, WSTOPSIG(status));
        }
        //if terminated process did not catch signal
        if(WIFSIGNALED(status))
        {
            printf("Job [%d] (%d) terminated by signal %d\n", job->jid, pid, WTERMSIG(status));
            //delete job from job list via pid
            deletejob(jobs,pid);
        }
        //if process terminated normally 
        if(WIFEXITED(status))
        {
            //delete job from job list via pid
            deletejob(jobs,pid);
        }
    }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigint_handler - The kernel sends a SIGINT to the shell whenver the
//    user types ctrl-c at the keyboard.  Catch it and send it along
//    to the foreground job.  
//
void sigint_handler(int sig) 
{
    pid_t pid;
    //if pid is 0 then there is no foreground job and so nothing will happen but if pid is not zero then...
    if((pid = fgpid(jobs)) != 0)
    {
        //send SIGINT to current foreground process
        kill(-pid, sig);
    }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//     the user types ctrl-z at the keyboard. Catch it and suspend the
//     foreground job by sending it a SIGTSTP.  
//
void sigtstp_handler(int sig) 
{
    pid_t pid;
    //if pid is 0 then there is no foreground job and so nothing will happen but if pid is not zero then...
    if((pid = fgpid(jobs)) != 0)
    {
        //send SIGSTP to current foreground process
        kill(-pid, sig);
    }
  return;
}

/*********************
 * End signal handlers
 *********************/




