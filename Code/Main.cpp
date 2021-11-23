#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<iostream>
#include<string>
#include<map>
#include<algorithm>
using namespace std;
const char* _filePath="..\\Text\\Store.txt";
const char* _fileWordPath="..\\Text\\Code.txt";
const char* _outPut_ReservedWord="..\\Result\\Result_Reservedword.txt";
const char* _outPut_Word="..\\Result\\Result_Word.txt";
const char* _outPut_Number_Integer="..\\Result\\Result_Number_Integer.txt";
const char* _outPut_Number_Decimal="..\\Result\\Result_Number_ Decimal.txt";
const char* _outPut_Operator="..\\Result\\Result_Operator.txt";
const char* _outPut="..\\Result\\Result.txt";

/*
    0-25是符号
    26-42是关键字
    43 整数
    44 字符
    45 bool
    46 实常数
    47 标识符
*/
#define OK 1
#define ERRO 0

enum Length{
    MAXLEN=80,
    WORDSNUM=17,
    OPERATORNUM=26,
    INTEGERNUMBER=43,
    CHARNUMBER=44,
    BOOLNUMBER=45,
    ALLNUMBER=46,
    BIAOSHINUM=47,
};
typedef struct 
{
    char ch[MAXLEN];
    int num=OPERATORNUM;
}Point;

//FILE Pointer
Point* point;
FILE* _out_r;
FILE* _out_w;
FILE* _out_n_i;
FILE* _out_n_d;
FILE* _out_o;
FILE* _out;

//map
map<string,int> mp_rw;//reserved word
map<string,int> mp_w;//word
map<string,int> mp_o;//operator
map<string,int> mp_int;
map<string,int> mp_double;

//numbers
int num_Ope=0;
int num_Int=0;
int num_Double=0;
int num_Word=0;
int num_RWord=0;

//status
int isComment_line=0;
int isComment_block=0;

