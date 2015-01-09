#include <vector>
#include <string>
#include "ofxUI.h"

#define CHANNEL_IMAGE_WIDTH (ofGetWidth() - 3.0*10.0/100*ofGetWidth())/3.0 +20
#define CHANNEL_IMAGE_HEIGHT (ofGetHeight() - HEIGHT_BUTTONS*2 - 2.0*1.0/9*ofGetHeight() - 1.0/15 * ofGetHeight())/2.0

#define PREVIEW_IMAGE_WIDTH 2.0/5*ofGetWidth() 
#define PREVIEW_IMAGE_HEIGHT 2.0/5*ofGetHeight()

class Channel{
	public:
	
		//TODO: inverser les deux car mauvaise dénomination
		ofImage channelImage;
		ofImage previewImage;
		
		string title;
		string creator;
		string price;
		string description;
		string imageUrl;
		vector<string> tags;//not used
		string tagsString;
		int ID;
		int rate;
		
		int imgVidRate, transitionSpeed, colorHarmony, sensorInput, sensorSensitivity;
		
		ofTrueTypeFont titleFont;
		ofTrueTypeFont infoFont;
		
		ofxUICanvas *guiDescription;
		ofxUITextArea *descriptionArea;
		
		ofxUICanvas *guiTags;
		ofxUITextInput *tagsTextInput;
		
		//automix canvas
		ofxUICanvas *guiTitle;
		ofxUITextInput *titleTextInput;
		ofxUICanvas *guiDescriptionEdit;
		ofxUITextInput *descriptionTextInput;
		
		
		Button *playButton;
		
		ofPoint tempPosition;//position on home page
		
		bool automixImageFound;//via osc
		
		ofxUIColor cb;
		ofxUIColor cb2;
		ofxUIColor co;
		ofxUIColor coh;
		ofxUIColor cf;
		ofxUIColor cfh;
		ofxUIColor cp;
		ofxUIColor cpo;
		
		ofxXmlSettings modSettings;
		
		
		//Creation depuis un xml
		Channel(int ID, string path):ID(ID)
		{
		
			xml2channel(path);
			
			imgVidRate = 0;
			transitionSpeed = 0;
			colorHarmony = 0;
			sensorInput = 0;
			sensorSensitivity = 0;
			
			rate = rand()%5;
			automixImageFound = true;
			tempPosition.set(0, 0);
			
			titleFont.loadFont("open-sansbold-italic.ttf", 15);
			infoFont.loadFont("OpenSans-Regular.ttf", 0.02*ofGetHeight());
			
			cb = ofxUIColor(0, 0, 0, 0); //Background 
			cb2 = ofxUIColor(40, 40, 40, 150); //BG liste déroulante
		    co = ofxUIColor(255, 0, 0, 255); // ???
		    coh = ofxUIColor(255, 255, 255, 255); //tour quand sélectionné
		    cf = ofxUIColor(40, 40, 40, 255); //texte non sélectionné
		    cfh = ofxUIColor(255, 255, 255, 255); //texte sélectionné + curseur
		    cp = ofxUIColor(255, 255, 255, 255); // ???
		    cpo = ofxUIColor(255, 0, 0, 255);
		    
		    playButton = new Button ("play", ID, 0, 0, 1.0/10*ofGetWidth(), HEIGHT_BUTTONS-20, 3, "PLAY", "PLAY", 40);
		    
		    guiDescription = new ofxUICanvas(  50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+40+titleFont.getSize()*2, 3.0/5*ofGetWidth()-60, ofGetHeight() - 2*HEIGHT_BUTTONS+40);
			guiDescription->setFont("OpenSans-Regular.ttf");
			guiDescription->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiDescription->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			descriptionArea = guiDescription->addTextArea("description", description); 
			
			guiDescription->setVisible(false);
		}
		
