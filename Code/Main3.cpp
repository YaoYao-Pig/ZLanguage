#include<iostream>
#include<stack>
#include<queue>
#include<vector>
#include<string>
#include<fstream>
using namespace std;
string FilePath="..\\Result\\TheMiddle.txt";
string StringPath="..\\Result\\TheString.txt";
fstream File_theMidlle;
fstream File_String;
int currentPointer=0;
vector<int> theNumber;
vector<string> theString;
int test=0;
struct Quadruple{
    string op;
    string arg1;
    string arg2;
    string resualt;
    Quadruple(string o,string a1,string a2,string r):op(o),arg1(a1),arg2(a2),resualt(r){}
};
// struct Quadruple_J{
//     string op;
//     string arg1;
//     string arg2;
//     int resualt;
//     Quadruple_J()=default;
//     Quadruple_J(string p,string a1,string a2,int r):op(p),arg1(a1),arg2(a2),resualt(r){}
// };
//-------Function
bool OpenFile(fstream& _file,string _filePath);
int InversionStringtoInt(string s);
void InputNumber(vector<int>& vec,fstream& _file);
void InputString(vector<string>& vec,fstream& _file);
void Fun_Seg(vector<int>& vec,stack<string>& stack);
int Fun_SlicedSegement(vector<int>& vec,stack<string>& stack);
void Fun_SegTable(vector<int>& vec,stack<string>& stack);
string IntToString(int i);
vector<Quadruple> store;
bool IsRationalOperator(int i){
    if(i==13||i==14||i==9)  return true;
    return false;
}
bool IsOperator_J(int i){
    if(i==13||i==14||i==20||i==21||i==9){
        return true;
    }
    return false;
}
bool IsAboutOperation(int i){
    if(i==32||i==39)    return true;
    return false;
}
int step=0;
int NewTmp(){
    return step;
}
void AddTmp(){
    step++;
}
string GetTmp(){
    string t;
    if(NewTmp()<=9)
        t='0'+NewTmp();
    else{
        int num=NewTmp();
        t=IntToString(num);
    }
    string tmp_s="tmp";
    tmp_s+=t;
    return tmp_s;
}

void QuaOut(string op,string arg1,string arg2,string tmp){
    cout<<"("<<op<<","<<arg1<<","<<arg2<<","<<tmp<<")"<<endl;
    //test++;
}
void QuaOut(Quadruple q){
    cout<<"("<<q.op<<","<<q.arg1<<","<<q.arg2<<","<<q.resualt<<")"<<endl;
    //test++;
}
void QuaOut(string op,string arg1,string arg2,int tmp){
    cout<<"("<<op<<","<<arg1<<","<<arg2<<","<<tmp<<")"<<endl;
}
bool IsOperator(int num){
    if(num==0||num==1||num==2||num==3)  return true;
    return false;
}
string IntToString(int i){
    string a;
    do{
        a=(char)(i%10+'0')+a;
        i/=10;
    }while(i!=0);
    return a;
}
vector<Quadruple> andStore;
vector<int> andStepSotre;
int andTag=0;
int andStep=0;
vector<Quadruple> orStore;
vector<int> orStepSotre;
vector<int> andVec;
int orTag=0;
int orStep=0;
void Fun_Con(vector<int>& vec,stack<string>& stack){//条件判断
    while (1){
        
        if(vec[currentPointer]==38||vec[currentPointer]==35){
            string num=IntToString(test+2);
            step--;
            if(store[test-1].op[0]=='J'){
                AddTmp();
            }
            else{
                store.push_back(Quadruple("Jnz",GetTmp(),"_",num));
                AddTmp();
                test++;
                
            }
            
            //QuaOut("Jnz",GetTmp(),"_",num);
            break;
        }

        if(IsOperator_J(vec[currentPointer])){
            string j_kind="";
            switch(vec[currentPointer])
            {
            case 9:
                j_kind="J=";
                break;
            case 13:
                j_kind="J>";
                break;
            case 14:
                j_kind="J<";
                break;
            case 20:
                j_kind="J>=";
                break;
            case 21:
                j_kind="J<=";
                break;
            default:
                break;
            }
            string tmp1=stack.top();
            currentPointer++;
            Fun_SlicedSegement(vec,stack);
            Quadruple q(j_kind,tmp1,stack.top(),IntToString(test+2));
            store.push_back(q);
            test++;
            if(andTag==1){
                andTag=0;
                andStore[andStep++].resualt="0";
                andStepSotre.push_back(test);
                store.push_back(andStore[0]);
                test++;
            }
            if(orTag==1){
                orTag=0;
                orStore[orStep++].resualt="0";
                orStepSotre.push_back(test);
                store.push_back(orStore[0]);
                test++;
            }
        }
        else if(andTag){
            andTag=0;
            andStore[andStep++].resualt="0";
            andVec.push_back(test);
            store.push_back(andStore[0]);
            test++;
            store.push_back(Quadruple("J","_","_","0"));
            andStepSotre.push_back(test);
            test++;
        
        }
        else if(orTag){
            orTag=0;
            orStore[orStep++].resualt="0";
            orStepSotre.push_back(test);
            store.push_back(orStore[0]);
            test++;
        }
        else{
            Fun_SlicedSegement(vec,stack);
        }
    }
}
void Fun_Begin(vector<int>& vec,stack<string>& stack){
    currentPointer++;//begin的下一个
    Fun_SegTable(vec,stack);
    //正好指向end
}
void Fun_SegTable(vector<int>& vec,stack<string>& stack){
    while(1){
        if(theNumber[currentPointer]==42)   break;
        Fun_Seg(vec,stack);
    }
    
}

