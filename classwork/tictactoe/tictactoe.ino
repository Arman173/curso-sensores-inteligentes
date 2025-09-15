/*
  Integrantes de equipo:
    Armando Haziel Canul Cen
    Julian Adrian Viana Palomo
    Astrid Maribel Yah Vazquez
    Alan Roberto Nava Carpio
  Author: Armando Canul
  Tic Tac Toe
*/
#define R 'r'
#define B 'b'
#define NONE 'n'

/***** ESTRUCTURAS *****/
struct Cell {
  uint blueLed = 0;
  uint redLed  = 0;
  char state = NONE;
};

/***** VARIABLES PARA EL HARDWARE DEL JUEGO *****/
uint N = 18; // numero total de leds
uint M = 3;  // dimension de la matriz cuadrada (Grid)
Cell Grid[3][3] = {
  // b, r,  state,   b, r,  state,   b, r,  state
  {{ 2, 18, NONE}, { 4, 19, NONE}, { 5, 21, NONE}},
  {{12, 22, NONE}, {13, 23, NONE}, {14, 25, NONE}},
  {{15, 26, NONE}, {33, 27, NONE}, {17, 32, NONE}}
};
uint leds[] = {2, 4, 5, 12, 13, 14, 15, 33, 17,    // 2 - 17 azules (1re jugador)
              18, 19, 21, 22, 23, 25, 26, 27, 32}; // 18 - 32 rojos (2do jugador)

/***** VARIABLES DEL JUEGO *****/
bool win = false;
int count = N/2; // numero de celdas disponibles
bool player_one_turn = true;        // NOTA: el jugador uno es el azul
int row, column; // el # de fila y columna

// funcion para apagar/prender todos los leds
void turn_all(int state) {
  for (int i = 0; i < N; i++) {
    digitalWrite(leds[i], state);
  }
}

// configuramos todo para un nuevo juego
void setNewGame() {
  win = false;
  player_one_turn = true;
  count = N/2;

  // asignamos a todos los leds, un estado nulo
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      setNewState(i, j, NONE);
    }
  }
  // turn_all(HIGH);

  Serial.println("Nuevo juego Iniciado...");
}

void flashing(int pin, int times = 3, int milis = 250) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, LOW);
    delay(milis);
    digitalWrite(pin, HIGH);
    delay(milis);
  }
}

void setNewState(uint r, uint c, char new_state) {
  if (r >= M || c >= M) {
    Serial.println("Rango fila o columna excedido...");
    return;
  }
  Grid[r][c].state = new_state;
  int bL = Grid[r][c].blueLed;
  int rL = Grid[r][c].redLed;
  switch (new_state) {
    case NONE: digitalWrite(bL, LOW); digitalWrite(rL, LOW); break;
    case B:    digitalWrite(bL, HIGH); digitalWrite(rL, LOW); break;
    case R:    digitalWrite(bL, LOW); digitalWrite(rL, HIGH); break;
    default: Serial.println("No existe ese estado...");
  }
}

char is_a_winner() {
  // Revisar filas
  for (int i = 0; i < M; i++) {
    if (Grid[i][0].state != NONE &&
        Grid[i][0].state == Grid[i][1].state &&
        Grid[i][1].state == Grid[i][2].state) {
      return Grid[i][0].state; // devuelve B o R
    }
  }

  // Revisar columnas
  for (int j = 0; j < M; j++) {
    if (Grid[0][j].state != NONE &&
        Grid[0][j].state == Grid[1][j].state &&
        Grid[1][j].state == Grid[2][j].state) {
      return Grid[0][j].state;
    }
  }

  // Revisar diagonal principal
  if (Grid[0][0].state != NONE &&
      Grid[0][0].state == Grid[1][1].state &&
      Grid[1][1].state == Grid[2][2].state) {
    return Grid[0][0].state;
  }

  // Revisar diagonal secundaria
  if (Grid[0][2].state != NONE &&
      Grid[0][2].state == Grid[1][1].state &&
      Grid[1][1].state == Grid[2][0].state) {
    return Grid[0][2].state;
  }

  // Nadie ha ganado
  return NONE;
}

void set_play(int r, int c) {
  // int pinLed = player_one_turn ? Grid[r][c].blueLed:Grid[r][c].redLed;
  if (Grid[r][c].state != NONE) {
    // ya hay una jugada en esa celda
    int ledUsed = Grid[r][c].state == B ? Grid[r][c].blueLed:Grid[r][c].redLed;
    flashing(ledUsed, 5, 100);
    return;
  }
  // Cambiamos el estado de la celda
  setNewState(r, c, player_one_turn ? B:R);
  count--;

  player_one_turn = !player_one_turn;

  // comprobamos si alguien gano
  char winner = is_a_winner();
  if (winner != NONE) {
    char WINNER_STATE = (winner == B) ? B:R;
    if (winner == B)
      Serial.println("Jugador uno (azul) gano!...");
    else
      Serial.println("Jugador dos (rojo) gano!...");

    win = true;

    // ...
    delay(3000);
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        setNewState(i, j, WINNER_STATE);
      }
    }

    delay(6000);
    setNewGame();
  }

  if (count == 0) { // si no hay mas celdas vacias
    // empateee
    Serial.println("Empateee! nadie gano.");
    delay(5000);
    setNewGame();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Iniciando el juego");
  for (int i = 0; i < N; i++){
    pinMode(leds[i], OUTPUT);
  }
  Serial.println("pines asignados");

  setNewGame();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    // Lee el primer entero
    row = Serial.parseInt();

    // Lee el segundo entero
    column = Serial.parseInt();

    if (!win && count > 0) {
      set_play(row, column);
      // Serial.println(Grid[row][column].blueLed); // 34 -> 17 (tx)
    }
    // else if (win) {
    //   // mostramos la raya ganadora
    // } else {
    //   // nadie gano...
    // }

    //Serial.println("--- Listo, escribe otros dos ---");
  }
}
