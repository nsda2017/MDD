#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "LinkedList.h"
#include "Node.h"
#include "SinglyLinkedList.h"
#include <cmath>
#include<ctime>
#include <time.h>
#include <process.h>

using namespace std;
int filesize=25;
char filename[25];
int filtering(string toplist[], string whitelist[], string domain, int topcount, int whitecount);
void dnsprocess(LinkedList<string>* domainlist, LinkedList<string>* iplist, string domainname,string ipaddress); //把讀取得ip與查詢domain加入到linkedlist 
void PrintList(LinkedList<string>* list);																		//列出linkedlist所有的資訊 
void erase_isolated_graph(LinkedList<string>* domainlist,LinkedList<string>* iplist);							//移除只有一個ip查詢一個domain情況 
void similarity_calculate(LinkedList<string>* domainlist);														//計算相似度矩陣並輸出鄰接矩陣 
void write_domain_to_file(LinkedList<string>* domainlist, string filename);										//有時linkedlist太長，寫入到檔案中 
void random_walk_list_process(LinkedList<string>* domainlist, string domainname, string ipaddress);				//執行group identify中的randomwalk 
int find_place(string domain[], string comparedomain, int size);												//尋找節點在群體的編號 
void group_verify(LinkedList<string>* domainlist);																//近行群體驗證並且輸出惡意群體 
float stand_deviation(int data[], int data_size);																//計算每個點被拜訪之標準差 
int ipfiltering(string ipaddress[], string ip, int ipcount);													//過濾掉mailserver 


int main(int argc, char *argv[])											//必須先給要分析的檔案名稱 
{
	remove("simulationmatrix.txt");
	remove("cluster.txt");
	string mail_server[11];													//把地區中的mailserver去除 
	mail_server[0]="140.116.91.1";
	mail_server[1]="127.0.0.1";
	mail_server[2]="140.116.229.75";
	mail_server[3]="140.116.241.73";
	mail_server[4]="140.116.35.25";
	mail_server[5]="140.116.247.47";
	mail_server[6]="140.116.229.71";
	mail_server[7]="140.116.241.74";
	mail_server[8]="140.116.72.158";
	mail_server[9]="140.116.200.11";
	mail_server[10]="140.116.78.1";

	int lins=0;
	LinkedList<string>* domainlist = new SinglyLinkedList<string>();	//domain list 
	LinkedList<string>* iplist = new SinglyLinkedList<string>();		//ip list
	//cout << "Enter what the data do you want to process :"<<endl;
	//cin.getline(filename,filesize);
	strcpy(filename, argv[1]);
	cout<<filename<<" process!!"<<endl;
	char filterbuffer[300];												//讀取白名單與百大網站 
	int linecount=0,topcount=0,whitecount=0;
	fstream white, top;
	
	string *toplist;													//百大 
	top.open("TopAndHonest.txt",ios::in);
	if(!top)
		cout<<"top.txt readfile error!!!!!!";
	else
	{
		while(!top.eof())
		{
			if(linecount==0)
			{
				top.getline(filterbuffer,sizeof(filterbuffer));
				topcount=atof(filterbuffer);
				toplist=new string[topcount];
				linecount++;
			}
			else
			{
				top.getline(filterbuffer,sizeof(filterbuffer));
				toplist[linecount-1].assign(filterbuffer);
				linecount++;
			}
		}
		linecount=0;	
	}
	
	string *whitelist;													//白名單 
	white.open("WhiteList.txt",ios::in);
	if(!white)
		cout<<"result.txt readfile error!!!!!!";
	else
	{
		while(!white.eof())
		{
			if(linecount==0)
			{
				white.getline(filterbuffer,sizeof(filterbuffer));
				whitecount=atof(filterbuffer);
				whitelist=new string[whitecount];
				linecount++;
			}
			else
			{
				white.getline(filterbuffer,sizeof(filterbuffer));
				whitelist[linecount-1].assign(filterbuffer);
				linecount++;
			}
		}
	}
	
	
	
	
	fstream fin;														//開始讀檔 
	char buffer[800];
	fin.open(filename,ios::in);
	if(!fin)
		cout<<"analysis readfile error!!!!!!";
	else{
		while(!fin.eof())
		{                   					  
			fin.getline(buffer,sizeof(buffer));
  			char *pch = strtok(buffer," ");	
			int i=1;
			string domainname, ipaddress ,recard;
			while (pch != 0)
			{
    			if(i==12)
				{
    				ipaddress.assign(pch);											//提取ip位址 
    				ipaddress.erase(ipaddress.find("#"),ipaddress.size());			//移除ip後面接的port 
				}                          
    			if(i==14)															//提取網域名稱 
    				domainname.assign(pch);
				if(i==16)															//提取recard內容 
					recard.assign(pch);
   				pch = strtok (NULL," ");
    			i++;
    		
    			
    			
  			}
  			if(recard=="PTR"||recard=="MX"||recard=="TXT")							//只分析A recard 
  			{
  				int fil;
  			}
  			else if(domainname!="."&&domainname!=" "&&filtering(toplist, whitelist, domainname, topcount, whitecount)==1&&ipfiltering(mail_server,ipaddress,11)==1)
			{
				dnsprocess(domainlist,iplist,domainname,ipaddress);
  			}
			lins++;
		}
		
	}
	
	domainlist->erase_single_querydomain();											//移除掉只被查詢一次網域 
	fstream writefile;																//把分析資訊寫進檔案中 
	char adj[11]={"result.txt"};
	strcat(filename,adj);
	writefile.open(filename,ios::out | ios::app);
	if(writefile.fail())
		cout<<"file written error"<<endl;
	else
	{
		writefile<<"The totally lines : "<<lins<<endl;
		writefile<<"The size of IP linked list is : "<<iplist->size()<<endl;	
		writefile<<"The size of domain linked list before remove isograph is : "<<domainlist->size()<<endl;
		erase_isolated_graph(domainlist,iplist);
		writefile<<"The size of domain linked list after remove isograph is : "<<domainlist->size()<<endl;
		domainlist->erase_single_querydomain();
		writefile<<"The size of domain linked list after remove single query domain is : "<<domainlist->size()<<endl;
		writefile<<"			DNS process finished!!!!!!!"<<endl;
		writefile.close();
	}
	similarity_calculate(domainlist);												//計算網域之間的相似度並輸出鄰接矩陣 
	char temp[300],temp1[30];														//打要進行的 
	strcpy(temp, "groupidentify.exe ");
	strcat(temp,"simulationmatrix.txt ");
	itoa(domainlist->size(), temp1, 10);
	strcat(temp,temp1);
	spawnl(P_WAIT, "groupidentify.exe", temp, NULL);
	group_verify(domainlist);
	remove("simulationmatrix.txt");
	remove("cluster.txt");
	//write_domain_to_file(domainlist, filename);
	cout<<filename<<" final!!"<<endl;
    return 0;
}

