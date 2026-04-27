# Project title - OS Project Custom Shell


## Project overview
- This shell allows the user to execute shell commands such as system commands as well as built-in ones like procread and history


## Purpose 
- This project is made to practice and learn the basic OS cpncepts of creating processes and threads 


## Technologies used
1. A Virtual Box with Debian 11 and Linux Kernel 5.15.67


## Setup/installation: How to Compile
1. step one: To build the program, run the command: "make build"
2. step two: To run the program without valgrind, run the command: "make run"
3. step three: To run the program with valgrind, run the command: "make ValRun"
4. step four: To remove the exe, run the command: "make clean"


## Key features/functionality/Example usage
1. Can do basic linux commands for example: "ls /dir_name", "history"
2. Can record and save your previous commands
3. To exit the shell type "exit"


# Lesson notes:
1. The Fork Linux function duplicates file
2. The exec function replaces the current process with a new program thus needed the Fork function to still have the shell program running


# Your role or contribution
- As a student in operating systems class I coded a basic linux shell from scratch and have implemented basic linux commands for the custom shell to execute


## Sources Used
1. https://man7.org/linux/man-pages/man3/getline.3.html (Used to see the doc of getline is used in C)
2. https://www.geeksforgeeks.org/cpp/strtok-strtok_r-functions-c-examples/ (Used to see how strtok function is used in C)
3. https://www.geeksforgeeks.org/c/array-of-strings-in-c/ (Used to see how string arrays are made in C)
4. http://man7.org/linux/man-pages/man2/fork.2.html (Used to see how fork is used)
5. https://www.w3schools.com/c/ref_string_strcmp.php (Used to see how strcmp is used)
6. https://www.youtube.com/watch?v=5BE9qM79fgM (Used to see how execv is used)
7. https://stackoverflow.com/questions/28507950/calling-ls-with-execv (Used to see how execv is used with commands)
8. https://www.geeksforgeeks.org/techtips/fgetc-fputc-c/ (Used to see how fgetc is used)
9. https://pubs.opengroup.org/onlinepubs/9699919799/functions/exec.html (Used to see how the exec family works)
