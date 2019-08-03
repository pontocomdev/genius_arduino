#define NOTE_B0 31

#define NOTE_C1 33

#define NOTE_CS1 35

#define NOTE_D1 37

#define NOTE_DS1 39

#define NOTE_E1 41

#define NOTE_F1 44

#define NOTE_FS1 46

#define NOTE_G1 49

#define NOTE_GS1 52

#define NOTE_A1 55

#define NOTE_AS1 58

#define NOTE_B1 62

#define NOTE_C2 65

#define NOTE_CS2 69

#define NOTE_D2 73

#define NOTE_DS2 78

#define NOTE_E2 82

#define NOTE_F2 87

#define NOTE_FS2 93

#define NOTE_G2 98

#define NOTE_GS2 104

#define NOTE_A2 110

#define NOTE_AS2 117

#define NOTE_B2 123

#define NOTE_C3 131

#define NOTE_CS3 139

#define NOTE_D3 147

#define NOTE_DS3 156

#define NOTE_E3 165

#define NOTE_F3 175

#define NOTE_FS3 185

#define NOTE_G3 196

#define NOTE_GS3 208

#define NOTE_A3 220

#define NOTE_AS3 233

#define NOTE_B3 247

#define NOTE_C4 262

#define NOTE_CS4 277

#define NOTE_D4 294

#define NOTE_DS4 311

#define NOTE_E4 330

#define NOTE_F4 349

#define NOTE_FS4 370

#define NOTE_G4 392

#define NOTE_GS4 415

#define NOTE_A4 440

#define NOTE_AS4 466

#define NOTE_B4 494

#define NOTE_C5 523

#define NOTE_CS5 554

#define NOTE_D5 587

#define NOTE_DS5 622

#define NOTE_E5 659

#define NOTE_F5 698

#define NOTE_FS5 740

#define NOTE_G5 784

#define NOTE_GS5 831

#define NOTE_A5 880

#define NOTE_AS5 932

#define NOTE_B5 988

#define NOTE_C6 1047

#define NOTE_CS6 1109

#define NOTE_D6 1175

#define NOTE_DS6 1245

#define NOTE_E6 1319

#define NOTE_F6 1397

#define NOTE_FS6 1480

#define NOTE_G6 1568

#define NOTE_GS6 1661

#define NOTE_A6 1760

#define NOTE_AS6 1865

#define NOTE_B6 1976

#define NOTE_C7 2093

#define NOTE_CS7 2217

#define NOTE_D7 2349

#define NOTE_DS7 2489

#define NOTE_E7 2637

#define NOTE_F7 2794

#define NOTE_FS7 2960

#define NOTE_G7 3136

#define NOTE_GS7 3322

#define NOTE_A7 3520

#define NOTE_AS7 3729

#define NOTE_B7 3951

#define NOTE_C8 4186

#define NOTE_CS8 4435

#define NOTE_D8 4699

#define NOTE_DS8 4978



#define CHOICE_OFF      0 //Usado para controlar LEDs

#define CHOICE_NONE     0 //Usado para verificar botões

#define CHOICE_RED  (1 << 0)

#define CHOICE_GREEN    (1 << 1)

#define CHOICE_BLUE (1 << 2)

#define CHOICE_YELLOW   (1 << 3)



#define LED_RED     10

#define LED_GREEN   3

#define LED_BLUE    13

#define LED_YELLOW  5



// Definições dos pinos de botão

#define BUTTON_RED    9

#define BUTTON_GREEN  2

#define BUTTON_BLUE   12

#define BUTTON_YELLOW 6



// Definições dos pinos do Buzzer

#define BUZZER1  4

#define BUZZER2  7



// Parâmetros do jogo

#define ROUNDS_TO_WIN      13 // Número de rodadas para vencer o Jogo.

#define ENTRY_TIME_LIMIT   3000 // Tempo para pressionar um botão antes que o jogo acabe. 3000ms = 3 seg



#define MODE_MEMORY  0

#define MODE_BATTLE  1

#define MODE_BEEGEES 2



// Variáveis de estado do jogo

byte gameMode = MODE_MEMORY; //Por padrão, vamos jogar o jogo da memória

byte gameBoard[32]; //Salva a combinação de botões à medida que avançamos

byte gameRound = 0; //Conta o número de rodadas de sucesso que o jogador fez



void setup()

