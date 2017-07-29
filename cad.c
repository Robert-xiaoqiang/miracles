#include <stdio.h>
#include <stdlib.h>
#include "acllib.h"

#define WIDTH        600
#define HEIGHT       600
#define MAXGRAPH      50
#define TEXTSIZE      10
#define ERASER_RADIUS 10

typedef struct{
     int x;
     int y;
} Point;

typedef struct {
	Point leftUp;
	Point rightDown;
} Rect;

typedef struct {
	Point initial;
	Point final;
} Line;

typedef struct {
    Point left;
    Point right;
    Point down;
} _Ellipse;

typedef struct _node{
    int flag;//0 R L E ERASER
    union{
		Rect r;
		Line l;
		_Ellipse e;//eraser
	}graph;
	ACL_Color Lcolor;
	ACL_Color Bcolor;
	int isEmpty;//0(NULL)1	
} Gitem;

typedef struct {
	 Gitem *base;
	 Gitem *top;
} Stack;

enum {
   NOGRAPH,
   R,
   L,
   E,
   ERASER
};

// stack
//void initStack(Stack **ps);
void pop(Stack *s);
void push(Stack *s, Gitem *g);
void free(Stack *s);

//CAD 
// CAD M
void mouseEvent(int x, int y, int button, int event);

// CAD C
void rectFlag(Stack *s, Gitem *current, int x, int y, int button, int event);
void lineFlag(Stack *s, Gitem *current, int x ,int y, int button, int event);
void ellipseFlag(Stack *s, Gitem *current, int x, int y, int button, int event);
void eraserFlag(Stack *s, Gitem *current, int x, int y, int button, int event);

// CAD V
void drawStack(Stack *s);
void initCAD(Gitem *current);

void pop(Stack *s)
{
  if(!s || !s->top) return;
  if(s -> top > s -> base) --s->top;
  return;
}

void push(Stack *s, Gitem *g)
{
	printf("push\n");
	if(!s || !s->top) return;
	*(s->top)++ = *g;
	return;
}

void free(Stack *s)
{
	if(!s) return;
	free(s -> base);
	free(s);
	return;
}

