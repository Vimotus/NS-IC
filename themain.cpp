//#include "stdafx.h"
#include<iostream>
#include<set>
#include<map>
#include<ctime>
#include<string.h>
#include<random>
#include<vector>
#include<algorithm>
#include<math.h>
#include<fstream>
using namespace std;
int Cir = 100;
float rate = 0.5;
const int MAX=10000;
long long Y_Y, Y_N, N_Y, N_N,F1,MSE;
long long D[21] ;
long long D_N[21];
double P[21]= {0.066,0.074,0.111,0.307,0.069,0.091,0.067,0.106,0.381,0.165,0.038,0.186,0.399,0.063,0.619,0.444,0.070,0.420,0.662,0.485}; //Pc
long long sum_D[21];
struct node
{
    int user;
    set<int>p;
    map<int, long long> num;
};

struct temNode
{
    int label;
    double psv;
};

bool cmp(const temNode &a, const temNode &b)
{
    if (a.psv != b.psv)
        return a.psv>b.psv;
    else return a.label>b.label;

}

bool cmp1(const temNode &a, const temNode &b)
{
    if (a.psv != b.psv)
        return a.psv<b.psv;
    else return a.label<b.label;

}

set<int>Event_testSet;
set<int>Event_dataSet;
map<int, double>Pvs[1800000];
map<int,int>S_num;
map<int,int>testS_num;
vector<node>_eventPattern[MAX];
vector<node>N_eventPattern[MAX];
double PL[21];

void inital()
{
    for (int i = 0; i<20; i++)
    {
        
        PL[i]=D[i]*1.0/(D[i]+D_N[i]);
        cout<<PL[i]<<endl;


        P[i]=0.4;
        ///  P[i]=D[i]*1.0/(D[i] + D_[i]);//D[i]*1.0/(D[i] + D_[i]*0.11);

        cout<<i<<" : "<<P[i]<<endl;
    }
    for (int i = 0; i<20; i++)
    {
       
        sum_D[i] =D[i]+D_N[i]*0.05;//D[i]*2;
        ///sum_D[i] =D[i]+D[i]*2;

      
    }

}
int num;

void readEPF_Active(char * eventPatternFile_Active,bool fag)
{
    
    cout<<"---------------read active node---------------"<<endl;
    int K, S, V, N, P;
    int M;
    fstream     f(eventPatternFile_Active);
    string line;
    int a,b,c;
    char l[190];
    while(getline(f,line))
    {
        int n;
        const  char *p;
        p=line.c_str();
        sscanf(p,"%d %d",&S,&n);
        if(n!=0)
        {
            
            if(fag){
                S_num[S]=num;
                Event_dataSet.insert(S);
            }

            else
            {
                testS_num[S]=num;
                Event_testSet.insert(S);
            }
            S=num++;
            _eventPattern[S].clear();
        }
        
        while(n--)
        {
            getline(f,line);
            const  char *p;
            p=line.c_str();
            sscanf(p,"%d   %[ 0-9]",&V,l);
            node node1;
            node1.user = V;
            while(strcmp(l,"")!=0)
            {
                char q[190]="";
                sscanf(l,"%d %d %[ 0-9]",&P,&M,q);
                strcpy(l,q);
                node1.p.insert(P);
                node1.num[P] = M;
                if(fag)D[P]+=M;
            }
            _eventPattern[S].push_back(node1);
        }
    }
    f.close();


    cout<<"read over"<<endl;
}

void readEPF_N_active(char *eventPatternFile_N_Active,bool fag)
{
    
    cout<<"read not active node"<<endl;
    int K, S, V, N, P;
    int M;
    fstream     f(eventPatternFile_N_Active);
    string line;
    char l[190];
    while(getline(f,line))
    {
        int n;
        const  char *p;
        p=line.c_str();
        sscanf(p,"%d %d",&S,&n);
        if(n!=0)
        {
            if(fag)
            {
                if(S_num[S]!=0)
                    S=S_num[S];
                else
                {
                    S_num[S]=num;
                    S=num++;
                    Event_dataSet.insert(S);
                }
            }
            else
            {
                if(testS_num[S]!=0)
                {
                    S=testS_num[S];
                }
                else
                {
                    testS_num[S]=num;
                    S=num++;
                    Event_testSet.insert(S);
                }

            }
            N_eventPattern[S].clear();
        }

        while(n--)
        {
            getline(f,line);
            const  char *p;
            p=line.c_str();
            sscanf(p,"%d   %[ 0-9]",&V,l);
            node node1;
            node1.user = V;
            while(strcmp(l,"")!=0)
            {
                char q[190]="";
                sscanf(l,"%d %d %[ 0-9]",&P,&M,q);
                strcpy(l,q);
                node1.p.insert(P);
                node1.num[P] = M;
                D_N[P]+=M;
            }
            
            int a=rand()%10000;
            if(a<=100)
                N_eventPattern[S].push_back(node1);
           
        }
    }
    f.close();
    cout<<"read over"<<endl;
}