{

  // Configuração dos pinos de entradas e saídas



  // Ativa pull ups em entradas

  pinMode(BUTTON_RED, INPUT_PULLUP);

  pinMode(BUTTON_GREEN, INPUT_PULLUP);

  pinMode(BUTTON_BLUE, INPUT_PULLUP);

  pinMode(BUTTON_YELLOW, INPUT_PULLUP);



  pinMode(LED_RED, OUTPUT);

  pinMode(LED_GREEN, OUTPUT);

  pinMode(LED_BLUE, OUTPUT);

  pinMode(LED_YELLOW, OUTPUT);



  pinMode(BUZZER1, OUTPUT);

  pinMode(BUZZER2, OUTPUT);



  // Verificação de modo

  gameMode = MODE_MEMORY; // Por padrão, vamos jogar o jogo da memória



  // Verifique se o botão inferior direito está pressionado

  if (checkButton() == CHOICE_YELLOW) play_beegees();



  // Verifique se o botão superior direito está pressionado

  if (checkButton() == CHOICE_GREEN)

  {

    gameMode = MODE_BATTLE; // Coloque o jogo no modo de batalha



    //Ligue o LED superior direito (verde)

    setLEDs(CHOICE_GREEN);

    toner(CHOICE_GREEN, 150);



    setLEDs(CHOICE_RED | CHOICE_BLUE | CHOICE_YELLOW); // Ligue os outros LEDs até você soltar o botão



    while(checkButton() != CHOICE_NONE) ; // Aguarde até que o usuário pare de pressionar o botão



    //Agora não faça nada. O modo de batalha esta ativado

  }



  play_winner(); // Após a conclusão da configuração, diga olá ao mundo

}



void loop()

{

  attractMode(); // Pisca luzes enquanto aguarda o usuário apertar um botão



  // Indique o início do jogo

  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); // Ativar todos os LEDs

  delay(1000);

  setLEDs(CHOICE_OFF); // Desligue os LEDs

  delay(250);



  if (gameMode == MODE_MEMORY)

  {

    // Play no jogo de memória e recebe com resultado

    if (play_memory() == true) 

      play_winner(); // Ganhou, toca som vitória

    else 

      play_loser(); // Perdeu, toca som derrota

  }



  if (gameMode == MODE_BATTLE)

  {

    play_battle(); // Jogue o jogo até que alguém perca



    play_loser(); // Perdeu, toca som derrota

  }

}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//As seguintes funções estão relacionadas apenas ao jogo



// Jogue o jogo regular da memória

// Retorna 0 se o jogador perde ou 1 se o jogador ganhar

boolean play_memory(void)

{

  randomSeed(millis()); // Gerador aleatório



  gameRound = 0; // Redefinir o jogo para o começo



  while (gameRound < ROUNDS_TO_WIN) 

  {

    add_to_moves(); // Adicione um botão aos movimentos atuais e reproduza-os



    playMoves(); // Jogue de volta o tabuleiro do jogo atual



    // Em seguida, solicite ao jogador que repita a sequência.

    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)

    {

      byte choice = wait_for_button(); // Veja o botão que o usuário pressiona



      if (choice == 0) return false; // Se a espera expirar, o jogador perde



      if (choice != gameBoard[currentMove]) return false; // Se a escolha estiver incorreta, o jogador perde

    }



    delay(1000); // O jogador estava correto, espera antes de jogar

  }



  return true; // O jogador cumpriu todas as rodadas para ganhar!

}



// Jogue o modo de batalha especial de 2 jogadores

// Um jogador começa pressionando um botão e entregando ao outro jogador

// Esse jogador repete o botão e adiciona um, depois passa de volta.

// Esta função retorna quando alguém perde

boolean play_battle(void)

{

  gameRound = 0; // Redefinir o quadro do jogo de volta para um quadro



  while (1) // Loop até alguém falhar

  {

    byte newButton = wait_for_button(); // Aguarde até que o usuário insira o próximo passo

    gameBoard[gameRound++] = newButton; // Adicionar este novo botão ao array do jogo



    // Em seguida, solicite ao jogador que repita a sequência.

    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)

    {

      byte choice = wait_for_button();



      if (choice == 0) return false; // Se a espera expirar, o jogador perde.



      if (choice != gameBoard[currentMove]) return false; // Se a escolha estiver incorreta, o jogador perde.

    }



    delay(100); // Dê ao usuário 100ms extras para entregar o jogo ao outro jogador

  }



  return true; // Nós nunca devemos chegar aqui

}



// Reproduz o conteúdo atual dos movimentos do jogo

void playMoves(void)

{

  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 

  {

    toner(gameBoard[currentMove], 150);



    // Aguarde algum tempo entre a reprodução do botão

     // Encurtar isso para tornar o jogo mais difícil

    delay(150); // 150 funciona bem. 75 fica rápido.

  }

}



// Adiciona um novo botão aleatório à sequência do jogo

void add_to_moves(void)