void initCAD(Gitem *current)
{
	//printf("initCAD  %d\n", type);
	int flag;
	ACL_Color color;
	setPenColor(RED);
	setBrushColor(EMPTY);
	line(WIDTH/4, 0, WIDTH/4, HEIGHT);
	line(0, HEIGHT/3, WIDTH/4, HEIGHT/3);
	line(0,HEIGHT*2/3, WIDTH/4, HEIGHT*2/3);
	line(WIDTH*9/10-WIDTH/40, 0, WIDTH*9/10-WIDTH/40, HEIGHT);
	rectangle(WIDTH/32, HEIGHT/24, WIDTH*7/32, HEIGHT*7/24);
	line(WIDTH/32, HEIGHT*3/8, WIDTH*7/32, HEIGHT*5/8);
    ellipse(WIDTH/32, HEIGHT*3/8 + HEIGHT/3, WIDTH*7/32, HEIGHT*5/8 + HEIGHT/3);
    setBrushColor(RED);
    rectangle(9*WIDTH/10, 0, 19*WIDTH/20, HEIGHT/20);
    rectangle(9*WIDTH/10, HEIGHT/5, 19*WIDTH/20, HEIGHT/4);
    setBrushColor(BLUE);
    setPenColor(BLUE);
    rectangle(19*WIDTH/20, 0, WIDTH, HEIGHT/20);
    rectangle(19*WIDTH/20, HEIGHT/5, WIDTH, HEIGHT/4);
    setBrushColor(GREEN);
    setPenColor(GREEN);
    rectangle(9*WIDTH/10, HEIGHT/20, 19*WIDTH/20, HEIGHT/10);
    rectangle(9*WIDTH/10, HEIGHT/4, 19*WIDTH/20, 3*HEIGHT/10);
    setTextColor(BLACK);
    setTextSize(TEXTSIZE);
    setBrushColor(BLACK);
    setPenColor(BLACK);
    paintText(19*WIDTH/20, HEIGHT/20, "line");
    rectangle(19*WIDTH/20, HEIGHT/4, WIDTH, 3*HEIGHT/10);
    paintText(9*WIDTH/10, 3*HEIGHT/10, "brush");
    paintText(9*WIDTH/10, HEIGHT/2, "eraser");
    setBrushColor(EMPTY);
    rectangle(9*WIDTH/10, HEIGHT/2, WIDTH, 11*HEIGHT/20);
	if((flag = current -> flag) == R){
        rectangle(0, 0, WIDTH/4, HEIGHT/3);
        rectangle(WIDTH/32, HEIGHT/24, WIDTH*7/32, HEIGHT*7/24);
    } else if(flag == L){
    	rectangle(0, HEIGHT/3, WIDTH/4, HEIGHT*2/3);
        line(WIDTH/32, HEIGHT*3/8, WIDTH*7/32, HEIGHT*5/8);
    } else if(flag == E){
    	rectangle(0, HEIGHT*2/3, WIDTH/4, HEIGHT);
        ellipse(WIDTH/32, HEIGHT*3/8 + HEIGHT/3, WIDTH*7/32, HEIGHT*5/8 + HEIGHT/3);
    } else if(flag == ERASER){
    	setPenColor(MAGENTA);
    	rectangle(9*WIDTH/10, HEIGHT/2, WIDTH, 11*HEIGHT/20);
    	paintText(9*WIDTH/10, HEIGHT/2, "eraser");
    }
    setPenColor(WHITE);
    if((color = current -> Lcolor) == RED) {
    	rectangle(9*WIDTH/10, 0, 19*WIDTH/20, HEIGHT/20);
    }else if(color == BLUE){
    	rectangle(19*WIDTH/20, 0, WIDTH, HEIGHT/20);
    } else if(color == GREEN){
    	rectangle(9*WIDTH/10, HEIGHT/20, 19*WIDTH/20, HEIGHT/10);
    }
    if(current -> isEmpty){
    	 if((color = current -> Bcolor) == RED){
    	    rectangle(9*WIDTH/10, HEIGHT/5, 19*WIDTH/20, HEIGHT/4);
        } else if(color == BLUE){
    	    rectangle(19*WIDTH/20, HEIGHT/5, WIDTH, HEIGHT/4);
        } else if(color == GREEN){
    	    rectangle(9*WIDTH/10, HEIGHT/4, 19*WIDTH/20, 3*HEIGHT/10);
        } else if(color == BLACK){
    	    rectangle(19*WIDTH/20, HEIGHT/4, WIDTH, 3*HEIGHT/10);
        }
    }
    return;
} 

void drawStack(Stack *s)
{
    printf("drawStack\n");
	Gitem *work;
    int n, x1, y1, x2, y2, x3, y3;
    if(!s || !(s -> top)) return;
	setPenColor(MAGENTA);
	setPenWidth(3);
	setBrushColor(EMPTY);
	for(work = s -> base; work < s -> top; work ++){
		if(work -> Lcolor != EMPTY){
			setPenColor(work->Lcolor);
		}
		if(work -> isEmpty){
			setBrushColor(work->Bcolor);
		}
		if((n = work -> flag) == R){
			rectangle(work->graph.r.leftUp.x, work->graph.r.leftUp.y,\
			work->graph.r.rightDown.x, work->graph.r.rightDown.y);
		} else if(n == L){
			line(work->graph.l.initial.x, work->graph.l.initial.y, \
				work->graph.l.final.x,work->graph.l.final.y);
		} else if(n == E || n == ERASER){
			x1 = work->graph.e.left.x;
			y1 = work->graph.e.left.y;
			x2 = work->graph.e.right.x;
			y2 = work->graph.e.right.y;
			x3 = work->graph.e.down.x;
			y3 = work->graph.e.down.y;
			ellipse(x1, y3-2*(y3-y1), x2, y3);
		}
	}
	return;
}

