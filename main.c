/********************************************
 *	Names: 		CHUA CHIACO, Ronn Christian
 *				ROXAS, Ronell John 
 *	Group:		26 
 *	Section: 	S-16
 ********************************************/

#include<stdio.h>
#include<string.h>
#include"fcfs.h"
#include"nsjf.h"
#include"psjf.h"
#include"rr.h"

int main() {
	FILE *fp;
	int x, y, z;
	char filename[21];	//21 characters filename
	
	//get filename
	printf("Filename: ");
	scanf("%s", filename);
	
	if(strcmp(".txt", filename + (strlen(filename) - 4))) {	//check if last 4 characters is ".txt"
		strcat(filename, ".txt");	//add .txt to end if not in input filename	
	}

	//open file
	fp = fopen(filename , "r");
	
	if(fp != NULL) {	//if file found
		fscanf(fp, "%d %d %d", &x, &y, &z);	
		print(x, y, z);	//test function for import (from fcfs.c)
	}
	else {	//if filename not found
		printf("File %s not found!", filename);
	}
	
	fclose(fp);
	
	return 0;
}