void similarity_calculate(LinkedList<string> *domainlist)
{
	float threshold=0.7;
	int data_height=domainlist->size(), data_width=domainlist->size();		//宣告動態矩陣 
	float **data;
	data = new float*[data_height];
	for(int i = 0; i < data_height; i++)
    	data[i] = new float[data_width];
    for(int w=0;w<domainlist->size(); w++)
	{
		for(int e=0;e<domainlist->size(); e++)
		{
			data[w][e]=0;
		}
	}
			
	fstream writematrix;										
	writematrix.open("simulationmatrix.txt",ios::out | ios::app);
	if(writematrix.fail())
		cout<<"simulation matrix written error"<<endl;
	
	int i=0;
	Node<string> *startnode =domainlist->front();
	while(startnode!=0)														//關係矩陣計算 
	{
		int j=0;
		Node<string> *comparenode=domainlist->front();
		while(comparenode!=0)
		{
			if(i==j)
			{
				data[i][j]=1;
				break;
			}
			else
			{
				int samenode=0;
				Node<string> *tempnode1=startnode->previous;
				while(tempnode1!=0)
				{
					Node<string> *tempnode2=comparenode->previous;
					while(tempnode2!=0)
					{
						if(tempnode1->value==tempnode2->value)
						{
							samenode++;
						}
						tempnode2=tempnode2->previous;
					}
					tempnode1=tempnode1->previous;
				}
			
				float c=(float)samenode/((startnode->ipcount)+(comparenode->ipcount)-samenode);
				if(c<0.05)
				{
					data[i][j]=0;
				}
				else
				{
					data[i][j]=c;
				}
				
			}
			comparenode=comparenode->next;
			j++;
		}
		startnode=startnode->next;
		i++;
	}
	for(int a=0; a<data_height; a++)
		for(int s=0; s<a; s++)
		{
			data[s][a]=data[a][s];
		}
	
	float **sim;											//宣告動態矩陣
	sim = new float*[data_height];
	for(int i = 0; i < data_height; i++)
    	sim[i] = new float[data_width];
    for(int w=0;w<domainlist->size(); w++)
	{
		for(int e=0;e<domainlist->size(); e++)
		{
			sim[w][e]=0;
		}
	}
    
    /*for(int a=0; a<data_width; a++)						//歐式距離計算 
    {	
    	float maxidistance=0;
    	for(int s=0; s<data_height; s++)
    	{	
			float sqersum=0;
    		for(int d=0; d<data_width; d++)
    		{
    			sqersum=pow(data[a][d]-data[s][d],2)+sqersum;
    		}
    		sim[a][s]=sqrt(sqersum);
    		if(sim[a][s]>maxidistance)
    			maxidistance=sim[a][s];
		}
		//for(int s=0; s<domainlist->size(); s++)
		//sim[a][s]=sim[a][s]/maxidistance;
    }*/ 
    
    for(int a=0; a<data_width; a++)
    {
    	for(int s=0; s<data_height; s++)
    	{
    		float entry=0;
    		float non_zerocount=0;
    		for(int d=0; d<domainlist->size(); d++)
    		{
    			if(data[a][d]==0&&data[s][d]==0)
    			int asd;
				else
				{
					entry=entry+fabs(data[a][d]-data[s][d]);
					non_zerocount=non_zerocount+1;
    			}
    		}
    		if(non_zerocount==0)
    			sim[a][s]=0;
    		else
				sim[a][s]=1-entry/non_zerocount;
    	}
    }
    
	/*float threshold=0.7;
	for(int w=0;w<domainlist->size(); w++)			
	{
		for(int e=0;e<domainlist->size(); e++)
		{
			if(w==e)
			sim[w][e]=0;
			if(sim[w][e]<=threshold)
			sim[w][e]=0;
			else
			sim[w][e]=1;
		}
	}*/
		
	
	for(int q=0; q<data_width; q++)					//把矩陣寫入檔案 
	{
		for(int w=0; w<data_height; w++)
		{
			if(q==w)
				writematrix<<0<<" ";
			else if(sim[q][w]>threshold)
				writematrix<<1<<" ";
			else
				writematrix<<0<<" ";
		}
		writematrix<<endl;
	}
	for(int i=0; i<data_height; i++)			//刪除宣告矩陣 
    		delete []data[i];		
	delete []data; 
	for(int i=0; i<data_height; i++)
    		delete []sim[i];
	delete []sim;
		
}

