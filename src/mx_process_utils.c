#include "../inc/ush.h"

static int set_process_status(t_cmd_utils* utils, pid_t pid, int status) {

    if (pid > 0) {

        for (t_process* p = utils->processes; p; p = p->next) {

            if (p->pid == pid) {
                
                p->status = status;
                if (WIFSTOPPED (status)) {
                    p->stopped = 1;
                    mx_created_process_push_back(&utils->stopped_jobs, p);
                    printf("Process stopped, pid: %d\n", (int)pid);
                } else {
                    p->completed = 1;
                    if (WIFSIGNALED (status))
                        fprintf (stderr, "%d: Terminated by signal %d.\n", (int) pid, WTERMSIG (p->status));
                }
                return 0;
            }

        }
          
        return -1;
    } else if (pid == 0 || errno == ECHILD) {
        return -1;
    } 
    else {
        perror("waitpid");
        return -1;
    }

}

bool mx_any_job_stopped(t_process* p) {

    t_process* curr = p;
    while (curr) {

        if (curr->stopped)
            return true;
        curr = curr->next;

    }
    return false;

}

bool mx_any_job_completed(t_process* p) {

    t_process* curr = p;
    while (curr) {

        if (curr->completed)
            return true;
        curr = curr->next;

    }
    return false;

}

void mx_wait_for_job(t_cmd_utils* utils) {

    int status;
    pid_t pid = waitpid(-1, &status, WUNTRACED);
    while (!set_process_status(utils, pid, status) && !mx_any_job_stopped(utils->processes) && !mx_any_job_completed(utils->processes)) {
    
        printf("waiting...\n");
        pid = waitpid(-1, &status, WUNTRACED);
        
    }
    printf("waiting overr -- wait for job\n");

}

void mx_foreground_job(t_cmd_utils* utils, t_process* p, bool to_continue) {

    tcsetpgrp(0, p->pid);

    if (to_continue) {

        // tcgetattr (0, &p->sh_modes);
        printf("Continuing the process...\n");
        // tcsetattr(0, TCSADRAIN, &p->sh_modes);;
        if (kill(- p->pid, SIGCONT) < 0) {
            perror("kill (SIGCONT)");
        }

    }

    mx_wait_for_job(utils);
    tcsetpgrp (0, utils->shell_pgid);
    printf("terminal passed to shell\n");

    // tcgetattr (0, &p->sh_modes);
    tcsetattr (0, TCSADRAIN, &utils->shell_modes);

}