		//création d'un nouveau channel dans l'appli (automix)
		Channel(string text)
		{
			creator = "user";
			rate = 0;
			automixImageFound = false;
			imageUrl="";
			ID = 0;
			
			price = "free";
			tempPosition.set(0, 0);
			
			imgVidRate = 0;
			transitionSpeed = 0;
			colorHarmony = 0;
			sensorInput = 0;
			sensorSensitivity = 0;
			
			titleFont.loadFont("open-sansbold-italic.ttf", 15);
			//titleFont.setEncoding(OF_ENCODING_ISO_8859_15);
			infoFont.loadFont("OpenSans-Regular.ttf", 0.02*ofGetHeight());
			
			cb = ofxUIColor(0, 0, 0, 0); //Background 
			cb2 = ofxUIColor(40, 40, 40, 150); //BG liste déroulante
		    co = ofxUIColor(255, 0, 0, 255); // ???
		    coh = ofxUIColor(255, 255, 255, 255); //tour quand sélectionné
		    cf = ofxUIColor(40, 40, 40, 255); //texte non sélectionné
		    cfh = ofxUIColor(255, 255, 255, 255); //texte sélectionné + curseur
		    cp = ofxUIColor(255, 255, 255, 255); // ???
		    cpo = ofxUIColor(255, 0, 0, 255);
		    
		    
		    
		    guiTitle= new ofxUICanvas(25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight(), 2.0/8*ofGetWidth()-20, HEIGHT_BUTTONS);
			guiTitle->setFont("open-sansbold-italic.ttf");
			guiTitle->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiTitle->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			titleTextInput = guiTitle->addTextInput("Write your title here", "Write your title here", -1); 
			titleTextInput->setAutoUnfocus(true);
			titleTextInput->setAutoClear(true);
			guiTitle->setVisible(false);
			
			guiTags= new ofxUICanvas(25 + titleFont.stringWidth("# :"), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+35, 2.0/8*ofGetWidth()-20, HEIGHT_BUTTONS);
			guiTags->setFont("OpenSans-Regular.ttf");
			guiTags->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiTags->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			tagsTextInput = guiTags->addTextInput("tags input", text, -1); 
			tagsTextInput->setAutoUnfocus(true);
			tagsTextInput->setAutoClear(true);
			guiTags->setVisible(false);
			
			guiDescriptionEdit= new ofxUICanvas( 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+40+titleFont.getSize()*2, 3.0/5*ofGetWidth()-60, ofGetHeight() - 2*HEIGHT_BUTTONS+40);
			guiDescriptionEdit->setFont("OpenSans-Regular.ttf");
			guiDescriptionEdit->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiDescriptionEdit->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			descriptionTextInput = guiDescriptionEdit->addTextInput("Write your description here...", "Write your description here...", -1); 
			descriptionTextInput->setAutoUnfocus(true);
			descriptionTextInput->setAutoClear(true);
			guiDescriptionEdit->setVisible(false);
				
		}
		
		
		//en dure (plus utilisé)
		Channel(string url, string title, string creator, int rate, int ID): imageUrl(url), title(title), creator(creator), rate(rate), ID(ID)
		{
			channelImage.loadImage(url);
			channelImage.resize(CHANNEL_IMAGE_WIDTH, CHANNEL_IMAGE_HEIGHT);
			previewImage.loadImage(url);
			previewImage.resize(PREVIEW_IMAGE_WIDTH, PREVIEW_IMAGE_HEIGHT);
			
			titleFont.loadFont("open-sansbold-italic.ttf", 15);
			titleFont.setEncoding(OF_ENCODING_ISO_8859_15);
			infoFont.loadFont("OpenSans-Regular.ttf", 0.02*ofGetHeight());
			price = "free";
			tempPosition.set(0, 0);
			
			imgVidRate = (rand()%4)*25;
			transitionSpeed = (rand()%4)*25;
			colorHarmony = (rand()%4)*25;
			sensorInput = rand()%1;
			sensorSensitivity = (rand()%4)*25;
			
			playButton = new Button ("play", ID, 0, 0, 1.0/10*ofGetWidth(), HEIGHT_BUTTONS-20, 3, "PLAY", "PLAY", 40);
						
			description = " Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer tempor a ipsum quis lobortis. Nunc nec orci condimentum lectus mollis suscipit. Nunc fringilla lorem ac maximus imperdiet. Aenean neque massa, laoreet ut lorem nec, ultrices eleifend nulla. Duis auctor arcu dolor, id fermentum leo accumsan et. Morbi ullamcorper, nunc eget elementum lobortis, massa justo feugiat dolor, eu porttitor nibh nulla nec arcu. Ut vitae purus aliquam, convallis mi et, ultricies sapien. Mauris elementum lectus ante, laoreet facilisis mi aliquam nec. Sed imperdiet, ex vel efficitur faucibus, sem nibh bibendum metus, a lobortis enim sem lobortis ex. Nulla facilisi. Morbi finibus augue sed enim congue, a porttitor enim elementum. Nullam nec sollicitudin dolor. Vivamus ornare efficitur nunc vel dignissim. Praesent semper orci in nisi viverra, ut maximus mauris dictum. Nunc lorem sem, volutpat ac orci ut, venenatis eleifend magna. Duis eget ornare nisl. Donec nunc felis, posuere non aliquet ut, accumsan aliquam sapien. Nunc semper suscipit mattis. Sed eu velit sapien. Integer fermentum eget turpis sit amet elementum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur felis velit, dignissim sit amet rhoncus vitae, consectetur at elit. Cras faucibus, felis ac tristique bibendum, velit erat dapibus tortor, quis faucibus nisl velit id ante. Duis varius nisi dolor, efficitur lacinia metus condimentum viverra. Interdum et malesuada fames ac ante ipsum primis in faucibus. Fusce et erat faucibus, egestas metus ut, vestibulum eros. Curabitur et urna fermentum massa iaculis dignissim at id arcu. Aliquam tincidunt sagittis libero, a accumsan ligula pulvinar et. ";
			tags.push_back("poulet");
			tags.push_back("arc-en-ciel");
			tags.push_back("jambon");
			
			for (int i = 0; i < tags.size(); i++){
				tagsString += tags[i] + ", ";
			}
			
			cb = ofxUIColor(0, 0, 0, 0); //Background 
			cb2 = ofxUIColor(40, 40, 40, 150); //BG liste déroulante
		    co = ofxUIColor(255, 0, 0, 255); // ???
		    coh = ofxUIColor(255, 255, 255, 255); //tour quand sélectionné
		    cf = ofxUIColor(40, 40, 40, 255); //texte non sélectionné
		    cfh = ofxUIColor(255, 255, 255, 255); //texte sélectionné + curseur
		    cp = ofxUIColor(255, 255, 255, 255); // ???
		    cpo = ofxUIColor(255, 0, 0, 255);
		    
			guiDescription = new ofxUICanvas(  50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+40+titleFont.getSize()*2, 3.0/5*ofGetWidth()-60, ofGetHeight() - 2*HEIGHT_BUTTONS+40);
			guiDescription->setFont("OpenSans-Regular.ttf");
			guiDescription->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiDescription->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			descriptionArea = guiDescription->addTextArea("description", description); 
			
			guiDescription->setVisible(false);
		}
		
