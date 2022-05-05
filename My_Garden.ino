#include <Wire.h>
#include <RTClib.h> // se busca las librerias para usar el reloj

RTC_DS3231 rtc; // creamos un objeto 
const int humedad_suelo = A0; // a0 lo configuro para humedad suelo 
const int RELE1 = 3;//rele1 es el que activa la bomba de agua
const int RELE2 = 4;//rele2 es el que activa la ILUMINACION
const int NIVEL = 5;// nivel de agua en el tanque

void setup() {
 Serial.begin(9600); //arrancamos el puerto serie a 9600
 
 //CONFIGURACION DE ENTRADAS Y SALIDAS:
 pinMode(RELE1, OUTPUT); //salida
 pinMode(humedad_suelo,INPUT);//entrada
 pinMode(RELE2, OUTPUT);
 pinMode(NIVEL,INPUT);//entrada

  // ahora creamos un algoritmo para verificar la comunicacion con el reloj
  if (! rtc.begin()){
    // con el signo "!" invertimos el valor. De manera de que si es falso, entramos al ciclo
    Serial.println("Modulo RTC no encontrado! ");
    while (1); // 1 equivale a true. Por lo que con este while, hacemos un bucle infinito. Deteniendo el programa si no tenemos comunicacion. 
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));// si se logro la comunicacion. Osea paso el while, aqui se le pasa el horario de la PC al reloj.
}


void loop() {
 int lectura = analogRead (A0); //leemos los datos del sensor de humedad con esta variable
 int lecturaPorcentaje = map (lectura, 1023,0, 0, 100); // mapeamos los valores, para convertirlos en porcentajes. Los primeros dos numeros son del rango que da nuestro sensor, el segundo los nuevos valores.
 DateTime fecha = rtc.now(); // aqui hacemos que la variable fecha tenga los datos de fecha, mes, año, hora, minutos y segundos
 int hora = fecha.hour(); //guardamos las numeros del horario en variables mas faciles
 int minuto = fecha.minute(); 
 int ValorNivel = digitalRead(NIVEL);// Leo el valor del nivel del tanque, y lo ponemos en la variable ValorNivel

 //IMPRIMIMOS NUSTROS VALORES
 Serial.println("==============================="); 
 Serial.println("Hora: ");Serial.print(hora); Serial.print(" : ");Serial.println(minuto); 
 Serial.println("humedad de suelo: ");Serial.print(lecturaPorcentaje); Serial.println(" %"); 
 delay(4000);

if (ValorNivel == 0){
  Serial.println("ESTADO DE TANQUE: sin agua");
}
else{
  Serial.println("ESTADO DE TANQUE: con agua");
}

 //RIEGO:
if (ValorNivel == 1){ 
  Serial.println("Ciclo riego:"); 
  if ( lecturaPorcentaje < 20 && lecturaPorcentaje > 5){
    Serial.println(" Riego Tipo 1: ...");
     digitalWrite(RELE1, HIGH); // encendemos la bomba DC
     delay(5000); // esperamos aque el motor bombee 10sec
     digitalWrite(RELE1, LOW); // apagamos la bomba DC
     delay(5000);
   }
   else if (lecturaPorcentaje > 20 && lecturaPorcentaje < 40){
     Serial.println(" Riego Tipo 2: ...");
     digitalWrite(RELE1, HIGH); // encendemos la bomba DC
     delay(2000); // esperamos aque el motor bombee 5 sec
     digitalWrite(RELE1, LOW); // apagamos la bomba DC
     delay(5000);
     }
}
  //ILUMINACION: 
  Serial.println("Estado de Iluminacion:");
  if (fecha.hour() >= 07 && fecha.hour() <= 20){
    digitalWrite(RELE2,HIGH);
    Serial.println("ENCENDIDO...");
  }
  else {
    digitalWrite(RELE2,LOW);
    Serial.println("APAGADDO...");
  }
 Serial.println("==============================="); 
 delay(5000);
  }
