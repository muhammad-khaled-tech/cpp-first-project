#include "input.hpp" 
#include <stdio.h>   
#include <unistd.h>  
#include <termios.h> 


KeyType readKey() {
    int nread;
    char c;
    nread = read(STDIN_FILENO, &c, 1);

    if (nread <= 0) {
        return KEY_UNKNOWN; 
    }

    if (c == '\033') {
        char seq[3]; 
 
        if (read(STDIN_FILENO, &seq[0], 1) == 0) {
     
            return KEY_ESCAPE;
        }
        
   
        if (read(STDIN_FILENO, &seq[1], 1) == 0) {
      
            return KEY_UNKNOWN;
        }

    
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return KEY_UP;
                case 'B': return KEY_DOWN;
                case 'H': return KEY_HOME; 
                case 'F': return KEY_END;  
            }
        }
        
   
        return KEY_UNKNOWN;
    }


    if (c == '\n' || c == '\r') {
        return KEY_ENTER;
    }

    if (c == 'q') {
        return KEY_QUIT;
    }


    return KEY_UNKNOWN;
}