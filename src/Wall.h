
#define X_BUTTONS 0.73*ofGetWidth()
#define Y_BUTTONS 0.17*ofGetHeight()

#define SCREEN_MAX_NUMBER 10

class Wall{

public:
	int IDmodulesCount;
	
	ofRectangle gridSize;
	ofRectangle gridLines[30];//Contient les coordonnées de chaque ligne de la grille (en relatif par rapport au xy de gridSize)
	vector<int> touchOrder; //classe les modules par ordre de dernier touché
	vector<ofPoint> splitableModules;//liste de couples de modules splitables
	vector<ofImage*> previewImages;
	int gridRepresentation[7][7];//not used
	int IDbutton = 0;
	
	int bordureX; 
	int bordureY;
	
	bool isLibraryOpened;
	bool isButtonActioned;
	bool newWallCreated;
	
	vector<Module*> modules;
	vector<Module*> libraryModules;
	vector<Button*> wallButtons;
	int modulesNumber;
	ofxUICanvas *saveWallGUI;
	ofxUITextInput *saveWallTextInput;
	
	ofxXmlSettings modSettings;

	string xmlStructure;
	int lastTagNumber;

        string link2images;
	
	bool drawMedia;//not used --> for preview ?
	
	vector<string> names;//walls names
	
	Wall(){}
	
	void setup(string pathToServer){
		IDmodulesCount = 0;
		modulesNumber = -1;
		isLibraryOpened = false;
		newWallCreated = false;
		
		gridSize.set( (float)(0.09*ofGetWidth()+WIDTH_BUTTONS), 0, (float)(ofGetWidth()-0.09*ofGetWidth()-WIDTH_BUTTONS), (float)(ofGetHeight()-0.15*ofGetHeight()-HEIGHT_BUTTONS));
		gridSetup();
		
		wallButtons.push_back(new Button("library", IDbutton++, 25, ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "LIBRARY", "LIBRARY", 40));
		wallButtons.push_back(new Button("delete", IDbutton++, 25+1.0/2*2.0/5*ofGetWidth(), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "DELETE", "DELETE", 40));
		wallButtons.push_back(new Button("speaker", IDbutton++, 25+2*(1.0/2*2.0/5*ofGetWidth()), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "EXT SPEAKER", "EXT SPEAKER", 40));
		wallButtons.push_back(new Button("save", IDbutton++, ofGetWidth()-25-(1.0/2*2.0/5*ofGetWidth()-10), ofGetHeight() - HEIGHT_BUTTONS*2 - 25, 1.0/2*2.0/5*ofGetWidth()-10, HEIGHT_BUTTONS , WALLCREATION_PAGE, "SAVE", "SAVE", 40));
		
		
		//Create a module of each type and size for the library
		for (int i = 0; i < 4; i++)
			for (int j = 0; j <3; j++)
				libraryModules.push_back(new Module(j, i, -1));
				
		//on part d'une base de walls
		ofFile tempXML, tempXML2;
		ofBuffer dataBuffer; 
		
		tempXML.open(ofToDataPath("walls.xml"), ofFile::ReadWrite, false);
		tempXML.clear();
		tempXML2.open(ofToDataPath("wallspc.xml"), ofFile::ReadWrite, false);
		dataBuffer = tempXML2.readToBuffer();//ofLoadURL(pathToServer + "wallspc.xml").data;
		ofBufferToFile("walls.xml", dataBuffer);
		tempXML.close();
		tempXML2.close();
		//fill the walls names depending on a xml file
		getWallListName();
				
		ofxUIColor cb = ofxUIColor(0, 0, 0, 0); //Background 
		ofxUIColor cb2 = ofxUIColor(255,255,255,255); //BG liste déroulante
		ofxUIColor co = ofxUIColor(255, 0, 0, 255); // ???
		ofxUIColor coh = ofxUIColor(40, 40, 40, 255); //tour quand sélectionné
		ofxUIColor cf = ofxUIColor(40, 40, 40, 255); //texte non sélectionné
		ofxUIColor cfh = ofxUIColor(40, 40, 40, 255); //texte sélectionné + curseur
		ofxUIColor cp = ofxUIColor(255, 255, 255, 255); // ???
		ofxUIColor cpo = ofxUIColor(255, 0, 0, 255);
				
		saveWallGUI = new ofxUICanvas(30, ofGetHeight()/2+20, 2.0/8*ofGetWidth()-20, HEIGHT_BUTTONS);
		saveWallGUI->setFont("open-sansbold-italic.ttf");
		saveWallGUI->setWidgetFontSize(OFX_UI_FONT_LARGE);
		saveWallGUI->setUIColors( cb2, cb, coh, cf, cfh, cp, cpo );
		saveWallTextInput = saveWallGUI->addTextInput("Wall title", "Wall title", -1); 
		saveWallTextInput->setAutoUnfocus(true);
		saveWallTextInput->setAutoClear(true);
		saveWallGUI->setVisible(false);
		
		
		
		
	}
	
