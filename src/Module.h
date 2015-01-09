#include <vector>
#include <string>
#include "ofxUI.h"
#include "ofApp.h"


#define WIDTH_13 1.7
#define HEIGHT_13 1
#define WIDTH_24 1.7
#define HEIGHT_24 3
#define WIDTH_27 3.4
#define HEIGHT_27 2

#define GRID_RATIO 65

#define SCREEN_TYPE 0
#define LED_TYPE 1
#define LEDSCREEN_TYPE 2
#define TEXTURE_TYPE 3


class Module{
	private:
		int typeSize;//13, 24 ou 27
		int typeMat; // screen, LED, screen LED, texture
		int ID;
		ofPoint pos;//absolute pos
		ofPoint locationInGrid;//pos in grid
		ofPoint size;//depending on typeSize
		ofPoint touch;//xy between the user finger et the left right angle of the module
		ofTrueTypeFont font;
		
		bool isTouched;
		bool isDeleted;
		
		bool isWellPlaced;
		
		ofColor color;
		
		//calcule la marge nécessaire en fonction de la taille de la grille
		// /!\aussi dans wall --> faut changer les deux
		int bordureX = (((ofGetWidth()-50)/(1.7*GRID_RATIO) - (float)((int)((ofGetWidth()-50)/(1.7*GRID_RATIO))))*1.7*GRID_RATIO + 50) /2; 
		int bordureY = (((ofGetHeight()-200.0)/GRID_RATIO - (float)((int)((ofGetHeight()-200)/GRID_RATIO)))*GRID_RATIO + 25 /*GRID_RATIO*/) /2;
		
	
	public:
		bool isSelected;
		
	
	Module(int typeSize, int typeMat, int ID): typeSize(typeSize), ID(ID), typeMat(typeMat)
	{
		pos.set(bordureX, bordureY+50);//default pos
		touch.set(0, 0, 0);
		isTouched = false;
		isDeleted=false;
		isWellPlaced=true;
		isSelected=false;
		locationInGrid.set(-1, -1);
		
		font.loadFont("open-sansbold-italic.ttf", 12);
		
		switch(typeSize){
			case 0:
				size.set(WIDTH_13*GRID_RATIO, HEIGHT_13*GRID_RATIO);
				break;
			case 1:
				size.set(WIDTH_24*GRID_RATIO, HEIGHT_24*GRID_RATIO);
				break;
			case 2:
				size.set(WIDTH_27*GRID_RATIO-1, HEIGHT_27*GRID_RATIO);
				break;
		}
		
		switch(typeMat){
			case 0:
				color = ofColor(127, 127, 127);
				break;
			case 1:
				color = ofColor(157, 177, 250);
				break;
			case 2:
				color = ofColor(147, 217, 120);
				break;
			case 3:
				color = ofColor(184, 148, 193);
				break;
		}
		
	}
	
