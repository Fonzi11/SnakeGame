  /* Ruler 1         2         3         4         5         6         7        */
  /*****************************  snake.c ***************************************/
  /****************************** Arduino ***************************************/
  /* +-----------------------------------+                                      */
  /*|                                   |                                       */
  /*|              LCD (I2C)            |                                       */
  /*|                                   |                                       */
  /*|  SDA <--------------------------> A4 (analog pin)   (Data)                */
  /*|  SCL <--------------------------> A5 (analog pin)   (Clock)               */
  /*|  VCC <--------------------------> VCC               (Power)               */
  /*|  GND <--------------------------> GND               (Ground)              */
  /*|                                   |                                       */
  /*+-----------------------------------+                                       */
  /*|                                   |                                       */
  /*|              Joystick 1           |                                       */
  /*|                                   |                                       */
  /*|  VRx <--------------------------> A0 (analog pin)  (X-axis)               */
  /*|  VRy <--------------------------> A1 (analog pin)  (Y-axis)               */
  /*|  SW  <--------------------------> 2                 (Button)              */
  /*|                                   |                                       */
  /*+-----------------------------------+                                       */
  /*|                                   |                                       */
  /*|              Joystick 2           |                                       */
  /*|                                   |                                       */
  /*|  VRx <--------------------------> A2 (analog pin)  (X-axis)               */
  /*|  VRy <--------------------------> A3 (analog pin)  (Y-axis)               */
  /*|  SW  <--------------------------> 4                 (Button)              */
  /*|                                   |                                       */
  /*+-----------------------------------+                                       */
  /*|                                   |                                       */
  /*|              Speaker              |                                       */
  /*|                                   |                                       */
  /*|  +  <--------------------------> 3                 (Speaker Signal)       */
  /*|  -  <--------------------------> GND               (Ground)               */
  /*|                                   |                                       */
  /*+-----------------------------------+                                       */
  /*|                                   |                                       */
  /*|            Amplifier              |                                       */
  /*|                                   |                                       */
  /*|  In <--------------------------> Speaker +          (Amplifier Input)     */
  /*|  GND <--------------------------> Speaker -          (Amplifier Ground)   */
  /*|  VCC <--------------------------> VCC               (Amplifier Power)     */
  /*|                                   |                                       */
  /*+-----------------------------------+                                       */

  /*Description:                                                                */
  /*- LCD (I2C): Displays the game information, snake, and score.               */
  /*- Joystick 1 & 2: Control the snake's direction using X and Y axes, and     */ 
  /* button for user input.                                                     */
  /*- Speaker: Produces sound effects and melodies.                             */
  /* - Amplifier: Boosts the audio signal from the Arduino for better sound     */
  /* quality.                                                                   */

  /*The code implements a Snake game on the Arduino, where the snake is         */
  /* controlled by                                                              */ 
  /*Joystick 1, Joystick 2 serves as an second snake input, and the LCD displays*/
  /* the game state and score. Sound effects and melodies are played through    */
  /*the speaker via an amplifier.                                               */
  /*LCD managment was adapted from  Luis Cabrera Benito 14 de febrero de 2020   */
  /*Music was adapted by Ing. Ismael Fonseca from original JAVA game SNAKE III  */
  /*This game music was in my first phone, I remember it with great fondness    */
  /*POO uses were adapted by Ing. Julián Forero from olikraus                   */


/**************************** Symbolic  Constants *****************************/
#include <LiquidCrystal_I2C.h>

#define ANCHURA_LCD 20
#define ALTURA_LCD 4
#define DIRECCION_LCD 0x27

#define ALTURA_TABLERO 8
#define ANCHURA_TABLERO 19
#define MAXIMA_LONGITUD_SERPIENTE (ALTURA_TABLERO * ANCHURA_TABLERO)

#define DIRECCION_DERECHA 0
#define DIRECCION_IZQUIERDA 1
#define DIRECCION_ARRIBA 2
#define DIRECCION_ABAJO 3

#define pinX  A0
#define pinY  A1
#define pinX2 A2
#define pinY2 A3

