#ifndef FILEOPS
#define FILEOPS
#include "CmdQueue.h"
#define FAULTLINE -1
#define BUFFSIZE 2048
// #define BUFFSIZE 5
#define FILESIZE 1024*1024*100
#define LINEGROUP 100
// #define LINEGROUP 3

typedef struct Link{
    char buff[BUFFSIZE];//代表数据域
    struct Link * last;
    struct Link * next;//代表指针域，指向直接后继元素
    struct Link * down;//代表指针域，指向直接后继元素
}Line;

typedef struct 
{
    FILE *fp;
    Line* line_base[LINEGROUP]; //用以快速定位的line指针数组
    int line_count;//文件共有多少行
    int elem_count;//一组内的元素
    char* file_name;//文件路径
    Line *head;//头指针
    
}SimFile;

int init_file(SimFile* f); //初始化文件
int load_file(SimFile* fp);//加载文件
Line* search(SimFile* f, int start);//找到start行对应的行指针
SimFile* update_linebase(SimFile* f);//更新line_base数组
int display_file(SimFile* fp, int start, int end);//显示文件
int append_file(SimFile* fp);//在文件末尾新增行
int insert_file(SimFile* fp, int line);//在文件指定行新增行
int delete_file(SimFile* f, int start, int end);//删除行
int write_file(SimFile *f, char* save_path);//写回或者写入新文件

#endif