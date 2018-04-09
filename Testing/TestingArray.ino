void setup() {

  Serial.begin(9600);

  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);

}

void loop(){

if(digitalRead(12)==LOW){
  Serial.println("Pin12 working");
}
if(digitalRead(11)==LOW){
  Serial.println("Pin11 working");
}
if(digitalRead(10)==LOW){
  Serial.println("Pin10 working");
}
if(digitalRead(9)==LOW){
  Serial.println("Pin9 working");
}
if(digitalRead(8)==LOW){
  Serial.println("Pin8 working");
}

}

