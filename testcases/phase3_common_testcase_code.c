
#include <usloss.h>
#include <phase1.h>
#include <phase2.h>
#include <phase3.h>

#include "phase3_usermode.h"



/* dummy MMU structure, used for debugging whether the student called thee
 * right functions.
 */
int dummy_mmu_pids[MAXPROC];

int mmu_init_count = 0;
int mmu_quit_count = 0;
int mmu_switch_count = 0;



void startup(int argc, char **argv)
{
    for (int i=0; i<MAXPROC; i++)
        dummy_mmu_pids[i] = -1;

    /* all student implementations should have PID 1 (init) */
    dummy_mmu_pids[1] = 1;

    phase1_init();
    phase2_init();
    phase3_init();
    startProcesses();
}



/* force the testcase driver to priority 1, instead of the
 * normal priority for testcase_main
 */
int start3(char*);
static int start3_trampoline(char*);

int testcase_main()
{
    int pid_fork, pid_join;
    int status;

    pid_fork = fork1("start3", start3_trampoline, "start3", 4*USLOSS_MIN_STACK, 3);
    pid_join = join(&status);

    if (pid_join != pid_fork)
    {
        USLOSS_Console("*** TESTCASE FAILURE *** - the join() pid doesn't match the fork() pid.  %d/%d\n", pid_fork,pid_join);
        USLOSS_Halt(1);
    }

    return status;
}

static int start3_trampoline(char *arg)
{
    if (USLOSS_PsrSet(USLOSS_PSR_CURRENT_INT) != USLOSS_DEV_OK)
    {
        USLOSS_Console("ERROR: Could not disable kernel mode.\n");
        USLOSS_Halt(1);
    }

    int rc = start3(arg);

    Terminate(rc);
    return 0;    // Terminate() should never return
}



void mmu_init_proc(int pid)
{
    mmu_init_count++;

    int slot = pid % MAXPROC;

    if (dummy_mmu_pids[slot] != -1)
    {
        USLOSS_Console("TESTCASE ERROR: mmu_init_proc(%d) called, when the slot was already allocated for process %d\n", pid, dummy_mmu_pids[slot]);
        USLOSS_Halt(1);
    }

    dummy_mmu_pids[slot] = pid;
}

void mmu_quit(int pid)
{
    mmu_quit_count++;

    int slot = pid % MAXPROC;

    if (dummy_mmu_pids[slot] != pid)
    {
        USLOSS_Console("TESTCASE ERROR: mmu_quit(%d) called, but the slot didn't contain the expected pid.  slot: %d\n", pid, dummy_mmu_pids[slot]);
        USLOSS_Halt(1);
    }

    dummy_mmu_pids[slot] = -1;
}

void mmu_switch(int new_pid)
{
    mmu_switch_count++;

    int slot = new_pid % MAXPROC;

    if (dummy_mmu_pids[slot] != new_pid)
    {
        USLOSS_Console("TESTCASE ERROR: mmu_switch(%d) called, but the slot didn't contain the expected pid.  slot: %d\n", new_pid, dummy_mmu_pids[slot]);
        USLOSS_Halt(1);
    }
}



void phase4_start_service_processes()
{
    USLOSS_Console("%s() called -- currently a NOP\n", __func__);
}

void phase5_start_service_processes()
{
    USLOSS_Console("%s() called -- currently a NOP\n", __func__);
}



void finish(int argc, char **argv)
{
    USLOSS_Console("%s(): The simulation is now terminating.\n", __func__);
}

void test_setup  (int argc, char **argv) {}
void test_cleanup(int argc, char **argv) {}

