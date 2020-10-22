
#include "iostm8s003f3.h"

int main( void )
{
    int i,j;
    PD_DDR|=0x0C;
    PD_CR1|=0x0C;
    PD_CR2|=0x00;
    while (1)
    {
        for(i=0;i<100;i++)
            for(j=0;j<200;j++);
        PD_ODR^=0x0C;
        for(i=0;i<100;i++)
            for(j=0;j<200;j++);
    }
    return 0;
}