void rectFlag(Stack *s, Gitem *current, int x, int y, int button, int event)
{
	printf("rectFlag\n");
	if(event == BUTTON_DOWN){
			if(button == LEFT_BUTTON){
				if(x <= WIDTH/4 && y <= HEIGHT/3) {
					(*current).flag = 0;
				}else if(x <= WIDTH/4 && y <= HEIGHT*2/3) {
				    (*current).flag = L;
				}else if(x <= WIDTH/4 && y <= HEIGHT){ 
                    (*current).flag = E;
				}else if(x >= 9*WIDTH/10 && y >= HEIGHT/2 && y <= 11*HEIGHT/20){
					(*current).flag = ERASER; 
				} else {
					if(!((*current).graph.r.leftUp.x) && !((*current).graph.r.leftUp.y)){
						(*current).graph.r.leftUp.x = x;
						(*current).graph.r.leftUp.y = y;
					} else if(!((*current).graph.r.rightDown.x) && !((*current).graph.r.rightDown.y)){
						(*current).graph.r.rightDown.x = x;
						(*current).graph.r.rightDown.y = y;
						push(s, current);
						current -> Lcolor = current -> Bcolor = EMPTY;
					    current -> isEmpty = 0;
						(*current).graph.r =(Rect){(Point){0,0},(Point){0,0}};
					}
				}	
			} else if(button == RIGHT_BUTTON){
				pop(s);
			}
	}else if(event == MOUSEMOVE && (*current).graph.r.leftUp.x && (*current).graph.r.leftUp.y){
           
			rectangle((*current).graph.r.leftUp.x, (*current).graph.r.leftUp.y, x, y);
	} 
    
	return;
}

void lineFlag(Stack *s, Gitem *current, int x ,int y, int button, int event)
{
	
	if(event == BUTTON_DOWN){
            if(button == LEFT_BUTTON){
				if(x <= WIDTH/4 && y <= HEIGHT/3) {
					(*current).flag = R;
				}else if(x <= WIDTH/4 && y <= HEIGHT*2/3) {
				    (*current).flag = 0;
				}else if(x <= WIDTH/4 && y <= HEIGHT){ 
                    (*current).flag = E;
				}else if(x >= 9*WIDTH/10 && y >= HEIGHT/2 && y <= 11*HEIGHT/20){
					(*current).flag = ERASER; 
				} else {
					if(!((*current).graph.l.initial.x) && !((*current).graph.l.initial.y)){
						(*current).graph.l.initial.x = x;
						(*current).graph.l.initial.y = y;
					} else if(!((*current).graph.l.final.x) && !((*current).graph.l.final.y)){
						(*current).graph.l.final.x = x;
						(*current).graph.l.final.y = y;
						push(s, current);
						current -> Lcolor = current -> Bcolor = EMPTY;
					    current -> isEmpty = 0;
						(*current).graph.l = (Line){(Point){0,0},(Point){0,0}};
					}
				}	
			} else if(button == RIGHT_BUTTON){
				pop(s);
			}
	}else if(event == MOUSEMOVE&&(*current).graph.l.initial.x&&(*current).graph.l.initial.y){
            
			line((*current).graph.l.initial.x, (*current).graph.l.initial.y, x, y);
	}
	return;
}

