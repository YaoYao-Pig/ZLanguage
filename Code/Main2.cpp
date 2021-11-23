#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>
#include<queue>
using namespace std;
string path_Open="..\\Result\\Result.txt";
string path_Middle="..\\Result\\TheMiddle.txt";
string path_String="..\\Result\\TheString.txt";
vector<string> theBinary;   //存储二元式
vector<int> theNumber;      //按顺序存储二元式中的class
bool Judege_Sentence(const vector<int>& code,int& step);
class SentenceJudge{
public:
    static bool Judge_assignment_statement(const vector<int>&code,int& step);
    static bool Judge_while_statement(const vector<int>&code,int& step);
    static bool Judge_if_statement(const vector<int>&code,int& step);
    static bool Judge_arithmetic_statement(const vector<int>& code,int& step);
    static bool Judge_rational_statement(const vector<int>& code,int& step);
    
    static bool isArithmeticAbout(const int i){
        if(isOperator_Arithmetic(i)||i==47||i==43){
            return true;
        }
        return false;
    }
    static bool isOperator_Arithmetic(const int i){
        if(i==0||i==1||i==2||i==3||i==15||i==16){
            return true;
        }
        return false;
    }
};
bool IsRationalOperator(int i){
    if(i==9||i==13||i==14||i==20||i==21){
        return true;
    }
    return false;
}
bool Judege_SentenceTable(const vector<int>& code,int& step){
    //语句表
    int tag=0;
    while(Judege_Sentence(code,step)){
        if(code[step]==42){
            tag++;
            break;
        }
        tag++;
    }
    if(tag==0)  return false;
    if(step>=code.size()){
        return false;
    }
    return true;
}
bool Judege_Compound_Segement(const vector<int>& code,int& step){
    if(code[step]!=33){    //begin
        return false;
    }
    else   
        step++;
    if(!Judege_SentenceTable(code,step)) return false; 
    if(code[step]!=42) {    //end
        return false; 
    }
    //
    if(code[step]==42&&code[step-1]==8) return false;
    step++;
    return true;
}
bool SentenceJudge::Judge_assignment_statement(const vector<int>&code,int& step){
    step++;
    if(code[step]!=4){  //:=
        return false;
    }
    else{
        step++;
    }
    if(!Judge_arithmetic_statement(code,step)){
        return false;
    }
    return true;
}
int isLogicOperator(int i){
    if(i==40) return 1;
    else if(i==32||i==39){
        return 2;
    }
    return 0;
}
bool SentenceJudge::Judge_while_statement(const vector<int>&code,int& step){
   if(code[step]!=37){        //while
        return false;
    }
    step++;
    while(1){
        if(!SentenceJudge::Judge_rational_statement(code,step)){
            return false;
        }
        if(isLogicOperator(code[step])==2){
            step++;
        }
        else{
            break;
        }
    }
    
    if(code[step]!=38)  return false;//do
    if(code[step-1]!=47||code[step-1]!=43)
        step++;

    if(!Judege_Sentence(code,step)){
        return false;
    }
    return true;
}

