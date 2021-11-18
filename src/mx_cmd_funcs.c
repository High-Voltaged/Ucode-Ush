#include "../inc/ush.h"

// int mx_cd(t_cmd_utils* utils) {



// }

int mx_env(t_cmd_utils* utils) {

    for (int i = 0; utils->env_vars[i] != NULL; ++i) {

        printf("%s\n", utils->env_vars[i]);

    }
    return 0;

}