void ellipseFlag(Stack *s, Gitem *current, int x, int y, int button, int event)
{
	
	int x1, y1, x2, y2, x3, y3;
	if(event == BUTTON_DOWN){
           if(button == LEFT_BUTTON){
				if(x <= WIDTH/4 && y <= HEIGHT/3) {
					(*current).flag = R;
				}else if(x <= WIDTH/4 && y <= HEIGHT*2/3) {
				    (*current).flag = L;
				}else if(x <= WIDTH/4 && y <= HEIGHT){ 
                    (*current).flag = 0;
				}else if(x >= 9*WIDTH/10 && y >= HEIGHT/2 && y <= 11*HEIGHT/20){
					(*current).flag = ERASER; 
				} else {
					if(!((*current).graph.e.left.x) && !((*current).graph.e.left.y)){
						(*current).graph.e.left.x = x;
						(*current).graph.e.left.y = y;
					} else if(!((*current).graph.e.right.x) && !((*current).graph.e.right.y)){
						(*current).graph.e.right.x = x;
						(*current).graph.e.right.y = y;
					} else if(!((*current).graph.e.down.x) && !((*current).graph.e.down.y)){
						(*current).graph.e.down.x = x;
						(*current).graph.e.down.y = y;
						push(s, current);
						(*current).graph.e = (_Ellipse){(Point){0,0},(Point){0,0},(Point){0,0}};
					     current -> Lcolor = current -> Bcolor = EMPTY;
					     current -> isEmpty = 0;
					}
				}	
			} else if(button == RIGHT_BUTTON){
				pop(s);
			}
	}else if(event == MOUSEMOVE&&(*current).graph.e.left.x&&(*current).graph.e.left.y\
		&&(*current).graph.e.right.x&&(*current).graph.e.right.y){
			x1 = (*current).graph.e.left.x;
			y1 = (*current).graph.e.left.y;
			x2 = (*current).graph.e.right.x;
			y2 = (*current).graph.e.right.y;
			x3 = x;
			y3 = y;
		    ellipse(x1, y3-2*(y3-y1), x2, y3);
    }
    
	return;
}

void eraserFlag(Stack *s, Gitem *current, int x, int y, int button, int event)
{
	if(event == BUTTON_DOWN){
            if(button == LEFT_BUTTON){
				if(x <= WIDTH/4 && y <= HEIGHT/3) {
					(*current).flag = R;
				}else if(x <= WIDTH/4 && y <= HEIGHT*2/3) {
				    (*current).flag = L;
				}else if(x <= WIDTH/4 && y <= HEIGHT){ 
                    (*current).flag = E;
				}else if(x >= 9*WIDTH/10 && y >= HEIGHT/2 && y <= 11*HEIGHT/20){
					(*current).flag = 0; 
				} else {
				    current->graph.e.left.x = x - ERASER_RADIUS;
				    current->graph.e.left.y = y;
				    current->graph.e.right.x = x + ERASER_RADIUS;
				    current->graph.e.right.y = y;
				    current->graph.e.down.x = x;
				    current->graph.e.down.y = y + ERASER_RADIUS;
				    current->isEmpty = 1;
				    current->Lcolor = current -> Bcolor = WHITE;
				    push(s, current);
					current -> graph.e = (_Ellipse){(Point){0,0},(Point){0,0},(Point){0,0}};
					current -> Lcolor = current -> Bcolor = EMPTY;
					current -> isEmpty = 0;
				}
			
			}else if(button == RIGHT_BUTTON){
				pop(s);
            }
	} 
	return;
}