//计算Pvs
int K;
double LOGP=0;
void getPVS()
{
    set<int>::iterator iit, i;
    int it;
    K=0;
    for (i = Event_dataSet.begin(); i != Event_dataSet.end(); i++)
    {
        for (it = 0; it <_eventPattern[*i].size(); it++)
        {
            node  node1 = _eventPattern[*i][it];
            double Value = 1.0;
            for (iit = node1.p.begin(); iit != node1.p.end(); iit++)
            {
                int L = node1.num[*iit];
                Value *=pow((1 - P[*iit]),L) ;
            }
            Pvs[node1.user][*i] =1- Value;
        }

    }
}
///更新pc值
void getPc()
{
    set<int>::iterator iit, i;
    int it;
    double sum[21];
    memset(sum, 0, sizeof(sum));
    for (i = Event_dataSet.begin(); i != Event_dataSet.end(); i++)
    {
        for (it = 0; it < _eventPattern[*i].size(); it++)
        {
            {
                node  node1 = _eventPattern[*i][it];
                for (iit = node1.p.begin(); iit != node1.p.end(); iit++)
                {
                    if(Pvs[node1.user][*i]!=0)
                        sum[*iit] += (((P[*iit]*1.0)/ Pvs[node1.user][*i])*node1.num[*iit]);
                }
            }
        }
    }
    
    for (int j = 0; j < 20; j++)
    {
        P[j] = (sum[j]*1.0 )/ sum_D[j];
        cout<<"p"<<j<<":"<<P[j]<<endl;
    }
}

double get_rate(bool f,char *result)
{
    ofstream fout(result);
    Y_Y = 0, Y_N = 0, N_Y = 0, N_N = 0;
    double Y_Y100 = 0, Y_N100 = 0, N_Y100 = 0, N_N100 = 0;
    set<int>::iterator iit, i, q;
    vector<temNode> sample;
    int it;
    double MSE=0;
    double MAP=0;
    int k = 0;
    for (i = Event_testSet.begin(); i != Event_testSet.end(); i++)
    {
        //cout<<*i<<endl;
        for (it = 0; it <_eventPattern[*i].size(); it++)
        {

            node  node1 = _eventPattern[*i][it];
            double Value = 1.0;
            for (iit = node1.p.begin(); iit != node1.p.end(); iit++)
            {
                int L = node1.num[*iit];
                if(f)
                    Value *=pow((1 - P[*iit]),L) ;
                else
                    Value *=pow((1 - PL[*iit]),L) ;
            }

            Value = 1 - Value;//cout<<Value<<endl;
            temNode N;
            N.psv = Value;
            N.label = 1;
            sample.push_back(N);
            if (Value>rate)
                Y_Y++;
            else
                Y_N++;
            MSE=MSE+(1-Value)*(1-Value);
            fout<<Value<<"  "<<1<<endl;
        }
    }
    for (i = Event_testSet.begin(); i != Event_testSet.end(); i++)
    {

        for (it = 0; it <N_eventPattern[*i].size(); it++)
        {
            node  node1 = N_eventPattern[*i][it];
            double Value = 1.0;
            for (iit = node1.p.begin(); iit != node1.p.end(); iit++)
            {
                int L = node1.num[*iit];
                if(f)
                    Value *=pow((1 - P[*iit]),L) ;
                else
                    Value *=pow((1 - PL[*iit]),L) ;

            }

            Value = 1 - Value;//
            temNode N;
            N.psv = Value;
            N.label = 0;
            sample.push_back(N);
            if (Value>rate)
                N_Y++;
            else
                N_N++;
            MSE=MSE+Value*Value;
            fout<<Value<<"  "<<0<<endl;
        }

    }

    cout<<endl<<"实际--预测：Y_Y"<<"  "<<"Y_N"<<" "<<"N_N"<<"   "<<"N_Y"<<endl;
    cout<<"            "<<Y_Y<<"   "<<Y_N<<" "<<N_N<<"   "<<N_Y<<endl;
    sort(sample.begin(), sample.end(), cmp);
    int flag = 0, su = 0,last = -1, L = 1, M = 0, N = 0, RA = 0;
    int j=0;
    for (int sa =0; sa <sample.size(); sa++)
    {
        if(sample[sa].psv>rate&&sample[sa].label==1)
            Y_Y100++;
        else
            Y_N100++;
        if (sample[sa].psv>rate&&sample[sa].label==0)
            N_Y100++;
        else
            N_N100++;
    }

    for (int sa =0; sa <100; sa++,j++)
    {
        if(sample[sa].label==1)
        {
            MAP+=(j*1.0/(sa+1));
        }
    }
    MAP=MAP/100;
    sort(sample.begin(), sample.end(), cmp1);

    int n_pos=Y_Y+Y_N;
    int n_neg=sample.size()-n_pos;
    long long total=n_pos*n_neg;
    long long accumulated_neg=0;
    long long satisfied=0;
    for(int sa=0; sa<sample.size(); sa++)
    {
        if(sample[sa].label==1)
            satisfied+=accumulated_neg;
        else
            accumulated_neg+=1;

    }
    double AUC=satisfied*1.0/total;
    double P = Y_Y*1.0 / (Y_Y + N_Y), R = Y_Y*1.0 / (Y_Y + Y_N);
    double F1=2.0*(P*R)/(P+R);
    MSE=MSE*1.0/(Y_N + N_N + N_Y + Y_Y);
    cout<<"AUC:  "<<AUC<<endl;
    cout<<"MSE: "<<MSE<<endl;
    cout<<"F1："<<F1<<endl;
    cout << "召回率：" << R << endl;
    cout << "查准率：" << P << endl;;
    cout<<"取前100个被激活概率的大的点："<<endl;
    double P100 = Y_Y100*1.0 / (Y_Y100 + N_Y100),
    R100 = Y_Y100*1.0 / (Y_Y100 + Y_N100);
    double F1100=2.0*(P100*R100)/(P100+R100);
    cout<<"100MAP:"<<MAP<<endl;
    cout<<"100F1："<<F1100<<endl;
    cout << "100召回率：" << R100 << endl;
    cout << "100查准率：" << P100 << endl;
    fout.close();
    return 0;
}

