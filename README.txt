Project 1 for CS 3013
Completed by Eva Bruklich (ebruklich) and Noah Parker (nparker)

To complete the third phase of the homewrk, in particular to keep track of the background jobs, we created a struct which stored the jobs before time, its PID, and the command that was chosen by the command line. We then created an array of the structs. Using this array we were able to keep track of the jobs that were running in the background and which jobw as completed when, in order to output it. 

We tested our programs by creating text files custom.txt and multi.txt and writing linux commands and the cproclist command in them. Those text files were then used by custom.c and mulit.c respectively in order to ensure that the correct output was delivered by the terminal. 
