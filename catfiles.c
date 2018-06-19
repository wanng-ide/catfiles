#include <stdio.h> /* perror */
#include <stdlib.h> /* exit */
#include <sys/types.h> /* read, write */
#include <sys/uio.h> /* read, write */
#include <unistd.h> /* close, pipe, read, write */
#include <fcntl.h> /* perror */
#include <string.h> /* compare string */
#include <errno.h> /* error process */

int main(int argc, char *argv[]){
    
    // check for files input
    if (argc == 1){
        printf("Can't find any file.");
        exit(1);
    }

    // input files conatain 'context.txt' or not?
    int i = 0;
    for(i = 0; i < argc; i++){
        if(strcmp(argv[i], "context.txt") == 0){
            printf("Your input files should not contain 'context.text'!");
            exit(1);
        }
    }
    
    // open output file
    int out = open("context.txt", O_WRONLY | O_TRUNC | O_CREAT , 0666);
    if(out == -1){
        perror("Open context.txt");
        exit(1);
    }
    
    // copy files' content into context.txt one by one
    // initillize bytes buffer
    char buffer[1024]; 
    for (i = 0; i < 1024; i ++){
		buffer[i] = ' ';
	}

    int in, bytes_read, bytes_write;
    char *ptr;
    for(i = 1; i < argc; i++){
        // read input files one by one
        // check
        in = open(argv[i], O_RDONLY);
        if(in == -1){
            perror("Source file error");
            break;
        }
        printf("Open sucesslly: %s\n", argv[i]);
        while(bytes_read = read(in, &buffer, sizeof(buffer))){
            // may have an error
            if((bytes_read == -1) && (errno != EINTR)){
                break;
            }else{
                ptr = buffer;
                while(bytes_write = write(out, ptr, bytes_read)){
                    // may have an error
                    if ((bytes_write == -1) && (errno != EINTR)){
                        break;
                    // write all bytes -> break
                    }else if(bytes_write == bytes_read){
                        break;
                    // may only write a part of bytes, continue writing
                    }else if(bytes_write > 0){
                        ptr += bytes_write;
                        bytes_read -= bytes_write;
                    }
                }
                close(in);
                printf("Write sucessfully.\n");
            }
        }
    }
    close(out);
    printf("All files are writed.\n");
}