		void globalInit(){
			//TODO
		
		}
		
		void xml2channel(string path){
			
			//save the distant xml file in a temp local file
			ofFile tempXML;
		    ofBuffer dataBuffer;
		    
		    tempXML.open(ofToDataPath("temp.xml"), ofFile::ReadWrite, false);
		    dataBuffer = ofLoadURL(path).data;
		    ofBufferToFile("temp.xml", dataBuffer);
		    
		    //load the local xml file
		    modSettings.load("temp.xml");
		    tempXML.remove();
		    
		    modSettings.pushTag("channel");
				    creator = modSettings.getValue("by", "error");
				    price = modSettings.getValue("price", "error");
					imageUrl = "http://192.168.1.13:8000/wallChannels/"+modSettings.getValue("playfolder", "error")+"/image/1.jpg";
					description = modSettings.getValue("blurb", "error");
					tagsString = modSettings.getValue("keyword", "error");
					title = modSettings.getValue("title", "error");
			
			channelImage.loadImage(imageUrl);
			channelImage.resize(CHANNEL_IMAGE_WIDTH, CHANNEL_IMAGE_HEIGHT);
			previewImage.loadImage(imageUrl);
			previewImage.resize(PREVIEW_IMAGE_WIDTH, PREVIEW_IMAGE_HEIGHT);
		}
		
