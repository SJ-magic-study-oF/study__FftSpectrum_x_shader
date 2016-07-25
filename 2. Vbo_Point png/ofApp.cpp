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
	
	image.loadImage( "light.png" );
	
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
	shader.setGeometryInputType( GL_POINTS );
	shader.setGeometryOutputType( GL_TRIANGLE_STRIP );
	
	// Maximal possible number of output vertices
	// " * 1/2 - 1" しないと動作しなかった.(実験結果:don't know why...)
	printf("%d\n", shader.getGeometryMaxOutputCount());
	// shader.setGeometryOutputCount( 128 );
	shader.setGeometryOutputCount( shader.getGeometryMaxOutputCount() / 2 - 1 );

	//Load shader
	shader.load( "shaderVert.c", "shaderFrag.c", "shaderGeom.c" );
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
		gui.add(texture_size.setup("texture size", 10.0, 3.0, 100.0));
		gui.add(texture_space.setup("texture space", 1.1, 0.3, 3.0));
	}
	{
		float range = 1000;
		ofVec3f pos_init = ofVec3f(-100, 0, 0);
		ofVec3f pos_min = ofVec3f(-range, -range, -range);
		ofVec3f pos_max = ofVec3f(range, range, range);
		
		gui.add(line0_from.setup("0 : line from", pos_init, pos_min, pos_max));
	}
	{
		float range = 1000;
		ofVec3f pos_init = ofVec3f(-100, -100, 0);
		ofVec3f pos_min = ofVec3f(-range, -range, -range);
		ofVec3f pos_max = ofVec3f(range, range, range);
		
		gui.add(line0_to.setup("0 : line to", pos_init, pos_min, pos_max));
	}
	{
		float range = 1000;
		ofVec3f pos_init = ofVec3f(100, 0, 0);
		ofVec3f pos_min = ofVec3f(-range, -range, -range);
		ofVec3f pos_max = ofVec3f(range, range, range);
		
		gui.add(line1_from.setup("1 : line from", pos_init, pos_min, pos_max));
	}
	{
		float range = 1000;
		ofVec3f pos_init = ofVec3f(100, -100, 0);
		ofVec3f pos_min = ofVec3f(-range, -range, -range);
		ofVec3f pos_max = ofVec3f(range, range, range);
		
		gui.add(line1_to.setup("1 : line to", pos_init, pos_min, pos_max));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
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
	shader.setUniform4f("BaseColor", ofVec4f(1.0, 1.0, 1.0, 1.0));
	
	vbo_drawPoint(line0_from);
	vbo_drawPoint(line0_to);
	vbo_drawPoint(line1_from);
	vbo_drawPoint(line1_to);
	/*
	vbo_drawLine(line0_from, line0_to);
	vbo_drawLine(line1_from, line1_to);
	*/
	
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