	void update(int wallSelected){
		
		//suppression des modules
	    for (size_t i = 0; i<modules.size(); i++){
	    	if (modules[i]->getIsDeleted()){
	    		modules.erase(modules.begin()+i);//delete module 
	    		
	    		/*for (size_t j=i; j<modules.size(); j++){//decrement the ID of module following the one just deleted
	    			modules[j]->setID(modules[j]->getID()-1);
	    		}
	    		IDmodulesCount--;*/
	    		
	    		for (size_t j=0; j<touchOrder.size();j++){//remove the ID from the deleted module from touchOrder
	    			if (touchOrder[j]==i){
	    				touchOrder.erase(touchOrder.begin() + j);
	    				for (size_t k=0; k<touchOrder.size();k++)
	    					if (touchOrder[k] > i) touchOrder[k]--;
	    				break;
	    			}
	    		}
	    	}
	    }
	    
	    
	    for (int i = 0; i< modules.size(); i++){
	    	modules[i]->setIsWellPlaced(true);//init
	    }
	    //Detect collision --> not used
	    /*for (size_t i = 0; i< modules.size(); i++){
	    	for (size_t j = 0; j< modules.size(); j++){
	    		if(j!=i && modules[i]->isCollision(modules[j]) && rankInTouchOrder(i)>rankInTouchOrder(j))
	    			modules[i]->setIsWellPlaced(false);
	    	}
	    }*/
	    //Alone module
	    if (modules.size() > 1){
		    for (size_t i = 0; i< modules.size(); i++){
			    	bool alone=true;
			    	for (size_t j = 0; j< modules.size(); j++)
			    		if (i!=j && !modules[i]->isAlone(modules[j])) alone = false;
			    		
			    	if(alone) modules[i]->setIsWellPlaced(false);
			    	else modules[i]->setIsWellPlaced(true);
		    }
		}
		
		//splitable modules
		splitableModules.clear();
		if (modules.size() > 1){
		    for (size_t i = 0; i< modules.size(); i++){
		    	for (size_t j = i+1; j< modules.size(); j++){
		    		if (modules[i]->getTypeSize()==1 && modules[j]->getTypeSize()==1 && modules[i]->getTypeMat() == modules[j]->getTypeMat() && modules[i]->getTypeMat()!=3 && modules[i]->isSplitable(modules[j]) ){
						splitableModules.push_back(ofPoint(i, j));
					}
		    	}
			}
		}
		
		//sauvegarde du wall
		if ((saveWallGUI->isVisible() && !saveWallTextInput->isClicked())){
			saveWallGUI->setVisible(false);

			if (wallSelected == -1){
				wall2XML(names.size()-1);//save in xml
			} else {
				wall2XML(wallSelected);
			}
			
			getWallListName();//modify names list depending on the new xml
			newWallCreated = true;
		} 
	}

