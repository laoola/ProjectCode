#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    pid_t status;
    status = system("/opt/apps/cn.com.vrv.cems/files/cems//getpolicyfile 192.168.120.48 192.168.118.54  "
                    "/opt/apps/cn.com.vrv.cems/files/cems/esa/config-cache/sysweak/policydb/5BA733DB "
                    "dfd4592bbf9a5619af73da4ffa82ea28 29c4efe2a1544648a1d14866a6ac5b1d");

    if (-1 == status)
    {
        printf("system error!");
    }
    else
    {
        printf("exit status value = [0x%x]\n", status);

        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
                printf("run shell script successfully.\n");
            }
            else
            {
                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
            }
        }
        else
        {
            printf("exit status = [%d]\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