	void draw() {
		
		ofPushStyle();
			ofFill();
			ofSetColor(color);
			ofRect(pos.x, pos.y, size.x, size.y);
			
			//if far from the others modules
			if (!isWellPlaced){
				ofSetColor(255, 0, 0);
				ofLine(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
			}
			
			ofNoFill();
			if (isSelected && ID != -1) ofSetColor(255, 0, 0);
			else ofSetColor(255);
			ofRect(pos.x, pos.y, size.x, size.y);
			
			ofSetColor(40);
			switch(typeMat){
				case SCREEN_TYPE:
					font.drawString("Screen", pos.x+5, pos.y+5 + font.getSize());
					break;
				case LED_TYPE:
					font.drawString("LED", pos.x+5, pos.y+5+ font.getSize());
					break;
				case LEDSCREEN_TYPE:
					font.drawString("LED screen", pos.x+5, pos.y+5+ font.getSize());
					break;
				case TEXTURE_TYPE:
					font.drawString("texture", pos.x+5, pos.y+5+ font.getSize());
					break;
			}
			if(ID != -1)//library modules
				font.drawString(ofToString(ID), pos.x+size.x/2, pos.y+ size.y/2 + font.getSize());
		ofPopStyle();
	}
	
	//si les deux modules sont l'un sur l'autre
	bool isCollision(Module *module){
		if (( module->pos.x + module->size.x <= this->pos.x)
			|| (module->pos.y + module->size.y <= this->pos.y)
			|| (module->pos.x >= this->pos.x+this->size.x)
			|| (module->pos.y >= this->pos.y+this->size.y)){
				return false;
		}		
		else {
			return true;
		}
	}
	//si les deux modules ne sont pas l'un à coté de l'autre
	bool isAlone(Module *module){
		if (( module->pos.x + module->size.x -10 <= this->pos.x &&  module->pos.y + module->size.y -10 <= this->pos.y) //angle
			|| (module->pos.x +10 >= this->pos.x+this->size.x && module->pos.y + module->size.y -10 <= this->pos.y)
			|| (module->pos.x + module->size.x -10 <= this->pos.x && module->pos.y +10 >= this->pos.y + this->size.y)
			|| (module->pos.x +10 >= this->pos.x+this->size.x && module->pos.y +10 >= this->pos.y + this->size.y)
			
			|| (module->pos.x + module->size.x -10 <= this->pos.x-GRID_RATIO*1.7)
			|| (module->pos.x +10 >= this->pos.x + this->size.x + GRID_RATIO*1.7)
			|| (module->pos.y + module->size.y -10 <= this->pos.y - GRID_RATIO)
			|| (module->pos.y +10 >= this->pos.y+this->size.y + GRID_RATIO))
				return true;
		else return false;
	}
	
	//si le module se trouve à coté d'un autre de même type et taille
	bool isSplitable(Module *module){
		if (abs(this->pos.x - module->pos.x) == (int)(1.7*GRID_RATIO) && this->pos.y == module->pos.y) return true;
		else return false;
	}
	
	bool onTouchDown(int x, int y){
			if (x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y ) {
				isTouched = true;
				isSelected = true;
				touch.set(x-pos.x, y-pos.y);
				return true;
			}
		return false;
	}
	
	void onTouchUp(int x, int y){
		isTouched = false;
		if (!(x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y))
			isSelected = false;
	}
	
	void onTouchMove(int x, int y){
		if (ID != -1){//library modules don't move
			if ( x-touch.x > bordureX && x+(size.x-touch.x) < ofGetWidth()-bordureX && y-touch.y > bordureY+50 && y+(size.y-touch.y) < ofGetHeight()-100){ 
			   if (isTouched) {
			   		//déplacement de case en case et non en continue
			   		//modifier les 50 pour donner plus ou moins de marge pour le déplacement
			   		
			   		if ( (int)abs(x-touch.x - bordureX) % (int)(1.7*GRID_RATIO) < 50 && (int)abs(y - touch.y - bordureY - 50) % GRID_RATIO < 50 ){
			   			pos.set(x-touch.x - ((int)(x-touch.x - bordureX) % int(1.7*GRID_RATIO)), (y-touch.y) - ((int)(y-touch.y - bordureY - 50) % GRID_RATIO));
			   			getPosInGrid();
			   		}
			   	}
			}
		}
	}
	
	//not used
	void onDoubleClick(int x, int y){
		if (x > pos.x && x < pos.x + size.x && y > pos.y && y < pos.y + size.y )
			isDeleted=true;
	}
	
	
	void setTypeSize(int typeSize){
		this->typeSize=typeSize;
	}
	void setID(int ID){
		this->ID=ID;
	}
	void setPos(int x, int y){
		pos.set(x, y, 0);
	}
	void setIsWellPlaced(bool isWellPlaced){
		this->isWellPlaced=isWellPlaced;
	}
	void setIsTouched(bool isTouched){
		this->isTouched=isTouched;
	}
	
	bool getIsDeleted(){
		return isDeleted;
	}
	void setIsDeleted(bool isDeleted){
		this->isDeleted=isDeleted;
	}
	int getID(){
		return ID;
	}
	int getTypeSize(){
		return typeSize;
	}
	int getTypeMat(){
		return typeMat;
	}
	bool getIsTouched(){
		return isTouched;
	}
	bool getIsWellPlaced(){
		return isWellPlaced;
	}
	ofPoint getPos(){
		return pos;
	}
	
	ofPoint getPosInGrid(){
		ofPoint tempPoint;
		
		tempPoint.x = (pos.x - bordureX) / int (1.7*GRID_RATIO);
		tempPoint.y = (pos.y - bordureY - 50) / GRID_RATIO;
		
		return tempPoint;
	}


};