#define SPEAKER 3
#define NOTE_C4 261.63
#define NOTE_E4 329.63
#define NOTE_G4 392.00
#define NOTE_B4 493.88
#define NOTE_D4 293.66
/*unsigned long tiempoAnterior = 0;
unsigned long intervalo = 200;*/
int melodyIndex=0;
int velocidadInicial = 200; 
int velocidadMinima = 50;   
int velocidadActual = velocidadInicial;

LiquidCrystal_I2C pantalla(DIRECCION_LCD, ANCHURA_LCD, ALTURA_LCD);
int escenario[ALTURA_TABLERO][ANCHURA_TABLERO];
enum gameState {
  setupGame,
  pausedGame,
  inGame,
  gameOver,
};

gameState state = setupGame;
class PedazoSerpiente {
  public:
    int x, y;

    PedazoSerpiente(int a, int b) {
      x = a; y = b;
    }

    PedazoSerpiente() {
    }
};
PedazoSerpiente serpiente2[MAXIMA_LONGITUD_SERPIENTE];
int longitudSerpiente2 = 0;
int direccion2 = DIRECCION_DERECHA;

PedazoSerpiente serpiente[MAXIMA_LONGITUD_SERPIENTE];
int longitudSerpiente = 0;
int direccion = DIRECCION_DERECHA;

int comidaX, comidaY;
int puntaje = 0;

int obtenerDireccion() {
  int valorX = analogRead(pinX),
      valorY = analogRead(pinY);
  if (valorX > 1000) {
    return DIRECCION_ABAJO;
  } else if (valorX < 200) {
    return DIRECCION_ARRIBA;
  }

  if (valorY > 1000) {
    return DIRECCION_IZQUIERDA;
  } else if (valorY < 200) {
    return DIRECCION_DERECHA;
  }

  return -1;
}
int obtenerDireccion2() {
  int valorX2 = analogRead(pinX2);
  int valorY2 = analogRead(pinY2);
  if (valorX2 > 1000) {
    return DIRECCION_ABAJO;
  } else if (valorX2 < 200) {
    return DIRECCION_ARRIBA;
  }
  if (valorY2 > 1000) {
    return DIRECCION_IZQUIERDA;
  } else if (valorY2 < 200) {
    return DIRECCION_DERECHA;
  }
  return -1;

}
void cambiarDireccion2(int nuevaDireccion2) {
.  if (
    nuevaDireccion != DIRECCION_DERECHA
    && nuevaDireccion != DIRECCION_IZQUIERDA
    && nuevaDireccion != DIRECCION_ARRIBA
    && nuevaDireccion != DIRECCION_ABAJO
  ) {
    return;
  }
  if (
    (nuevaDireccion == DIRECCION_DERECHA || nuevaDireccion == DIRECCION_IZQUIERDA)
    && (direccion == DIRECCION_DERECHA || direccion == DIRECCION_IZQUIERDA)
  ) return;

  if (
    (nuevaDireccion == DIRECCION_ARRIBA || nuevaDireccion == DIRECCION_ABAJO)
    && (direccion == DIRECCION_ARRIBA || direccion == DIRECCION_ABAJO)
  ) return;

  direccion = nuevaDireccion;
}
void cambiarDireccion(int nuevaDireccion) {
  if (
    nuevaDireccion != DIRECCION_DERECHA
    && nuevaDireccion != DIRECCION_IZQUIERDA
    && nuevaDireccion != DIRECCION_ARRIBA
    && nuevaDireccion != DIRECCION_ABAJO
  ) {
    return;
  }
  if (
    (nuevaDireccion == DIRECCION_DERECHA || nuevaDireccion == DIRECCION_IZQUIERDA)
    && (direccion == DIRECCION_DERECHA || direccion == DIRECCION_IZQUIERDA)
  ) return;

  if (
    (nuevaDireccion == DIRECCION_ARRIBA || nuevaDireccion == DIRECCION_ABAJO)
    && (direccion == DIRECCION_ARRIBA || direccion == DIRECCION_ABAJO)
  ) return;

  direccion = nuevaDireccion;
}

