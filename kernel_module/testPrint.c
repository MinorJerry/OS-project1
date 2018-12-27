#include<stdio.h>
int is_string_inline(char* line,const char* mystring)
{
    int ret = 0;
    for(int i=0;line[i]!='\0';i++)
    {
        if(line[i]==(*mystring))
        {
            ret = 1;
            //check
            for(int j=0;*(mystring+j)!='\0';j++)
            {
                if(line[i+j]!=*(mystring+j))
                {
                    ret = 0;
                    break;
                }
            }
            if(ret == 1)break;
        }
    }
    return ret;
}
void printf_info(char *line)
{
    char print_line[1024];
    int i=0,j=0;
    //in order to delete some words
    int flag = 0;
    for(i=0;line[i]!='\0';i++)
    {
        //the m will represent at i = 16
        if(flag==0&&line[i]=='m'&&i<20)flag=1;
        if(flag==0)print_line[j++]=line[i];
        if(flag==1&&line[i]==']')flag=0;
    }
    print_line[j] = '\0';
    printf("%s",print_line);
}
int main(void)
{
    FILE* fp;
    char info[1024];
    const char* mystring = "MicroJerry@";
    if((fp=fopen("/var/log/kern.log","r"))==NULL)return -1;
    while(1)
    {
        if(fgets(info,1024,fp)==NULL)break;
        //judge if the line includes the string :"MicroJerry@"
        if(is_string_inline(info,mystring)) 
        {

            printf_info(info);
        }
    }
    return 0;
    
}