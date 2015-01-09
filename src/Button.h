#include <vector>
#include <string>

#define WIDTH_BUTTONS 0.245*ofGetWidth()
#define HEIGHT_BUTTONS 1.0/15*ofGetHeight()

class Button{
	private:
		char* name;
		ofRectangle dimensions;
		int associatedPages;
		string activName, passivName; //PLAY/PAUSE buttons for example
		bool isActiv;
		bool isTouched;
		bool isAvailable; //not used
		bool isVisible;
		int ID;
		ofTrueTypeFont font;
		int color;
		
	public:
	
	Button(char* name, int ID, int x, int y, int w, int h, int associatedPages, string activName, string passivName, int color = 255): name(name), associatedPages(associatedPages), ID(ID), activName(activName), passivName(passivName), color(color)
	{
		dimensions.set(x, y, w, h);
		isActiv = false;
		isTouched = false;
		isAvailable=true;
		isVisible=true;
		font.loadFont("OpenSans-Regular.ttf", 15);
	}
	
	void draw() {
		if (isVisible){
			ofPushStyle();
			//if (isAvailable){
				if (isTouched) ofFill();
				else ofNoFill();
				ofSetColor(color);
				ofRect(dimensions.getX(), dimensions.getY(), dimensions.getWidth(), dimensions.getHeight());
				
				if (isActiv) {
					font.drawString(activName, dimensions.getX() + dimensions.getWidth()/2-font.stringWidth(activName)/2, dimensions.getY()+dimensions.getHeight()/2+font.getLineHeight()/2);
				} else {
					font.drawString(passivName, dimensions.getX() + dimensions.getWidth()/2-font.stringWidth(passivName)/2, dimensions.getY()+dimensions.getHeight()/2+font.getLineHeight()/2);
				}
			/*} else {
				ofFill();
				ofSetColor(125, 50,50);
				font.drawString(passivName, dimensions.getX() + dimensions.getWidth()/2-font.stringWidth(passivName)/2, dimensions.getY()+font.getLineHeight());
			}*/
			ofPopStyle();
		}
	}
	
	void moveTo(int x, int y) {
		dimensions.setX(x);
		dimensions.setY(y);
	}
	
	bool isTouchedUp(int x, int y){
		if (x>dimensions.getX()&& x < dimensions.getX()+dimensions.getWidth() && y > dimensions.getY()&& y < dimensions.getY()+dimensions.getHeight()) {
			if (isTouched) isActiv = !isActiv;
			isTouched = false;
			return true;
		} else {
			isTouched = false;
			return false;
		}
	}
	bool isTouchedDown(int x, int y){
		if (x>dimensions.getX()&& x < dimensions.getX()+dimensions.getWidth() && y > dimensions.getY() && y < dimensions.getY()+dimensions.getHeight()) {
			isTouched = true;
			return true;
		} else {
			isTouched = false;
			return false;
		}
	}
	
	
	
	void setPos(int x, int y){
		dimensions.setX(x);
		dimensions.setY(y);
	}
	void setSize(int w, int h){
		dimensions.setWidth(w);
		dimensions.setHeight(h);
	}
	void setActivName(string activName){
		this->activName = activName;
	}
	void setPassivName(string passivName){
		this->passivName = passivName;
	}
	void setAssociatedPages(int associatedPages){
		this->associatedPages = associatedPages;
	}
	void setIsActiv(bool isActiv){
		this->isActiv = isActiv;
	}
	void setIsAvailable(bool isAvailable){
		this->isAvailable=isAvailable;
	}
	void setID(int ID){
		this->ID = ID;
	}
	ofPoint getPos(){
		return ofPoint(dimensions.x, dimensions.y);
	}
	int getAssociatedPages(){
		return associatedPages;
	}
	int getID(){
		return ID;
	}
	bool getIsActiv(){
		return isActiv;
	}
	void setIsTouched(bool isTouched){
		this->isTouched = isTouched;
	}
	void setVisible(bool isVisible){
		this->isVisible = isVisible;
	}
};
