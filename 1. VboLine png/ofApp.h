/************************************************************
************************************************************/


/************************************************************
************************************************************/
#pragma once

#include "ofMain.h"
#include "ofxGui.h"

/************************************************************
************************************************************/
class ofApp : public ofBaseApp{
	private:
		/****************************************
		param
		****************************************/
		/********************
		********************/
		ofxPanel gui;
		
		ofxFloatSlider texture_size;
		ofxFloatSlider texture_space;
		
		ofxVec3Slider line0_from;
		ofxVec3Slider line0_to;

		ofxVec3Slider line1_from;
		ofxVec3Slider line1_to;
		
		/********************
		********************/
		enum{
			NUM_VERTS = 2, // per line ... 1回のshader commandで渡す個数.
		};
		ofVbo Vbo;
		ofVec3f Verts[NUM_VERTS];
		ofFloatColor Color[NUM_VERTS];

		ofShader shader;	//Shader
		ofImage image;
		
		
		/****************************************
		****************************************/
		void setup_gui();
		void init_vbo();
		void vbo_drawLine(ofVec3f from, ofVec3f to);
		
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