bool Judge_Rational_Segement(const vector<int>& code,int &step){
    if(!SentenceJudge::Judge_rational_statement(code,step)){
        return false;
    }
    if(isLogicOperator(code[step])==2){
        step++;
        if(!Judge_Rational_Segement(code,step)){
            return false;
        }
    }
    return true;
}
bool SentenceJudge::Judge_if_statement(const vector<int>&code,int& step){
    if(code[step]!=34){        //if
        return false;
    }
    step++;
    while(1){
        if(!SentenceJudge::Judge_rational_statement(code,step)){
            return false;
        }
        if(isLogicOperator(code[step])==2){
            step++;
        }
        else{
            break;
        }
    }
    
    if(code[step]!=35)  return false;//
    if(code[step-1]!=47||code[step-1]!=43)
        step++;
    if(!Judege_Sentence(code,step)){
        return false;
    }
    if(code[step]==36){
        step++;
        if(!Judege_Sentence(code,step)){
            return false;
        }
    }
    return true;
}
bool SentenceJudge::Judge_rational_statement(const vector<int>&code,int& step){
    if(code[step]==40&&code[step+1]==47||code[step]==40&&code[step+1]==43||
    code[step]==40&&code[step+1]==15){
        step++;
    }
    else if(code[step]==40){
        return false;
    }
    if(!SentenceJudge::Judge_arithmetic_statement(code,step)){
        return false;
    }
    if(code[step]==35 or code[step]==38){
        return true;
    }
    if(isLogicOperator(code[step])==2){
        return true;//有and 和 or
    }
    if(!IsRationalOperator(code[step])){
        return false;
    }
    step++;
    if(!SentenceJudge::Judge_arithmetic_statement(code,step)){
        return false;
    }    
    return true;
}
bool SentenceJudge::Judge_arithmetic_statement(const vector<int>& code,int& step){
    //0.先遍历一边，看三件事：
    //0.1 (的右，必须是47和43
    //0.2 )的左，必须是47和43
    //0.3 二元运算符的两侧必须是43和47
    
    //1.先判断()是否正确
    int i=step;
    stack<int> judgetmp;
    queue<int> other;
    while(isArithmeticAbout(code[i])){
        if(code[i]==15) judgetmp.push(15);
        else if(code[i]==16){
            if(judgetmp.empty())    return false;
            else if(judgetmp.top()==15){
                judgetmp.pop();
            }
        }
        else{
            other.push(code[i]);
        }
        i++;
    }
    if(!judgetmp.empty())   return false;
    for(int j=step;j<i;++j){
        if(code[j]==15){
            if(code[j+1]!=47&&code[j+1]!=43&&code[j+1]!=15)  
                return false;
            if(code[j-1]==47||code[j-1]==43){
                return false;
            }
        }
        else if(code[j]==16){
            if(code[j-1]!=47&&code[j-1]!=43&&code[j-1]!=16){
                return false;
            }
            if(code[j+1]==47||code[j+1]==43){
                return false;
            }
        }
    }
    //2.再将所其他的内容压入队列，判断其他内容对错
    if(other.empty()){
        return false;
    }
    while(!other.empty()){
        int tmp=other.front();
        other.pop();
        if(tmp==47||tmp==43){//如果是数字或者是标识符
            if(other.empty())   continue;
            else{
                int tmp_2=other.front();
                other.pop();
                if(isOperator_Arithmetic(tmp_2)){
                    if(other.empty())return false;
                    if(other.front()==47||other.front()==43)
                        continue;
                    else{
                        return false;
                    }
                } 
                else return false;
            }
        }
        else{
            return false;
        }
    }
    step=i;//step指向第一个不符合的位置
    return true;
}
bool Open(fstream& _file){
    _file.open(path_Open,ios::in|ios::out);
    if(!_file.is_open()) return false;
    return true; 
}  
bool InPutBinary(fstream& _file){
    //读入二元式
    while(!_file.eof()){
        string tmp;
        _file>>tmp;
        theBinary.push_back(tmp);
    }
    return true;
} 
bool Judege_Sentence(const vector<int>& code,int& step){
    if(code[step]==47){ //赋值语句
        if(!SentenceJudge::Judge_assignment_statement(code,step)){
            return false;
        }
        else{
            if(code[step]==42){
                if(code[step-1]==43)    return true;
                else{
                    step--;
                    return true;
                }
            }
            if(code[step]==8&&code[step+1]!=42) {
                step++;
                return true; 
            }    //;
            else 
            return false;
        }
    }
    else if(code[step]==37){//While语句 
        if(!SentenceJudge::Judge_while_statement(code,step)){
            return false;
        }
        return true;
    }
    else if(code[step]==34){//if语句
        if(!SentenceJudge::Judge_if_statement(code,step)){
            return false;
        }
        return true;
    }
    else if(code[step]==33){    //复合语句 begin
        if(!Judege_Compound_Segement(code,step)){
            return false;
        }
        return true;
    }
    return false;
}