{

  byte newButton = random(0, 4); //min (incluido), max (excluido)



  // Temos que converter esse número, 0 até 3, para CHOICEs

  if(newButton == 0) newButton = CHOICE_RED;

  else if(newButton == 1) newButton = CHOICE_GREEN;

  else if(newButton == 2) newButton = CHOICE_BLUE;

  else if(newButton == 3) newButton = CHOICE_YELLOW;



  gameBoard[gameRound++] = newButton; // Adicionar este novo botão ao array do jogo

}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// As seguintes funções controlam o hardware



// ilumina um determinado LEDs

// Passar em um byte composto de CHOICE_RED, CHOICE_YELLOW, etc

void setLEDs(byte leds)

{

  if ((leds & CHOICE_RED) != 0)

    digitalWrite(LED_RED, HIGH);

  else

    digitalWrite(LED_RED, LOW);



  if ((leds & CHOICE_GREEN) != 0)

    digitalWrite(LED_GREEN, HIGH);

  else

    digitalWrite(LED_GREEN, LOW);



  if ((leds & CHOICE_BLUE) != 0)

    digitalWrite(LED_BLUE, HIGH);

  else

    digitalWrite(LED_BLUE, LOW);



  if ((leds & CHOICE_YELLOW) != 0)

    digitalWrite(LED_YELLOW, HIGH);

  else

    digitalWrite(LED_YELLOW, LOW);

}



// Aguarde até que um botão seja pressionado.

// Retorna uma das cores do LED (LED_RED, etc.) se for bem-sucedida, 0 se expirar

byte wait_for_button(void)

{

  long startTime = millis(); // Lembre-se da hora em que começamos o loop



  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Faz um loop até que passe muito tempo

  {

    byte button = checkButton();



    if (button != CHOICE_NONE)

    { 

      toner(button, 150); // Reproduzir o botão que o usuário acabou de pressionar



      while(checkButton() != CHOICE_NONE) ;  // Agora vamos esperar que o usuário libere o botão



      delay(10); // Isso ajuda com toques duplos acidentais



      return button;

    }



  }



  return CHOICE_NONE; // Se chegarmos aqui, expiramos!

}



// Retorna um bit '1' na posição correspondente a CHOICE_RED, CHOICE_GREEN, etc.

byte checkButton(void)

{

  if (digitalRead(BUTTON_RED) == 0) return(CHOICE_RED); 

  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN); 

  else if (digitalRead(BUTTON_BLUE) == 0) return(CHOICE_BLUE); 

  else if (digitalRead(BUTTON_YELLOW) == 0) return(CHOICE_YELLOW);



  return(CHOICE_NONE); // Se nenhum botão for pressionado, não retorne nenhum

}



// Acenda um LED e toque o tom

// Vermelho, superior esquerdo: 440Hz - 2.272ms - 1.136ms de pulso

// Verde, superior direito: 880Hz - 1.136ms - pulso de 0.568ms

// Azul, inferior esquerdo: pulso de 587.33Hz - 1.702ms - 0.851ms

// Amarelo, inferior direito: 784Hz - 1,276ms - pulso de 0,638ms

void toner(byte which, int buzz_length_ms)

{

  setLEDs(which); // Ligue um dado LED



  // Reproduz o som associado ao LED fornecido

  switch(which) 

  {

  case CHOICE_RED:

    buzz_sound(buzz_length_ms, 1136); 

    break;

  case CHOICE_GREEN:

    buzz_sound(buzz_length_ms, 568); 

    break;

  case CHOICE_BLUE:

    buzz_sound(buzz_length_ms, 851); 

    break;

  case CHOICE_YELLOW:

    buzz_sound(buzz_length_ms, 638); 

    break;

  }



  setLEDs(CHOICE_OFF); // Desligue todos os LEDs

}



// Alterna o buzzer a cada buzz_delay_us, por uma duração de buzz_length_ms.

void buzz_sound(int buzz_length_ms, int buzz_delay_us)

{

  // Converter tempo total de reprodução de milissegundos para microssegundos

  long buzz_length_us = buzz_length_ms * (long)1000;



  // Faz um loop até que o tempo restante de reprodução seja menor que um único buzz_delay_us

  while (buzz_length_us > (buzz_delay_us * 2))

  {

    buzz_length_us -= buzz_delay_us * 2; // Diminui o tempo de jogo restante



    // Alterna a campainha em várias velocidades

    digitalWrite(BUZZER1, LOW);

    digitalWrite(BUZZER2, HIGH);

    delayMicroseconds(buzz_delay_us);



    digitalWrite(BUZZER1, HIGH);

    digitalWrite(BUZZER2, LOW);

    delayMicroseconds(buzz_delay_us);

  }

}



