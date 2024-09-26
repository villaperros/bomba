/****************************************************************************************************

Creado por Joan Villaperros, este código controla un módulo LCD key Shield y muestra Bombas en español
haciendo uso de tildes y de una melodía costarricense para llamar a las bombas. 

Creado Sep 26 2024
******************************************************************************************************/


#include <LiquidCrystal.h>
#include "pitches.h"

// LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;

LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// Matrices para caracteres acentuados
byte a_acento[8] = { B00010, B00100, B01110, B00001, B01111, B10001, B01111, B00000 };
byte e_acento[8] = { B00010, B00100, B01110, B10001, B11111, B10000, B01110, B00000 };
byte i_acento[8] = { B00010, B00100, B00000, B01100, B00100, B00100, B01110, B00000 };
byte o_acento[8] = { B00010, B00100, B01110, B10001, B10001, B10001, B01110, B00000 };
byte u_acento[8] = { B00010, B00100, B10001, B10001, B10001, B10011, B01101, B00000 };
byte n_acento[8] = { B00000, B00111, B00101, B00100, B00100, B11100, B11100, B00000 };
byte m_acento[8] = { B00111, B00101, B00100, B00100, B11100, B11100, B00000, B00000 };
byte b_acento[8] = { B00000, B00000, B00111, B00101, B00100, B00100, B11100, B11100 };

// Notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_C5, NOTE_C5, 0, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_G4, 0,
  NOTE_B4, NOTE_B4, NOTE_B4, 0, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4, 0
};

// Note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  2, 2, 2, 2, 8, 8, 8, 8, 8, 8, 2, 2,
  2, 2, 2, 2, 8, 8, 8, 8, 8, 8, 2, 2
};

// Poema con códigos para caracteres personalizados á = \x01 é = \x02 í = \x03 ó = \x04 ú = \x05
const char* bomba[] = {

  "Qu\x02 linda       ",
  "es Costa Rica,  ",
  "la quiero con   ",
  "el coraz\x04n     ",
  "pero s\x02 que m\x01s",
  "me gustar(\x3)a     ",
  "sin tanta       ",
  "corrupci\x04n!!!!",


  "El trapiche     ",
  "de mi abuelo    ",
  "ya qued\x04",
  "por el suelo",
  "construyeron    ",
  "algo altote     ",
  "meti\x02ndonos ",
  "el elote!!!!",

  "Qu\x02 saicos",
  "esos carepichas",
  "que roban del",
  "fondo p(\x05)blico",
  "Cerdos!            ",
  "asquerosos!      ",
  "ah\x03 nos cuentan",
  "en el juicio",

  "Costa Rica",
  "es puro chorizo,",
  "y todos ven",
  "para otro lado",
  "aqu\x03 estamos",
  "programados",
  "nos jalamos",
  "como al ganado!!!",

  "Al mae",
  "Pura Vida",
  "ya no le",
  "dan fiado",
  "sus hijos ",
  "crecen flacos",
  "el mae est\x01 sin",
  "brete y arruinao",

  "Yo s\x02 que no es",
  "diciembre. ",
  "Pero qu\x02 rico",
  "un tamal!       ",
  "Costa Rica est\x01s",
  "muy violenta!   ",
  "C\x04mo safamos de",
  "este mal?       ",

  "Ayer pas\x02       ",
  "por tu casa     ",
  "y me tiraste    ",
  "una chancleta,   ",
  "por poco        ",
  "me la como    ",
  "deseando que    ",
  "fuera chuleta :(",

  "Ya se oyen      ",
  "las marimbas    ",
  "y las bombetas  ",
  "SONAR           ",
  "es porque ya    ",
  "comenz\x04 el turno",
  "y los pol\x03ticos",
  "A ROBAR   :(    ",

  "Ayer pas\x02       ",
  "por tu casa     ",
  "y me puse       ",
  "a llorar,       ",
  "lloraron        ",
  "tambi\x02n las aves",
  "viendo          ",
  "\x01rboles talar   ",

  "Qu\x02 rico       ",
  "el atolillo     ",
  "nos quieren     ",
  "hacer pensar !   ",
  "pero tanto      ",
  "con el dedo     ",
  "nos hace        ",
  "vomitar.        ",

  "Cuatro y cuatro ",
  "son ocho        ",
  "y he contado    ",
  "muchos m\x01s  ",
  "son camiones    ",
  "con madera      ",
  "que del bosque  ",
  "han de robar    ",

};

int numSonetos = sizeof(bomba) / sizeof(bomba[0]);

unsigned long tiempoAnterior = 0;      // Almacena el tiempo del último cambio
const unsigned long intervalo = 3000;  // 2.2 segundos en milisegundos
int indiceActual = 0;                  // Índice del soneto actual


void pieza() {


  lcd.clear();

  lcd.setCursor(0, 0);
  moverTextoIzquierda("Guiiippp        ", 80);

  lcd.setCursor(7, 1);
  lcd.write(byte(6));
  lcd.write(byte(7));
  lcd.write(byte(8));

  // Iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 24; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(3, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 0.59;
    delay(pauseBetweenNotes);

    noTone(3);  // Stop the tone
    delay(pauseBetweenNotes);
  }
}

