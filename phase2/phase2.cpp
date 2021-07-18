#include<iostream>
#include<fstream>
#include<ctype.h>
using namespace std;

 char M[300][4]; //Physical Memory
 char IR[4]; //Instruction Register (4 bytes)
 char R[4]; //General Purpose Register (4 bytes)
 int IC; //Instruction Counter Register (2 bytes)
 int SI; //Service Interrupt
 int PI; //program interrupt
 int TI; //Time interrupt
 bool C; //Toggle (1 byte)
 char buffer[40];
 int PTR[4]; //Page table register
 int VA; //Virtual Address
 int RA; //Real Address

 int jobid;
 int TTL; //Total Time Limit
 int TLL; //Total Line Limit
 int TTC; //Total Time Counter
 int LLC; //Line Limit Counter
 
 int filled[30];

 void init();
 void LOAD();
 void Execute();
 void MOS();
 int random();
 fstream infile;

 fstream outfile;

int random()
{
 for(int i=0;i<30;i++)
 {
 filled[i] = 0;
 }
 int frame;
 do{
 frame = (rand() % 30);
 }
 while(filled[frame] != 1);
 return frame;
}

void init()
{
 for(int i = 0; i<300; i++)
 {
 for(int j = 0; j<4; j++ )
 {
 M[i][j]=' ';
 }
 }
 /* for(int i = 0; i<300; i++)
 {
 cout<<"M["<<i<<"]\t";
 for(int j = 0; j<4; j++ )
 {
 cout<<M[i][j];
 }
 cout<<endl;
 } */
 char IR[4] = {'A'};
 char R[4] = {'A'};
 int IC;
 bool C = false;

 TTC = 0;
 LLC =0;
 PI=0;
 TI=0;
}

void MOS()
{

 if(SI == 1 && TI!=2 && PI==0) //Read Mode
 {
 for(int i=0;i<=39;i++)
 buffer[i]='\0';
 //infile.getline(buffer,40);
 //infile.getline(buffer,40);
 infile.getline(buffer,40);
 if(buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3
] == 'D')
 {
 PI = 6;
 MOS();
 }
 else
 {
 int k = 0;
 int i = IR[2]-48;
 i = i*10;
 for( int l=0 ; l<10 ; ++l)
 {
 for(int j = 0 ; j<4; ++j)
 {
 M[i][j] = buffer[k];
 k++;
 }
 if(k == 40)
 {
 break;
 }
 i++;
 }
 //cout<<buffer<<endl<<endl;
 // cout<<"\t";

 // for(int i = 0; i<300; i++)
 // {
 // cout<<"M["<<i<<"]\t";
 // for(int j = 0; j<4; j++ )
 // {
 // cout<<M[i][j]<<"\t";
 // }
 // cout<<"\n\t";
 // }
 }
 }
 else if(SI == 2 && TI!=2 && PI==0) //Write Mode
 {
 for(int i=0;i<=39;i++)
 buffer[i]='\0';
 int k = 0;
 int i = IR[2]-48;
 i = i*10;
 for( int l=0 ; l<10 ; ++l)
 {
 for(int j = 0 ; j<4; ++j)
 {
 buffer[k]=M[i][j];
 outfile<<buffer[k];
 //cout<<buffer[k];
 k++;
 }
 if(k == 40)
 {
 break;
 }
 i++;
 }
 outfile<<"\n";
 }
 else if(SI == 3 && TI!=2 && PI==0) //Terminate
 {
 outfile<<"Program terminated normally"<<endl;
 cout<<"Program terminated normally"<<endl;
 outfile<<"Total Time Limit->"<<TTL<<"\tTotal Line Limit->"<<TLL<<"\tTotal Time Count->"<<TTC<<"\tLine Limit Counter->"<<LLC<<endl;
 outfile<<"\n";
 outfile<<"\n";

 }
 else if(TI == 2 && PI==0) //Terminate
 {
 outfile<<"Program terminated abnormally- Total Time Count Exceeded"<<endl;
 outfile<<"Total Time Limit->"<<TTL<<"\tTotal Line Limit->"<<TLL<<"\tTotal Time Count->"<<TTC<<"\tLine Limit Counter->"<<LLC<<endl;
 outfile<<"\n";
 outfile<<"\n";
 }
 else if(PI == 4) //Terminate
 {
 outfile<<"Program terminated abnormally- Total Line Count Exceeded"<<endl;
 outfile<<"Total Time Limit->"<<TTL<<"\tTotal Line Limit->"<<TLL<<"\tTotal Time Count->"<<TTC<<"\tLine Limit Counter->"<<LLC<<endl;
 outfile<<"\n";
 outfile<<"\n";
 }
 else if(PI == 1) //Terminate
 {
 outfile<<"Program terminated abnormally- OPCODE ERROR"<<endl;
 outfile<<"Total Time Limit->"<<TTL<<"\tTotal Line Limit->"<<TLL<<"\tTotal Time Count->"<<TTC<<"\tLine Limit Counter->"<<LLC<<"\tINSTRUCTION COUNTER"<<IC<<endl;
 outfile<<"\n";
 outfile<<"\n";
 }
 else if(PI == 2) //Terminate
 {
 outfile<<"Program terminated abnormally- OPERAND ERROR"<<endl;
 outfile<<"Total Time Limit->"<<TTL<<"\tTotal Line Limit->"<<TLL<<"\tTotal Time Count->"<<TTC<<"\tLine Limit Counter->"<<LLC<<"\tINSTRUCTION COUNTER->"<<IC-1<<endl;
 outfile<<"\n";
 outfile<<"\n";
 }
 else if(PI == 6) //Terminate
 {
 outfile<<"Program terminated abnormally- OUT OF DATA ERROR"<<endl;
 outfile<<"Total Time Limit->"<<TTL<<"\tTotal Line Limit->"<<TLL<<"\tTotal Time Count->"<<TTC<<"\tLine Limit Counter->"<<LLC<<"\tINSTRUCTION COUNTER->"<<IC-1<<endl;
 outfile<<"\n";
 outfile<<"\n";
 }
 


}