bool isNotComment(){
    return !isComment_line&&!isComment_block;
}
int getFileMainWord(FILE* _file,Point* point){
    if(_file!=NULL&&point!=NULL&&WORDSNUM!=0){
        for(int i=0;i<WORDSNUM;++i){
            fscanf(_file,"%s",point[i].ch);
            point[i].num=i+OPERATORNUM;
        }
        return OK;
    }
    else{
        return ERRO;
    }
}
int isEmptyOperator(char ch){
    if(ch==' '||ch=='\n'||ch=='\t'){
        return OK;
    }
    return ERRO;
}
int isLetter(char ch){
    if(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z'){
        return OK;
    }
    return ERRO;
}
int isNumber(char ch){
    if((int)ch>='0'&&ch<='9'){
        return OK;
    }
    else{
        return ERRO;
    }
}
int isOperator(char ch){
    if(ch=='+'||ch=='-'||ch=='*'||
        ch=='-'||ch=='='||ch=='.'||ch==';'||ch==':'||
        ch==','||ch=='/'||ch=='>'||
        ch=='<'||ch=='('||ch==')'||
        ch=='&'||ch=='|'||ch=='!'||
        ch=='\"'||ch=='\''||ch=='.'){
        return OK;
    }
    else{
        return ERRO;
    }
}
int isBuitInOperator(char* string){
    for(int i=0;i<WORDSNUM;++i){
        if(strcmp(point[i].ch,string)==0){
            return OK;
        }
    }
    return ERRO;
}
Point* operator_hash;
void initialization_operator_hash(){
    operator_hash=(Point*)malloc(OPERATORNUM*sizeof(Point));
    strcpy(operator_hash[0].ch,"加号");
    strcpy(operator_hash[1].ch,"减号");
    strcpy(operator_hash[2].ch,"乘号");
    strcpy(operator_hash[3].ch,"除号");
    strcpy(operator_hash[4].ch,"赋值号");
    strcpy(operator_hash[5].ch,"界符");
    strcpy(operator_hash[6].ch,"逗号");
    strcpy(operator_hash[7].ch,"冒号");
    strcpy(operator_hash[8].ch,"分号");
    strcpy(operator_hash[9].ch,"等号");
    strcpy(operator_hash[10].ch,"左注释");
    strcpy(operator_hash[11].ch,"右注释");
    strcpy(operator_hash[12].ch,"行注释");
    strcpy(operator_hash[13].ch,"大于号");
    strcpy(operator_hash[14].ch,"小于号");
    strcpy(operator_hash[15].ch,"左括号");
    strcpy(operator_hash[16].ch,"右括号");
    strcpy(operator_hash[17].ch,"与");
    strcpy(operator_hash[18].ch,"或");
    strcpy(operator_hash[19].ch,"非");
    strcpy(operator_hash[20].ch,"大于等于");
    strcpy(operator_hash[21].ch,"小于等于");
    strcpy(operator_hash[22].ch,"左双引号");
    strcpy(operator_hash[23].ch,"右双引号");
    strcpy(operator_hash[24].ch,"左单引号");
    strcpy(operator_hash[25].ch,"右双引号");
}
string OperatorStore[OPERATORNUM]={
    "+","-","*","/",":=",".",",",":",";","=","/*",
    "*/","//",">","<","(",")","&",
    "|","!",">=","<=","\"","\'"
};
int theKindofOperator(char* op){
    if(0==strcmp(op,"+"))   return 0;   
    else if(0==strcmp(op,"-"))  return 1;
    else if(0==strcmp(op,"*"))  return 2;
    else if(0==strcmp(op,"/"))  return 3;
    else if(0==strcmp(op,":=")) return 4;
    else if(0==strcmp(op,"."))  return 5;
    else if(0==strcmp(op,","))  return 6;
    else if(0==strcmp(op,":"))  return 7;
    else if(0==strcmp(op,";"))  return 8;
    else if(0==strcmp(op,"="))  return 9;
    else if(0==strcmp(op,"/*")) return 10;
    else if(0==strcmp(op,"*/")) return 11;
    else if(0==strcmp(op,"//")) return 12;
    else if(0==strcmp(op,">"))  return 13;
    else if(0==strcmp(op,"<"))  return 14;
    else if(0==strcmp(op,"("))  return 15;
    else if(0==strcmp(op,")"))  return 16;
    else if(0==strcmp(op,"&")) return 17;
    else if(0==strcmp(op,"|")) return 18;
    else if(0==strcmp(op,"!"))  return 19;
    else if(0==strcmp(op,">="))  return 20;
    else if(0==strcmp(op,"<="))  return 21;
    else if(0==strcmp(op,"\""))  return 22;
    else if(0==strcmp(op,"\'"))  return 23;
    return -1;
}
string toString(char ch[],int len){
    string tmp;
    for(int i=0;i<len;++i){
        tmp+=ch[i];
    }
    return tmp;
}
int GetNumber_BuiltIn(string s){
    int len=s.size();
    int tag=0;
    for(int i=0;i<WORDSNUM;++i){
        tag=0;
        for(int j=0;j<len;++j){
            if(s[j]!=point[i].ch[j]){
                tag=1;
                break;
            }
        }
        if(tag==0){
            return point[i].num;
        }
    }
    return -1;
}
void getCh(FILE* _file){
    while(!feof(_file)){
        char ch;
        ch=getc(_file);
        char astring[MAXLEN];
        int _stringPos=0;
        if((int)ch==-1){
            break;
        }
        if(isEmptyOperator(ch)){
            if(ch=='\n'){
                isComment_line=0;
            }
            continue;
        }
        else if(isNotComment()&&isLetter(ch)&&isComment_line==0){
            _stringPos=0;
            char tmpString[MAXLEN]="";
            while(isLetter(ch)||isNumber(ch)&&!feof(_file)){
                tmpString[_stringPos++]=ch;
                ch=getc(_file);
            }
            strcpy(astring,tmpString);
            if(!feof(_file)){
                //because the function isLeterr==false break
                if(isEmptyOperator(ch)){
                    ;
                }
                else
                    fseek(_file,-1*sizeof(char),SEEK_CUR);
                    //the file pointer back 1 step
            }
            if(isNotComment()&&isBuitInOperator(astring)){
                string s=toString(tmpString,_stringPos);
                auto it=mp_rw.find(s);
                if(it==mp_rw.end()){
                    mp_rw.insert(pair<string,int>(s,num_RWord));
                    //fprintf(_out,"( %s , %s )\n","保留字",tmpString);
                    num_RWord++;
                    
                }
                else{
                    //fprintf(_out,"( %s , %s )\n","保留字",tmpString);
                }
                int _builtInNumber=GetNumber_BuiltIn(s);
                fprintf(_out,"(%d,%s)\n",_builtInNumber,tmpString);
                //output reseved word
                
            }
            else if(isNotComment()){
                //output the new word
                string s=toString(tmpString,_stringPos);
                auto it=mp_w.find(s);
                if(it==mp_w.end()){
                    mp_w.insert(pair<string,int>(s,num_Word));
                    //fprintf(_out,"( %s , %s )\n","标识符",tmpString);
                    num_Word++;
                    
                }
                else{
                    //fprintf(_out,"( %s , %s )\n","标识符",tmpString);
                }
                fprintf(_out,"(%d,%s)\n",BIAOSHINUM,tmpString);
            }
            continue;
        }
        else if(isOperator(ch)&&isComment_line==0){
            char tmpOperator=fgetc(_file);
            char _operator[3]="";
            _operator[0]=ch;
            if(isOperator(tmpOperator)){
                _operator[1]=tmpOperator;
            }
            else if(!feof(_file)){
                fseek(_file,-1*sizeof(ch),SEEK_CUR);
            }

            if(isComment_block==0&&0==strcmp("/*",_operator)){
                isComment_block=1;
            }
            else if(isComment_block==1&&0==strcmp("*/",_operator)){
                isComment_block=0;
            }
            if(isComment_line==0&&0==strcmp("//",_operator)){
                isComment_line=1;
            }
            int kind=theKindofOperator(_operator);
            if(kind==-1){
                //printf("New Operator: %s\n",_operator);
                char tmpS[2]="";
                tmpS[0]=_operator[0];
                int tmp=theKindofOperator(tmpS);
                fprintf(_out,"(%d,%s)\n",tmp,tmpS);
                fseek(_file,-1*sizeof(ch),SEEK_CUR);
                continue;
                //if(tmp!=-1){
                   // 
                //}
            }
            else if(isNotComment()||0==strcmp("/*",_operator)||0==strcmp("//",_operator)){
                string s=toString(_operator,2);
                auto it=mp_o.find(s);
                if(it==mp_o.end()){
                    mp_o.insert(pair<string,int>(s,num_Ope));
                    //fprintf(_out,"( %s , %s )\n",operator_hash[kind].ch,_operator);
                    num_Ope++;
                }
                else{
                    //fprintf(_out,"( %s , %s )\n",operator_hash[kind].ch,_operator);
                }
                fprintf(_out,"(%d,%s)\n",kind,_operator);
            }
            else{
                //printf("The index err");
            }
            
        }
        else if(isNotComment()&&isNumber(ch)){
            char _number[MAXLEN]="";
            int num=0;
            int tag_hasdot=0;
            while(isNumber(ch)||ch=='.'&&!feof(_file)){
                if(ch=='.') tag_hasdot=1;
                _number[num++]+=ch;
                ch=fgetc(_file);
            }
            if(!feof(_file)){
                fseek(_file,-1*sizeof(ch),SEEK_CUR);
            }
            if(tag_hasdot){
                string s=toString(_number,num);
                auto it=mp_double.find(s);
                if(it==mp_double.end()){
                    mp_double.insert(pair<string,int>(s,num_Double));
                    //fprintf(_out,"( 小数 , %s )\n",_number);num_Double++;
                }
                else{
                    //fprintf(_out,"( 小数 , %s )\n",_number);
                }
                fprintf(_out,"(%d,%s)\n",ALLNUMBER,_number);
                
            }
            else{
                string s=toString(_number,num);
                auto it=mp_int.find(s);
                if(it==mp_int.end()){
                    mp_int.insert(pair<string,int>(s,num_Int));
                    //fprintf(_out,"( 整数 , %s )\n",_number);num_Int++;
                    
                }
                else{
                    //fprintf(_out,"( 整数 , %s )\n",_number);
                }
                fprintf(_out,"(%d,%s)\n",INTEGERNUMBER,_number);
                
            }
        }
        else{
           //printf("New Operator: %c\n",ch);
        }
    }

}
void PutOutTheEXCEl(){
    //Operator
    for(int i=0;i<OPERATORNUM;++i){
        cout<<OperatorStore[i]<<"  "<<i<<endl;
    }
    //Words
    for(int i=0;i<WORDSNUM;++i){
        printf("%s",point[i].ch);cout<<"   "<<i+OPERATORNUM<<endl;
    }
    //Numbers
    cout<<"Integer"<<"   "<<INTEGERNUMBER<<endl;
    cout<<"Char   "<<"   "<<CHARNUMBER<<endl;
    cout<<"BOOL   "<<"   "<<BOOLNUMBER<<endl;
    cout<<"ALL    "<<"   "<<ALLNUMBER<<endl;
    cout<<"ID     "<<"   "<<47<<endl;
}
void InitFilePointer(){
    _out_r=fopen(_outPut_ReservedWord,"w");
    _out_w=fopen(_outPut_Word,"w");
    _out_n_i=fopen(_outPut_Number_Integer,"w");
    _out_n_d=fopen(_outPut_Number_Decimal,"w");
    _out_o=fopen(_outPut_Operator,"w");
    _out=fopen(_outPut,"w");
}
void closeFilePointer(FILE* _file){
    fclose(_out_r);
    fclose(_out_w);
    fclose(_out_n_i);
    fclose(_out_n_d);
    fclose(_out_o);
    fclose(_file);
    fclose(_out);

}
int main(void){
    FILE* _file =fopen(_filePath,"a+");
    InitFilePointer();
    initialization_operator_hash();
    fseek(_file,0,SEEK_SET);//定位_file的位置到文件开头
    if(_file==NULL){
        printf("Hi");
    }
    else{
        point=(Point*)malloc(sizeof(Point)*WORDSNUM);
        getFileMainWord(_file,point);
        PutOutTheEXCEl();
        FILE* _fileWord=fopen(_fileWordPath,"a+");
        getCh(_fileWord);
        
        closeFilePointer(_file);
    }
}