void agregarPedazo(int x, int y) {
  if (longitudSerpiente >= MAXIMA_LONGITUD_SERPIENTE) return;
  if (x + 1 >= ANCHURA_TABLERO || x < 0) return;
  if (y + 1 >= ALTURA_TABLERO || y < 0) return;
  serpiente[longitudSerpiente] = PedazoSerpiente(x, y);
  longitudSerpiente++;
}

void moverSerpiente() {
  // Obtener la posición siguiente de la cabeza
  int direccion2 = obtenerDireccion2();
  cambiarDireccion2(direccion2);
  int nuevaX = serpiente[0].x;
  int nuevaY = serpiente[0].y;

  switch (direccion) {
    case DIRECCION_DERECHA:
      nuevaX++;
      break;
    case DIRECCION_IZQUIERDA:
      nuevaX--;
      break;
    case DIRECCION_ARRIBA:
      nuevaY--;
      break;
    case DIRECCION_ABAJO:
      nuevaY++;
      break;
  }

  // Verificar colisión con los bordes
  if (nuevaX < 0 || nuevaX >= ANCHURA_TABLERO || nuevaY < 0 || nuevaY >= ALTURA_TABLERO) {
    state = gameOver;
    return;
  }
  // Mover la serpiente
  for (int i = longitudSerpiente - 1; i >= 1; i--) {
    serpiente[i].x = serpiente[i - 1].x;
    serpiente[i].y = serpiente[i - 1].y;
  }

  serpiente[0].x = nuevaX;
  serpiente[0].y = nuevaY;

  // Verificar colisión con el cuerpo
  for (int i = 1; i < longitudSerpiente; i++) {
    if (serpiente[0].x == serpiente[i].x && serpiente[0].y == serpiente[i].y) {
      state = gameOver;
      return;
    }
  }
}
void aumentarVelocidad() {
  // Ajusta la velocidad basándote en el puntaje
  if (puntaje > 0 && puntaje % 2 == 0 && velocidadActual > velocidadMinima) {
    velocidadActual-=10;
  }
}
void colocarSerpienteEnMatriz() {
  
  for (int i = 0; i < longitudSerpiente; i++) {
    int x = serpiente[i].y;
    int y = serpiente[i].x;
    escenario[x][y] = 1;
  }
}

void randomizarComida() {
  comidaX = random(0, ANCHURA_TABLERO);
  comidaY = random(0, ALTURA_TABLERO);
}

void acomodarComida() {
  escenario[comidaY][comidaX] = 2;
}

void setup() {
  setup_xd();
}
void setup_xd(){
  pantalla.clear();
  randomSeed(analogRead(0));
  pantalla.init();
  pantalla.backlight();
  crearCaracterPersonalizado();
  for (int y = 0; y < ALTURA_TABLERO; y++) {
    for (int x = 0; x < ANCHURA_TABLERO; x++) {
      escenario[y][x] = 0;
    }
  }

  for (int i = 0; i < 3; i++) {
    agregarPedazo(1, i);
  }
    for (int i = 0; i < 3; i++) {
    agregarPedazo(1, i);
    agregarPedazo(ANCHURA_TABLERO - 2, i); // Inicializa la segunda serpiente en el lado opuesto
  }

  randomizarComida();
  dibujarMatriz();
  pantalla.setCursor(0, 0);
  pantalla.print("Snake");
  pantalla.setCursor(0, 1);
  pantalla.print("By Ismael Fonseca & Julian Forero");
  delay(2000);
  pantalla.clear();
  state = inGame;
}

