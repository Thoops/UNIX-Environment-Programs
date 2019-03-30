/* Program that will create two pipes, create a clone of itself and the parent
process prompts the user for a string. Parent sends the string over one pipe to 
the child process. The child process will reverse the characters in the string and 
send them back to the parent over the second pipe. The parent will display the 
string that was sent to it. */

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <algorithm>

using namespace std;

int main(){
	string input;
	int i = 0;
	
	char buf1[1024];
	char buf2[1024];
	int length;
	int fd_c_p[2];
	int fd_p_c[2];
	
	pipe(fd_c_p);
	pipe(fd_p_c);
	
	int pid;
	
	if((pid = fork()) > 0){
	    printf("I am the parent with PID: %d\n", getpid());
	    cout << "Enter a string: \n";
	    getline(cin, input);
		
		if(write(fd_p_c[1], input.c_str(), input.length()) == -1){
		    perror("Write failed in parent\n");
		    return 1;
		}
		
		if(read(fd_c_p[0], &buf1, input.length()+1) == -1){
		    perror("Read failed in parent\n");
		    return 1;
		}
		
    	printf("%s\n", buf1);
	}
	
	else if(pid == 0){
	    
    	printf("I'm the child process with PID: %d\n", getpid());
		printf("Sending reversed string to parent\n");
		
		
		if(read(fd_p_c[0], &buf2, sizeof(buf1)) == -1){
		    perror("Read failed in child\n");
		    return 1;
		}
		string str(buf2);
		
		reverse(str.begin(), str.end());
        
		if(write(fd_c_p[1], str.c_str(), str.length()) == -1){
		    perror("Write failed in child\n");
		    return 1;
		}
	}
	
	return 0;
}