double LOG=-999999999;

double logP()
{
    set<int>::iterator iit, i;
    int it;
    double pvs_tmp ;
    LOGP=0;
    for (i = Event_dataSet.begin(); i != Event_dataSet.end(); i++)
    {

        for (it = 0; it <N_eventPattern[*i].size(); it++)
        {
            node  node1 = N_eventPattern[*i][it];
            double Value = 1.0;
            for (iit = node1.p.begin(); iit != node1.p.end(); iit++)
            {
                int L = node1.num[*iit];
                Value *=pow((1 - P[*iit]),L) ;
            }
            pvs_tmp =1- Value;
            if(pvs_tmp!=1)
                LOGP+=log(1-pvs_tmp);
        }
        for (it = 0; it <_eventPattern[*i].size(); it++)
        {
            node  node1 = _eventPattern[*i][it];
            double Value = 1.0;
            for (iit = node1.p.begin(); iit != node1.p.end(); iit++)
            {
                int L = node1.num[*iit];
                Value *=pow((1 - P[*iit]),L) ;
            }
            pvs_tmp =1- Value;
            if(pvs_tmp!=0)
                LOGP+=log(pvs_tmp);
        }

    }
    return LOGP;

}
void EM()
{
    double pk[20];
    int t = 0;
    int num;
    inital();
    for(int i=0; i<20; i++)
    {
        pk[i]=P[i];
    }
    while (t<Cir)
    {
        cout<<"第"<<t<<"圈"<<endl;
        getPVS();
        logP();
        cout<<LOGP<<"    "<<LOG<<"  "<<LOGP-LOG<<endl;
        if(LOGP-LOG<0.1)
        {
            for(int i=0; i<20; i++)
            {
                P[i]=pk[i];
            }
            return;
        }
        LOG=LOGP;
        for(int i=0; i<20; i++)
        {
            pk[i]=P[i];
        }
        getPc();
        t++;
    }
    
    for(int i=0; i<20; i++)
    {
        cout<<i<<"    "<<P[i]<<endl;
    }
}
void test()
{
    cout<<"into test function"<<endl;
    cout<<"the new pc"<<endl;
    
   // get_rate(true,"E:/my_result.txt");
    get_rate(true,"E:/论文调参指标/pc随机-0.4/my_result.txt");
    cout<<endl<<endl;
    cout<<"pc=x/(x+y)"<<endl;
    //get_rate(false,"E:/qing_result.txt");
    get_rate(false,"E:/论文调参指标/pc随机-0.4/qing_result.txt");
    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
int main()
{

    char* eventPatternFile = "E:/论文/800trainx.txt";///事件被激活节点及其周边的活跃节点构成的模式和模式实例数的文件
    char * N_eventPatternFile = "E:/论文/800trainy.txt";///事件不被激活节点及其周边的活跃节点构成的模式和模式实例数的文件
    memset(D,0,sizeof(D));
    memset(D_N,0,sizeof(D_N));
    num=1;
    readEPF_Active(eventPatternFile,true);
    readEPF_N_active(N_eventPatternFile,true);///读取训练集不活跃节点及一定不激活它的模式
    for(int i=0; i<20; i++)
    {
        cout<<"x:"<<D[i]<<"  y:"<<D_N[i]<<endl;
    }
    EM();
    num=1;
    readEPF_Active("E:/论文/200testx.txt",false);///读取测试集活跃节点及可能激活它的模式
    readEPF_N_active("E:/论文/200testy.txt",false);
    test();
    int a;
    cin>>a;
    return 0;
}