	void updatePreview(string path2images, int wallSelected, string pathToServer){
	  
	  XML2Wall(wallSelected, pathToServer);
	  //getModulesNumber(wallSelected);
	  if (wallSelected != -1){
	    previewImages.clear();
	    for (int i=0; i<modulesNumber; i++){
	      previewImages.push_back(new ofImage());
	      previewImages[i]->loadImage(path2images + ofToString(i+1)+ ".jpg");
	      //previewImages[i]->resize(modules[i]->getSize().x);//, modules[i]->getSize().y);
	      //modules[i]->getPlane().setPosition(modules[i]->getPos().x, modules[i]->getPos().y, 0);
	      cropImage(i);
	    }
	  }
	  
	}

	void cropImage(int i){

	    if ((float)previewImages[i]->getWidth() / modules[i]->getSize().x > (float)previewImages[i]->getHeight() / modules[i]->getSize().y){
	      previewImages[i]->resize((float) previewImages[i]->getWidth() *   modules[i]->getSize().y / previewImages[i]->getHeight() , modules[i]->getSize().y);
	      int cropX = (float) previewImages[i]->getWidth() *   modules[i]->getSize().y / previewImages[i]->getHeight() - modules[i]->getSize().x;
	      previewImages[i]->crop(cropX/2, 0, modules[i]->getSize().x, modules[i]->getSize().y);
	      
	    } else {
	      previewImages[i]->resize(modules[i]->getSize().x, (float) previewImages[i]->getHeight() * modules[i]->getSize().x / previewImages[i]->getWidth());
	      int cropY = (float) previewImages[i]->getHeight() * modules[i]->getSize().x / previewImages[i]->getWidth() - modules[i]->getSize().y;
	      previewImages[i]->crop(0, cropY/2, modules[i]->getSize().x, modules[i]->getSize().y);
	    }
	}
	
	void draw(){
    		
		gridDraw();
		
		for (size_t i=0; i < wallButtons.size(); i++)
			wallButtons[i]->draw();
		for (size_t i = 0; i<modules.size();i++)
    		modules[touchOrder[i]]->draw();
	}

	void drawPreview(){
	  for (int i=0; i<modulesNumber; i++){
	    if (modules[i]->getTypeMat() == 0 || modules[i]->getTypeMat() == 2) {
	      previewImages[i]->draw(modules[i]->getPos().x, modules[i]->getPos().y);
	      
	    } else {
	      modules[i]->draw();
	    }

	    ofPushStyle();
	      ofSetColor(0);
	      ofNoFill();
	      ofSetLineWidth(5);
	      ofRect(modules[i]->getPos().x, modules[i]->getPos().y, modules[i]->getSize().x, modules[i]->getSize().y);
		ofPopStyle();
	      
	      /*previewImages[i]->getTextureReference().bind();
	    modules[i]->getPlane().draw();
	    previewImages[i]->getTextureReference().unbind();*/
	    
	  }
	}
	
	void drawLibrary(){
		for(int i = 0; i < libraryModules.size(); i++){
			if ( libraryModules[i]->getTypeSize() == 2)
				libraryModules[i]->setPos(bordureX +libraryModules[i]->getTypeMat() * 2.5 *1.7*GRID_RATIO, bordureY + 50 + 5 * GRID_RATIO);
			else libraryModules[i]->setPos(bordureX +libraryModules[i]->getTypeMat() * 2.5 *1.7*GRID_RATIO, bordureY + 50 + libraryModules[i]->getTypeSize() *1.5*libraryModules[i]->getTypeSize() * GRID_RATIO);
			libraryModules[i]->draw();
		}
	}
	
	
	//**********************Touch****************************/
	
