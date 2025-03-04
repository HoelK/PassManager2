#include <sys/stat.h>
#include </usr/include/unistd.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#define MAX_PASS_LENGHT 32
#define PASS_FILE_PATH ".test/usr/pass.txt"
#define DATA_FILE_PATH ".test/usr/database.txt"

void get_mainpass(){
	
	//Variables
	char input_mainpass[MAX_PASS_LENGHT] = {0};
	char file_mainpass[MAX_PASS_LENGHT] = {0};

	//Get the mainpass from the file
	FILE *passfile = fopen(PASS_FILE_PATH, "r");
	fgets(file_mainpass, MAX_PASS_LENGHT, passfile);
	fclose(passfile);

	//Ask mainpass while it's not the right one
	do{
		printf("Main pass : ");
		fgets(input_mainpass, MAX_PASS_LENGHT, stdin);
	}while(strcmp(input_mainpass, file_mainpass) != 0);
}

void create_mainpass(){

	//Variables
	char mainpass[MAX_PASS_LENGHT] = {0};
	
	//Create directories and file
	mkdir(".test", 0777); 
	mkdir(".test/usr", 0777);
	FILE *passfile = fopen(PASS_FILE_PATH, "w");

	//Ask mainpass
	printf("Create your main password : ");
	fgets(mainpass, MAX_PASS_LENGHT, stdin); 

	//Put it in pass.txt
	fprintf(passfile, "%s", mainpass);
	fclose(passfile); 
} 


void display_password(){ //need to keep working on this one, fileinput needs to take the password and the description
	//Get the content of the file
	char fileinput[MAX_PASS_LENGHT] = {0};
	FILE *dataFILE = fopen(DATA_FILE_PATH,"r");
	
	do{
		fgets(fileinput, MAX_PASS_LENGHT, dataFILE);
		printf("%s", fileinput);
		for(int i = 0; i < MAX_PASS_LENGHT; i++){
			fileinput[i] = '\0';
		};
	}while(!feof(dataFILE));

	fclose(dataFILE);
}

void add_password(){ //Still void the entire file, openning process is bad

	//Variables
	char description[MAX_PASS_LENGHT] = {0};
	char password[MAX_PASS_LENGHT] = {0};
	
	//Ask the description then the password			// not sure if the two fgets use the same cursor
	printf("Choose a description (25 char max) : ");
	fgets(description, MAX_PASS_LENGHT, stdin);
	printf("Choose a password (25 char max) : ");
	fgets(password, MAX_PASS_LENGHT, stdin);
	
	//Put everything into the database file
	FILE *databaseFILE = fopen(DATA_FILE_PATH, "a");
	fputs(description, databaseFILE);
	fputs(password, databaseFILE);
	fclose(databaseFILE);

}

void delete_password(){

	//Variables
	char description[MAX_PASS_LENGHT] = {0};
	char password[MAX_PASS_LENGHT] = {0};
	int line = 0, choice = 0, buffer = 0;
	
	//Ask the line to delete
	printf("Whitch line do you want to delete ? ");
	scanf("%d", &choice);
	while((buffer = getchar()) != '\n' && buffer != EOF);

	FILE *databaseFILE = fopen(DATA_FILE_PATH, "r");
	FILE *newdatabaseFILE = fopen(".test/usr/newdatabasefile.txt", "w+");
	
	//Put the data into a new file without the selected line
	
	do
	{
		//increase line //optimisation : shouldnt keep increase after line deleted
		line++;
		
		//Get the lines (1 line is 1 description AND 1 pass)
		fgets(description, MAX_PASS_LENGHT, databaseFILE);
		fgets(password, MAX_PASS_LENGHT, databaseFILE);
		

		//Exclude selected line //optimisation : shouldnt check twice the same con has bellow
		if(line != choice && !(feof(databaseFILE)))
		{
			fputs(description, newdatabaseFILE);
			fputs(password, newdatabaseFILE);
		}

	}while(!feof(databaseFILE));
	
	//Close the files, Delete old one & Rename new one
	fclose(databaseFILE);
	fclose(newdatabaseFILE);
	remove(DATA_FILE_PATH);
	rename(".test/usr/newdatabasefile.txt", DATA_FILE_PATH);
}

void user_choices(){

	//Variables
	int choice = -1;
	
	//Print the different choices in console
	printf("Display your passwords................1\nAdd a password........................2\nDelete a password.....................3\nQuit..................................0\n");
	
	//Get the user choice 
	//infinite loop when input is string
	do{
		printf("choice : ");
		scanf("%d", &choice);

		int c = 0;
		while((c = getchar()) != '\n' && c != EOF); //Empty the buffer (\n causing issues)
	}while(choice < 0 || choice > 3);

	//Invoke the asked function
	switch(choice){
		case 1:
		     display_password();
		     break;
		case 2:
		     add_password();
		     break;
		case 3:
		     delete_password();
		     break;
		case 0:
		     exit(EXIT_SUCCESS);
	}
}

int main(){
	(access(PASS_FILE_PATH, F_OK) == -1) ? create_mainpass() : get_mainpass(); //Try if mainpass file exist, react acordingly
	for(;;){user_choices();} //Ask what to do until shutdown by user



	return 0;
}
