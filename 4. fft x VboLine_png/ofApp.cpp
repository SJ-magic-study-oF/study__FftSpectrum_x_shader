#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	
	// ofEnableDepthTest(); // 注意!!!! 2d imageで、αが効かなくなる.
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	image.loadImage( "light_gray.png" );
	
	setup_gui();
	
	/********************
	********************/
	init_vbo();
	
	/********************
	setGeometryInputType(), setGeometryOutputType()に指定可能なtypeは決まっている。
		http://openframeworks.cc/documentation/gl/ofShader/#show_setGeometryOutputType
		
	入力は、単に１入力プリミティブあたりに何個の頂点を送るか、だけと考えて良い。
		http://miffysora.wikidot.com/geometry-shader
		
	並べた順にshaderに渡される。
	
	outputは、「typeと順番」で描画内容が変わってくるので注意してoutputする。
	inputの時に、混乱なきよう、順序を考慮しておくのが良い。
		http://openframeworks.cc/documentation/gl/ofVbo/
	
	shader.setGeometryOutputCount()で設定できるCountは、それほど大きくない(1024とか、そんな所)。
	今回のケースでは、Lineを渡すと、始点から終点まで、textureを繰り返し描画(4点 x texture枚数)。
	n本のLineを描画するのに、Vboへ全ての点を設定して一気に描画したくなるが、すぐCountがmaxを超えてしまう。
	つまり、n本のLineは、都度 描画コマンドを発行すること。
	********************/
	//Enable detailed logging of openFrameworks messages in console
	ofSetLogLevel( OF_LOG_VERBOSE );

	//We must to specify geometry shader parameters before loading shader
	shader.setGeometryInputType( GL_LINES );
	shader.setGeometryOutputType( GL_TRIANGLE_STRIP );
	
	// Maximal possible number of output vertices
	// " * 1/2 - 1" しないと動作しなかった.(実験結果:don't know why...)
	printf("%d\n", shader.getGeometryMaxOutputCount());
	// shader.setGeometryOutputCount( 128 );
	shader.setGeometryOutputCount( shader.getGeometryMaxOutputCount() / 2 - 1 );

	//Load shader
	shader.load( "shaderVert.c", "shaderFrag.c", "shaderGeom.c" );


	/********************
	********************/
	sound.load( "surface.wav" );	
	sound.setLoop( true );
	sound.play();
	// sound.setVolume(0.0);

	//Set spectrum values to 0
	for (int i=0; i<N; i++) {
		spectrum[i] = 0.0f;
	}
}

/******************************
******************************/
void ofApp::init_vbo()
{
	for(int i = 0; i < NUM_VERTS; i++){
		Color[i].set(1.0, 1.0, 1.0, 1.0);
		Verts[i].set(ofVec3f(0.0, 0.0, 0.0));
	}
	Vbo.setVertexData(Verts, NUM_VERTS, GL_DYNAMIC_DRAW);
	Vbo.setColorData(Color, NUM_VERTS, GL_DYNAMIC_DRAW);
}

/******************************
******************************/
void ofApp::vbo_drawLine(ofVec3f from, ofVec3f to)
{
	Verts[0].set(from);
	Verts[1].set(to);
	
	Vbo.updateVertexData(Verts, NUM_VERTS);
	Vbo.draw(GL_LINES, 0, NUM_VERTS);
}

/******************************
******************************/
void ofApp::vbo_drawPoint(ofVec3f p)
{
	Verts[0].set(p);
	
	Vbo.updateVertexData(Verts, NUM_VERTS);
	Vbo.draw(GL_POINTS, 0, NUM_VERTS);
}

/******************************
******************************/
void ofApp::setup_gui()
{
	/********************
	********************/
	gui.setup();
	
	/********************
	********************/
	{	
		gui.add(texture_size.setup("texture size", 3.0, 2.0, 100.0));
		gui.add(texture_space.setup("indicator space", 1.1, 0.3, 3.0));
		
		gui.add(x_ofs.setup("x offset", 0.0, -500, 500));
		gui.add(x_space.setup("x space", 10, 5, 50));

		gui.add(y_ofs.setup("y offset", 0.0, -500, 500));
		
		gui.add(volume.setup("volume", 1.0, 0, 1.0));
	}
	{
		ofVec4f initColor = ofVec4f(0, 0, 1.0, 1.0);
		ofVec4f minColor = ofVec4f(0, 0, 0, 0);
		ofVec4f maxColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
		
		gui.add(texture_color.setup("texture color", initColor, minColor, maxColor));
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	sound.setVolume(volume);
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
	/********************
	<注意>
	glDisable(GL_DEPTH_TEST);
	しないと、2d 描画にてαが効かなくなる.
	********************/
	glDisable(GL_DEPTH_TEST);
	ofBackground(0, 0, 0, 0);
	
	/********************
	********************/
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	shader.begin();
	
	// ofSetColor(255, 255, 255, 255);
	shader.setUniform1f( "TextureSize", texture_size ); // 描画サイズ
	shader.setUniform1f( "texture_space", texture_space );
	shader.setUniform1f( "OriginalTextureImageSize", image.getWidth() );
	shader.setUniformTexture( "texture", image.getTextureReference(), 1 );
	shader.setUniform4f("BaseColor", texture_color);
	
	for(int i = 0; i < N; i++){
		vbo_drawLine( ofVec3f(x_ofs + i * x_space, y_ofs, 0), ofVec3f(x_ofs + i * x_space, y_ofs - 100 * spectrum[i], 0) );
	}
	
	shader.end();
	ofPopMatrix();
	
	/********************
	********************/
	glDisable(GL_DEPTH_TEST);
	// glDisable(GL_CULL_FACE);
	gui.draw();
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