	void touchDown(int x, int y){
	
		//---- Gestion des modules
		//Trie les modules par ordre de dernière sélection
		for (size_t i=0; i < modules.size(); i++){
			modules[touchOrder[i]]->onTouchDown(x, y);
			if (modules[touchOrder[i]]->getIsTouched()){
				touchOrder.push_back(touchOrder[i]);
				touchOrder.erase(touchOrder.begin()+i);

			}
		}
		//Ne laisse sélectionné que celui du dessus en cas de 2 modules superposés
		for (size_t i=0; i < modules.size(); i++){
			for (int j=0; j<modules.size();j++){
				if(i!=j && modules[j]->getIsTouched() && rankInTouchOrder(j)<rankInTouchOrder(i)){
						modules[i]->setIsTouched(false);
				}
			}
		}
		
		if (isLibraryOpened){//si c'est un module de la library
			for (int i = 0; i < libraryModules.size(); i++){
				if (libraryModules[i]->onTouchDown(x, y)){
					isLibraryOpened = false;
					
					modules.push_back(new Module(libraryModules[i]->getTypeSize(), libraryModules[i]->getTypeMat(), IDmodulesCount++));//on ajoute le module au wall
					modules[(int)modules.size()-1]->isSelected = true;
					touchOrder.push_back(modules.size()-1);
				}
			}
		}	
		
		//---- Gestion des boutons
		for (size_t i=0; i < wallButtons.size(); i++)
				wallButtons[i]->isTouchedDown(x, y);
	}
	
	
	//-----------------------------------------
	void touchUp(int x, int y, int wallSelected){
	
		for (size_t i=0; i < 4; i++){
			if (wallButtons[i]->isTouchedUp(x, y)){
				switch (wallButtons[i]->getID()){
					case 0: //library
						//on ouvre une fenêtre avec tous les modules
						//if ((int)modules.size()-1 < SCREEN_MAX_NUMBER)//si nombre max d'écrans
							isLibraryOpened = true;
						break;
					case 1://delete
						//on suppr le module sélectionné
						if (moduleSelected() != -1)
							modules[moduleSelected()]->setIsDeleted(true);
						break;
					case 2://ext speaker
						//page musique
						break;
					case 3://save
						//TODO: if new wall -> empty else nom du wall
						if (wallSelected!=-1 && wallSelected!=names.size()-1)
							saveWallTextInput->setTextString(names[wallSelected]);
						saveWallGUI->setVisible(true);
						saveWallTextInput->click();
						break;
				}
			}
		}
		
		for (size_t i=0; i < modules.size(); i++) modules[i]->onTouchUp(x, y);

	}
	
	void touchMoved(int x, int y){
		for (size_t i=0; i < modules.size(); i++) modules[i]->onTouchMove(x, y);
	}
	
	
	/****************************Grid**************************/
	
	//calcule toutes les lignes à tracer pour la grille
	void gridSetup(){
	
		// /!\aussi dans module --> faut changer les deux
		bordureX = (((ofGetWidth()-50)/(1.7*GRID_RATIO) - (float)((int)((ofGetWidth()-50)/(1.7*GRID_RATIO))))*1.7*GRID_RATIO + 50) /2; //63;
		bordureY = (((ofGetHeight()-200.0)/GRID_RATIO - (float)((int)((ofGetHeight()-200)/GRID_RATIO)))*GRID_RATIO +25 /*+GRID_RATIO*/) /2;
		
		for (int i = 0; i <17; i++){//verticales
			gridLines[i].set(bordureX + i*(int)(1.7*GRID_RATIO), 50+bordureY-10, bordureX + i*(int)(1.7*GRID_RATIO), ofGetHeight() - 120 - bordureY);
		}
		for (int i = 0; i < 13; i++){//horizontales
			gridLines[i+17].set(bordureX-10, 50+bordureY+i*GRID_RATIO, ofGetWidth() - bordureX+10, 50+bordureY+i*GRID_RATIO);
		}
	}
	
	void gridDraw(){
		for (int i=0; i<30; i++){
			ofPushStyle();
	    	ofSetColor(40);
	    	ofLine(gridLines[i].getX(), gridLines[i].getY(), gridLines[i].getWidth(), gridLines[i].getHeight());
	    	ofPopStyle();
		}
	}
	
	//-------------------------XML--------------------------------------//
	
