#include <unistd.h>
#include <stdio.h>

int DoOutput()
{
    printf("this is server App test\n");

    return 0;
}

int main(int argc, char *argv[])
{
    daemon(1,1);

    while(1)
    {
        DoOutput();

        sleep(1);
    }

    return 0;
}
