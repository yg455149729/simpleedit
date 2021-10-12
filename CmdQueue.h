#ifndef SIMEDIT
#define SIMEDIT
#include "stdio.h"

#define LENCMDMAX 256
#define OVERFLOW -1
#define INVALID '\0'
typedef struct 
{
    char cmdline[LENCMDMAX];
    int front;
    int rear;

} CmdQueue;
int queue_init(CmdQueue* cmd);//初始化队列
int is_fullQueue(CmdQueue *cmd);//判断队满
int queue_in(CmdQueue* cmd, char input);//入队
int isemptyQueue(CmdQueue *Cmd);//判断队空
char queue_out(CmdQueue* cmd);//出队
int queue_empty(CmdQueue* cmd);//为用户下一次输入清空缓冲区
#endif