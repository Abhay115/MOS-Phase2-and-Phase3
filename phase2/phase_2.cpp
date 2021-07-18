#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>
using namespace std;
//GD,PD,CR,LR,SR,H,BT DONE
//Global Declarations ********
ifstream fin;
ofstream fout;
int rfcount=0;
char mem[300][4], buff[100], ir[5], r[4];
string str;
int wfcount=0;
int ic, si=0, num;
bool c;
int flag=0;
int buffsize=0;
int lcount=0;
int PTR, RA;
int PTE;
int PI=0;
int SI=0;
int TI=0;
char err[50];
int pg_fault=0;
int term=0;
class pcb
{  public:
    int pid;
    int ttl,tll,ttc,tlc;//ttl=total time limit , tll=total line limit, ttc= total time count ,tlc= total line count
    void initialize(int x,int y)
     {
         ttl=x;
         tll=y;
         ttc=0;
         tlc=0;
     }

}p1;

/////////////////////////////  ADD MAP////////////////////////////
void addMap(int VA)
{
    PI=0;
    int VA1, a, b;
    int frameNo;
    if(VA>=0 && VA <= 99)
    {
        VA1 = VA/10;
        PTE = PTR + VA1;
        cout<<"Value of pte: "<<PTE<<endl;
        cout<<"Value of ptr: "<<PTR<<endl;
        if(mem[PTE][2] == ' ' && mem[PTE][3] == ' ')
        {
            PI=3; cout<<"Value of  pi  in mos: "<<PI<<endl;
            return;
        }
        a = (mem[PTE][2] - '0');
        b = (mem[PTE][3] - '0');
        frameNo = a*10 + b;
        cout<<"Frame Number: "<<frameNo;
        RA = (frameNo * 10) + (VA%10);
        cout<<"....."<<VA  %10<<endl;
        cout<<"Ra in addmap "<<RA<<endl;
    }
    else
    {
        PI = 2;
    }
}


void load();
//*********initialsize*********************
void init()
{
  for(int i=0; i<300; i++)
  {
      for(int j=0; j<4; j++)
          mem[i][j]=' ';
  }
  for(int k=0; k<100; k++)
      buff[k]=' ';

  for(int l=0; l<4; l++)
      ir[l]=' ';

    ic=0;
    TI=0;PI=0;SI=0;
}

//Allocate*************
int allocate()
{
    label:
    int allnum;
    allnum = rand()%30;
    cout<<"lcount= "<<lcount<<endl;
    cout<<"Allnum= "<<allnum<<endl;
    if(lcount==0)
        { cout<<"Value of lcount= "<<lcount<<endl;lcount++; return allnum;}
    else
    { if((mem[allnum*10][0]=='*')||(mem[allnum*10][0]!=' '))
        {   cout<<"value of allnum" <<mem[allnum][0]<<endl;
            goto label;
        }
       else
        return allnum;
    }

}

//********************TERMINATE PROGRAM**********************
void terminateFun(char err[50])
{
    fout.open("output2.txt", ios::app);
    fout<<err<<endl;
    fout<<"IR: ";
    for(int i=0;i<4;i++)
        fout<<ir[i];
    fout<<endl;

    fout<<"IC= "<<ic<<endl;
    fout<<"TI= "<<TI<<"  PI="<<PI<<"  SI="<<SI<<endl;
    fout<<"TTC= "<<p1.ttc<<"   TTL= "<<p1.ttl<<"   TLC= "<<p1.tlc<<"   TLL= "<<p1.tll<<endl;
    fout<<endl;
    fout<<endl;
    fout.close();
}


void Terminate(int a)      //Displays two line termination message on the screen
{   term=1; cout<<"In terminate function....."<<endl;
    switch(a)
    {
        case 0 :p1.ttc=p1.ttc+1;
                 ic=ic+1;
                cout<<"NO ERROR"<<endl;
                strcpy(err,"No error");
                break;

        case 1:
                cout<<"\nError (1) :Out Of Data Error"<<endl;
                cout<<"Program Terminated Abnormally"<<endl;
                 strcpy(err,"Out of Data Error");
                 terminateFun(err);
                break;
        case 2://working
                cout<<"\nError(2) :Line limit Exceeded"<<endl;
                cout<<"Program Terminated Abnormally"<<endl;
                strcpy(err,"Line limit exceeded");
                terminateFun(err);
                break;
        case 3:  //working
                cout<<"\n Error(3) :Time Limit Exceeded"<<endl;
                cout<<"Program Terminated Abnormally"<<endl;
                strcpy(err,"Time limit exceeded");
                terminateFun(err);
                  break;
    case 4: //working
        cout<<"\n Error(4) :Operation Code Error"<<endl;
        cout<<"Program Terminated Abnormally"<<endl;
        strcpy(err,"Operation code error");
        terminateFun(err);
        break;
    case 5: //working
                cout<<"\n Error(5) :Operand Error"<<endl;
                cout<<"Program Terminated Abnormally"<<endl;
                strcpy(err,"Operand error");
                terminateFun(err);
                break;
    case 6:
                cout<<"\n Error (6) :Invalid Page Fault"<<endl;
                cout<<"Program Terminated Abnormally"<<endl;
                strcpy(err,"Invalid page fault");
                terminateFun(err);

                break;
    }
   

}

