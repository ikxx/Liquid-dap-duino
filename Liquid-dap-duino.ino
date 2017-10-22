#include <LiquidCrystal4.h>
//#include <SoftwareSerial.h>

//SoftwareSerial Serial1(3, 2);


LiquidCrystal lcd(11, 10,   9,  12,  8,  7,  6,  5);
//                rs, rw, en1, en2, d0, d1, d2, d3


//fix these if using different LCD
byte maxCols = 40;
byte maxRows = 4;

int posStart, posEnd, posNext, pos;

char playerId[18], title[40], artist[40], album[40], type[5];
float samplerate, samplesize;
byte volume, repeat, mode;
char inChar;

boolean changedValue;

String buff;


String centered(String text)
{
  text.trim();
  int len = text.length();

  if (len > maxCols)
  {
    return text.substring(0,  maxCols - 4) + "...";
  }
  len = (maxCols - len) / 2;
  text.reserve(maxCols);

  for (int i = 0; i < len; i++)
  {
    text = " " + text + " ";
  }
  if (len % 2)
  {
    text = text + " ";
  }
  return text;
}

void printLines(String *lines, byte size, bool cls)
{
  if(cls)
  {
    lcd.clear();
  }
  
  for (int i = 0; i < size; i++)
  {
    lcd.setCursor(0, i);
    lcd.print(centered(lines[i]));
  }
}


void setup()
{
  delay(1000);

  lcd.begin(maxCols, maxRows);
  Serial.begin(9600);
  
  String initLines[] = { "Liquid-dap-duino", "DAPHILE 2 DDDAC1794S", "", "waiting for player..." };
  printLines(initLines,4,true);

  delay(1000);
  findPlayerId();

  while (String(playerId).indexOf(":") < 0)
  {
    delay(5000);
    findPlayerId();
  }

  lcd.setCursor(0, 2);
  lcd.print(playerId);

  Serial.println(String(playerId) + " status - 1 tags:oTIalG subscribe:5");


  //5a:bd:60:86:ed:fb status - 1 tags:oTIalG subscribe:0

  /*
    5a:bd:60:86:ed:fb status - 1 tags:oTIalG subscribe:0
    player_name:WaveIO USB Audio 2.0
    player_connected:1
    player_ip:127.0.0.1:53222
    power:1
    signalstrength:0
    mode:play
    time:337.414047037125
    rate:1
    duration:347.4
    can_seek:1
    mixer volume:100
    playlist repeat:2
    playlist shuffle:0
    playlist mode:off
    seq_no:0
    playlist_cur_index:1
    playlist_timestamp:1480528920.84089
    playlist_tracks:10
    digital_volume_control:0
    playlist index:1
    id:12685
    title:Ne si go prodavaj Koljo...
    type:flc
    samplerate:44100
    samplesize:16
    artist:Vlatko Stefanovski %26 Miroslav Tadic
    album:Treta Majka
    genres:Instrumental
  */
}

void findPlayerId()
{
  //empty buffer...
  String strPlayerId;
  Serial.readString();

  Serial.println("player id 0 ?");
  while (Serial.available() < 39)
  {
    delay(5);
  }
  delay(5);
  Serial.readString();

  // found some jibberish stuff in first response... dump it...

  Serial.println("player id 0 ?");
  while (Serial.available() < 39)
  {
    delay(5);
  }
  strPlayerId = Serial.readString();

  strPlayerId.replace("%3A", ":");
  strPlayerId.replace("player id 0 ", "");
  strPlayerId.replace("\r", "");
  strPlayerId.replace("\n", "");
  strPlayerId.trim();
  strPlayerId.toCharArray(playerId, 18);
}


String secondsToHMS(const uint32_t seconds)
{
  uint32_t s, m, h;
  uint32_t t = seconds;
  s = t % 60;
  t = (t - s) / 60;
  m = t % 60;
  t = (t - m) / 60;
  h = t;
  return String(String(h) + ":" + String(m) + ":" + String(s));
}


