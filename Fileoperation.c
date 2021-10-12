#include "Fileoperation.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <execinfo.h>
int init_file(SimFile* f)
{
    f->fp = fopen (f->file_name, "r+");
    if(NULL == f->fp)
    {
        printf("Failed to open the file!\n");
        exit (0);
    }
    // printf("init\n");
    f->line_count = 0;
    f->elem_count = 0;
    f->head = (Line*) malloc(sizeof(Line));
    f->head->next = NULL;
    f->head->last = NULL;
    for(int i = 0;i<LINEGROUP;i++)
        f->line_base[i] = NULL;
    fclose(f->fp);
    // printf("init complete\n");
    return 1;
}
SimFile* update_linebase(SimFile* f)
{
    Line* iter = f->head;
    if(f->line_count >= LINEGROUP)
        f->elem_count = f->line_count%LINEGROUP==0? f->line_count/LINEGROUP: f->line_count/LINEGROUP + 1 ;
    // printf("[YGYG]]elem_count %d\n",f->elem_count);
    if(f->elem_count == 1 && f->line_count!=0)
    {
        f->line_base[0] = f->head->next;
    }
    for(int i = 1; i <= f->line_count;i++)
    {
        iter = iter->next;
        if(i%f->elem_count == 1)
            f->line_base[i/f->elem_count] = iter;
    }
    return f;
}
int load_file(SimFile* f)
{
    init_file(f);
    f->fp = fopen (f->file_name, "r+");
    Line * line = f->head;
    Line * test = line;
    memset(line->buff, INVALID, BUFFSIZE);
    while(!feof(f->fp))
    {
        Line * tmp = (Line*) malloc(sizeof(Line));
        memset(tmp->buff, INVALID, BUFFSIZE);
        tmp->next = NULL;
        tmp->down = NULL;
        tmp->last = NULL;
        if(fgets(tmp->buff, BUFFSIZE, f->fp))
        {
            Line *data = tmp;
            Line *data_tmp = (Line*) malloc(sizeof(Line));
            data_tmp->next = NULL;
            data_tmp->down = NULL;
            memset(data_tmp->buff, INVALID, BUFFSIZE);
            // printf("head\n");
            // for(int j = 0;j<BUFFSIZE;j++)
            // {
            //     printf("%c",data->buff[j]);
            // }
            // printf("\n");
            // printf("%d,%d\n",data->buff[BUFFSIZE-2]!=INVALID,data->buff[BUFFSIZE-2]!='\n');
            while(data->buff[BUFFSIZE-2]!=INVALID && data->buff[BUFFSIZE-2]!='\n')
            {
                if(fgets(data_tmp->buff, BUFFSIZE, f->fp))
                {
                    data->down = data_tmp;
                    data = data_tmp;
                    // printf("middle:\n");
                    // for(int j = 0;j<BUFFSIZE;j++)
                    // {
                    //     printf("%c",data_tmp->buff[j]);
                    // }
                    // printf("\n");
                    data_tmp = (Line*) malloc(sizeof(Line));
                    data_tmp->next = NULL;
                    data_tmp->down = NULL;
                    memset(data_tmp->buff, INVALID, BUFFSIZE);
                    //  printf("[YGYG]data%c\n", data_tmp->buff[0]);
                } else {
                    printf("fgets wrong1!");
                    break;
                } 
            }
            
            tmp->last = line;
            line->next = tmp;
            f->line_count = f->line_count + 1 ;
            line = line->next;
            tmp = (Line*) malloc(sizeof(Line));
            memset(tmp->buff, INVALID, BUFFSIZE);
            tmp->next = NULL;
            tmp->down = NULL;
            tmp->last = NULL;
            // printf("[YGYG]line %c\n", line->buff[0]);
        } else {
            printf("fgets complete!\n");
            break;
        }
    }
    f->head->next->last = line;
    f = update_linebase(f);
    fclose(f->fp);
    // printf("[YGYG]%d\n", f->line_count);
    return 1;
}
Line* search(SimFile* f, int start)
{
    Line * iter = f->head;
    Line * dataiter;
    int base, offset;
    base = (start-1) / f->elem_count;
    offset = (start-1) % f->elem_count;
    iter = f->line_base[base];
    printf("[YGYG]base %d offset %d elemcount %d iter %d\n",base, offset, f->elem_count, iter == NULL);
    
    for(int i = 0;i < offset;i++)
    {
        iter = iter->next;
    }
    return iter;
}
int display_file(SimFile* f, int start,  int end)
{
    Line * iter;
    Line * dataiter;
    int count;
    iter = search(f, start);
    count = end - start + 1;
    // printf("[YGYG]display start %d %d\n",count,iter == NULL);
    while(iter != NULL && count > 0)
        {
            dataiter = iter->down;
            int i;
            for(i = 0 ;i < BUFFSIZE && iter->buff[i]!='\n';i++)
            {
                printf("%c", iter->buff[i]);
            }
            if(iter->buff[i] == '\n')
                printf("%c", iter->buff[i]);
            while(dataiter != NULL)
            { 
                for(i = 0 ;i < BUFFSIZE && iter->buff[i]!='\n';i++)
                {
                    printf("%c", dataiter->buff[i]);
                }
                if(iter->buff[i] == '\n')
                    printf("%c", iter->buff[i]);
                dataiter = dataiter->down;
            }
            iter = iter->next;
            count--;
        }
    // printf("display end %d",count);
    return 1;
 }
 //按esc代表输入完毕,边敲边处理