//*****************GET DATA*******************
void readFun()
{ string s;
	
	if(rfcount==0)
	{
	   getline(fin,str,'\n');  cout<<"String :" <<str<<endl;
	   rfcount++;
	}
        else
         {  getline(fin,s,'\n');
            if((s[0]=='$')&&(s[1]=='E'))
                { ic++; Terminate(1);
                 return;
                }
             if(s[0]!='$')
               {
                 int a;
                   for(a=0;s[a]!='\0';a++)
                       { str[a]=s[a];}
                   str[a]='\0';
               }
         }
	int k=0;
	int i,j;
	for(i=0;str[i]!='\0';i++,k++)
	{
		buff[k]=str[i];
	}
	buff[k]='\0';

	k=0;
	j=RA;

	for(int i=0; buff[i]!='\0'; i++)
	{
	    mem[j][k++]=buff[i];
		if(k==4)
		{
			j++;
			k=0;
		}
	}
//cout<<"At the end of read function memory stack : ******"<<endl;
//            for(int l=0;l<300;l++)
//            {
//                cout<<"mem["<<l<<"]=";
//                    for(int j=0;j<4;j++)
//                    {
//                        cout<<mem[l][j];
//                    }
//                cout<<endl;
//           }
}

//******************PUT DATA*******************************
void writeFun()
{   p1.tlc=p1.tlc+1;
     if(p1.tlc>p1.tll)
     {
        Terminate(2);
     }
     if(term==0)
    {
     cout<<"Inside write function"<<endl;
	if(wfcount!=0)
		{ fout.open("output2.txt", ios::app); }
	else
	{
		fout.open("output2.txt",ios::out);
		wfcount++;
	}

     cout<<"Value of num in write function= "<<num<<endl;
     cout<<"Addmap(num) = "<<endl;
     addMap(num);
     int k=RA;
     cout<<"Writing from memory location "<<k<<endl;

     int o,j, i=0;
     char Tbuff[40];

	for(int o=k; i<40; i++,o++)
	{
		for(j=0; j<4;j++,i++)
		{
			Tbuff[i]=mem[o][j];
		}
	}

    cout<<endl;
	int pos=0;
	for(int i=39;i>=0;i--)
	{
		if(Tbuff[i]!=' ')
		{
        	pos=i;
			break;
		}
	}
	k=0;
       cout<<" writing: ";
	for(int i=RA; k<=pos;i++,k++)
	{

		for(j=0;j<4;j++,k++)
		{
			fout<<mem[i][j];
			//cout<<mem[i][j];
		}

	}
	    fout<<endl;
		fout.close();
    }

}


//***********************MOS**************************
void mos()
{   int pos100;  cout<<"  TI= "<<TI<<" PI= "<<PI<<" IR= "<<ir[0]<<ir[1]<<endl;
    if(TI==0)
    {
         if((PI==3)&&(ir[0]!='G'))
           {    cout<<"calling terminate(6) function"<<endl;
                Terminate(6);
           }
          else
          {

        switch(SI)
        {  cout<<"t1==0 and si..."<<endl;
            case 1: if(pg_fault==1)   //working
		            {  cout<<"Handling valid page fault"<<endl;
		               pos100=allocate();
                        mem[PTE][0]=' ';
                        mem[PTE][1]=' ';
                        mem[PTE][2]=pos100/10+48;
                        mem[PTE][3]=pos100%10+48;
		            }
                    if(pg_fault==0)
                    readFun();
                    break;
            case 2: writeFun();   //working
                    break;
            case 3: Terminate(0);   //working
                    break;
            default:
                    break;
	  }
          }
	           if(PI==1)
           { cout<<"TI==0 AND PI==1....."<<endl;  //working
             Terminate(4);
           }
        else if(PI==2)
            {   if(term==0)
                 { cout<<"TI==0 AND PI==2...."<<endl;  //operand error not thought off
                   Terminate(5);
                 }
            }

  }
    
    else if(TI==2)
    {  
        cout<<"if ti==2 and si...."<<endl;cout<<"Value of si= "<<SI<<endl;
        switch(SI)
        {
            case 1: Terminate(3);
                     break;
            case 2:
                     Terminate(3);
                    break;
            case 3: Terminate(0);
                   break;
        }
        cout<<"TI==2 AND PI...."<<endl;
        switch(PI)
        {  
            case 1: Terminate(3);
                    break;
            case 2: Terminate(3);
                    break;
            case 3: Terminate(3);
                    break;
        }
    }

}


