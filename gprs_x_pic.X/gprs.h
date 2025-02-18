
#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

 char* data;
 int error;
 int h_error;
 int response;
 int err_count;
 int change;
char http_error_msg; 

struct Config 
{
    int GET;
    int PUSH;
    int RESPONSE;
    int ERROR;
    int INIT;
    int COM;
    
};

 struct Config con;
 
 
void delay_ms(unsigned int milliseconds);

void sendATCommand(const char *command);

void GPRS_init();

void GPRS_retry();   // this function is there so that if one of config AT commands failed it will retry it twice if that fails then retry state

void HTTP_init();

void HTTP_get(char val[256]);

void HTTP_POST(char val[256]);

void no_resp_handle(); // this is when there is no response from gsm module so we restart the module

bool resp_check();  // checks for OK or error if nothing is received then sends it to the restart state


void GET_disp(char val[256]); // is to get the response and store it in a variable for sending the data off


void detect_http_error(char value[256]);

void force_restart();

void init_err_handle(int err );
void init_com();
void Command_init();


