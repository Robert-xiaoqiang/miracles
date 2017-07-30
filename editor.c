#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "acllib.h"

#define MAXCHARS 200
#define WIDTH    600
#define HEIGHT   600
#define TEXTSIZE  20

typedef struct{
	char *work;
	char *top;
} String;

  char chars[MAXCHARS], *base = chars;// 栈低base 
  int isIns = 0, x0 = 0, y0 = 0, end;//isIns 是插入标志，x0, y0 是光标位置 
  String _str = (String){chars, chars};//全局栈 
  String *str = &_str;

  // M
void keyboardEvent(int button, int event);
void charEvent(char ch);

// C
void insert(String *str, int ch);
void jump(String *str, int ch);
void back(String *str);
void dele(String *str);

//V
void initEditor(void);
//每次键盘事件和字符事件回调后paintText，setCaret 以全局变量为参数 
 /*void append(char **top, int char)

{
	**top = char;
	*(*top + 1) = 0;
	*top = *top + 1;
	return;
}*/

void insert(String *str, int ch)
{
    char *i = str -> top + 1, *j = str -> top;
    while(i > str -> work) *i-- = *j--;
    *(str -> work) = ch;
    str -> top += 1;
    str -> work += 1;
    return;
}

void jump(String *str, int ch)
{
	*(str -> work) = ch;
	if(str -> work == str -> top) {
		*(str -> top + 1) = 0;
		str -> top += 1;
	}
	str -> work += 1;
	return;
}

void back(String *str)
{
	strcpy(str -> work - 1, str -> work);
	str -> work -= 1;
	str -> top -= 1;
	return;
}

void dele(String *str)
{
    strcpy(str -> work, str -> work + 1);
    return;
}

void keyboardEvent(int button, int event)
{
	  
	  beginPaint();
	  if(end) {
	    clearDevice();
	    endPaint();
		return;	
	  }
      switch(button){
      	case VK_LEFT :
      	     if(event == KEY_DOWN){
      	     	str -> work -= 1;
      	     	x0 -= TEXTSIZE;
      	     }
      	     break;
      	case VK_RIGHT :
      	     if(event == KEY_DOWN && str -> work < str -> top){
                str -> work += 1;
                x0 += TEXTSIZE;
      	     }
      	     break;
      	case VK_BACK :
      	     if(event == KEY_DOWN && str -> work > base){
                back(str);
                x0 -= TEXTSIZE;
      	     }
      	     break;
      	case VK_DELETE :
      	    if(event == KEY_DOWN && str -> work < str -> top){
      	    	dele(str);
      	    }
      	   
      	    break;
      	case VK_RETURN :
      	     if(event == KEY_DOWN){
                clearDevice();
                hideCaret();
                printf("%s\n", base);
                end = 1;
                endPaint();
                return;
      	     }
      	     break;
      	case VK_INSERT :
      	     if(event == KEY_DOWN){
                isIns ++;
               }
      	     break;
      	default : break;

     }
     
     clearDevice();
     setTextSize(2*TEXTSIZE - 3);
	 paintText(0,0,base);
     setCaretPos(x0, y0);
     if(isIns%2 && str -> work <= str -> top){
        rectangle(x0, y0, x0+TEXTSIZE, y0+TEXTSIZE*2);
     }
     initEditor();
     //printf("#%s#\n", base);
	 endPaint();
     return;
}

void charEvent(char ch)
{
	 //putchar(ch);
	 //printf("x0 = %d, y0 = %d\n",x0, y0);
	 //static int end = 0;
	 beginPaint();
	 if(end){
	 	clearDevice();
	 	endPaint();
	 	return;
	 }
	 clearDevice();
     if(isalnum(ch) || ch == ' '){
     	if(!(isIns%2)){
     		 insert(str, ch);
     		 x0 += TEXTSIZE;
     	} else {
     		 jump(str, ch);
     		 if(str -> work <= str -> top)x0 += TEXTSIZE;
     	}
     }
     if(ch == '\n'){
     	clearDevice();
     	hideCaret();
		end = 1;
		endPaint();
     	return;
     } else {
      setTextSize(2*TEXTSIZE-3);
      setTextColor(BLACK);
      paintText(0, 0, base);
      setCaretPos(x0, y0);
      initEditor();
      if(isIns%2){
	  rectangle(x0, y0, x0+TEXTSIZE, y0+TEXTSIZE*2);
     }
     }
     //printf("#%s#\n", base);
     endPaint();
     return;
}

void initEditor(void)
{
	setTextColor(BLACK);
    setTextSize(TEXTSIZE/2);
	paintText(0,HEIGHT/2 ,"This is a line editor\n Only char , \'Enter\'\
	,\'Insert\', \'Left\', \'Right\'is useful\n");
	return;
}

int Setup(void)
{
    initConsole();
	initWindow("Editor",100, 100, WIDTH, HEIGHT);
	beginPaint();
	registerCharEvent(charEvent);
    registerKeyboardEvent(keyboardEvent);
    
    setCaretSize(1, 2*TEXTSIZE);
    setCaretPos(0,0);
	showCaret();
    setTextColor(RED);
    setTextSize(TEXTSIZE/2);
    initEditor();
    endPaint();
    return 0; 
}
