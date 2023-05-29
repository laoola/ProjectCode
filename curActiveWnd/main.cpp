#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    for(int i = 0; i < 10; ++i)
    {
        FILE *fp;
        char szRlt[256] = {0};
        vector<string> vtRlt;
        if((fp = popen("xprop -id $(xprop -root _NET_ACTIVE_WINDOW | cut -d ' ' -f 5) _NET_WM_NAME WM_CLASS _NET_WM_PID | awk -F '=' '{print$2}'", "r")) == NULL)
        {
            perror("Fail to popen\n");

            return 0;
        }

        //    "终端"
        //    "deepin-terminal", "deepin-terminal"
        //    5688
        string strTmp = "";
        while(fgets(szRlt, 256, fp) != NULL)
        {
            strTmp += szRlt;
            strTmp.erase(strTmp.find_last_not_of('\n') + 1);
            strTmp += " ";
        }
        printf("%s\n", strTmp.c_str());
        pclose(fp);
        sleep(5);
    }



    return 0;
}
