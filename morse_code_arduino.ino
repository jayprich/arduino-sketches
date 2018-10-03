void setup();
void loop();

// morse code lookup 0 not allowed, 1 blank for space, 2..63 binary representation with leading 1
unsigned char m[] = "_ ETIANMSURWDKGOHVF_L?PJBXCYZQ__54_3___2&_+____16=/__>(_7___8_90";

void setup() {
  Serial.begin(19200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  static unsigned char n=0, p=0, q=0, b[1024]; // counter and circular buffer for background transmission
  register unsigned char i, j, k;

  if(Serial.available()>0){
    j = Serial.read();                   // get character to be processed
    if(j!=10){                           // ignore Line Feeds
      for(i=1;m[i]&&(m[i]!=j);i++){};    // i seeks j in m
      for(k=64;(k>1)&&(!(k&i));k>>=1){}; // k ignores leading zero bits
      for(k>>=1;k;k>>=1){
        Serial.print(".-"[0<(k&i)]);     // write morse to terminal
        q = 1023&(q+1);                  // advance circular pointer BEFORE when writing
        b[q] = 2 + 3 * (0<(k&i));        // store 2 "dot" or 5 "dash" in buffer
      }
      if(i==1)Serial.print('/');
      Serial.print(' ');
      q = 1023&(q+1);                    // advance circular pointer BEFORE when writing
      b[q] = 3;                          // store 3 pause
    }
  } else {
    if( n==0 && p==q ){
      Serial.println();                  // terminal newline once transmission finishes
      n--;
    }else{
      if(n==0 && 1==(3&(p-q)))Serial.print("H"); // terminal heartbeat every 4 units
    };
    if(p!=q){
      if(n<=b[p]){                       // stay on according to buffered value
        if(n==0 && b[p]!=3){
          digitalWrite(LED_BUILTIN, HIGH);
        }
        n++;
      }else{                             // n = b[p]
        digitalWrite(LED_BUILTIN, LOW);
        p = 1023&(p+1);                  // advance circular pointer AFTER on reading
        n = 0;
      }
    }
    delay(80);                           // determines speed of morse code, pause 3+1 "    " dot 2+1 "== " dash 5+1 "===== " units
  }
}