void exeUserProgram()
{  cout<<"Buffsize = "<<buffsize<<endl;
   cout<<"In execUserPorgram**************"<<endl;
    TI=0;
    for(ic=0;ic<buffsize;)
       {  
           if(term==0)
           {
            cout<<"\nIteration starts......"<<endl;
               // PI=0;
             cout<<"TLC= "<<p1.tlc<<"  TTC= "<<p1.ttc<<endl;
          addMap(ic);
          SI=0;

      cout<<"pi= "<<PI<<endl;
           if(PI == 2)
            {
               cout<<"Operand Error Occurred!!"<<endl;
               break;
            }
             cout<<"RA Value:"<<RA<<endl;

            for(int j=0; j<4; j++)
        	ir[j]=mem[RA][j];
    	cout<<"IC : "<<ic<<endl;
    	num=ir[2]-'0';
    	num=num*10+(ir[3]-'0');
    	cout<<"num= "<<num<<endl;
        addMap(num);
        for(int p=0;p<4;p++)
            cout<<ir[p];
        cout<<endl;
        cout<<"RA Value:"<<RA<<endl;

        if(ir[0]=='G' && ir[1]=='D')
    	{
    	    SI=1;
    	     if(PI == 3)
            {  p1.ttc=p1.ttc-1;
               cout<<"Valid page fault occurred "<<endl;
               pg_fault=1;
               mos();
               p1.ttc=p1.ttc+1;
               ic--;
               cout<<"IC after decrementing "<<ic<<endl;
               pg_fault=0;

            }
    	    else
	      {
	          mos();
	      }
    	}
    	else if(ir[0]=='P' && ir[1]=='D')
    	{  cout<<"Value of TI= "<<TI<<endl;
    	    SI=2;
            mos();
    	}
    	else if(ir[0]=='H')
    	{
    	    SI=3; PI=0;
	       mos();
	       break;
	    }
	    else if(ir[0]=='L'&&ir[1]=='R')
        {   addMap(num);
             mos();
            for(int j=0;j<4;j++)
                r[j]=mem[RA][j];
            cout<<"Content of register = "<<endl;
            cout<<endl;
        }
        else if(ir[0]=='S'&&ir[1]=='R')
        {   addMap(num);

             int pos100=allocate();
                        mem[PTE][0]=' ';
                        mem[PTE][1]=' ';
                        mem[PTE][2]=pos100/10+48;
                        mem[PTE][3]=pos100%10+48;
            addMap(num);
            for(int l=0;l<4;l++)
                mem[RA][l]=r[l];
                cout<<"Content of memeory location "<<RA<<endl;
             for(int l=0;l<4;l++)
                cout<<mem[RA][l];

             cout<<endl;
        }

        else if(ir[0]=='C'&&ir[1]=='R')
        {  addMap(num);
            mos();
            for(int l=0;l<4;l++)
            {
			if(r[l]==mem[RA][l])
				c=true;
			else
				{c=false; break;}
            }
            cout<<"Value of C= "<<c<<endl;
        }
        else if(ir[0]=='B'&&ir[1]=='T')
        {   mos();
            if(c==true)
			 ic=num-1;
		    cout<<endl<<"Contents of ic: "<<ic<<endl;
        }
        else
         {
            PI=1;
            mos();
         }

   ic=ic+1;
   cout<<"IC value after increamenting: "<<ic<<endl;
//   for(int i=0;i<300;i++)
//{  cout<<"mem["<<i<<"] = ";
//    for(int j=0;j<4;j++)
//        cout<<mem[i][j];
//    cout<<endl;
//}
      if(term==0)
        p1.ttc=p1.ttc+1;

        if(p1.ttc==p1.ttl)
         TI=2;

     }
     else
        break;

}
}
void stexe()
{
    ic=0;
    exeUserProgram();
}