bool Judege_VariableTable(const vector<int>& code,int& step){
    //变量表:a,  b,  c,
    if(code[step]==47){
        step++;
        return true;
    }
    return false;
}
bool Judege_Type(const vector<int>& code,int& step){
    //类型
    if(code[step]==28||
    code[step]==30||code[step]==31){
        step++;
        return true;
    }
    return false;
}
bool Judege_Variable_Description(const vector<int>& code,int& step){
    //变量说明表
    int times=0;
    while(1){
        if(Judege_VariableTable(code,step)){
            if(code[step]==6){
                step++;
                times++;
            }   //6==,
            else if(code[step]==7){    //7==:
                times++;
                step++;
                break;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    if(times==0)    return false;
    if(code[step-1]!=7) return false;
    if(!Judege_Type(code,step))    return false;
    if(code[step]!=8)   return false;
    step++;
    return true;
}
bool Judege_Variable(const vector<int>& code,int& step){
    //变量说明
    if(code[step]!=27){    //27==var
        return false;
    }else step++;
    int times=0;
    while(Judege_Variable_Description(code,step)){
        times++;
    }
    //cout<<code[step];
    //cout<<code[step-1];
    if(times==0)    return false;
    if(code[step]==33){//是begin
        if(code[step-1]!=8)
            return false;
        return true;
    }
    return false;
}
bool Judege_Segement(const vector<int>& code,int& step){
    //分程序
    if(!Judege_Variable(code,step)) return false;
    //cout<<"Hi!---step1"<<endl;
    if(!Judege_Compound_Segement(code,step)){
            
        return false;
    }
    if(code[step]!=5)   return false;
    return true;
}
bool Judege_Main(const vector<int>& code,int& step){
    //主处理程序
    //程序开始，读入：
    //1.program
    if(code[step]!=26){
        
        return false;
    }else{
        step++;
    } 
    //2.<标识符>
    if(code[step]!=47){
        
        return false;
    }else{
        step++;
    }  
    //cout<<code[step];
    //3.;
    if(code[step]!=8){
        
        return false;
    }else{
        step++;
    }   
    //4.分程序
    //cout<<"1";
    if(Judege_Segement(code,step))
        return true;
    return false;
}
bool OutPut_TheBinary(vector<string>& vec){
    int len=vec.size();
    if(len==0) return false;
    for(int i=0;i<len;++i){
        cout<<vec[i]<<endl;
    }
    return true;
}
bool OutPut_TheNumber(const vector<int>& vec){
    int len=vec.size();
    if(len==0) return false;
    for(int i=0;i<len;++i){
        cout<<i<<" "<<vec[i]<<endl;
    }
    return true;
}
//belong to TheNumber()
bool isNumber(char c){

    if(c>='0'&&c<='9') return true;
    return false;
}
int TheNumber(string s){
    //分离数字
    int l=s.size();
    int num=0;
    for(int i=1;i<l;++i){
        if(isNumber(s[i])){
            num=num*10+(s[i]-'0');
        }
        else{
            return num;
        }  
    }
    return num;
}
void GetBinaryNumber(vector<int>& vec,const vector<string>& store){
    //得到二元式的数字(class)
    int len=store.size();
    int comment_start=0;
    int comment_end=0;
    for(int i=0;i<len-1;++i){
        int tmp=TheNumber(store[i]);
        if(tmp==10){
            comment_start=1;
        }
        else if(tmp==11){
            comment_start=0;
        }
        if(comment_start==0&&tmp!=11&&tmp!=12)    vec.push_back(tmp);
    }
}
bool OutPutTheResult(fstream& _file){
    _file.open(path_Middle,ios::in|ios::out|ios::trunc);
    if(!_file.is_open()) return false;
    return true; 
}
int main(void){
    fstream file;
    fstream outFile;
    fstream stringFile;
    if(Open(file)){
        InPutBinary(file);
        //OutPut_TheBinary(theBinary);
        GetBinaryNumber(theNumber,theBinary);
        OutPut_TheNumber(theNumber);
        int step=0;
        cout<<"-------------"<<endl;
        bool j=Judege_Main(theNumber,step);
        cout<<j<<endl;
        if(j){  
            stringFile.open(path_String,ios::out|ios::in|ios::trunc);
            if(OutPutTheResult(outFile)){
                int len=theNumber.size();
                for(int i=0;i<len;++i){
                    if(theNumber[i]!=15&&theNumber[i]!=16){
                        string tmp=theBinary[i];
                        stringFile<<tmp<<endl;
                        outFile<<theNumber[i]<<endl;
                    }
                        
                }
                outFile.close();
            }
            
            // int len=theBinary.size();
            // for(int l=0;l<len;++l){
            // }
        }
    }
    else{
        cout<<"Open Error!"<<endl;
    }
    //cout<<"HelloWorld"<<endl;
    //cout<<"HelloWorld1"<<endl;
    //cout<<"HelloWorld2"<<endl;
    //cout<<"HelloWorld3"<<endl;
    //cout<<"HelloWorld4"<<endl;
    //cout<<"HelloWorld5"<<endl;
    return 0;
}
