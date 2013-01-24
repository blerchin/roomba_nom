// See the full tutorial at http://www.ladyada.net/learn/sensors/ir.html

const boolean DEBUG = false;

const int buttonPin = 12;
const int hvoltSwitchPin = 8;
const int IRledPin =  9;    // LED connected to digital pin 9
const int statusLedPin = 10;
  
const int IRlen = 16;

const int IRpwr[] = {
// ON, OFF (in 10's of microseconds)
	278, 94,
	90, 276,
	90, 280,
	86, 276,
	270, 94,
	92, 276,
	270, 94,
	90, 1962
  };

const int IRclean[] = {
        278, 92,
	92, 276,
	90, 276,
	88, 278,
	270, 94,
	90, 276,
	90, 276,
	90, 1938 
  };
 
const int IRspot[] = {
        278, 92,
	94, 276,
	88, 276,
	90, 276,
	90, 276,
	270, 94,
	90, 278,
	88, 1962
   };
 
 unsigned long lastOn = 0;
 unsigned long lastOff = 0;
 boolean isOn = false;
 boolean statusLedIsOn = false;
 const int statusLedInterval = 3000;
 const int statusLedDuration = 500;
 unsigned long runDuration = 300000; //5 minutes
 long statusLedLastChange = 0;
 long currentMillis = 0;

void setup(){ 
  if( DEBUG) {
   Serial.begin(9600);
   Serial.print("run Duration:");
   Serial.print(runDuration);
   Serial.print("\n");
  }
  
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH); //pull-up
  
  pinMode(statusLedPin, OUTPUT);
  digitalWrite(statusLedPin, LOW);
  
  pinMode(IRledPin, OUTPUT);      
  digitalWrite(IRledPin, LOW);

  pinMode(hvoltSwitchPin, OUTPUT);
  digitalWrite(hvoltSwitchPin, LOW);
}

void loop(){
  currentMillis = millis();
    if( DEBUG ){
     //Serial.println("flashing LED");
     Serial.print(lastOn);
     Serial.print("\t");
     Serial.print(currentMillis);
     Serial.print("\t");
     Serial.print(currentMillis - lastOn);
     Serial.print("\n");
  }
  
   if( !isOn && digitalRead( buttonPin) == LOW ) {
    turnOn();
// if it has been 5 mins since end of cycle, send docking signal
  } else if (!isOn && lastOff != 0 && ( (currentMillis - lastOff ) > ( runDuration )) ){
    if(DEBUG){
       Serial.println("callling dock()"); 
    }
    dock();
// if it is running and 5 mins have elapsed since start, stop system
  } else if ( isOn && lastOn != 0 && ( (currentMillis - lastOn ) > ( runDuration )) ){
    if(DEBUG){
       Serial.println("calling turnOff()"); 
    }

    turnOff(); 
// if it is running and the button is pushed, stop system
  } else if ( isOn && digitalRead( buttonPin ) == LOW ){
    turnOff();
  }
  
   tickLed();
}
      
    
void turnOn() {
      if(DEBUG){
      Serial.print("button state is: ");
      Serial.print( digitalRead(buttonPin) );
      Serial.print("\n");
      Serial.println("turning system on");
    }
    
     digitalWrite(statusLedPin, HIGH);
//turn on power to Raspi and printer
     digitalWrite(hvoltSwitchPin, HIGH);  
//save current time so

        sendRoombaCode(IRpwr, IRlen, 10);
        delay(2000);
        sendRoombaCode(IRclean, IRlen, 10);
        delay(2*1000); 
    lastOn = millis();    
    isOn = true;
}

void turnOff(){
  //spot*n followed by clean*2 seems to work as the dock command
  //pressing dock during a clean cycle seems to effectively pause it
        
        isOn = false;
        digitalWrite(statusLedPin, LOW);
        
        if(DEBUG)
          Serial.println("turning system off");
          
        sendRoombaCode(IRspot, IRlen, 10);
        sendRoombaCode(IRclean, IRlen, 2);
        delay(1000);
        sendRoombaCode(IRpwr, IRlen, 10);
        delay(1000);
        
        digitalWrite(hvoltSwitchPin, LOW);
        
        lastOn = 0;
        lastOff = millis();
}

void dock(){
  //while paused, dock command starts docking routine
        if(DEBUG)
          Serial.println("Sending dock signal");
  
        sendRoombaCode(IRpwr, IRlen, 10);
        sendRoombaCode(IRspot, IRlen, 10);
        sendRoombaCode(IRclean, IRlen, 2);
        lastOff = 0;
        
  
}

void tickLed(){
  currentMillis = millis(); 
  
   if( !isOn && !statusLedIsOn && ( currentMillis - statusLedLastChange > statusLedInterval) ) {
     if( DEBUG)
       Serial.println("LED ON");    
     statusLedIsOn = true;
     digitalWrite(statusLedPin, HIGH);
     statusLedLastChange = currentMillis;

   } else if( !isOn && statusLedIsOn && (currentMillis - statusLedLastChange > statusLedDuration ) ) {
     statusLedIsOn = false;
     Serial.println("LED OFF");
     digitalWrite(statusLedPin, LOW);
     statusLedLastChange = currentMillis;
   }
     
}


// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(9);         // hang out for 10 microseconds, you can also change this to 9 if its not working
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(9);         // hang out for 10 microseconds, you can also change this to 9 if its not working
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}


void sendRoombaCode(const int IRcode[], const int len, int times) {

  for( int j = 0; j<times; j++){
   for( int i = 0; i<len; i++ ){
     if(i%2 == 0) {
      pulseIR(IRcode[i]*10);
     } else {
      delayMicroseconds(IRcode[i]*10);
     }
   }
  }
}

