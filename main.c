#include "Fileoperation.h"
#include "CmdQueue.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
int command_run(CmdQueue *cmd, SimFile* f)
{
    char c;
    c = queue_out(cmd);
    if(c == 'a')
    {
        printf("[Append mode]Please input the text you want to append:\n");
        append_file(f);
    }
    else if(c == 'l')
    {
        int start = 0;
        int end = 0;
        c = queue_out(cmd);
        if(c == '\n')
        {
            start = 1;
            end = f->line_count;
        }
        else if(c == ' ')
        {
            c = queue_out(cmd);
            while(c != ' ' &&  c != '\n')
            {
                if(c < '0' || c > '9')
                {
                    printf("Command Style Failed!!");
                    return 0;
                }
                start = start * 10 + (c - '0');
                c = queue_out(cmd);
            }
            if(c != '\n')
            {
                c = queue_out(cmd);
                while(c != ' ' &&  c != '\n')
                {
                    if(c < '0' || c > '9')
                    {
                        printf("Command Style Failed!!\n\n");
                        return 0;
                    }
                    end = end * 10 + (c - '0');
                    c = queue_out(cmd);
                }
            }
            else 
            {
                end = f->line_count;
            }
        }
        else
        {
            printf("Command Style Failed!!\n\n");
            return 0;
        }
        printf("start:%d,end:%d\n\n", start ,end);
        display_file(f, start, end);
    }
    else if(c == 'i')
    {
        int line = 0;
        c = queue_out(cmd);
        if(c == '\n')
        {
            line = f->line_count;
            printf("the insert line will be at the end of file\n");

        }
        else if(c == ' ')
        {
            c = queue_out(cmd);
            while(c != ' ' &&  c != '\n')
            {
                if(c < '0' || c > '9')
                {
                    printf("Command Style Failed!ß!");
                    return 0;
                }
                line = line * 10 + (c - '0');
                c = queue_out(cmd);
            }
        }
        else
        {
            printf("1111Command Style Failed!!");
            return 0;
        }

        printf("[Insert mode]Please input the text you want to insert after line %d:\n", line);
        insert_file(f, line);
    }
    else if(c == 'd')
    {
        int start = 0;
        int end = 0;
        c = queue_out(cmd);
        if(c == '\n')
        {
            start = 1;
            end = f->line_count;
        }
        else if(c == ' ')
        {
            c = queue_out(cmd);
            while(c != ' ' &&  c != '\n')
            {
                if(c < '0' || c > '9')
                {
                    printf("Command Style Failed!!");
                    return 0;
                }
                start = start * 10 + (c - '0');
                c = queue_out(cmd);
            }
            if(c == '\n')
                end = f->line_count;
            
            c = queue_out(cmd);
            while(c != ' ' &&  c != '\n')
            {
                if(c < '0' || c > '9')
                {
                    printf("Command Style Failed!!\n\n");
                    return 0;
                }
                end = end * 10 + (c - '0');
                c = queue_out(cmd);
            }
        }
        else
        {
            printf("Command Style Failed!!\n\n");
            return 0;
        }
        printf("start:%d,end:%d\n\n", start ,end);
        delete_file(f, start, end);
    }
    else if(c == 'r')
    {
        char* new_filename = (char*)malloc(sizeof(char) * LENCMDMAX);
        int i = 0;
        c = queue_out(cmd);
        if(c != ' ' && c != '\n')
        {
            printf("Command Style Failed!!\n");
            free(new_filename);
            return 1;
        }
        else if(c == '\n')
        {
            printf("No new file!\n");
            free(new_filename);
            return 1;
        }
        else 
        {
            c = queue_out(cmd);
            if(c == '\n')
            {
                printf("No new file!\n");
                free(new_filename);
                return 1;
            }
            while(c != ' ' && c != '\n')
            {
                new_filename[i] = c;
                i++;
                c = queue_out(cmd);
            }
        }
        f->file_name = new_filename;
        load_file(f);

    }
    else if(c == 'w')
    {
        char* new_filename = (char*)malloc(sizeof(char) * LENCMDMAX);
        int i = 0;
        c = queue_out(cmd);
        if(c != ' ' && c != '\n')
        {
            printf("Command Style Failed!!\n");
            free(new_filename);
            return 1;
        }
        else if(c == '\n')
        {
            new_filename = f->file_name;
            return 1;
        }
        else 
        {
            c = queue_out(cmd);
            if(c == '\n')
            {
                new_filename = f->file_name;
                return 1;
            }
            while(c != ' ' && c != '\n')
            {
                new_filename[i] = c;
                i++;
                c = queue_out(cmd);
            }
        }
        write_file(f, new_filename);
        free(new_filename);
    } 
    else 
    {
        return -1;
    }

    return 1;
}

int main(int argc, char* argv[])
{
    SimFile file;
    CmdQueue cmd;
    char c;
    int flag = -1;
    queue_init(&cmd);
    file.file_name =argv[1];
    load_file(&file);
    argc+=1;
    printf("Please enter the command: \n");
    while(1)
    {
        c = getchar();
        queue_in(&cmd, c);
        if(c == '\n')
        {
            flag = command_run(&cmd, &file);
            if(flag == 1 || flag == 0)
                printf("\nPlease enter the command: \n");
            queue_empty(&cmd);
        }
        if(c == 27)
        {
            break;
        }
    }
    printf("Good Bye!\n");
}