void moverTextoIzquierda(String mensaje, int velocidad) {
  int longitud = mensaje.length();

  // Desplaza el mensaje hasta que se haya mostrado completamente
  for (int i = 0; i < longitud + 36; i++) {
    lcd.clear();  // Limpia el LCD
    // Muestra el texto desde la posición adecuada
    lcd.setCursor(16 - i, 0);  // Ajusta la posición del cursor
    lcd.print(mensaje);        // Imprime el mensaje en la LCD
    delay(velocidad);          // Espera el tiempo de la velocidad deseada
  }
}
void setup() {
  lcd.begin(16, 2);  // Configurar LCD con 16 columnas y 2 filas
  pieza();
  // Crear caracteres acentuados personalizados
  lcd.createChar(1, a_acento);  // 'á'
  lcd.createChar(2, e_acento);  // 'é'
  lcd.createChar(3, i_acento);  // 'í'
  lcd.createChar(4, o_acento);  // 'ó'
  lcd.createChar(5, u_acento);  // 'ú'
  lcd.createChar(6, n_acento);  // 'ú'
  lcd.createChar(7, m_acento);  // 'ú'
  lcd.createChar(8, b_acento);  // 'ú'

  // Mostrar el primer par de líneas del poema
  lcd.clear();
  mostrarSonetoConTildes(indiceActual);  // Mostrar las primeras dos líneas
}

void loop() {
  unsigned long tiempoActual = millis();  // Obtener el tiempo actual

  // Verifica si han pasado 2.2 segundos (2200 milisegundos)
  if (tiempoActual - tiempoAnterior >= intervalo) {
    // Actualiza el tiempo
    tiempoAnterior = tiempoActual;

    // Avanza al siguiente par de líneas del poema
    indiceActual += 2;  // Avanzar al siguiente par de líneas
    if (indiceActual >= numSonetos) {
      indiceActual = 0;  // Volver al principio cuando llegue al final
    }

    // Mostrar el siguiente par de líneas
    lcd.clear();                           // Limpiar la pantalla
    mostrarSonetoConTildes(indiceActual);  // Mostrar las nuevas líneas

    int sonetoCounter = 0;  // Counter for how many sonetos have been displayed
    while (true) {

      for (int i = 1; i < 5; i++) {
        lcd.clear();
        delay(300);
        lcd.setCursor(0, 0);
        lcd.print("BOOOMBAAAAA");
        delay(300);
      }


      for (int i = 0; i < numSonetos; i += 2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(bomba[i]);

        if (i + 1 < numSonetos) {  // Check for the next line
          lcd.setCursor(0, 1);
          lcd.print(bomba[i + 1]);
        }
        delay(intervalo);
        sonetoCounter += 2;  // Two sonetos shown
        if (sonetoCounter == 8 | sonetoCounter == 16 | sonetoCounter == 24) {
          for (int i = 1; i < 5; i++) {

            lcd.clear();
            delay(300);
            lcd.setCursor(0, 1);
            lcd.print(" UYUYUY  BAJURA");
            delay(300);
          }
        }



        if (sonetoCounter >= 32) {
          for (int i = 1; i < 5; i++) {

            lcd.clear();
            delay(300);
            lcd.setCursor(0, 1);
            lcd.print(" UYUYUY  BAJURA");
            delay(300);
          }
          lcd.clear();
          pieza();            // Play melody
          sonetoCounter = 0;  // Reset counter after the melody
        }
      }
    }
  }
}




// Función para mostrar una línea del poema en el LCD
void mostrarSonetoConTildes(int indice) {
  lcd.setCursor(0, 0);                    // Fila 0
  mostrarLineaConAcentos(bomba[indice]);  // Imprime la línea superior

  if (indice + 1 < numSonetos) {
    lcd.setCursor(0, 1);                        // Fila 1
    mostrarLineaConAcentos(bomba[indice + 1]);  // Imprime la línea inferior
  }
}

// Función para imprimir una línea y gestionar caracteres acentuados
void mostrarLineaConAcentos(const char* linea) {
  for (int i = 0; i < 16 && linea[i] != '\0'; i++) {
    if (linea[i] == '\x00') {
      lcd.write((uint8_t)0);  // Mostrar 'á'
    } else if (linea[i] == '\x01') {
      lcd.write((uint8_t)1);  // Mostrar 'é'
    } else if (linea[i] == '\x02') {
      lcd.write((uint8_t)2);  // Mostrar 'í'
    } else if (linea[i] == '\x03') {
      lcd.write((uint8_t)3);  // Mostrar 'ó'
    } else if (linea[i] == '\x04') {
      lcd.write((uint8_t)4);  // Mostrar 'ú'
    } else {
      lcd.print(linea[i]);  // Imprimir cualquier otro carácter normal
    }
  }
}