int append_file(SimFile* f)
 {
    Line* tail = f->head->next->last;
    int esc = 27;
    int c,count = 0;
    int new_line = 0;
    Line* tmp = (Line*) malloc(sizeof(Line)) ;
    memset(tmp->buff, INVALID, BUFFSIZE);
    tmp->next = NULL;
    tmp->down = NULL;
    tmp->last = NULL;
    Line* data = (Line*) malloc(sizeof(Line));
    memset(data->buff, INVALID, BUFFSIZE);
    data->next = NULL;
    data->down = NULL;
    data->last = NULL;
    Line* data_tmp = (Line*) malloc(sizeof(Line)) ;
    memset(data_tmp->buff, INVALID, BUFFSIZE);
    tmp->next = NULL;
    tmp->down = NULL;
    tmp->last = NULL;
    // printf("[YGYG] line_before :%d\n", f->line_count);
    while(esc != (c=getchar()))
    {
        if(count == 0)
        {
            tail->next =tmp;
            tmp->last = tail;
            tail = tmp;
            f->head->next->last = tail;
            data = tmp;
            new_line++;
        }
        count++;
        // printf("[YGYG]write char %c ,count %d\n",c,count);
        if(count != 1 && count % BUFFSIZE == 1 && c!='\n')
        {
            data_tmp = (Line*) malloc(sizeof(Line));
            memset(data_tmp->buff, INVALID, BUFFSIZE);
            data_tmp->next = NULL;
            data_tmp->down = NULL;
            data_tmp->last = NULL;
            data->down = data_tmp;
            data = data_tmp;
        }

        data->buff[(count-1)%BUFFSIZE] = c;
        
        if(c=='\n')
        {
            tmp = (Line*) malloc(sizeof(Line)) ;
            memset(tmp->buff, INVALID, BUFFSIZE);
            tmp->next = NULL;
            tmp->down = NULL;
            tmp->last = NULL;
            count = 0;
        }

    }
    f->line_count += new_line;
    f = update_linebase(f);
    printf("[Append mode] line_after :%d\n", f->line_count);
    return 1;
 }
 int insert_file(SimFile* f, int line)
 {
    Line* insert = f->head->next;
    if(line <= 0 || line >= f->line_count)
    {
        printf("The line illegal!!!");
        return 1;
    }
    insert = search(f, line);
    if(insert == NULL)
    {
        printf("Find insert fail!!");
        return 1;
    }
    int c,count = 0;
    Line* tmp = (Line*) malloc(sizeof(Line)) ;
    memset(tmp->buff, INVALID, BUFFSIZE);
    tmp->next = NULL;
    tmp->down = NULL;
    tmp->last = NULL;
    Line* data = (Line*) malloc(sizeof(Line)) ;
    memset(data->buff, INVALID, BUFFSIZE);
    data->next = NULL;
    data->down = NULL;
    data->last = NULL;
    Line* data_tmp = (Line*) malloc(sizeof(Line)) ;
    memset(data_tmp->buff, INVALID, BUFFSIZE);
    tmp->next = NULL;
    tmp->down = NULL;
    tmp->last = NULL;
    while('\n' != (c=getchar()))
    {
        if(count == 0)
        {
            insert->next->last = tmp;
            tmp->next = insert->next;
            insert->next =tmp;
            tmp->last = insert;
            data = tmp;
        }
        count++;
        if(count != 1 && count % BUFFSIZE == 1 && c!='\n')
        {
            data_tmp = (Line*) malloc(sizeof(Line));
            memset(data_tmp->buff, INVALID, BUFFSIZE);
            data_tmp->next = NULL;
            data_tmp->down = NULL;
            data_tmp->last = NULL;
            data->down = data_tmp; 
            data = data_tmp;
        }
        data->buff[count%BUFFSIZE - 1] = c;
    }
    data->buff[count%BUFFSIZE] = '\n';
    f->line_count += 1;
    f = update_linebase(f);
    fclose(f->fp);
    return 1;
 }

int delete_file(SimFile* f, int start, int end)
{
    Line * source;
    Line * dest;
    Line * iter;
    if(start > f->line_count || start <= 0)
    {
        printf("startline illegal\n");
        return 0;
    }
    if(end > f->line_count || end <= 0)
    {
        printf("endline illegal\n");
        return 0;
    }
    source = search(f, start);
    dest = search(f, end);
    source->last->next = dest->next;
    iter = source;
    if(dest->next != NULL){
        dest->next->last = source->last;
        f->head->next->last = source->last;
    }
    f->line_count = f->line_count - end + start - 1;
    while(iter != dest)
    {
        iter = source->down;
        while(iter!=NULL)
        {
            Line * tmp = iter;
            iter = iter->next;
            free(tmp);
        }
        iter = source->next;
        free(source);
        source = iter;
    }
    free(dest);
    return 1;

}
int write_file(SimFile *f, char* save_path)
{
    if(save_path == NULL)
        save_path = f->file_name;
    f->fp = fopen(save_path, "w+");
    if(NULL == f->fp)
    {
        printf("Failed to open the file!\n");
        exit (0);
    }
    Line * iter;
    Line * dataiter;
    iter = f->head->next;
    int i = 0;
    int err;
    while(iter != NULL)
    {
        dataiter = iter->down;
        err = fputs(iter->buff, f->fp);
        while(dataiter != NULL)
        { 
            fputs(dataiter->buff, f->fp);
            dataiter = dataiter->down;
        }
        iter = iter->next;
        i++;
    }
    // printf("[YGYG]have %d line,out %d line\n",f->line_count,i);
    fclose(f->fp);
    return 1;
}