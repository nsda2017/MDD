#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cmath>
#include <process.h>

using namespace std;


int main()
{
	int daysize=20;
	char day[20],execute_file[100]="feature_identification";
	cout << "Enter file name:"<<endl;
	cin.getline(day, daysize);
	
	for(int i=0; i<10; i++)
	{
		char execute_file_log[400],temp[10];
		strcpy(execute_file_log, execute_file);
		strcat(execute_file_log, " ");
		strcat(execute_file_log, day);
		itoa(i, temp, 10);
		strcat(execute_file_log, "0");
		strcat(execute_file_log, temp);
		strcat(execute_file_log, ".log");
		spawnl(P_WAIT, execute_file, execute_file_log, NULL);
		cout<<day<<"0"<<i<<" finished !! "<<endl;
	}
	for(int i=10; i<24; i++)
	{
		char execute_file_log[400],temp[10];
		strcpy(execute_file_log, execute_file);
		strcat(execute_file_log, " ");
		strcat(execute_file_log, day);
		itoa(i, temp, 10);
		strcat(execute_file_log, temp);
		strcat(execute_file_log, ".log");
		spawnl(P_WAIT, execute_file, execute_file_log, NULL);
		cout<<day<<i<<" finish !! "<<endl;
	}
	
	cout<<"--------Process finished!--------";
	system("pause");
	return 0;
}
