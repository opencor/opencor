#include <stdio.h>
#include <time.h>

int main()
{
    time_t now = time(0);
    struct tm *now_tm = localtime(&now);

    printf("%d-%02d-%02d", 1900+now_tm->tm_year, 1+now_tm->tm_mon, now_tm->tm_mday);

    return 0;
}
