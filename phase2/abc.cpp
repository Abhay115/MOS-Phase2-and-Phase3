#include<bits/stdc++.h>
using namespace std;

unordered_map<int, string> errors = {{0, "No Error"},
                                     {1, "Out of Data"},
                                     {2, "Line Limit Exceeded"},
                                     {3, "Time Limit Exceeded"},
                                     {4, "Operation Code Error"},
                                     {5, "Operand Error"},
                                     {6, "Invalid Page Fault"}};

class OS
{
public:
    int job_id;
    int TTL;
    int TLL;
    int TTC = 0;
    int TLC = 0;
    int ptr;
    int visited[30];
    char Memory[300][4];
    char IR[4],R[4];
    int IC;
    bool Toggle;
    int SI,VA,RA,PI,TI,EM;
   
    fstream inFile;
    fstream outFile;

public:
    void init();
    void LOAD();
    void ExecuteUserProgram();
    void mos();
    int allocate();
    int AddressMap(int);
    void Terminate(int);
    void simulation();

};

void OS :: Terminate(int Code)
{
   // cout<<"\n"<< errors[Code]<<endl;
    outFile<<"Program Terminated Abnormally"<<endl;
    outFile<<errors[Code]<<endl;
    //outFile<<"IC = "<<IC<<"\t"<<"IR = "<<IR<<"\t"<<endl;
    cout<<errors[Code]<<endl;
}

int OS ::allocate()
{
    return (rand() % 30);
}

int OS ::AddressMap(int va)
{
    int pte = ptr * 10 + va / 10;

    string temp = "";

    if (Memory[pte][0] == '*')
    {
        cout << "Page Fault" << endl;
        //outFile << "Page Fault"<< endl;
        return -1;
    }
    
    else
    {
        for (int i = 0; i < 4; i++)
        {
            if(Memory[pte][i]!=' ')
                temp += Memory[pte][i];
        }
        
        return (stoi(temp) * 10 + va % 10);
    }
}

void OS ::init()
{
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Memory[i][j] = ' ';
        }
    }

    for(int i=0;i<30;i++)
    {
        visited[i]=0;
    }

    IR[4] = {'\0'};
    R[4] = {'\0'};
    IC = 0;
    Toggle = false;
    TTC = 0;
    TLC = 0;
    ptr = 0;
    VA = 0;
    PI = 0;
    TI = 0;
    EM = 0;
}

void OS ::mos()
{
    if (SI == 1)
    {
        string Buffer;
        getline(inFile, Buffer);

        if(Buffer[0]=='$' && Buffer[1]=='E' && Buffer[2]=='N' && Buffer[3]=='D')
        {
            EM=1;
            Terminate(1);
            return;
        }

        // int add = IR[2] - 48;
        // add = (add * 10) + (IR[3] - 48);

        int frame = allocate();
        while (visited[frame] != 0)
        {
            frame = allocate();
        }

        visited[frame] = 1;

        int i = ptr;
        i = i*10;
        //cout<<"\nPTR = "<<ptr<<endl;
        while (Memory[i][0] != '*')
        {
            i++;
        }

        int temp = frame / 10;
        
        Memory[i][0] = ' ';
        Memory[i][1] = ' ';
        Memory[i][2] = temp + 48;
        Memory[i][3] = frame % 10 + 48;

        int l = 0;
        frame = frame * 10;
        for (int i = 0; i < Buffer.length() && Buffer.length() < 40; i++)
        {
            Memory[frame][l++] = Buffer[i];
            if (l == 4)
            {
                l = 0;
                frame += 1;
            }
        }
    }
//write
    else if (SI == 2)
    {
        TLC+=1;
        if(TLC > TLL)
        {
            EM = 2;
            Terminate(2);
            return;
        }

        int add = IR[2] - 48;
        add = (add * 10);

        int ra = AddressMap(add);

        if (ra != -1)
        {
            string out;
 
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    out += Memory[ra][j];
                }
                ra += 1;
            }

            outFile << out << "\n";
        }

        else
        {
            EM = 6;
            Terminate(6);
            PI=3;
        }
    }

    else if (SI == 3)
    {
        outFile<<"IC = "<<IC<<"\t"<<"IR = "<<IR<<"\t"<<endl;
        outFile<<"TLC = "<<TLC<<"\t\t"<<"TTC = "<<TTC<<"\n\n\n";
    }
}

