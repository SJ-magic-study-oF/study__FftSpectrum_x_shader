#include "ofApp.h"

// const int N = 256;		//Number of bands in spectrum
const int N = 128;
float spectrum[ N ];	//Smoothed spectrum values


//--------------------------------------------------------------
void ofApp::setup(){
	//Set up sound sample
	sound.load( "surface.wav" );	
	sound.setLoop( true );
	sound.play();

	//Set spectrum values to 0
	for (int i=0; i<N; i++) {
		spectrum[i] = 0.0f;
	}
}

//--------------------------------------------------------------
void ofApp::update(){	
	//Update sound engine
	ofSoundUpdate();	

	//Get current spectrum with N bands
	float *val = ofSoundGetSpectrum( N );
	//We should not release memory of val,
	//because it is managed by sound engine

	//Update our smoothed spectrum,
	//by slowly decreasing its values and getting maximum with val
	//So we will have slowly falling peaks in spectrum
	for ( int i=0; i<N; i++ ) {
		spectrum[i] *= 0.97;	//Slow decreasing
		spectrum[i] = max( spectrum[i], val[i] );
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground( 0, 0, 0);

	//Draw background rect for spectrum
	ofSetColor( 230, 230, 230 );
	ofFill();
	ofRect( 10, 700, N * 5, -100 );

	//Draw spectrum
	ofSetColor( 0, 0, 0 );
	for (int i=0; i<N; i++) {
		ofSetColor( 255, 0, 120);
		
		ofRect( 10 + i * 5, 700, 3, -spectrum[i] * 100 );
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 't':
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
