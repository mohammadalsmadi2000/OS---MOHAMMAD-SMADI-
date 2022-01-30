

/*
    CPE 460L: PROJECT
    Mohammad Smadi 2018980093,
   

    First we create two pipes, pipe 1 recieves data from process 1 (the parent that displays the menu)
    and sends it to process 2 (the child that finds the result).

    We then fork to create two processes. In the parent, we close the reading end of pipe 1 and the writing end of pipe 2.
    In the child, we close the writing end of pipe 1 and the reading end of pipe 2.

    We then perform the following in an infinite loop:
        1. We display the menu for the user

        2. We recieve input from the user for the choice, if the choice is ("X" Or 4), terminate. Otherwise
                check if the choice is valid (either 1, 2 or 3). We display error messages when appropriate and return to step 1


        3.If the user has entered the two choice (1 OR 2), we will ask him to enter only one text, but         if he enters option 3, we will ask him to enter two texts

        4.If the user entered a wrong choice,
                we display appropriate error messages and loop until the user enters a correct choice. 
            
            

        5. By this stage, we can guarantee that the choice and the text are all correct, so we send them through pipe 1
                to the child process in the following order: choice, str 1, str 2
        
        6. We then read the result from pipe 2, and display it to the user.
    
    In the child process (process 2), we do the following infinite loop:
        1. We read the choice, str 1 and str 2.

        2. Then, we perform the appropriate operation determined by the choice.

        3. We then send the result through pipe 2 to process 1.
    
    The child is terminated when the parent terminates.

    We also perform appropriate error handling and show error messages when the program fails to write / read a pipe.
*/

//Required libraries

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

//----------------


#define BUFFER_SIZE 50

//This function was created to convert text to uppercase letters by converting text letter by letter.
char* upper(char* s){
   int i;
   for(int i = 0; s[i]; i++){
     s[i] = toupper(s[i]);
    }
   return s;
}

//----------------

//This function was created to convert text to lowercase letters by converting text letter by letter.
char* lower(char* s){
   for(int i = 0; s[i]; i++){
     s[i] = tolower(s[i]);
     }
   return s;
}

//-----------------

int main() {
    int pipe1[2];//create pipe1
    pipe(pipe1);//pipe system call in C

    int pipe2[2];//create pipe2
    pipe(pipe2);//pipe system call in C

    int pid = fork();//create fork and save value in pid


    if (pid < 0) {
      //check if fork failed
        printf("\n  An error occured while trying to fork.\n");
        exit(1);
    }

    else if (pid > 0) {
      //parent process

        close(pipe1[0]);//close the reading end of pipe 1
        close(pipe2[1]);//close the writing end of pipe 2

        while (1) { //infinit loop

          //---------Menu------------------
            printf("\n SMADI TEAM\n-----------------\n");
            printf("  * Please enter one of the following choice:\n");
            printf("    1) for convert str1 to upper \n    2) for convert str1 to lower\n    3) for concatenation str1 & str2\n    4) X (Terminate)\n");
            printf("  >> ");

            char choice[BUFFER_SIZE];//create Buffer choice
            scanf("%s", choice);//input value from user


            char ExitCheck[2];
            //check if the user sure want to quit
            if ((strcmp(choice, "X")==0)||(strcmp(choice, "4")) == 0) {
              printf("  * Are you sure you want to quit? (Y,N) ");
              scanf("%s", ExitCheck);
            if((strcmp(ExitCheck,"Y")==0)||(strcmp(ExitCheck,"y")==0)){
                close(pipe1[1]);
                close(pipe2[0]);
                exit(0);}
            else 
              continue;
            }
            //------------------------

            //check if the choice is valid 
            else if (strcmp(choice, "1") != 0 &&
                     strcmp(choice, "2") != 0 &&
                     strcmp(choice, "3") != 0 ) {
                printf("\n  The entered choice (%s) is incorrect, please enter a correct choice.\n", choice);
                continue;
            }

            char str1[BUFFER_SIZE];
            char str2[BUFFER_SIZE];

              //we will ask user to enter only one text OR two text 

              if(strcmp(choice, "1") == 0){
                 printf("  * Please enter the first string: ");
                 scanf("%s", str1);
              }else if(strcmp(choice, "2") == 0){
                 printf("  * Please enter the first string: ");
                 scanf("%s", str2);
              }else{
                  printf("  * Please enter the first string: ");
                  scanf("%s", str1);
                  printf("  * Please enter the second string: ");
                  scanf("%s", str2);
              }

            int temp;

            //error handling

            temp = write(pipe1[1], choice,BUFFER_SIZE);
            if (temp < 0) {
                printf("\n  An error occured while trying to write to pipe1.\n");
                exit(1);
            }

            temp = write(pipe1[1], str1,BUFFER_SIZE);
            if (temp < 0) {
                printf("\n  An error occured while trying to write to pipe1.\n");
                exit(1);
            }

            temp = write(pipe1[1], str2,BUFFER_SIZE);
            if (temp < 0) {
                printf("\n  An error occured while trying to write to pipe1.\n");
                exit(1);
            }

            char res[BUFFER_SIZE];
            temp = read(pipe2[0], res,BUFFER_SIZE);
            if (temp < 0) {
                printf("\n  An error occured while trying to read from pipe2.\n");
                exit(1);
            }

            if(strcmp(choice, "1") == 0){
            printf("\n  * Converted text from (%s) to Uppercase : \"%s\" \n",str1, res);
            }else if(strcmp(choice, "2") == 0){
            printf("\n  * Converted text from (%s) to Lowercase : \"%s\" \n", str2,res);
            }else if(strcmp(choice, "3") == 0){
            printf("\n  * Result of concatenation two texts : \"%s\" \n", res);
            }
        }
    }
    
    else {
      // //parent process
        close(pipe1[1]);
        close(pipe2[0]);

        while (1) {
            char choice[BUFFER_SIZE];
            read(pipe1[0], choice,BUFFER_SIZE);

            char str1[BUFFER_SIZE];
            read(pipe1[0], str1,BUFFER_SIZE);
            char n1[BUFFER_SIZE];
             strcpy( n1,str1);
             char*s1=upper(n1);
             strcpy(n1,s1);
             

            char str2[BUFFER_SIZE];
            read(pipe1[0], str2, BUFFER_SIZE);
            char n2[BUFFER_SIZE];
            strcpy(n2,str2);
            char*s2=lower(n2);
            strcpy(n2,s2);

            char n3[BUFFER_SIZE];
            strcpy(n3,strcat(str1,str2));
            
            char res[BUFFER_SIZE];
            switch (choice[0]) {
              
            case '1':{
                  
                strcpy(res,n1);
                }
                break;
            
            case '2':{
                strcpy(res,n2);
            }
                break;
            case '3':{
               
               strcpy(res,n3);
            }
                break;
            }
            write(pipe2[1], res,BUFFER_SIZE);
        }
    }
}