void OS ::ExecuteUserProgram()
{
    while (true)
    {
        if(PI!=0 || TI!=0 || EM!=0)
        {
            outFile<<"IC = "<<IC<<"\t"<<"IR = "<<IR<<"\t"<<endl;
            outFile<<"TLC = "<<TLC<<"\t"<<"TTC = "<<TTC<<"\n\n\n";
            break;
        }

        RA = AddressMap(IC);
        cout<<"\nRA - "<<RA<<endl;
        
        for (int i = 0; i < 4; i++)
        {
            IR[i] = Memory[RA][i];
        }

        
        if(Memory[RA][0]!='H' && (!isdigit(Memory[RA][2]) || !isdigit(Memory[RA][3])))
        {
            EM = 5;
            Terminate(EM);
            outFile<<"IC = "<<IC<<"\t"<<"IR = "<<IR<<"\t"<<endl;
            outFile<<"TLC = "<<TLC<<"\t\t"<<"TTC = "<<TTC<<"\n\n\n";
            break;
        }

       // IC++;

        int add = IR[2] - 48;
        add = (add * 10) + (IR[3] - 48);

        if((IR[0]=='G' && IR[1]=='D') || (IR[0]=='S' && IR[1]=='R'))
            TTC+=2;
        else
            TTC+=1;
        
        if(TTC > TTL)
        {
            EM = 3;
            TI = 2;
            Terminate(EM);
            outFile<<"IC = "<<IC<<"\t"<<"IR = "<<IR<<"\t"<<endl;
            outFile<<"TLC = "<<TLC<<"\t\t"<<"TTC = "<<TTC<<"\n\n\n";
            break;
        }

        if (IR[0] == 'L' && IR[1] == 'R')
        {
            int ra = AddressMap(add);
            if(ra == -1)
            {
                EM=6;
                Terminate(6);
            }
            else
            {
                for (int i = 0; i < 4; i++)
                    R[i] = Memory[ra][i];
            }
        }

        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int ra = AddressMap(add);
            
            if(ra!=-1)
            {
                for (int i = 0; i < 4; i++)
                    Memory[ra][i] = R[i];
            }

            else
            {
                int frame = allocate();
                while (visited[frame] != 0)
                {
                    frame = allocate();
                }

                visited[frame] = 1;

                int i = ptr;
                i = i*10;
                while (Memory[i][0] != '*')
                {
                    i++;
                }

                int temp = frame / 10;

                
                Memory[i][0] = ' ';
                Memory[i][1] = ' ';
                Memory[i][2] = temp + 48;
                Memory[i][3] = frame % 10 + 48;

                frame = frame * 10;
                for (int i = 0; i < 4; i++)
                    Memory[frame][i] = R[i];
            }
        }

        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            int flag = 0;
        
            int ra = AddressMap(add);
            
            if(ra = -1)
            {
                EM=6;
                Terminate(6);
            }

            else
            {
                for (int i = 0; i < 4; i++)
                {
                    if (R[i] != Memory[ra][i])
                        flag = 1;
                }

                if (flag == 1)
                    Toggle = false;
                else
                    Toggle = true;
            }
        }

        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            if (Toggle == true)
                IC = add;
        }

        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            mos();
        }

        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            mos();
        }

        else if (IR[0] == 'H')
        {
            SI = 3;
            mos();
            cout << "Program ended" << endl;
            break;
        }

        else
        {
            EM = 4;
            Terminate(EM);
            outFile<<"IC = "<<IC<<"\t"<<"IR = "<<IR<<"\t"<<endl;
            outFile<<"TLC = "<<TLC<<"\t\t"<<"TTC = "<<TTC<<"\n\n\n";
            break;
        }

        IC++;

    }

     //simulation();
}
void OS :: simulation()
{
    TTC++;
    if( TTC==TTL)
    {
        TI=2;
        mos();
    }
    if(SI!=0 || PI!=0 || TI!=0)
    {
            mos();

    }
    else{
        ExecuteUserProgram();
    }
}

void OS ::LOAD()
{
    int block = 0;
    string Buffer;
    cout<<"Reading data"<<endl;
    while (getline(inFile, Buffer))
    {
        string str = "";
        for (int i = 0; i < 4; i++) 
        {
            str += Buffer[i];
        }

        if (str == "$AMJ")
        {
            init();
            ptr = allocate();
            cout<<"PTR - "<<ptr*10<<endl;

            for (int i = ptr * 10; i < ptr * 10 + 10; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    Memory[i][j] = '*';
                }
            }

            visited[ptr] = 1;

            string Sjobid = "";
            string STTL = "";
            string STLL = "";

            for (int i = 0; i < 4; i++)
            {
                Sjobid += Buffer[i + 4];
                STTL += Buffer[i + 8];
                STLL += Buffer[i + 12];
            }

            job_id = stoi(Sjobid);
            TTL = stoi(STTL);
            TLL = stoi(STLL);
            cout<<"Job ID - "<<job_id<<endl;
            cout<<"TTL - "<<TTL<<endl;
            cout<<"TLL - "<<TLL<<endl;

        }

        else if (str == "$DTA")
        {
            ExecuteUserProgram();
            // return;
        }

        else if (str == "$END")
        {
            for(int i = 0; i<150; i++)
            {
                cout<<"M["<<i<<"]\t";
                for(int j = 0; j<4; j++ )
                {
                    cout<<Memory[i][j];
                }
                cout<<"\t\tM["<<i+150<<"]\t";
                for(int j = 0; j<4; j++ )
                {
                    cout<<Memory[i+150][j];
                }

                cout<<endl;
            }
            cout << "\n**************END**************\n\n" <<endl;
        }

        else
        {
            int frameNo = allocate();
            
            while (visited[frameNo] != 0)
            {
                frameNo = allocate();
            }
            cout<<"Frame No - "<<frameNo<<endl;
            visited[frameNo] = 1;

            int i = ptr;
            while (Memory[i][0] != '*')
            {
                i++;
            }

            int temp = frameNo / 10;

            Memory[i][0] = ' ';
            Memory[i][1] = ' ';
            Memory[i][2] = temp + 48;
            Memory[i][3] = frameNo % 10 + 48;

            int k = 0;
            for (int i = frameNo * 10; i < frameNo * 10 + 10 && k < Buffer.length(); i++)
            {
                for (int j = 0; j < 4 && k < Buffer.length(); j++)
                {
                    if (Buffer[k] == 'H')
                    {
                        Memory[i][j] = Buffer[k++];
                        break;
                    }

                    else
                    {
                        Memory[i][j] = Buffer[k++];
                    }
                }
            }

            

            Buffer.clear();
        }
    }

}

int main()
{
    OS os;
    os.inFile.open("input2.txt", ios::in);
    os.outFile.open("Output2.txt", ios::out);

    if (!os.inFile)
    {
        cout << "Failure (File not Exist)" << endl;
    }
    else
    {
        cout << "File Exist" << endl;
    }

    os.LOAD();
    return 0;
}