void load()
{
    int allnum;
do
{
  si=0;
  int k=0;
    getline(fin,str,'\n');
    cout<<str<<endl;
    for(int i=0; str[i]!='\0'; i++)
    {     buff[k]=str[i];
          k++;
    }
        buff[k]='\0';
    cout<<"buff= ";
//     for(int l=0;buff[l]!='\0';l++)
//          cout<<buff[l];
         char str100[40];

         strcpy(str100,buff);

if(buff[0]=='$' && buff[1] == 'A' && buff[2] == 'M' && buff[3] == 'J')
{    init();
      term=0;   TI=0; PI=0;  SI=0;
           buffsize=0; flag=0;
           lcount=0;
       //For TTL
       char a[4];
       int z;
       int i;
       for(i=0, z=8; i<4; i++)
       {
            a[i]=str100[z+i];
       }
    cout<<"str100= ";
    for(i=0;str[i]!='\0';i++)
         cout<<str100[i];

       int TTL;
       TTL=a[2]-'0';
    	TTL=TTL*10+(a[3]-'0');

       //For TLL
       char b[4];
       for(i=0, z=12; i<4; i++)
       {
           b[i]=str100[z+i];
       }
        int TLL;
       TLL=b[2]-'0';
    	TLL=TLL*10+(b[3]-'0');

     //Print for checking
     cout<<"a[i]=";
     for(i=0; i<4; i++)
     {
         cout<<a[i];
     }
     cout<<endl;
     cout<<"b[i]=";
      for(i=0; i<4; i++)
     {
         cout<<b[i];
     }

     p1.initialize(TTL, TLL);
     cout<<"\n TTL ="<<p1.ttl<<"  TLL= "<<p1.tll<<endl;
     allnum = allocate();
    PTR=allnum*10;
    cout<<"PTR=  "<<PTR<<endl;

	getline(fin,str,'\n');
	k = 0;
    for(int i=0; str[i]!='\0'; i++)
 	buff[k++]=str[i];//It stores GD10PD10H
        buff[k]='\0';

    buffsize=0;
    cout<<"Value of buf=  "<<buff<<endl;
    for(int p=0;buff[p]!='\0'&&p<=k;)
    {
        p=p+4;
        buffsize++;
    }

    for(int i=PTR; i<PTR+10; i++)
   {
        mem[i][0]='*';
        cout<<"        mem["<<i<<"][0]= "<<mem[i][0]<<endl;
   }
   cout<<"Buffsize = "<<buffsize<<endl;

   int j, instruction_pos=0;
  PTE=allocate();
  instruction_pos=PTE*10;
for(int i=PTR;i<(PTR+10);i++)
{
    if(mem[i][0]=='*')
    {
        mem[i][0]=' ';
        mem[i][1]=' ';
        mem[i][2]=PTE/10+48;
        mem[i][3]=PTE%10+48;
        break;
    }
}

int count=0;
cout<<"Buffer= "<<buff<<endl;
 z=0;
int a1=0,h;
cout<<"value of k="<<k<<endl;
for(int i=0;buff[i]!='\0'&&i<k;i=i+4)
{
    if(count>=10)
    {  count=0;
         PTE=allocate();
         for(h=PTR;h<(PTR+10);h++)
            {  cout<<"Value of pte:"<<PTE<<endl;
                if(mem[h][0]=='*')
                {
                    mem[h][0]=' ';
                    mem[h][1]=' ';
                    mem[h][2]=PTE/10+48;
                    mem[h][3]=PTE%10+48;
                    break;
                }
            }
                if(a1%10==0)
                  {instruction_pos=PTE*10; z=0;}

                a1++;

    }
    for(j=0;j<4;j++)
       {
            mem[instruction_pos+z][j]=buff[i+j];
            if(buff[i+j]=='H')
            {
                i=i-3;
                break;
            }

       }
        //buffsize=j;
       // cout<<"Buffisize= "<<buffsize<<endl;
       z++;
       count++;
}

cout<<"IN LOAD FUNCTION******************"<<endl;
/*for(i=0;i<300;i++)
    {
        cout<<"mem["<<i<<"]=";
        for(j=0;j<4;j++)
        {
            cout<<mem[i][j];
        }
        cout<<endl;
    }*/
  // buffsize=j;
   //cout<<"Buffsize= "<<buffsize<<endl;
}
else if(buff[0]=='$' && buff[1] == 'D' && buff[2] == 'T' && buff[3] == 'A')
	stexe();
else if(buff[0]=='$'&&buff[1]=='E'&&buff[2]=='N'&&buff[3]=='D')
{
   terminateFun(err);
}
}while(!fin.eof());

init();
fin.close();

}

int main()
{
  fin.open("input2.txt");
  fout.open("output2.txt");
  load();
return 0;
}