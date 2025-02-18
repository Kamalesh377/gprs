#include <xc.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart2.h"
#include "gprs.h"
#define FCY 4000000UL
#include <stdio.h>

void detect_http_error(char value[256]) {
   char *ptr = strstr(value, "+HTTPACTION:");  // Locate the response
    if (ptr == NULL) {
        http_error_msg = "No HTTP response detected";
        return;
    }

    int method, status, data_length;
    
    // Extract HTTP status code
    if (sscanf(ptr, "+HTTPACTION: %d,%d,%d", &method, &status, &data_length) != 3) {
        http_error_msg = "Invalid HTTP response format";
        return;
    }

    // Check error type
    if (status == 200) {
        http_error_msg = "Success: HTTP 200 OK";
    } else if (status == 601) {
        http_error_msg = "Network Error (601)";
    } else if (status >= 400 && status < 500) {
        switch (status) {
            case 400: http_error_msg = "Client Error: Bad Request (400)"; break;
            case 401: http_error_msg = "Client Error: Unauthorized (401)"; break;
            case 403: http_error_msg = "Client Error: Forbidden (403)"; break;
            case 404: http_error_msg = "Client Error: Not Found (404)"; break;
            case 408: http_error_msg = "Client Error: Request Timeout (408)"; break;
            case 411: http_error_msg = "Client Error: Length Required (411)"; break;
            case 413: http_error_msg = "Client Error: Payload Too Large (413)"; break;
            case 414: http_error_msg = "Client Error: URI Too Long (414)"; break;
            case 415: http_error_msg = "Client Error: Unsupported Media Type (415)"; break;
            case 429: http_error_msg = "Client Error: Too Many Requests (429)"; break;
            default:  http_error_msg = "Client Error: Unknown";
        }
    } else if (status >= 500 && status < 600) {
        switch (status) {
            case 500: http_error_msg = "Server Error: Internal Server Error (500)"; break;
            case 501: http_error_msg = "Server Error: Not Implemented (501)"; break;
            case 502: http_error_msg = "Server Error: Bad Gateway (502)"; break;
            case 503: http_error_msg = "Server Error: Service Unavailable (503)"; break;
            case 504: http_error_msg = "Server Error: Gateway Timeout (504)"; break;
            case 505: http_error_msg = "Server Error: HTTP Version Not Supported (505)"; break;
            default:  http_error_msg = "Server Error: Unknown";
        }
    } else {
        http_error_msg = "Unknown Status Code";
    }
}
 

void delay_ms(unsigned int milliseconds) {
    unsigned long cycles = (FCY / 1000) * milliseconds; // Total instruction cycles
    while (cycles > 0) {
        Nop(); // No Operation: Keeps the CPU busy for one cycle
        cycles--;
    }
}


void sendATCommand(const char *command) {
    // Iterate through the string until the null terminator
    while (*command != '\0') {
        delay_ms(10);
        UART2_Write(*command);  // Send each byte to UART2
        command++;  // Move to the next character
    }
}