void loop()
//After creating a setup() function, which initializes and sets the initial values, the loop() function does precisely what its name suggests, and loops consecutively, allowing your program to change and respond. Use it to actively control the Arduino board.
{

volume = getValue("mixer volume", volume);
  getValue("title", 40, title);

  getValue("type", 5, type);
  samplerate = getValue("samplerate", samplerate);
  samplesize = getValue("samplesize", samplesize);
  getValue("artist", 40, artist);
  getValue("album", 40, album);





  if (String("flc") == String(type))
  {
    String("FLAC").toCharArray(type, 5);
  }
  else if (String("ape") == String(type))
  {
    String("APE").toCharArray(type, 5);
  }
  else if (String("mp3") == String(type))
  {
    String("MP3").toCharArray(type, 5);
  }
  else if (String("alc") == String(type))
  {
    String("ALAC").toCharArray(type, 5);
  }


  //displayInfo[3] = type + " " + samplerate + "bits " + samplesize + "Hz";


  if (changedValue)
  {
    char sRate[9], sSize[9];
    dtostrf(samplerate / 1000, 1, 1, sRate);
    dtostrf(samplesize, 1, 0, sSize);
    String initLines[] = { artist, album, title, String(type) + " " + String(sRate) + "kHz " + String(sSize) + "bit"};
    printLines(initLines,4,false);

    changedValue = false;
  }



}


void getValue(String sFind, int len, char tmpArr[])
{
  posStart = buff.indexOf(sFind);
  posNext = buff.indexOf(" ", posStart);
  posEnd = buff.indexOf("\n", posStart);
  if (posStart > -1 && (posNext > posStart || posEnd > posStart))
  {
    if (posNext > posStart)
      pos = posNext;
    else
      pos = posEnd;
    String X = buff.substring(posStart + sFind.length() + 3, pos);
    X.replace("%3A", ":");
    X.replace("%20", " ");
    X.replace("%26", "&");
    X.replace("%22", """");
    X.replace("%2C", ",");
    X.toCharArray(tmpArr, len);
    buff = buff.substring(pos);
    changedValue = true;
  }
}
float getValue(String sFind, float val)
{
  posStart = buff.indexOf(sFind);
  posNext = buff.indexOf(" ", posStart);
  posEnd = buff.indexOf("\n", posStart);
  if (posStart > -1 && (posNext > posStart || posEnd > posStart))
  {
    if (posNext > posStart)
      pos = posNext;
    else
      pos = posEnd;
    String X = buff.substring(posStart + sFind.length() + 3, pos);
    X.trim();
    val = X.toFloat();
    buff = buff.substring(pos);
    changedValue = true;
  }
  return val;
}

byte getValue(String sFind, byte val)
{
  posStart = buff.indexOf(sFind);
  posNext = buff.indexOf(" ", posStart);
  posEnd = buff.indexOf("\n", posStart);
  if (posStart > -1 && (posNext > posStart || posEnd > posStart))
  {
    if (posNext > posStart)
      pos = posNext;
    else
      pos = posEnd;
    String X = buff.substring(posStart + sFind.length() + 3, pos);
    X.trim();
    val = X.toInt();
    buff = buff.substring(pos);
    changedValue = true;
  }
  return val;
}

/*
    SerialEvent occurs whenever a new data comes in the
   hardware serial RX.  This routine is run between each
   time loop() runs, so using delay inside loop can delay
   response.  Multiple bytes of data may be available.
   */
void serialEvent()
{
  pullSerial();

  if (false)
  {

    lcd.clear();
    lcd.home();
    /* lcd.print(
       "len:" + String(lmsStatus.length())
       + " title:" + String(lmsStatus.indexOf("title"))
       + " n end:" + String(lmsStatus.indexOf("\n"))
       + " r end:" + String(lmsStatus.indexOf("\r")));*/
  }

}

void pullSerial()
{
  pinMode(13, OUTPUT);
  while (Serial.available() > 0)
  {
    digitalWrite(13, HIGH);
    inChar = (char)Serial.read();
    buff += inChar;
  }
  digitalWrite(13, LOW);
}

/*
  9600
  -parenb cs8     NO  generate parity bit in output and expect parity bit in input
  -cstopb         use two stop bits per character (one with '-')
  -crtscts        NO  enable RTS/CTS handshaking
  -ixon           NO  enable XON/XOFF flow control
  -ixoff          NO  enable sending of start/stop characters
  ignbrk          YES ignore break characters
  -brkint         NO  breaks cause an interrupt signal
  -imaxbel        NO  beep and do not flush a full input buffer on a character
  -opost          NO  postprocess output
  -onlcr          NO  translate newline to carriage return-newline
  -isig           NO  enable interrupt, quit, and suspend special characters
  -icanon         NO  enable erase, kill, werase, and rprnt special characters
  -iexten         NO  enable non-POSIX special characters
  -echo           NO  echo input characters
  -echoe          NO  echo erase characters as backspace-space-backspace
  -echok          NO  echo a newline after a kill character
  -echoctl        NO  echo control characters in hat notation ('^c')
  -echoke         NO  kill all line by obeying the echoctl and echok settings
  noflsh          YES disable flushing after interrupt and quit special characters
*/


