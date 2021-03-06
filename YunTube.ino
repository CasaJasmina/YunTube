/*

  With this sketch you will
  be able to read from a webpage
  hosted on the Arduino/Genuino Yùn101.
  If a valid YouTube URL is detected
  the board downloads it and plays it.

  Compatible boards:
  Arduino/Genuino Yùn
  Arduino/Genuino Yùn101
  Arduino/Genuino YùnShield

  For a step-by-step tutorial and other material visit:
  https://create.arduino.cc/projecthub/Arduino_Genuino/yuntube-00c1a6

  Created 24 May 2016
  by Tommaso Laterza

  This example code is in the public domain.

*/

#include <Process.h>
#include <Mailbox.h>
#include <Bridge.h>
#include <EEPROM.h>
#include <BridgeSSLClient.h>
BridgeSSLClient sslClient;
Process download;
Process play;

// in order to avoid the overwriting of different songs everytime you reset the board, store the names in the flash memory
int addr = 0; /** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int songNumber = EEPROM.read(addr); 
String old_URL = "";
String link;


void setup() {
  Serial.begin(9600);

  if(songNumber==255){ // needed only the very first time you power the board
    songNumber=0;
  }

  Serial.println("Starting bridge");
  Bridge.begin();
  Mailbox.begin();
  Serial.println("Ready!");

}
void loop() {

  String message;

  //Read messages from the website
  if (Mailbox.messageAvailable()) {
    while (Mailbox.messageAvailable()) {
      Mailbox.readMessage(message);
      Serial.println(message);
      link = message;
    }
  }


  if (!link.equals(old_URL) && link != "") { //check if it is valid link
    link.trim();
    old_URL = link;

    if (play.running()) { // If another song is beeing played stop it
      play.runShellCommand(F("killall madplay"));
    }

    //DOWNLOAD
    Serial.println(F("Downloading video ..."));
    char commandToExecute[160];
    sprintf(commandToExecute, "curl --location https://www.youtubeinmp3.com/fetch/?video=%s > /mnt/sda1/song%d.mp3", link.c_str(), songNumber);
    download.runShellCommand(commandToExecute);
    Serial.print(F("Download completed! Song saved as:\tsong"));
    Serial.print(songNumber);
    Serial.println(F(".mp3"));

    Serial.print(F("Playing:\t song"));
    Serial.print(songNumber);
    Serial.println(F(".mp3"));

    //PLAY THE SONG
    sprintf(commandToExecute, "madplay /mnt/sda1/song%d.mp3", songNumber);
    play.runShellCommandAsynchronously(commandToExecute);
    songNumber++;
    EEPROM.write(addr, songNumber);
  }
  delay(1000);
}
