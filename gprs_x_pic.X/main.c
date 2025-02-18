#include <xc.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart2.h"
#include <libpic30.h>
#define FCY 4000000UL
#include "gprs.h"



int main(void)
{
    SYSTEM_Initialize();
    Command_init ();

    while (1)
    {         
       init_com();
    }

    return 1;

}

