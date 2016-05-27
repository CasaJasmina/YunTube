#include <Process.h>
#include <Mailbox.h>
#include <Bridge.h>
#include <BridgeSSLClient.h>
BridgeSSLClient sslClient;
Process download;
Process play;

String old_URL = "";
int songNumber = 0;
String link;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Starting bridge");
  Bridge.begin();
  Mailbox.begin();
  Serial.println("Ready!");

}
void loop() {

  String message;

  if (Mailbox.messageAvailable()) {
    // read all the messages present in the queue
    while (Mailbox.messageAvailable()) {
      Mailbox.readMessage(message);
      Serial.println(message);
      link = message;
    }}



    if (!link.equals(old_URL) && link != "") {
      link.trim();
      old_URL = link;

      if (play.running()) {
        play.runShellCommand(F("killall madplay"));
      }

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

      sprintf(commandToExecute, "madplay /tmp/song%d.mp3", songNumber);
      play.runShellCommandAsynchronously(commandToExecute);
      songNumber++;
    }
    delay(1000);
  }
