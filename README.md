

			length
	
		|-------------------------------> x
		|
		|
		|
width	|
		|
       y|


nc -lnup 12345

installer javafx :

wget https://download2.gluonhq.com/openjfx/21/openjfx-21_linux-x64_bin-sdk.zip
unzip openjfx-21_linux-x64_bin-sdk.zip
mv javafx-sdk-21 ~/javafx-sdk

compiler :

javac --module-path ~/javafx-sdk/lib --add-modules javafx.controls,javafx.fxml Poisson.java AquariumApp.java Client.java

executer :

java --module-path ~/javafx-sdk/lib --add-modules javafx.controls,javafx.fxml Client 0.0.0.0 100