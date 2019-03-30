/* Program that will schedule 3 clones. Each clone will communicate to the parent process 
over a pipe. (i.e. There are 3 pipes.) Each process will write Process n.t (where n is 
the number of the process 1, 2, or 3 and t is the number of messages that the process n has 
sent) to the parent 10 times at time intervals of n*2 + 1. The parent displays these 
messages as soon as they are received. Make sure that only one message is received at a time. */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int nb = 0;
    fd_set rfds;
    struct timeval tv;
    int retval;
    int A, B, C;
    int fd_a_p[2];
    int fd_b_p[2];
    int fd_c_p[2];
    char buf1[1000];
    char buf2[1000];
    char buf3[1000];
    string str1;
    string str2;
    string str3;
    int t1 = 0, t2 = 0, t3 = 0;
    
    
    
    
    pipe(fd_a_p);
    pipe(fd_b_p);
    pipe(fd_c_p);
    
    if((A = fork()) == 0){
        while(t1 <= 10){
            t1++;
            str1 = "Process 1.";
            str1 += to_string(t1);
            sleep(1 * 2 + 1);
            if(write(fd_a_p[1], str1.c_str(), 20) == -1){
                perror("Write failed");
            }
        }
    }
    
    else if((B = fork()) == 0){
        while(t2 <= 10){
            t2++;
            str2 = "Process 2.";
            str2 += to_string(t2);
            sleep(2 * 2 + 1);
            if(write(fd_b_p[1], str2.c_str(), 20) == -1){
                perror("Write failed");
            }
        }
    }
    
    else if((C = fork()) == 0){
        while(t3 <= 10){
            t3++;
            str3 = "Process 3.";
            str3 += to_string(t3);
            sleep(3 * 2 + 1);
            if(write(fd_c_p[1], str3.c_str(), 20) == -1){
                perror("Write failed");
            }
        }
    }
    
    else{
        while(1){
            FD_ZERO(&rfds);
            FD_SET(fd_a_p[0], &rfds);
            FD_SET(fd_b_p[0], &rfds);
            FD_SET(fd_c_p[0], &rfds);
            int nbSelect;
            
            if( ( nbSelect = select(4, &rfds, NULL, NULL, NULL)) == -1 ) {
                perror( "select" );
                return 1;
            }
            
            
            if( FD_ISSET( fd_a_p[0], &rfds ) ) {

                if( (nb = read(fd_a_p[0], &buf1, 20)) == -1 ) {
                     perror( "read" );
                     return 1;
                }
                if( nb == 0 ) {
                    cout << "No more data" << endl;
                }
                else{
                    printf("%s\n", buf1);
                }
            }
            
            
            
            
            if( FD_ISSET( fd_b_p[0], &rfds ) ) {

                if( (nb = read(fd_b_p[0], &buf2, 20)) == -1 ) {
                     perror( "read" );
                     return 1;
                }
                if( nb == 0 ) {
                    cout << "No more data" << endl;
                }
                else{
                    printf("%s\n", buf2);
                }
            } 
            
            
            
            if( FD_ISSET( fd_c_p[0], &rfds ) ) {

                if( (nb = read(fd_c_p[0], &buf3, 20)) == -1 ) {
                     perror( "read" );
                     return 1;
                }
                if( nb == 0 ) {
                    cout << "No more data" << endl;
                }
                else{
                    printf("%s\n", buf1);
                }
            } 
            
            
            /*
            if(read(fd_a_p[0], &buf1, 20) == -1){
                perror("Read failed");
            }
            printf("%s\n", buf1);
        
        
            if(read(fd_b_p[0], &buf2, 20) == -1){
                perror("Read failed");
            }
            printf("%s\n", buf2);
        
        
            if(read(fd_c_p[0], &buf3, 20) == -1){
                perror("Read failed");
            }
            printf("%s\n", buf3);
            */
        }
    }
    return 0;
}
