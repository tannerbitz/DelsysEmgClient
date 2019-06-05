#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <thread>
#include <time.h>
#include <chrono>
#include <ratio>
#include <unistd.h>

// Function prototypes
void GetKeyboardInputToEndProgram(bool * bEndKeyHit);

int main(int argc, char * argv[]){

    // Clock Start
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = std::chrono::high_resolution_clock::now();

    bool bEndKeyHit = false;
    std::thread first(GetKeyboardInputToEndProgram, &bEndKeyHit);
    while (!bEndKeyHit){}

    high_resolution_clock::time_point t_end = std::chrono::high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);

    printf("The program lasted %9.8f seconds\n", time_span);

    first.join();
    return 0;

}


void GetKeyboardInputToEndProgram(bool * bEndKeyHit){
    char keyboard_input[256] = "";
    char * pos;
    char exit_key[10] = "q";
    while (strcmp(keyboard_input, exit_key) != 0){
        fgets(keyboard_input, 256, stdin);
        // remove newline char from fgets
        if ((pos = strchr(keyboard_input, '\n')) != NULL){
            *pos = '\0';
        }
    }
    *bEndKeyHit = true;
}