	void wall2XML(int wallID){
	
	
	//---------------WORK IN PROGRESS : save several walls -----------------------//
		bool wallFound = false;
		if (modSettings.load("walls.xml")){ //si le fichier existe
			int numTagsWALL = modSettings.getNumTags("WALL");
			for (int i = 0; i < numTagsWALL; i++){
	    		modSettings.pushTag("WALL", i);
	    			if (modSettings.getValue("ID", i) == wallID){ //si le wall existe déjà dans le fichier on modifie le wall existant
				    				wallFound = true;

				    				//on remplace les modifs
				    				modSettings.setValue("NAME", saveWallTextInput->getTextString());
				    				modSettings.pushTag("SPLIT");
				    					int numTagsS = modSettings.getNumTags("S");
					    				for (int i=0; i < splitableModules.size(); i++){
					    					if (i < numTagsS){
					    						modSettings.pushTag("S", i);
											    	modSettings.setValue("A", splitableModules[i].x);
											    	modSettings.setValue("B", splitableModules[i].y);
										    	modSettings.popTag();
					    					} else {
					    						modSettings.addTag("S");
										   		modSettings.pushTag("S", i);
										   			modSettings.addValue("A", splitableModules[i].x);
											    	modSettings.addValue("B", splitableModules[i].y);
					    						modSettings.popTag();
					    					}
					    				}
									modSettings.popTag();//("SPLIT")
									
									int numTagsMODULES = modSettings.getNumTags("MODULES");
									for(int i = 0; i<numTagsMODULES; i++){ //on supprime tout
										modSettings.removeTag("MODULES", 0);
									}
									
									for(int i = 0; i<modules.size(); i++){ // on reremplis tout
										    lastTagNumber = modSettings.addTag("MODULES");
										    modSettings.pushTag("MODULES", i);
										    modSettings.setValue("ID", modules[i]->getID(), 0);
										    modSettings.setValue("SIZE", modules[i]->getTypeSize(), 0);
											modSettings.setValue("TYPE", modules[i]->getTypeMat(), 0);
											modSettings.addTag("POS");
											modSettings.pushTag("POS");
											    modSettings.setValue("X", modules[i]->getPos().x, 0);
											    modSettings.setValue("Y", modules[i]->getPos().y, 0);
											modSettings.popTag();
											modSettings.addTag("POSGRID");
											modSettings.pushTag("POSGRID");
											    ofPoint tmpPosGrid = modules[i]->getPosInGrid();
											    modSettings.setValue("X", tmpPosGrid.x, 0);
											    modSettings.setValue("Y", tmpPosGrid.y, 0);
											modSettings.popTag(); 
										    modSettings.popTag();
								    }
	    			}
	    		modSettings.popTag();//("WALL")
	    	}		
		} else {//walls.xml n'existe pas
			ofFile newFile(ofToDataPath("walls.xml"), ofFile::ReadWrite);
			newFile.create();
		}
		
		if (!wallFound){
			//on créé à la suite ou dans le fichier vide
			
			modSettings.addTag("WALL");
		    modSettings.pushTag("WALL", wallID);
		    modSettings.addValue("ID", wallID);
		    modSettings.addValue("NAME", saveWallTextInput->getTextString());
		    modSettings.addTag("SPLIT");
			modSettings.pushTag("SPLIT");
		    for (int i=0; i < splitableModules.size(); i++){
		    	modSettings.addTag("S");
			   	modSettings.pushTag("S");
			    	
			    	modSettings.addValue("A", splitableModules[i].x);
			    	modSettings.addValue("B", splitableModules[i].y);
		    	modSettings.popTag();
		    }
			modSettings.popTag();
			
		    for(int i = 0; i<modules.size(); i++){
		
		        lastTagNumber = modSettings.addTag("MODULES");
		        modSettings.pushTag("MODULES", i);
		        	modSettings.setValue("ID", modules[i]->getID(), 0);
		        	modSettings.setValue("SIZE", modules[i]->getTypeSize(), 0);
			        modSettings.setValue("TYPE", modules[i]->getTypeMat(), 0);
			        modSettings.addTag("POS");
			        modSettings.pushTag("POS");
			        	modSettings.setValue("X", modules[i]->getPos().x, 0);
			        	modSettings.setValue("Y", modules[i]->getPos().y, 0);
			        modSettings.popTag();
			        modSettings.addTag("POSGRID");
			        modSettings.pushTag("POSGRID");
			        	ofPoint tmpPosGrid = modules[i]->getPosInGrid();
			        	modSettings.setValue("X", tmpPosGrid.x, 0);
			        	modSettings.setValue("Y", tmpPosGrid.y, 0);
			        modSettings.popTag(); 
		        modSettings.popTag();
		    }
		    modSettings.popTag();
		}
		modSettings.save("walls.xml");
		
		/*ofFile tempFile;
		tempFile.open("walls.xml", ofFile::ReadWrite, false);
		ofBuffer buff = tempFile.readToBuffer();
		ofLogNotice("walls.xml info") << buff;*/
		
	}
	
