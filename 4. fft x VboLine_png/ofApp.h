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
		
		ofxFloatSlider x_ofs;
		ofxFloatSlider x_space;
		
		ofxFloatSlider y_ofs;
		
		ofxFloatSlider volume;
		
		ofxVec4Slider texture_color;
		
		
		/********************
		********************/
		enum{
			N = 128,
		};
		float spectrum[ N ];
		ofSoundPlayer sound;
		
		/********************
		********************/
		enum{
			NUM_VERTS = 2, // per dot ... 1回のshader commandで渡す個数.
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
		void vbo_drawPoint(ofVec3f p);
		
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