void drawGameOver() {
  pantalla.setCursor(0, 0);
  pantalla.print("Game Over!");
  pantalla.setCursor(0, 1);
  pantalla.print("SCORE ");
  pantalla.print(puntaje);
  playGameOverSound();
  delay(2000);
  longitudSerpiente = 1; // Reiniciar la longitud de la serpiente
  puntaje = 0; // Reiniciar el puntaje
  limpiarMatriz(); // Limpiar la matriz
  state = setupGame;
}
void playGameOverSound() {
  // Frecuencias de las notas para el sonido de Game Over
  int melody[] = {
    659, 0, 659, 0, 659, 0, 523, 0, 659, 0, 784, 0, 392, 0, 523, 0,
    392, 0, 329, 0, 440, 0, 523, 0, 329, 0, 659, 0, 784, 0, 880, 0,
    987, 0, 659, 0, 784, 0, 880, 0, 987, 0, 1046, 0, 523, 0, 659, 0,
    784, 0, 392, 0, 523, 0, 392, 0, 329, 0, 440, 0, 523, 0
  };
  int noteDurations[] = {
    500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100,
    500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100,
    500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100, 500, 100,
    500, 100, 500, 100, 500, 100, 500, 100, 500, 100
  };
    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    if (melody[i] == 0) {
      noTone(SPEAKER); // Pausa si la frecuencia es 0
    } else {
      tone(SPEAKER, melody[i], noteDurations[i]); // Toca la nota
      delay(10); // Pequeña pausa entre las notas
    }
  }
}
void playSnakeMusic() {
  // Frecuencias de las notas para la música de Snake
  int melody[] = {
    NOTE_C4, NOTE_E4,NOTE_G4, NOTE_B4 , NOTE_D4
  };

  // Duraciones de las notas
  int noteDurations[] = {
    500, 500, 500, 500, 500
  };
  static unsigned long lastMelodyTime = 0;
  unsigned long currentMillis = millis();

  if (currentMillis - lastMelodyTime >= noteDurations[melodyIndex]) {
    if (melody[melodyIndex] != 0) {
      tone(SPEAKER, melody[melodyIndex], noteDurations[melodyIndex]);
      delay(noteDurations[melodyIndex] + 10);
    } else {
      noTone(SPEAKER);
      delay(noteDurations + 10);
    }

    lastMelodyTime = currentMillis;
    melodyIndex++;

    if (melodyIndex == sizeof(melody) / sizeof(melody[0])) {
      melodyIndex = 0; // Reinicia la melodía al llegar al final
    }
  }
}
void dibujarPuntaje() {
  pantalla.setCursor(19, 0);
  pantalla.print("S");
  pantalla.setCursor(19, 1);
  pantalla.print(puntaje);
}

void dibujarMatriz() {
  pantalla.clear();
  for (int y = 0; y < ALTURA_TABLERO; y++) {
    for (int x = 0; x < ANCHURA_TABLERO; x++) {
      if (escenario[y][x] == 1) {
        pantalla.setCursor(x, y);
        pantalla.write(byte(1));
      } else if(escenario[y][x] == 2){
        pantalla.setCursor(x, y);
        pantalla.write(byte(2));  // Carácter personalizado de la comida
      }
    }
  }
}

void crearCaracterPersonalizado() {
  byte figura[8] = {
   B00000,
   B00000,
   B00001,
   B01110,
   B11011,
   B01110,
   B00000,
   B00000
  };
  pantalla.createChar(1, figura);
  byte comida[8] = {
   B00000,
   B01110,
   B11011,
   B11111,
   B11011,
   B11111,
   B01110,
   B00000
  };
  pantalla.createChar(2, comida);
}

void limpiarMatriz() {
  for (int y = 0; y < ALTURA_TABLERO; y++) {
    for (int x = 0; x < ANCHURA_TABLERO; x++) {
      escenario[y][x] = 0;
    }
  }
}

bool colisionaConComida() {

  return serpiente[0].x == comidaX && serpiente[0].y == comidaY;
}
void loop() {
  switch (state) {
    case setupGame:
      setup_xd();
      break;
    case inGame:
      //unsigned long tiempoActual = millis();
      
      limpiarMatriz();
      cambiarDireccion(obtenerDireccion());
      moverSerpiente();
      colocarSerpienteEnMatriz();
      acomodarComida();
      dibujarMatriz();
      if (colisionaConComida()) {
        int frecuencia = 1000; // Hz
        tone(SPEAKER, frecuencia);
        delay(100); 
        noTone(SPEAKER);
        puntaje++;
        randomizarComida();
        agregarPedazo(0, 0);
      }
        /*if (intervalo > 50) {
          intervalo -= 10;  
      }*/
      dibujarPuntaje();
      delay(velocidadActual);
      //tiempoAnterior = tiempoActual;
      //playSnakeMusic();
      break;
    case gameOver:

      drawGameOver();
      state=setupGame;
      break;
  }
}