	void XML2Wall(int wallID, string pathToServer){
	
	    modules.clear();
	    touchOrder.clear();

		//on enregistre le xml en local
	    /*ofFile tempXML;
	    ofBuffer dataBuffer;
	    
	    tempXML.open(ofToDataPath("temp.xml"), ofFile::ReadWrite, false);
	    dataBuffer = ofLoadURL(pathToServer + "test2.xml").data;
	    ofBufferToFile("temp.xml", dataBuffer);
	    
	    //on charge le xml
	    modSettings.load("temp.xml");
	    tempXML.remove();*/
	    
	    //Fichier externe copié localement à l'initialisation
	    modSettings.load("walls.xml");
		
	    int numTagsWALL = modSettings.getNumTags("WALL");
	    
	    for (int i = 0; i < numTagsWALL; i++){
	    	modSettings.pushTag("WALL", i);
	    	if (modSettings.getValue("ID", i) == wallID){ //Si c'est le wall qu'on veut
		    		modulesNumber = modSettings.getNumTags("MODULES");
		    		for (int j = 0; j < modulesNumber; j++){
		    			modSettings.pushTag("MODULES", j);
		    			
		    				int tmpSize;
		    				tmpSize = modSettings.getValue("SIZE", j);
		    				int tmpType;
		    				tmpType = modSettings.getValue("TYPE", j);
		    				
		    				
		    				ofPoint tmpPos;
		    				modSettings.pushTag("POS");
			        			tmpPos.x = modSettings.getValue("X", j);
			        			tmpPos.y = modSettings.getValue("Y", j);
			        		modSettings.popTag();
			        		
			        		int tmpID;
			        		tmpID = modSettings.getValue("ID", j);
			        		if (IDmodulesCount < tmpID) IDmodulesCount = tmpID+1;
			        		
			        		modules.push_back(new Module(tmpSize, tmpType, tmpID));
		    				modules[j]->setPos(tmpPos.x, tmpPos.y);
		    				touchOrder.push_back(j);
						
		    			modSettings.popTag();//modules
		    		}
	    	}
	    	modSettings.popTag();//wall
	    }
	}

	int getModulesNumber(int selectedWall){


	  if (selectedWall != -1){
	    modSettings.load("walls.xml");
	    modSettings.pushTag("WALL", selectedWall);
	    modulesNumber = modSettings.getNumTags("MODULES");
	    modSettings.popTag();

	    return modulesNumber;
	  }
	  return -1; 
	}
	
	//fill names depending on a xml file and end with "create a wall"
	void getWallListName(){
		
	  modSettings.load("walls.xml");
		
	  names.clear();
	  int numTagsWALL = modSettings.getNumTags("WALL");
	  for (int i = 0; i < numTagsWALL; i++){
	    modSettings.pushTag("WALL", i);
	    names.push_back((string)modSettings.getValue("NAME", ""));
	    modSettings.popTag();
	  }
	  names.push_back("CREATE A NEW WALL");
	}
	    
	//-------------------
	
	int moduleSelected(){
		for (int i = 0; i < modules.size(); i++){
			if(modules[i]->isSelected) 
				return i;
		}
		return -1;
	}
	
	
	//Argument : ID of the module
	//return : order rank 
	int rankInTouchOrder(int value){
		for (size_t i = 0; i<touchOrder.size();i++){
			if (touchOrder[i]==value) return i;
		}
		return -1;
	}
	
	
	
};