// Reproduzir o som e as luzes do vencedor

void play_winner(void)

{

  setLEDs(CHOICE_GREEN | CHOICE_BLUE);

  winner_sound();

  setLEDs(CHOICE_RED | CHOICE_YELLOW);

  winner_sound();

  setLEDs(CHOICE_GREEN | CHOICE_BLUE);

  winner_sound();

  setLEDs(CHOICE_RED | CHOICE_YELLOW);

  winner_sound();

}



// Som vencedor

// Este é apenas um som (chato) único que criamos, não há mágica nisso

void winner_sound(void)

{

  // Alterna a campainha em várias velocidades

  for (byte x = 250 ; x > 70 ; x--)

  {

    for (byte y = 0 ; y < 3 ; y++)

    {

      digitalWrite(BUZZER2, HIGH);

      digitalWrite(BUZZER1, LOW);

      delayMicroseconds(x);



      digitalWrite(BUZZER2, LOW);

      digitalWrite(BUZZER1, HIGH);

      delayMicroseconds(x);

    }

  }

}



// Jogue o som perdedor / luzes

void play_loser(void)

{

  setLEDs(CHOICE_RED | CHOICE_GREEN);

  buzz_sound(255, 1500);



  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);

  buzz_sound(255, 1500);



  setLEDs(CHOICE_RED | CHOICE_GREEN);

  buzz_sound(255, 1500);



  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);

  buzz_sound(255, 1500);

}



// Mostra uma tela de "modo de atração" enquanto aguarda o usuário pressionar o botão.

void attractMode(void)

{

  while(1) 

  {

    setLEDs(CHOICE_RED);

    delay(100);

    if (checkButton() != CHOICE_NONE) return;



    setLEDs(CHOICE_BLUE);

    delay(100);

    if (checkButton() != CHOICE_NONE) return;



    setLEDs(CHOICE_GREEN);

    delay(100);

    if (checkButton() != CHOICE_NONE) return;



    setLEDs(CHOICE_YELLOW);

    delay(100);

    if (checkButton() != CHOICE_NONE) return;

  }

}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// As seguintes funções estão relacionadas apenas ao Easter Egg de Beegees



// Notas na melodia. Cada nota é sobre uma nota 1/8, "0" são descansos.

int melody[] = {

  NOTE_G4, NOTE_A4, 0, NOTE_C5, 0, 0, NOTE_G4, 0, 0, 0,

  NOTE_E4, 0, NOTE_D4, NOTE_E4, NOTE_G4, 0,

  NOTE_D4, NOTE_E4, 0, NOTE_G4, 0, 0,

  NOTE_D4, 0, NOTE_E4, 0, NOTE_G4, 0, NOTE_A4, 0, NOTE_C5, 0};



int noteDuration = 115; // Isso essencialmente define o ritmo, 115 é quase certo para um groove disco :)

int LEDnumber = 0; // Mantém o controle de qual LED estamos ligados durante o ciclo de beegees



// Não faça nada além de tocar beegees ruins

// Esta função é ativada quando o usuário segura o botão inferior direito durante a energização

void play_beegees()

{

  // Ligue o LED inferior direito (amarelo)

  setLEDs(CHOICE_YELLOW);

  toner(CHOICE_YELLOW, 150);



  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE); // Ligue os outros LEDs até você soltar o botão



  while(checkButton() != CHOICE_NONE) ; // Aguarde até que o usuário pare de pressionar o botão



  setLEDs(CHOICE_NONE); // Desligue os LEDs



  delay(1000); // Espere um segundo antes de tocar música



  digitalWrite(BUZZER1, LOW); // configure o lado "BUZZER1" da campainha para ficar baixo, enquanto tocamos o tom no outro pino.



  while(checkButton() == CHOICE_NONE) // Reproduzir música até você pressionar um botão

  {

    // iterar sobre as notas da melodia:

    for (int thisNote = 0; thisNote < 32; thisNote++) {

      changeLED();

      tone(BUZZER2, melody[thisNote],noteDuration);

        // para distinguir as notas, defina um tempo mínimo entre elas.

       // a duração da nota + 30% parece funcionar bem:

      int pauseBetweenNotes = noteDuration * 1.30;

      delay(pauseBetweenNotes);

      // pare o tom tocando:

      noTone(BUZZER2);

    }

  }

}



// Cada vez que esta função é chamada, a placa se move para o próximo LED

void changeLED(void)

{

  setLEDs(1 << LEDnumber); // Mude o LED



  LEDnumber++; // Ir para o próximo LED

  if(LEDnumber > 3) LEDnumber = 0; // Enrole o balcão, se necessário

}