int filtering(string toplist[], string whitelist[], string domain, int topcount, int whitecount)
{
	for(int k=0; k<topcount; k++)
	{
		if(domain.find(toplist[k])<10000)
		return 0;
	}
	for(int k=0; k<whitecount; k++)
	{
		if(domain.find(whitelist[k])<10000)
		return 0;
	}	
	return 1;
}

int ipfiltering(string ipaddress[], string ip, int ipcount)
{
	for(int k=0; k<ipcount; k++)
	{
		if(ip.find(ipaddress[k])<10000)
		return 0;
	}	
	return 1;
}

void dnsprocess(LinkedList<string>* domainlist,LinkedList<string>* iplist,string domainname,string ipaddress)
{
	if(domainlist->search_domain(domainname)==0)
	{
		domainlist->push_back(domainname);
		domainlist->push_IP(ipaddress,domainlist->last);	
	}
	else
		domainlist->push_IP(ipaddress,domainlist->search_domain(domainname));
	if(iplist->search_domain(ipaddress)==0)
	{
		iplist->push_back(ipaddress);
		iplist->push_IP(domainname,iplist->last);	
	}
	else
		iplist->push_IP(domainname,iplist->search_domain(ipaddress));
	
}


void PrintList(LinkedList<string>* list)
{
    Node<string>* node = list->front();
    while (node != 0)
    {
    	Node<string>* tempNode = node;
    	while(node->previous!=0)
    	{
			cout << node->previous->value << " XXX ";
			node = node->previous;
    	}
        cout <<tempNode->value <<"OOO "<<tempNode->ipcount<<" OOO "<<endl;
        node = tempNode->next;
	}
}

