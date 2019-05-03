/* Este codigo es para leer datos especificos de la tarjeta SD conectada a la placa.
 * Lo que se lee en este caso es el valor del contador para determinar cual fue el ultimo valor guadado. De esta manera
 * retoma ese valor para continuar con el contaje y no tener que empezar de 0. Lo que se cuenta son las lecturas 
 * hechas por un sensor.
 */
#include <SPI.h>
#include <SD.h>

String lectura;
char caracter;
int  cont;
File archivo;

void setup() {
  pinMode(7,INPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(10)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  bool nuevoFile = false; // Variable para saber si se ha creado un nuevo archivo "datalog.txt", se hace
                          // verdadero solo si se elimana el archivo.
  int pin = digitalRead(7); // Cuando se activa un boton conectado en el pin 7 se elimina el archivo "datalog.txt".
  if (pin == HIGH){
  if (SD.exists("datalog.txt")) {        // Determina si existe el archivo
    SD.remove("datalog.txt");           // Se elimina el archivo
    Serial.println("ARCHIVO BORRADO");  // Se muestra el mensaje
    nuevoFile = true;
  } else {
    Serial.println("EL ARCHIVO NO EXISTE");// En caso que no exista el archivo, se muestra el mensaje.
  }
  }
  
  archivo = SD.open("datalog.txt",FILE_READ);// Abrimos el archivo para ver cual es el valor del contador, y lo guardamos. 
  if (archivo){ // Si el archivo esta abierto, continua.
    int c = archivo.size(); // Nos posicionamos en el ultimo valor.
    int i = c-1; // Nos posicionamos en el penultimo valor, porque suele dar error si empesamos directamente del ultimo.
    while (archivo.available()){ // Este while siempre es necesario cuando se hace operaciones de lectura en la SD.
      while(1){
        archivo.seek(i);// Se mantiene ubicado en el penultimo valor.
        caracter = archivo.read(); // Lee cada valor en el que este ubicado la funcion de arriba.
        //Serial.print(caracter);
        i--; // Se empieza a leer de derecha a izquierda. De esta forma se obtiene todo lo que esta en la ultima fila.
        if (caracter=='>')break; // La lectura continua hasta encontrar el simbolo '>'. Cuando lo encuentra termina.
        }
        while(1){ // Una vez que se obtiene toda la ultima fila se busca el valor del contador que esta en esa fila.
          archivo.seek(i+2); // Se empieza a leer despues de 2 lugares para evitar leer el simbolo '>'.
          caracter = archivo.read();
          //Serial.print(caracter);
          i++; // Ahora se lee de izq a der.
          if (caracter=='|'){ // La lectura continua hasta encontar el simbolo '|'.
            break;}
            else{
              lectura+=caracter; // Cada lectura es guardada en una variable String.
              }
             }
              break;
            }
           }
           archivo.close(); // una vez encontrado el valor cerramos el archivo.
           cont = lectura.toInt(); // La variable String es convertida a una variable entera. 
           Serial.println();
           Serial.print("El contador habia quedado en: ");
           Serial.println(cont);
           
           if (nuevoFile==true){ // Para crear el encabezado del archivo nuevamente, si es que se ha borrado.
            archivo = SD.open("datalog.txt",FILE_WRITE);
            if (archivo) {
              archivo.println(" Num|Lecturas ");
              archivo.close();
              }
            }
            
              for ( int k=0; k<11 ; k++){ // Se hacen las mediciones con el sensor.
              float sensor = analogRead(A0)*5.0/1024;
                cont++;
                archivo = SD.open("datalog.txt",FILE_WRITE);
                if (archivo) {
                  archivo.print(">");
                  archivo.print(" ");
                  archivo.print(cont);
                  archivo.print("|");
                  archivo.println(sensor);
                  archivo.close();}
                  else {
                    Serial.println("error opening datalog.txt");
                    }
              }
                      // Se muestra los datos almacenados en la SD.
                    archivo = SD.open("datalog.txt",FILE_READ); 
                    if (archivo) {
                      Serial.println("Lo que hay en el archivo datalog.txt es:");
                      while (archivo.available()) {
                        Serial.write(archivo.read());
                       
                        }} else {
                          Serial.println("error opening datalog.txt");
                          }
              archivo.close();
}

void loop() { }          