void GPRS_init(){

    sendATCommand("AT/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+CSQ/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+COPS=0/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+CSTT=\"www\",\"\",\"\"/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+CGATT=1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+CGACT=1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+CIICR/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+CIFSR/r/n");
    if (!resp_check()){
        
    }
    
    sendATCommand("AT+SAPBR = 1,1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+SAPBR = 3,1,\"CONTYPE\",\"GPRS\"/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+SAPBR = 3,1,\"APN\",\"www\"/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    
    
    sendATCommand("AT+SAPBR = 2,1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }


}

void GPRS_retry(){

    sendATCommand("AT/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+CSQ/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+COPS=0/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    
    
    sendATCommand("AT+CGATT=1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    
    
    
    sendATCommand("AT+CIFSR/r/n");
    if (!resp_check()){
        
    }
    
    sendATCommand("AT+SAPBR = 1,1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+SAPBR = 3,1,\"CONTYPE\",\"GPRS\"/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+SAPBR = 3,1,\"APN\",\"www\"/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    
    
    sendATCommand("AT+SAPBR = 2,1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }

    error = 0;
}

void HTTP_init(){
    
    sendATCommand("AT+HTTPINIT/r/n");
    if (!resp_check()){
        h_error = 1;
        return; 
    }
    
    sendATCommand("AT+HTTPPARA=\"URL\",\"www.google.com\"/r/n");
    if (!resp_check()){
        h_error = 1;
        return; 
    }
    
    sendATCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"/r/n");
    if (!resp_check()){
        h_error = 1;
        return; 
    }    
}

void HTTP_get(char val[256]){
    
    sendATCommand("AT+HTTPACTION=0/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+HTTPREAD/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    volatile uint8_t *ptr = (volatile uint16_t *)0x970; 
    // Pointer to address 0x970

    for (int i = 0; i < 256; i++) {
         val[i] = ptr[i];  // Read each value
        // Process or store the value
    }
    con.GET = 2;
}

void HTTP_POST(char val[256]){
    
    sendATCommand("AT+HTTPDATA=\"268\",\"2000\"/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }

    UART2_Write(val);
    
    sendATCommand("AT+HTTPACTION=1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    sendATCommand("AT+HTTPREAD/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    volatile uint8_t *ptr = (volatile uint16_t *)0x970; 
    // Pointer to address 0x970

    for (int i = 0; i < 256; i++) {
         val[i] = ptr[i];  // Read each value
        // Process or store the value
    }
    con.PUSH = 2;
}


void no_resp_handle(){
    
    sendATCommand("AT+CFUN=1,1/r/n");
    if (!resp_check()){
        error = 1;
        return; 
    }
    
    con.COM = 1;
            
}

bool resp_check_OK(char* resp){
    
    for (int i = 0; i < 49; i++){
       if ( resp[i] == 'O' && resp [i+1] == 'K'){
           response = 1;
           return 1;
       }
    }
    return 0;
}

bool resp_check(){
    volatile uint8_t *ptr = (volatile uint16_t *)0x970; 
    // Pointer to address 0x970
    delay_ms(100);
    char value [256];
    for (int i = 0; i < 256; i++) {
         value[i] = ptr[i];  // Read each value
        // Process or store the value
    }
    
    for (int i = 0; i < 256; i++){
       if ( value[i] == 'O' && value [i+1] == 'K'){
           response = 1;
           return 1;
           
       }
       
       else if (i < 252 && value[i] == 'E' && value[i+1] == 'R' &&
                      value[i+2] == 'R' && value[i+3] == 'O' &&
                      value[i+4] == 'R') {
            
            return 0;
        }
       else {
           
           return 0;
       }    
    }
}


void GET_disp(char val[256]){
    volatile uint8_t *ptr = (volatile uint16_t *)0x970; 
    // Pointer to address 0x970
    delay_ms(100);
    char value [256];
    for (int i = 0; i < 256; i++) {
         val[i] = ptr[i];  // Read each value
        // Process or store the value
    }
}




void init_err_handle(int err ){
    
    for (int j= 0; j < 2; j++){
        if (error == 1){
            GPRS_retry();
            err_count +=1;
        }
                
        if (err_count > 1){
            force_restart();
        }
               
    }
    
}




void init_com(){
    

    switch (con.COM) {
        
        
        case 0 :
            GPRS_init();
            init_err_handle(error);
            HTTP_init();            
            con.COM = 4;
            break;
        
        case 1 :
            HTTP_get(data);
            detect_http_error(data);
            if (resp_check_OK(http_error_msg)){
                con.COM = 4;
            }
            else {                
                con.COM = 5;
            }
            break;
        
        case 2 :
            HTTP_POST(data);
            detect_http_error(data);
            if (resp_check_OK(http_error_msg)){
                con.COM = 4;
            }
            else {                
                con.COM = 6;
            }
            break;
            
        case 3 :    
            no_resp_handle();
            con.COM = 1;                    
            break;
        
        case 4 :  
            
            if (change == 1){
                con.COM = 2; // or 3 
                break;
            }
            
            else {
                con.COM = 4;
                break;
            }
            
        case 5 :
            
            printf("%s",http_error_msg);
            con.COM = 4;
            break;
        
        case 6 :
            
            printf("%s",http_error_msg);
            con.COM = 4;
            break;
            
    }
}
void Command_init(){
    
    con.COM=0;
//    con.ERROR=0;
    con.GET=0;
    con.INIT=0;
    con.PUSH=0;
    con.RESPONSE=0;
}

void force_restart(){
    con.COM = 3;
}