void Execute()
{
 LLC = 0;
 TTC = 0;

 while(TTC<TTL)
 {
 for(int i = 0; i<4; i++) //Load in register
 {
 IR[i] = M[IC][i];
 //cout<<IR[i];
 }
 IC++;
 if(IR[0]== 'H')
 {
 IR[1]= 'A';
 IR[2]= '0';
 IR[3]= '0';
 }
 if(!isdigit(IR[2]) && !isdigit(IR[3]))
 {
 PI=2;
 MOS();
 break;
 }
 if(IR[0] == 'G' && IR[1] == 'D') //GD
 {
 SI = 1;
 MOS();
 TTC+=2;
 }
 else if(IR[0] == 'P' && IR[1] == 'D') //PD
 {
 if(LLC<TLL)
 {
 SI = 2;
 MOS();
 LLC+=1;
 TTC+=1;
 }
 else

 {
 PI = 4;
 MOS();
 break;
 }
 }
 else if(IR [0] == 'H') //H
 {
 SI = 3;
 TTC+=1;
 MOS();
 break;
 }
 else if(IR[0] == 'L' && IR[1] == 'R') //LR
 {
 int i = IR[2]-48;
 i = i*10 +( IR[3]-48) ;
 for(int j=0;j<=3;j++)
 R[j]=M[i][j];
 TTC+=1;
 //for(int j=0;j<=3;j++)
 //cout<<R[j];
 // cout<<endl;
 }
 else if(IR[0] == 'S' && IR[1] == 'R') //SR
 {
 int i = IR[2]-48;
 i = i*10 +( IR[3]-48) ;
 // cout<<i;
 for(int j=0;j<=3;j++)
 M[i][j]=R[j];
 TTC+=2;
 cout<<endl;
 }
 else if(IR[0] == 'C' && IR[1] == 'R') //CR
 {
 int f=0;
 int i = IR[2]-48;
 i = i*10 +( IR[3]-48) ;
 for(int j=0;j<=3;j++)
 if(M[i][j] != R[j])
 f=1;
 if(f==0)

 C=true;
 else
 C=false;
 TTC+=1;
 // cout<<C;
 }
 else if(IR[0] == 'B' && IR[1] == 'T') //BT
 {
 if(C== true)
 {
 int i = IR[2]-48;
 i = i*10 +( IR[3]-48) ;
 IC = i;
 TTC+=1;
 }
 }
 else if(isalpha(IR[0]) && isalpha(IR[1]))
 {
 //cout<<IR[0]<<IR[1];
 PI=1;
 MOS();
 break;
 }

 if(TTC>=TTL)
 {
 cout<<TTC;
 TI=2;
 MOS();
 break;
 }
 }
}

void LOAD()
{
 // for(int i = 0; i<300; i++)
 // {

 // for(int j = 0; j<4; j++ )
 // {
 // M[i][j]=' ';
 // }
 // }
 char buffer[300];
 cout<<"Reading Data from the input file"<<endl;
 do
 {
 for(int i=0;i<=99;i++) //clear buffer
 buffer[i]=' ';
 int i = 0;
 infile.getline(buffer,300);
 if(buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
 {
 jobid = buffer[6] - 48;
 jobid = jobid * 10 + (buffer[7]- 48);

 TTL = buffer[10]-48;
 TTL = TTL * 10 + (buffer[11]-48);
 TLL = buffer[14]-48;
 TLL = TLL * 10 + (buffer[15]-48);
 cout<<jobid<<"\t"<<TTL<<"\t"<<TLL<<endl;
 init();
 }
 else if(buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
 {
 IC = 00;
 Execute();
 }
 else if(buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
 {
 continue;
 }

 else{
 int k = 0;
 for(; i <300; ++i)
 {


 for(int j = 0 ; j<4; ++j)
 {
 if((buffer[k]=='P' && buffer[k+1]=='D' && buffer[k]=='H')||(buffer[k]=='C' && buffer[k+1]=='R' && buffer[k-1]=='H'))
 {
 i++;
j=0;
 }
 M[i][j] = buffer[k];
 k++;
 }

 if(k == 300)
 {
 break;
 }
 }
 }
 }while(!infile.eof()) ;
 /*for(int i = 0; i<300; i++)
 {
 cout<<"M["<<i<<"]\t";
 for(int j = 0; j<4; j++ )
 {
 cout<<M[i][j];
 }
 cout<<endl;
 } */
}
int main()
{


infile.open("input2.txt");
outfile.open("output2.txt");
 
 LOAD();
 return 0;
}