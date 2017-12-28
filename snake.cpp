#include <cheerp/clientlib.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <cheerp/client.h>
#include <math.h>
#include <chrono>
using namespace client;
using namespace std;

void mainLoop();


class [[cheerp::genericjs]] Graphics{
  private:
  	static HTMLCanvasElement* canvas;
  	static CanvasRenderingContext2D* canvasCtx;
  	static int width;
  	static int height;
  	static void rafHandler(){
  		mainLoop();
  		requestAnimationFrame(cheerp::Callback(rafHandler));
  	}
  public:
  	static void keyDownHandler(KeyboardEvent* e);
    static void log(int value){;
        console.log("%s", value);
      }
  	static void initializeCanvas(int w, int h){
  		width = w;
  		height = h;
  		canvas = (HTMLCanvasElement*)document.getElementById("canvas");
  		canvas->set_width(w);
  		canvas->set_height(h);
  		document.get_body()->appendChild(canvas);
  		canvasCtx = (CanvasRenderingContext2D*)canvas->getContext("2d");
  		requestAnimationFrame(cheerp::Callback(rafHandler));
  		document.addEventListener("keydown", cheerp::Callback(keyDownHandler));
  	}
  	static void drawRect(int x, int y, int w, int h, int rgb){
  		int r = rgb&0xff;
  		int g = (rgb>>8)&0xff;
  		int b = (rgb>>16)&0xff;
  		canvasCtx->set_fillStyle(String("").concat("rgb(", r, ",", g, ",", b, ")"));
  		canvasCtx->fillRect(x, y, w, h);
  	}
};

const int unitSpeed = 10;
const int maxWidth = 1024;
const int maxHeight = 768;

class Box {
private:
  int posX;
  int posY;

public:
  Box() {
    posX = -10;
    posY = -10;
  }
  Box(int x, int y){
    posX = x;
    posY = y;
  }

  void render(){
    Graphics::drawRect(posX, posY, 10, 10, 0xffffff);
  }

  void setCoords(int x, int y){
    posX = x;
    posY = y;
  }
  int getX() {
    return posX;
  }
  int getY() {
    return posY;
  }
};

class Snake {
private:
  int posX;
  int posY;
  int dX;
  int dY;
  int current_length;
  int max_length = 200;
  Box boxes[200];

public:
  Snake(){};
  Snake(int x, int y){
    current_length = 25;
    posX = x;
    posY = y;
    dX = unitSpeed;
    dY = 0;
    for (int i= 0; i < current_length; i++) {
      boxes[i].setCoords((posX-i * 10), posY);
      }
    }

  int getX(){
    return posX;
  }

  int getY() {
    return posY;
  }

  void incCurrentLength(){
    current_length += 10;
  }

  void render(){
    posX += dX;
    posX = (posX + maxWidth) % maxWidth;
    posY += dY;
    posY = (posY + maxHeight) % maxHeight;
    for (int i= current_length; i > 0 ; i--) {
      boxes[i].setCoords(boxes[i-1].getX(), boxes[i-1].getY());
    }
    boxes[0].setCoords(posX, posY);
    for (int i = 0; i < current_length; i++){
      boxes[i].render();
    }
  }

  void moveLeft(){
    if (dX == 0) {
      dY = 0;
      dX = -unitSpeed;
    }
  };

  void moveRight(){
    if (dX == 0) {
      dY = 0;
      dX = unitSpeed;
    }
  };

  void moveUp(){
    if (dY == 0) {
      dX = 0;
      dY = -unitSpeed;
    }
  };

  void moveDown(){
    if (dY == 0) {
      dX = 0;
      dY = unitSpeed;
    }
  };
};

Snake snake;
Box food;
void Graphics::keyDownHandler(KeyboardEvent *e){
	if(e-> get_keyCode() == 37){
		snake.moveLeft();
}	else if(e-> get_keyCode() == 39){
		snake.moveRight();
}  else if(e-> get_keyCode() == 38){
    snake.moveUp();
  }
  else if(e-> get_keyCode() == 40){
    snake.moveDown();
  }
}

void mainLoop(){
  if ((abs(snake.getX()-food.getX())< 10) && (abs(snake.getY()-food.getY())< 10)){
    snake.incCurrentLength();
    int now = snake.getX() * 2 ;
    int ranXAxis = now % maxWidth;
    int ranYAxis = now % maxHeight;
    food.setCoords(ranXAxis,ranYAxis);
  }
	Graphics::drawRect(0, 0, maxWidth, maxHeight, 0x000000);
  snake.render();
  food.render();
}

void webMain(){
	Graphics::initializeCanvas(maxWidth, maxHeight );
  snake = Snake(100,100);
  int now = snake.getX() * 2 ;
  int ranXAxis = now % maxWidth;
  int ranYAxis = now % maxHeight;
  food = Box(ranXAxis,ranYAxis);
}