		//temporarly save localy the channel from automix (don't survive a reboot)
		void saveChannel(int buttonID){
			playButton = new Button ("play", buttonID, 0, 0, 1.0/10*ofGetWidth(), HEIGHT_BUTTONS-20, 3, "PLAY", "PLAY", 40);
			guiDescription = new ofxUICanvas(  50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+40+titleFont.getSize()*2, 3.0/5*ofGetWidth()-60, ofGetHeight() - 2*HEIGHT_BUTTONS+40);
			guiDescription->setFont("OpenSans-Regular.ttf");
			guiDescription->setWidgetFontSize(OFX_UI_FONT_LARGE);
			guiDescription->setUIColors( cb, cb, coh, cf, cfh, cp, cpo );
			descriptionArea = guiDescription->addTextArea("description", description); 
			
			guiDescription->setVisible(false);
		}
		
		//on home page
		void drawPreview(/*int x, int y*/){
			int x = tempPosition.x;
			int y = tempPosition.y;
			if (x != -1 && y != -1){
				ofPushMatrix();
				ofPushStyle();
					ofSetColor(40);
					titleFont.drawString(title, x, y);
					ofSetColor(255);
					channelImage.draw(x, y+5/*+titleFont.getSize()*/);
					ofSetColor(127);
					infoFont.drawString("By: " + creator, x, y/*+1.0/32*ofGetHeight()*/+CHANNEL_IMAGE_HEIGHT+ infoFont.getSize()+10);
					infoFont.drawString("Price: " + price, x, y/*+1.0/32*ofGetHeight()*/+CHANNEL_IMAGE_HEIGHT+ infoFont.getSize()*2 + 15);
					
					/*ofSetCircleResolution(100);
					for (int i = 0; i < 5; i ++ ){ //rate
						if (rate>i) ofFill();
						else ofNoFill();
						
						ofSetColor(36, 37, 38);
						ofCircle(x+CHANNEL_IMAGE_WIDTH*2.0/3+i*infoFont.getSize()+i*0.5*infoFont.getSize(), y+CHANNEL_IMAGE_HEIGHT+2*infoFont.getSize(), infoFont.getSize()/1.5);
					}*/
					
				//tempPosition.set(x, y);
							
							
				ofPopStyle();
				ofPopMatrix();
			}
		}
		
