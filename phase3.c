/**
 * @file phase3.c
 * @author your name (you@domain.com)
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


// TODO: Handle SyscallIntVec[]

typedef struct Semaphore Semaphore;

struct Semaphore
{
    int id;
    int value;
};

int currentID = 1;

/**
 * @brief TODO
 * 
 */
void phase3_init(void)
{

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
    if (args->number == SYS_SPAWN)
    {
        int result = fork1(args->arg5, args->arg1, args->arg2, args->arg3, args.4);
        if (result <= 0)
        {
            args->arg1 = -1;
            args->arg4 = -1;
        }
        args->arg1 = result;
        args->arg4 = 0;
    }

    if (args->number == SYS_WAIT)
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

    if (args->number == SYS_TERMINATE)
    {
        // Joins until we removed all processes
        while (join(args->arg1) != -2)
        {
            ;
        }
        quit(args->arg1);
    }

    if (args->number == SYS_SEMCREATE)
    {
        // TDOD
        Semaphore new;
        new.id = currentID;
        new.value = args->arg1;
        args->arg4 = 0;
        if (args->arg1 < 0)
        {
            args->arg4 = -1;
        }
        currentID++;
    }

    if (args->number == SYS_SEMP)
    {
        // TODO
    }

    if (args->number == SYS_SEMV)
    {
        // TODO
    }
    
    if (args->number == SYS_GETTIMEOFDAY)
    {
        args->arg1 = currentTime();
    }

    if (args->number == SYS_GETPROCINFO)
    {
        args->arg1 = readtime();
    }

    if (args->number == SYS_GETPID)
    {
        args->arg1 = getpid();
    }
}


/**
 * @brief TODO
 * 
 * @param value 
 * @param semaphore 
 * @return int 
 */
int SemCreate(int value, int *semaphore)
{
    if (value < 0)
    {
        return -1;
    }
    return 0;
}

/**
 * @brief TODO
 * 
 * @param semaphore 
 * @return int 
 */
int SemP(int semaphore)
{

}

/**
 * @brief TODO
 * 
 * @param semaphore 
 * @return int 
 */
int SemV(int semaphore)
{

}