/*A simple (probably) brainfuck interpreter
If does not work, blame Jason */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024
typedef struct {
  int command[100];
  int top;
} stack;

typedef struct {
  int index_of_open;
  int index_of_close;
} dict;

stack command_stack;
#define CREATE() command_stack.top = -1
#define PUSH(c) command_stack.command[++(command_stack.top)] = c
#define POP() command_stack.command[(command_stack.top)--]
int *memcells;
int memptr = 0;
void char_lt() {
  if(memptr <= 0) {
    fprintf(stderr,"Trying to point before starting of memory\n");
    exit(3);
  }
  memptr--;
}

void char_gt() {
  if(memptr >= 30000 - 1) {
    fprintf(stderr,"Trying to point outside memory\n");
    exit(3);
  }
  memptr++;
}

void char_plus() {
  memcells[memptr] = (memcells[memptr]+1) % 256;
}

void char_minus() {
  memcells[memptr] = (memcells[memptr]-1)%256;
}

void char_dot() {
  putchar(memcells[memptr]);
}

void char_comma() {
  char c;
  c = getchar();
  if(c != '\n') memcells[memptr] = c;
}

void loopmap(char *code,int len,dict *map) {
  int index,pos = 0;
      for(int i = 0;i <= len; i++) {
        if(code[i] == '[') {
          PUSH(i);
        }

        if(code[i] == ']') {
          if(command_stack.top < 0){
            fprintf(stderr,"Number of ] more than [");
            exit(1);
          }
            index = POP();
            map[pos].index_of_open = index;
            map[pos].index_of_close = i;
            pos++;
        }
      }
      if(command_stack.top != -1)
      {
        fprintf(stderr,"Number of [ more than ]");
        exit(1);
      }
}
int filter_code(char *content,char *clean)
{
  int i = 0;
  int index = 0;
  for(i = 0;i<=strlen(content);i++)
  {
    if(content[i] == '>'||content[i] == '<'||content[i] == '+' || content[i] == '-' || content[i]=='['||content[i] == ']'
  ||content[i] == '.'||content[i] == ',')
    {
      clean[index] = content[i];
      index++;
    }
  }
  clean[++index] = -1;
  return index;
}

void evaluate(char *cleaned_code,dict *map_loops) {
  int pc = 0;
  stack loop;
  loop.top = -1;
  while(cleaned_code[pc] != -1)
  {
    char i = cleaned_code[pc];
    if(i == '>') char_gt();
    else if(i == '<') char_lt();
    else if(i == '+') char_plus();
    else if(i == '-') char_minus();
    else if(i == '.') char_dot();
    else if(i == ',') char_comma();
    else if(i == '[') {
      if(memcells[memptr] > 0) {
        loop.command[++(loop.top)] = pc;
      }
      else {
        int k = 0;
        while(map_loops[k].index_of_open != pc) k++;
        pc = map_loops[k].index_of_close;
      }
    }
    else if(i == ']') {
      pc = loop.command[(loop.top)--] - 1;
    }
    pc++;
  }
}
void main(int argc,char *argv[]) {


  int i=0;
  int len;
  int bufsize = BUFFER_SIZE;
  char c,*cleaned_code,*buffer;
  dict *map_loops;
  FILE *fp;
  command_stack.top = -1;
  fp = fopen(argv[1],"r");
  if(!fp) return;
  buffer = (char *) calloc(bufsize,sizeof(char));
  if(!buffer) {
    fprintf(stderr,"Can't allocate memory\n");
    exit(2);
  }
  while(!feof(fp))
  {
    c = fgetc(fp);
    buffer[i] = c;

    if(i>=bufsize)
    {
      bufsize += BUFFER_SIZE;
      buffer = realloc(buffer,bufsize);
      if(!buffer) {
        fprintf(stderr,"Can't allocate memory\n");
        exit(2);
      }
    }
    i++;
  }
buffer[--i] = '\0';
cleaned_code = (char *) malloc((strlen(buffer))*sizeof(char));

len = filter_code(buffer,cleaned_code);
free(buffer);
memcells = (int *) calloc(30000,sizeof(char));
if(!memcells) {
  fprintf(stderr,"Can't allocate memory\n");
  exit(2);
}
map_loops = (dict *) malloc(len*sizeof(dict));
if(!map_loops) {
  fprintf(stderr,"Can't allocate memory\n");
  exit(2);
}
loopmap(cleaned_code,len,map_loops);
evaluate(cleaned_code,map_loops);
free(cleaned_code);
free(map_loops);
}