void mouseEvent(int x, int y, int button, int event)
{
	static Gitem  Gs[MAXGRAPH], current =(Gitem){0,{0},EMPTY,EMPTY,0};	
	static Stack _s = (Stack){Gs, Gs};
	static Stack *s = &_s;
	static ACL_Color Lcolor = EMPTY, Bcolor = EMPTY ;
	static int isEmpty = 0;
	//printf("%d\n", s->top - s->base);
	
	beginPaint();
	clearDevice();
	setPenColor(YELLOW);
	setBrushColor(EMPTY);
	if(Lcolor != EMPTY) setPenColor(Lcolor);
	if(isEmpty)setBrushColor(Bcolor);
	if(event == BUTTON_DOWN && button == LEFT_BUTTON){
		if(x >= WIDTH*9/10&&x <= 19*WIDTH/20 && y <= HEIGHT/20 ||\
		x >= WIDTH*9/10&&x <= 19*WIDTH/20&&y>=HEIGHT/5&&y<=HEIGHT/4){
			 if(x >= WIDTH*9/10&&x <= 19*WIDTH/20 && y <= HEIGHT/20)Lcolor = (Lcolor != RED) ? RED : EMPTY;
			  if(x >= WIDTH*9/10&&x <= 19*WIDTH/20&&y>=HEIGHT/5&&y<=HEIGHT/4) {
					if(!isEmpty || Bcolor != RED){
					  	 isEmpty = 1;
					  	 Bcolor = RED;
					}else {
					  	 isEmpty = 0;
					  	 Bcolor = EMPTY;
					}
			 }
		}else if(x >= 19*WIDTH/20 && y <= HEIGHT/20 ||\
		x >= 19*WIDTH/20 &&y>=HEIGHT/5&&y<=HEIGHT/4){
			if(x >= 19*WIDTH/20 && y <= HEIGHT/20)Lcolor = (Lcolor != BLUE) ? BLUE : EMPTY;
			if(x >= 19*WIDTH/20 &&y>=HEIGHT/5&&y<=HEIGHT/4){
					if(!isEmpty || Bcolor != BLUE){
					  	 isEmpty = 1;
					  	 Bcolor = BLUE;
					}else {
					  	 isEmpty = 0;
					  	 Bcolor = EMPTY;
					}
			}
		}else if(x >= 9*WIDTH/10 && x <= 19*WIDTH/20 && y<= HEIGHT/10||\
		x >= 9*WIDTH/10 && x <= 19*WIDTH/20&&y>=HEIGHT/4&&y<=3*HEIGHT/10){
			if(x >= 9*WIDTH/10 && x <= 19*WIDTH/20 && y<= HEIGHT/10)Lcolor = (Lcolor != GREEN) ? GREEN : EMPTY;
			if(x >= 9*WIDTH/10 && x <= 19*WIDTH/20&&y>=HEIGHT/4&&y<=3*HEIGHT/10){
					if(!isEmpty || Bcolor != GREEN){
					  	 isEmpty = 1;
					  	 Bcolor = GREEN;
					}else {
					  	 isEmpty = 0;
					  	 Bcolor = EMPTY;
					}
			}
		}else if(x>=19*WIDTH/20&&y>=HEIGHT/4&&y<=3*WIDTH/10){
			if(!isEmpty || Bcolor != BLACK){
				isEmpty = 1;
				Bcolor = BLACK;
			}else {
				isEmpty = 0;
				Bcolor = EMPTY;
			}
		}
	}
	current.Lcolor = Lcolor;
	current.Bcolor = Bcolor;
	current.isEmpty = isEmpty;
	if(!(current.flag)){
		if(event == BUTTON_DOWN){
			if(button == LEFT_BUTTON){
				if(x <= WIDTH/4 && y <= HEIGHT/3) {
					current.flag = R;
				}else if(x <= WIDTH/4 && y <= HEIGHT*2/3) {
				    current.flag = L;
				}else if(x <= WIDTH/4 && y <= HEIGHT){ 
                    current.flag = E;
				}else if(x >= 9*WIDTH/10 && y >= HEIGHT/2 && y <= 11*HEIGHT/20){
					current.flag = ERASER;
				}
			}else if(button == RIGHT_BUTTON){
				pop(s);
			}
		}
    }else if(x <= WIDTH*9/10 || (x >= 9*WIDTH/10 && y >= HEIGHT/2 && y <= 11*HEIGHT/20)){
        if(current.flag == R){
		   rectFlag(s, &current, x, y, button, event);
	   }else if(current.flag == L){
		   lineFlag(s, &current, x, y, button, event);
	   }else if(current.flag == E){
		   ellipseFlag(s, &current, x, y, button, event);
	   }else if(current.flag == ERASER){
	   	   eraserFlag(s, &current, x, y, button, event);
	   }
    }
    printf("mouseEvent In  flag%d\n", current.flag);
    initCAD(&current);
    drawStack(s);
    endPaint();
    return;
}

int Setup(void)
{
	Gitem init = {0,{0},EMPTY,EMPTY,0};
	initConsole();
	initWindow("CAD", 100, 100, WIDTH, HEIGHT);
	registerMouseEvent(mouseEvent);
	beginPaint();
	setPenColor(RED);
	setBrushColor(EMPTY);
	initCAD(&init);
	endPaint();
}