void Fun_If(vector<int>& vec,stack<string>& stack){
    currentPointer++;//if的下一个
    Fun_Con(vec,stack);//这时应该指在then上
    Quadruple qual("J","_","_",IntToString(0));
    //test--;
    int tmp=test;
    store.push_back(qual);
    test++;
    //test--;
    currentPointer++;
    if(theNumber[currentPointer]!=33){
        Fun_Seg(vec,stack);
    }
    else{
        Fun_SegTable(vec,stack);
    }
    store[tmp].resualt=IntToString(test);
    for(int i=0;i<andStepSotre.size();++i){
        if(andStepSotre[i]>=tmp)  break;
        //cout<<"11:"<<store[andStepSotre[i]].resualt<<endl;
        store[andStepSotre[i]].resualt=IntToString(tmp);
    }
    for(int i=0;i<orStepSotre.size();++i){
        if(orStepSotre[i]>=tmp)  break;
        store[orStepSotre[i]].resualt=IntToString(tmp+1);
    }
    for(int i=0;i<andVec.size();++i){
        if(andVec[i]>tmp) break;
        store[andVec[i]].resualt=IntToString(andVec[i]+2);
    }
    //处理if里面的内容
    //Fun_Seg(vec,stack);//这里应该指在else上W
    currentPointer++;
    if(theNumber[currentPointer]==36){
        currentPointer++;
    }
    int t_tmp=test;
    store.push_back(Quadruple("J","_","_","0"));
    test++;
    if(theNumber[currentPointer]!=33){
        Fun_Seg(vec,stack);
    }
    else{
        Fun_SegTable(vec,stack);
    }
    store[t_tmp].resualt=IntToString(test);
    //回填else的位置
    //处理else里面的内容
    //Fun_Seg(vec,stack);
}
void Fun_While(vector<int>& vec,stack<string>& stack){
    currentPointer++;//while的下一个
    int t=test;
    Fun_Con(vec,stack);//这时应该指在do上
    Quadruple qual("J","_","_",IntToString(0));
    int tmp=test;
    store.push_back(qual);
    test++;
    currentPointer++;
    if(theNumber[currentPointer]!=33){
        Fun_Seg(vec,stack);
    }
    else{
        
        Fun_SegTable(vec,stack);
    }
    
    //Fun_Seg(vec,stack);//结束后指在while语句结束后的一个上
    store.push_back(Quadruple("J","_","_",IntToString(t)));
    test++;
    for(int i=0;i<andStepSotre.size();++i){
        if(andStepSotre[i]>=tmp)  break;
        store[andStepSotre[i]].resualt=IntToString(tmp);
    }
    for(int i=0;i<orStepSotre.size();++i){
        if(orStepSotre[i]>=tmp)  break;
        store[orStepSotre[i]].resualt=IntToString(tmp+1);
    }
    for(int i=0;i<andVec.size();++i){
        if(andVec[i]>tmp) break;
        store[andVec[i]].resualt=IntToString(andVec[i]+2);
    }
    store[tmp].resualt=IntToString(test);
    //currentPointer++;
    //回填while的结束位置
    
}
int Fun_SlicedSegement(vector<int>& vec,stack<string>& stack){
    //a+b
    int tmp1=currentPointer++;
    if(vec[currentPointer]==8||vec[currentPointer]==42
    ||IsRationalOperator(vec[currentPointer])||
    vec[currentPointer]==35||vec[currentPointer]==38){
        string tmp_s=theString[tmp1];
        stack.push(tmp_s);
        return tmp1;
    }
    if(IsAboutOperation(vec[currentPointer])){
        string tmp_s=theString[tmp1];
        stack.push(tmp_s);        
        if(vec[currentPointer]==32){
            currentPointer++;
            Quadruple q("Jand","_","_","0");
            andStore.push_back(q);
            andTag=1;
        }
        else{
            currentPointer++;
            Quadruple q("Jor","_","_","0");
            orStore.push_back(q);
            orTag=1;
        }
        return tmp1;
    }
    int opr=currentPointer++;
    int tmp2=Fun_SlicedSegement(vec,stack);
    theString[tmp2]=stack.top();
    stack.push(GetTmp());
    AddTmp();
    test++;
    store.push_back(Quadruple(theString[opr],theString[tmp1],theString[tmp2],stack.top()));
    return NewTmp();
}
void Fun_Id(vector<int>& vec,stack<string>& stack){
    //a:=a+b
    int tmp1=currentPointer++;//a
    int tmp2=currentPointer++;//:= 
    Fun_SlicedSegement(vec,stack);
    string t=stack.top();
    test++;
    store.push_back(Quadruple(theString[tmp2],theString[tmp1],stack.top(),"_"));
}
void Fun_Seg(vector<int>& vec,stack<string>& stack){
    if(vec[currentPointer]==33){//begin
        Fun_Begin(vec,stack);
    }
    else if(vec[currentPointer]==34){//if
        Fun_If(vec,stack);
    }
    else if(vec[currentPointer]==37){//while
        Fun_While(vec,stack);
    }
    else if(vec[currentPointer]==47){//ID
        Fun_Id(vec,stack);
        if(vec[currentPointer]==8){
            currentPointer++;
        }
    }
}
int main(void){
    if(OpenFile(File_theMidlle,FilePath)&&OpenFile(File_String,StringPath)){
        InputNumber(theNumber,File_theMidlle);
        InputString(theString,File_String);
        //找到最开始的位置:第一个begin
        for(int i=0;i<theNumber.size();++i){
            if(theNumber[i]==33){
                currentPointer=i;
                break;
            }
        }
        stack<string> s;
        Fun_Seg(theNumber,s);
        //Fun_Seg(theNumber,s);
        for(int i=0;i<store.size();++i){
            cout<<i<<" ";
            QuaOut(store[i]);
        }
    }
    else{
        cout<<"open is err!";
    }
    return 0;
}
bool OpenFile(fstream& _file,string _filePath){
    _file.open(_filePath,ios::out|ios::in);
    if(_file.is_open()){
        return true;
    }
    return false;
}
int InversionStringtoInt(string s){
    int j=0;
    int len=s.size();
    for(int i=0;i<len;++i){
        j=j*10+(s[i]-'0');
    }
    return j;
}
void InputNumber(vector<int>& vec,fstream& _file){
    while(!_file.eof()){
        string s="";
        _file>>s;
        vec.push_back(InversionStringtoInt(s));
    }
    vec.erase(vec.end()-1);
}
void InputString(vector<string>& vec,fstream& _file){
    while(!_file.eof()){
        string s="";
        _file>>s;
        int tag=0;
        string tmp="";
        int len=s.size();
        for(int i=1;i<len-1;++i){
            if(tag==0&&s[i]==','){
                tag=1;
                continue;
            }
            if(tag==1){
                tmp+=s[i];
            }
            
        }
        //cout<<tmp<<endl;
        vec.push_back(tmp);
    }
}