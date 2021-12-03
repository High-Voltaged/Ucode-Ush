#include "../inc/ush.h"

static int set_process_status(t_cmd_utils* utils, pid_t pid, int status) {

    if (pid > 0) {

        for (t_process* p = utils->processes; p; p = p->next) {

            if (p->pid == pid) {
                
                p->status = status;
                if (WIFSTOPPED (status)) {
                    p->stopped = true;
                    mx_created_process_push_back(&utils->stopped_jobs, p);
                    mx_printstr("\nush: suspended  ");
                    mx_print_strarr(utils->args, " ");
                } else {
                    p->completed = true;
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

void mx_wait_for_job(t_cmd_utils* utils, t_process* p) {

    int status;
    waitpid(p->pid, &status, WUNTRACED);
    while (!set_process_status(utils, p->pid, status) && !p->stopped && !p->completed) {
    
        waitpid(p->pid, &status, WUNTRACED);
        
    }

}

void mx_foreground_job(t_cmd_utils* utils, t_process* p, bool to_continue) {

    tcsetpgrp(0, p->pid);

    if (to_continue) {

        // tcgetattr (0, &p->sh_modes);
        printf("[%d] - continued  %s\n", p->node_id, p->cmd_name);
        // tcsetattr(0, TCSADRAIN, &p->sh_modes);;
        if (kill(- p->pid, SIGCONT) < 0) {
            perror("kill (SIGCONT)");
        }

    }

    mx_wait_for_job(utils, p);
    tcsetpgrp (0, utils->shell_pgid);

    // tcgetattr (0, &p->sh_modes);
    tcsetattr (0, TCSADRAIN, &utils->shell_modes);

}