void erase_isolated_graph(LinkedList<string>* domainlist,LinkedList<string>* iplist)
{
	
	Node<string>* node = domainlist->front();
	while(node!=0)
	{
		
		if(node->ipcount==1)
		{
			Node<string>* ipnode = iplist->front();
			Node<string>* chenode = node->previous;
			while(chenode->value!=ipnode->value)
			{
				ipnode = ipnode->next;
			}
			if(ipnode->ipcount==1)
			{
				domainlist->erase(node);
				iplist->erase(ipnode);
			}
		}
		node=node->next;
	}
}


void write_domain_to_file(LinkedList<string>* domainlist, string filename)
{
	fstream writedomain;
	writedomain.open("domainlist.txt",ios::out | ios::app);
	if(writedomain.fail())
		cout<<"file123 written error"<<endl;
		
	Node<string>* node = domainlist->front();
    while (node != 0)
    {
    	Node<string>* tempNode = node;
		writedomain<<tempNode->value <<" ";
    	while(node->previous!=0)
    	{
			writedomain<< node->previous->value <<" ";
			node = node->previous;
    	}
    	writedomain<<endl;
        node = tempNode->next;
	}
}

void group_verify(LinkedList<string>* domainlist)
{
	int groupcount=0, *group, totalldomainvertice=domainlist->size();
	group=new int[totalldomainvertice];
	fstream cluster_result;
	char buffer[400];
	cluster_result.open("cluster.txt",ios::in);
	if(!cluster_result)
		cout<<"cluster_result readfile error!!!!!!";
	else
	{
		int linecount=1;
		while(!cluster_result.eof())
		{
			if(linecount==1)
			{
				cluster_result.getline(buffer,sizeof(buffer));
				groupcount=atof(buffer);
				
				linecount++;
			}
			else
			{
				cluster_result.getline(buffer,sizeof(buffer));
				group[linecount-2]=atof(buffer);
				linecount++;
			}
		}
	}
	
	fstream writeresult;
	char resulttemp[500];
	strcpy(resulttemp,filename);
	strcat(resulttemp,"_Detectresult.csv");
	writeresult.open(resulttemp,ios::out | ios::app);
	if(writeresult.fail())
		cout<<"Result written error"<<endl;
	
	int numnodesep[groupcount];
	for(int r=0; r<groupcount; r++)
		numnodesep[r]=0;
	for(int e=0; e<totalldomainvertice; e++)
	{
		numnodesep[group[e]-1]++;
	}

	for(int j=1; j<=groupcount; j++)
	{
		
		int *placegroup, temp=0;
		placegroup=new int[numnodesep[j-1]];
		for(int g=0; g<numnodesep[j-1]; g++)
		{
			placegroup[g]=0;
		}
    	for(int y=0; y<totalldomainvertice; y++)
    	{
    		if(group[y]==j)
    		{
    			placegroup[temp]=y;
    			temp++;
    		}
    	}
    	string domainplace[numnodesep[j-1]];
		LinkedList<string>* random_walk = new SinglyLinkedList<string>();
		int w=0, e=0;
		Node<string>* node = domainlist->front();
		while (node != 0)
    	{
    		Node<string>* tempNode = node;
    		string tempdomain=node->value;
    		if(placegroup[e]==w)
    		{
    			domainplace[e]=tempdomain;
    			while(node->previous!=0)
    			{
    				string tempip=node->previous->value;
					random_walk_list_process(random_walk, tempdomain, tempip);
					random_walk_list_process(random_walk, tempip, tempdomain);
					node = node->previous;
    			}
    			e++;
    		}
    		if(e==numnodesep[j-1])
    		{
    			break;
    		}
        	node = tempNode->next;
        	w++;
		}
		
		string domain[random_walk->size()];
		Node<string>* tempnode =random_walk->front();
		int p=0;
		while(tempnode!=0)
		{
			domain[p]=tempnode->value;
			tempnode = tempnode->next;
			p++;
		}
			
		int **RW_matrix;
		RW_matrix = new int*[random_walk->size()];
		for(int i = 0; i < random_walk->size(); i++)
    		RW_matrix[i] = new int[random_walk->size()];
    	for(int s=0;s<random_walk->size(); s++)
		{
			for(int e=0;e<random_walk->size(); e++)
			{
				RW_matrix[s][e]=0;
			}
		}
		
		int u=0;
		Node<string>* matrixnode =random_walk->front();
		while(matrixnode!=0)
		{
			tempnode=matrixnode;
			while(matrixnode->previous!=0)
			{
				RW_matrix[u][find_place(domain,matrixnode->previous->value,random_walk->size())]=1;
				matrixnode=matrixnode->previous;
			
			}
			RW_matrix[u][u]=tempnode->ipcount;
			matrixnode=tempnode->next;
			u++;
		}
		
		int RW_step=100*random_walk->size(),RW_log[random_walk->size()];
		for(int d=0; d<random_walk->size(); d++)
			RW_log[d]=0;
		srand((unsigned) time(0));
		int start_node=(rand()%random_walk->size());
		for(int f=0; f<RW_step; f++)
		{
			int next_step[RW_matrix[start_node][start_node]], u=0;
			for(int g=0; g<random_walk->size(); g++)
			{
				if(RW_matrix[start_node][g]==1)
				{
					next_step[u]=g;
					u++;
				}
			}
			int l=RW_matrix[start_node][start_node];
			int rand_number=(rand()%l);
			start_node=next_step[rand_number];
			RW_log[next_step[rand_number]]++;
		}

		int visiteddomain[numnodesep[j-1]], visitedip[random_walk->size()-numnodesep[j-1]];
		int visiteddomain_count=0, visitedip_count=0;
		for(int m=0; m<random_walk->size(); m++)
		{
			int boolvalue=0;
			for(int n=0; n<numnodesep[j-1]; n++)
			{
				if(domain[m]==domainplace[n])
				{
					boolvalue++;
				}
			}
			if(boolvalue==1)
			{
				visiteddomain[visiteddomain_count]=RW_log[m];
				visiteddomain_count++;
			}
			else
			{
				visitedip[visitedip_count]=RW_log[m];
				visitedip_count++;
			}
		}

		if(numnodesep[j-1]==1)
		{
			int y;
		}
		else if(stand_deviation(visiteddomain, numnodesep[j-1])<=0.2&&stand_deviation(visitedip, random_walk->size()-numnodesep[j-1])<=0.2&&numnodesep[j-1]>5)
		{
			writeresult<<"totall domain : "<<","<<numnodesep[j-1]<<endl;
			int x=0, c=0;
			Node<string>* node = domainlist->front();
			while (node != 0)
    		{
    			Node<string>* tempNode = node;
    			if(placegroup[c]==x)
    			{
    				writeresult<<node->value<<",";
    				while(node->previous!=0)
    				{
    					writeresult<<node->previous->value<<",";
						node = node->previous;
    				}
    				c++;
    				writeresult<<endl;
    			}
    			if(c==numnodesep[j-1])
    			{
    				break;
    			}
        		node = tempNode->next;
        		x++;
			}
			writeresult<<endl;
		}
		
		
		
	}
}

void random_walk_list_process(LinkedList<string>* domainlist, string domainname, string ipaddress)
{
	if(domainlist->search_domain(domainname)==0)
	{
		domainlist->push_back(domainname);
		domainlist->push_IP(ipaddress,domainlist->last);	
	}
	else
		domainlist->push_IP(ipaddress,domainlist->search_domain(domainname));
}

int find_place(string domain[], string comparedomain, int size)
{
	for(int i=0; i<size; i++)
	{
		if(domain[i]==comparedomain)
		{
			return i;
		}
	}
	return -1;
}
float stand_deviation(int data[], int data_size)
{
	float sum=0;
	float mean=0,SD=0;
	//cout<<"data!! ";
	for(int y=0; y<data_size; y++)
	{
		//cout<<data[y]<<" ";
		sum=sum+data[y];
	}
	//cout<<endl;
	mean=(float)sum/data_size;
	//cout<<sum<<"  mean!  "<<mean<<endl;
	for(int y=0; y<data_size; y++)
	{
		//SD=pow(data[y]-mean,2)+SD;
		SD=pow(data[y]/mean-1,2)+SD;
	}
	SD=sqrt(SD/data_size);
	return SD;
}