		//on display channel
		void drawPage(){
			if (imageUrl != ""){
				previewImage.draw(20, HEIGHT_BUTTONS+20);
			} else {
				ofPushStyle();
    				ofSetColor(40, 40, 40, 255);
    				ofFill();
    				ofRect(20, HEIGHT_BUTTONS+20, 2.0/5*ofGetWidth(), 2.0/5*ofGetHeight());
    			ofPopStyle();
			
			}
			ofPushStyle();
			ofSetColor(40, 40, 40);
			titleFont.drawString(title + " @ ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+5);
			infoFont.drawString( creator, 25+titleFont.stringWidth(title + " @ :"), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+5);
			
			/*for (int i = 0; i < 5; i ++ ){//rate
				if (rate>i) ofFill();
				else ofNoFill();
					
				ofSetColor(36, 37, 38);
				ofCircle(20+2.0/5*ofGetWidth()-100+i*infoFont.getSize()+i*0.5*infoFont.getSize(), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()/1.5 / 2 + 10, infoFont.getSize()/1.5);
			}*/
			
			titleFont.drawString("# ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
			infoFont.drawString(tagsString, 25 + titleFont.stringWidth("# :"), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
			
			titleFont.drawString("Price: ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+100);
			infoFont.drawString(price, 25 + titleFont.stringWidth("Price : "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+100);
			
			titleFont.drawString("Suscribe now - Bookmark now ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+150);
			
			titleFont.drawString("Description", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize());
			guiDescription->setVisible(true);
			
			ofPopStyle();
		}
		
		//on select channel
		void drawMini(int x, int y){
			playButton->setPos(x, y);
			playButton->draw();
			infoFont.drawString(title, x+ 1.0/9*ofGetWidth() + 10, y+infoFont.getSize()+10);
		}
		
		//modify automix page
		void drawAutomixCreation(){
		
			guiDescriptionEdit->setVisible(false);
    		guiTitle->setVisible(false);
			if (!automixImageFound) {
    			ofPushStyle();
    				ofSetColor(40, 40, 40, 255);
    				ofFill();
    				ofRect(20, HEIGHT_BUTTONS+20, 2.0/5*ofGetWidth(), 2.0/5*ofGetHeight());
    			ofPopStyle();
    		} else {
    			previewImage.draw(20, HEIGHT_BUTTONS+20);
    		}
    		
    		ofPushStyle();
    			ofSetColor(40);
    			titleFont.drawString("#", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+40);
    			
    			guiTags->setVisible(true);
    			tagsString = tagsTextInput->getTextString(); // set in an update...
    						
				titleFont.drawString("SETTINGS", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize());
				infoFont.drawString ("Photo / Video", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+45);
				infoFont.drawString ("Transition Speed", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+100);
				infoFont.drawString ("Color Harmony", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+155);
				infoFont.drawString ("Sensors Input", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+210);
			
				titleFont.drawString("MOODS", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize()+350);
			ofPopStyle();
			
		}
		
		//save automix page
		void drawAutomixValidation(){
			guiTags->setVisible(false);
			if (!automixImageFound) {
    			ofPushStyle();
    				ofSetColor(40, 40, 40, 255);
    				ofFill();
    				ofRect(20, HEIGHT_BUTTONS+20, 2.0/5*ofGetWidth(), 2.0/5*ofGetHeight());
    			ofPopStyle();
    		} else {
    			previewImage.draw(20, HEIGHT_BUTTONS+20);
    		}
    		ofPushStyle();
    			ofSetColor(40);
    			
    			
    			titleFont.drawString("# ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
    			infoFont.drawString(tagsTextInput->getTextString(), 25 + titleFont.stringWidth("# :"), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+50);
    			
    			titleFont.drawString("Free Y/N ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+110);
    			ofCircle(50 + titleFont.stringWidth("Public Y/N "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()-5+110, 10);
    			ofLine(50 + titleFont.stringWidth("Public Y/N "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+110 - 5, 120 + titleFont.stringWidth("Public Y/N "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+110 - 5);
    			
    			titleFont.drawString("Price: ", 300, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+110);
    			infoFont.drawString("0.00 usd / mo", 300+infoFont.stringWidth("Price : "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+110);
    			
    			titleFont.drawString("Public Y/N ", 25, HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+150);
    			ofCircle(50 + titleFont.stringWidth("Public Y/N "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()-5+150, 10);
    			ofLine(50 + titleFont.stringWidth("Public Y/N "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+150 - 5, 120 + titleFont.stringWidth("Public Y/N "), HEIGHT_BUTTONS+20+2.0/5*ofGetHeight()+infoFont.getSize()+150 - 5);
    			
    			titleFont.drawString("Description", 50 + 2.0/5*ofGetWidth(), HEIGHT_BUTTONS+20+titleFont.getSize());
    			guiDescriptionEdit->setVisible(true);
    			guiTitle->setVisible(true);
    			
    			description = descriptionTextInput->getTextString();
    			title = titleTextInput->getTextString();
    			
    		ofPopStyle();
		
		}
		
		bool isTouched(int x, int y){
			if (tempPosition.x !=-1 && x>tempPosition.x && x < tempPosition.x+CHANNEL_IMAGE_WIDTH && y > tempPosition.y && y < tempPosition.y + CHANNEL_IMAGE_HEIGHT + 2*infoFont.getSize()) {
				return true;
			}
			return false;	
		}
		
		//set the tags from automixTextInput of the menu to the tagsTextInput of the automix page
		void setTagsText(string automixTags){
			tagsTextInput->setTextString(automixTags);
		}
};