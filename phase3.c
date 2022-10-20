
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
#include <string.h>
#include <phase3.h>

int currentSemID = 0;
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

Process *process_table[MAXPROC];
int current = 0;
int old_psr = 0;

void addToProcessTable(USLOSS_Sysargs *args)
{
    Process *new_node = malloc(sizeof(Process));
    new_node->id = current + 1;
    new_node->name = args->arg5;
    new_node->func = args->arg1;
    new_node->arg = args->arg2;
    new_node->stack_size = args->arg3;
    new_node->priority = args->arg4;

    process_table[current] = new_node;

    current += 1;
}

/**
 * @brief This function will set all of the elements
 * of the system call array with our syscallHandler()
 * function.
 * 
 */
void phase3_init(void)
{
    memset(&process_table, 0, sizeof(process_table));
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

void trampoline(void *arg)
{
    int old_psr = USLOSS_PsrGet();
    if (old_psr & 1 == 1)
    {
        USLOSS_PsrSet(old_psr & 254);
    }
    int pid = (int)arg;
    Process *process = process_table[pid];
    int retVal = process->func(process->arg);
    Terminate(retVal);
}

void syscallHandler(USLOSS_Sysargs *args)
{
    // Spawn() syscall
    if (args->number == SYS_SPAWN)
    {   
        addToProcessTable(args);
        void (*func_ptr)(void*) = &trampoline;
        int result = fork1(args->arg5, func_ptr, current-1, args->arg3, (int)args->arg4);
        args->arg1 = result;
        args->arg4 = 0;
        if (result < 0)
        {
            args->arg1 = -1;
            args->arg4 = -1;
        }
        // USLOSS_PsrSet(old);
        currentPID += 1;
    }

    // Wait() syscall
    else if (args->number == SYS_WAIT)
    {
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
        // Joins until we removed all processes
        int retVal = 0;
        while (retVal != -2) {
            int status;
            retVal = join(&status);
        }
        quit(args->arg1);
    }

    // SemCreate() syscall
    else if (args->number == SYS_SEMCREATE)
    {
        // TODO
        args->arg4 = 0;
        args->arg1 = currentSemID;
        if (args->arg1 < 0 || currentSemID >= MAXSEMS)
        {
            args->arg4 = -1;
            args->arg1 = 0;
        }
        currentSemID++;
    }

    // SemP() syscall
    else if (args->number == SYS_SEMP)
    {
        //while (args->arg1 == 0)
        //{
        //    ;
        //}
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
