#study__fft_x_shader

##環境
*	OS X El Capitan(10.11.6)
*	Xcode : 7.2
*	oF : 0.9.0

##add on
ofxGui  
ofxXmlSettings

##Contents
fft連動に使用する表現のstudy.

###VboLine Png
Lineを描画すると、png imageが展開されてインジケータ表現となる.  
Gui Control.

###Vbo_Point png
pointを描画すると、png imageが展開されて、そのサイズでインジケータ表現となる.  
Gui Control.

###fft spectrum
mastering oFより。  
06-Sound/06-DancingCloud.  
下記のWarningが出るが、現状対策方法が不明  
WARNING:  140: This application, or a library it uses, is using the deprecated Carbon Component Manager for hosting Audio Units.  

###fft x VboLine_png
fft結果を上記VboLine x Pngのshaderを使ってspectrum表示。


##note






