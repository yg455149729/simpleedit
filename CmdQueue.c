#include "CmdQueue.h"
int queue_init(CmdQueue* cmd)
{
    cmd->front = 0;
    cmd->rear = 0;
    for(int i = 0;i < LENCMDMAX;i++){
        cmd->cmdline[i] = INVALID;
    }
    return 1;
}
int is_fullQueue(CmdQueue *cmd)
{
    if((cmd->rear +1)% LENCMDMAX == cmd->front)
    {
        return 1;
    }
    else
        return 0;
}
int queue_in(CmdQueue* cmd, char input)
{
    if(is_fullQueue(cmd) == 1)
        return OVERFLOW;
    cmd->cmdline[cmd->rear] = input;
    cmd->rear = (cmd->rear + 1)%LENCMDMAX ;   
    return 0;
}
int isemptyQueue(CmdQueue *cmd)
{
    if(cmd->front == cmd->rear)
    {
        return 1;
    }
    else
        return 0;
}
char queue_out(CmdQueue* cmd)
{
    char temp = INVALID;
    if(isemptyQueue(cmd) == 1)
        return temp;
    temp = cmd->cmdline[cmd->front];
    cmd->cmdline[cmd->front] = INVALID;
    cmd->front = (cmd->front + 1)%LENCMDMAX ;
    return temp;
}

int queue_empty(CmdQueue* cmd)
{
    while(!isemptyQueue(cmd))
        queue_out(cmd);
    return 1;
}
