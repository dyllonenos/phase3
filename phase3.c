/**
 * @file phase3.c
 * @author Nicholas Eng and Dyllon Enos
 * @brief 
 * @version 0.1
 * @date 2022-10-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <phase1.h>
#include <phase2.h>
#include <stdio.h>
#include <stdlib.h>
#include <usloss.h>
#include <usyscall.h>

int currentSemID = 1;
int currentPID = 1;

void syscallHandler(USLOSS_Sysargs *args);
typedef struct Process Process;
struct Process
{
    int id;
    char *name;
    int (*func)(char*);
    char *arg;
    int stack_size;
    int priority;
};

/**
 * @brief This function will set all of the elements
 * of the system call array with our syscallHandler()
 * function.
 * 
 */
void phase3_init(void)
{
    for (int index = 0; index < MAXSYSCALLS; index++)
    {
        systemCallVec[index] = &syscallHandler;
    }
}

/**
 * @brief TODO
 * 
 */
void phase3_start_service_processes(void)
{
}

void syscallHandler(USLOSS_Sysargs *args)
{
    int old = USLOSS_PsrGet();
    USLOSS_PsrSet(2);
    // Spawn() syscall
    if (args->number == SYS_SPAWN)
    {       
        // int result = fork1(args->arg5, args->arg1, args->arg2, args->arg3, args->arg4);
        int (*func)(char *) = args->arg1;
        char *arg = args->arg2;
        
        func(arg);
        // USLOSS_PsrSet(old);
        currentPID += 1;
    }

    // Wait() syscall
    else if (args->number == SYS_WAIT)
    {
        USLOSS_PsrSet(old);
        int status = 0;
        int result = join(&status);
        args->arg1 = result;
        args->arg2 = status;
        args->arg4 = 0;

        if (result == -2)
        {
            args->arg4 = -2;
        }
        
        return 0;

    }

    // Terminate() syscall
    else if (args->number == SYS_TERMINATE)
    {
        USLOSS_PsrSet(old);
        // Joins until we removed all processes
        while (join(args->arg1) != -2)
        {
            ;
        }
        quit(args->arg1);
    }

    // SemCreate() syscall
    else if (args->number == SYS_SEMCREATE)
    {
        // TODO
        args->arg4 = 0;
        if (args->arg1 < 0)
        {
            args->arg4 = -1;
        }
        args->arg1 = currentSemID;
        currentSemID++;
    }

    // SemP() syscall
    else if (args->number == SYS_SEMP)
    {
        while (args->arg1 == 0)
        {
            ;
        }
        args->arg1 -= 1;
        args->arg4 = 0;
        if (args->arg1 < 1)
        {
            args->arg4 = -1;
        }
    }

    // SemV() syscall
    else if (args->number == SYS_SEMV)
    {
        args->arg1 += 1;
        args->arg4 = 0;
        if (args->arg1 < 1)
        {
            args->arg4 = -1;
        }
    }

    // GetTimeofDay() syscall
   else if (args->number == SYS_GETTIMEOFDAY)
    {
        args->arg1 = currentTime();
    }

    // CPUTime() syscall
    else if (args->number == SYS_GETPROCINFO)
    {
        args->arg1 = readtime();
    }

    // GetPID() syscall
    else if (args->number == SYS_GETPID)
    {
        args->arg1 = getpid();
    }
}
