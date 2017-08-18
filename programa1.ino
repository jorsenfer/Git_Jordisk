

// StackShotter_v1f.ino
// ------------------------------------------------- FOTOPILER 2.1 ----------------------------------
// Corretgir desajustos en les diferents velocitats del motor
// Corretgir errors en càlcul dels pasos a retornar en pas automàtic
// ------------------------------------------------- FOTOPILER 2.0 ----------------------------------
// Nou replantejament del control de posició del motor
// Nou control de debouncing per hardware    quinPolsador1 ()
// Nova distribució dels pins per adaptar-ho al PCB

/*
  _____  _____  _   _   _____ 
 |  __ \|_   _|| \ | | / ____|
 | |__) | | |  |  \| || (___  
 |  ___/  | |  | . ` | \___ \ 
 | |     _| |_ | |\  | ____) |
 |_|    |_____||_| \_||_____/ 
                              

PINS:
PIN  0 -  RX -  OUT - buz                 zumbador         !!!! CANVIAT DEL PIN A5
PIN  1 -  TX -  OUT - ACTIVAR CAMERA                       !!!!!!  CANVIAT DEL PIN 11          
PIN  2 -     -  IN  - polsador final carrera
PIN  3 - PWM -  OUT - Display  SCK       NokiaLCD
PIN  4 -     -  OUT - Display  MOSI      NokiaLCD
PIN  5 - PWM -  OUT - Display  DC        NokiaLCD
PIN  6 -     -  OUT - Display  RST       NokiaLCD
PIN  7 -     -  OUT - Display  CS        NokiaLCD
PIN  8 -     -  IN  - polsador +/AMUNT
PIN  9 - PWM -  IN  - polsador -/AVALL
PIN 10 - PWM -  OUT - enable            motor
PIN 11 - PWM -  IN -  polsador ACTIVAR CAMERA              !!!!! CANVIAT LDE PIN 1
PIN 12 -     -  OUT - stp               motor        (el 12 i 13 han estat invertits en aquesta versió)
PIN 13 -     -  OUT - dir               motor

pIN A0 [14]  -  IN  - polsador MODE    (menu)
PIN A1 [15]  -  IN  - polsador SELEC                             
PIN A2 [16]  -  OUT - ms1               motor
PIN A3 [17]  -  OUT - ms2               motor
PIN A4 [18]  -  IN  - polsador ENDARRERA    
PIN A5 [19]  -  IN  - polsador ENDAVANT                   !!!! CANVIAT DEL PIN 0           

=======================================================================================================================================================*/


// MIllor eliminar-ho
#include <NotesMusicals.h>

/*** MOTOR ***/
const byte stp = 12;  					// connectar pin 13 a step (quan pasa de LOW a HIGH fa un pas)
const byte dir = 13;  					// connectar pin 12 a dir (LOW endavant, HIGH al reves)
const byte enable = 10;				  // connectar pin 10 a enable (HIGH activa el driver)
const byte ms1 = A2;            // A2 i A3 (MS1 i MS2 al driver) controlen la velocitat del motor
const byte ms2 = A3;
boolean resMotor [] = {LOW,LOW, // Velocitat màxima
                  LOW,HIGH,     // 1/2 de la velocitat    canviat!! avans HIGH,LOW
                  HIGH,LOW,     // 1/4 de la velocitat    canviat!! avans LOW,HIGH
                  HIGH,HIGH};   // 1/8 de la velocitat -- amb MS3 en HIGH reduim a 1/16

byte velMotor = 6;              // quatre velocitats 0,2,4,6, 0 la mes ràpida

/*** DISPLAY NOKIA 3310 ***/
//#include <NokiaLCD.h>
//NokiaLCD NokiaLCD(3,4,5,6,7); // Pins on es conecta el display (SCK, MOSI, DC, RST, CS)
#include <LCD5110_Basic.h>
LCD5110 myGLCD(3,4,5,6,7);      // Pins on es conecta el display (SCK, MOSI, DC, RST, CS)
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
int LCDcontrast = 65;           // Contrast display, 55 per defecte

/*** polsadors ***/
const byte polsadorAmunt = 8;		// Pins on es connecta cada polsador
const byte polsadorAvall = 9;
const byte polsadorMode = A0;				
const byte polsadorSelec = A1;
const byte polsadorAvanti = A5;
const byte polsadorRecula = A4;
const byte polsadorDisparador = 11;   //  !!!!!! ULL CANVIAT PEL 1
//      boolean polsat1 = HIGH;					 // Guarda valor polsador apretat
//      boolean polsat2 = HIGH;					 // Guarda valor polsador apretat per comparar i evitar rebots (debouncing)
byte teclaPolsada = 0;					// Guarda la tecla polsada (polsador)

//*** camera ***//
//const byte cameraFocus = 10;     // pin que actua sobre el focus de la camera             NO FA FALTA!!!!!!!! 
const byte cameraDisparador = 1;// pin que actua sobre el disparador de la camera   //  !!!!!! ULL CANVIAT PE
boolean Bmirall = 0;            // Defineix si el mirall està bloqueixat o no, 1=Si 0=No

//*** altaveu ***//
const byte buz = 0;             // pin on està connectat el zumbador   !!!!!!!

/*** general ***/
const float versio = 2.1;       // Versió actual del programa
int contador = 0;   						// Contador genèric
int a = 0;									    // Contador genèric
boolean nivell = HIGH;          // Nivell a comparar per detectar si s'han premut els polsadors, contactes, etc (així els puc canviar tots de cop)
int tempsAntiRebot = 300;       // Milisegons de retard despres de polsar teclea per evitar l'efecte rebot (control per software)
int tempsAntiRebotMenu = 500;   // Retard per evitar l'efecte rebot al mantindre polsada la tecla quan s'entra en un menu
int retardMotor = 250;          // Temps despres que el motor s'activa i així evitem perdre pasos al primer moviment

/*** variables CONFIGURACIÓ ***/
byte fotosPresa = 1;            // Fotos per presa (cops que es dispara l'obturador en cada presa) de 1 a 99
boolean autoRetorn = 1;         // Retorn automàtic, 1=Si 0=No, 1 per defecte
char* SiNo [] = {"No","Si"};    // Guarda les paraules Si i No (0 = no, 1 = si)

/*** variables control camera ***/
int Tespera = 1000;             // Espera (milisegons) despres de moures i abans de fer la foto
//int TentreFotos = 500;          // Espera (milisegons) entre fotos -sempre que fotosPresa sigui superior a 1
int Trepos = 1000;              // Temps que reposa el sistema un cop bloquejat el mirall abans de fer la foto per evitar qualsevol vibració
int Tpols = 500;                // Durada del pols (milisegons) que s'envia per activar el disparador

/*** variables moviment motor ***/
// com que la quantitat de pasos a comptar en la màxima resolució (1600 x cada 1mm) poden ser mes de 100mil les variables han de ser long  
// long pasosEndavant = 0;         // Guarda els pasos que s'han fet polsant la tecla ENDAVANT
// long pasosEndarrera = 0;        // Guarda els pasos que s'han fet polsant la tecla ENDARRERA
long pasosXfoto = 0;            // Pasos necessaris per fer cada moviment (foto)
long pasosMotor = 1;            // Pasos que fa el motor cada vegada que es polsa ENDAVANT o ENDARRERA
long pActual = 400000;          // Posició actual del motor (en ralació a l'inici del moviment) - es fa servir un numero que superi el màxim de pasos
                                // necesaris per recorrer tota la distancia (300mmx200) posible així no treballarem mai amb numeros negatius, tenint
                                // en compte que la base està sempre posicionada per la mitad del seu recorregut total. Això es fa per evitar tindre
                                // que fer una autodetecció de la posició mínima cada vegada que s'arranca el sistema. Com que no sabem on està situat
                                // exactament, el sistema suposa que està a 200.000 pases de la posició 0 i fa tots els càlculs a partir d'aquí. Si en
                                // algun moment s'exedeix dels límits físiscs, els interruptors de final de carrera ens avisaran. Com que en la majoria
                                // de casos el motor es moura a la màxima ressolució 1600p/volta augmentem aquesta xifra fins a 400000
volatile long pasosAmoure = 1;  // Pasos que s'ha de moure el motor segons càlculs previs, es volatile pq tambe la modifica una interrupció
                                // segons el menú on s'estigui pot ser pasos fins a inici o pasos de cada foto. 1 és el mínim de pasos a fer
long posicioRetorn = 0;         // Posició d'inici del procés on s'ha de tornar si autoreturn està activat
const float micraXpas = 0.78125;// És el desplaçament en micres que fa cada pas a la velocitat mes lenta 1/8 (1600 pasos) 1,232mm/1600 = 0,77micres/pas
                                // (el càlcul anterior era : 1,25mm/1600 = 0,78125micres/pas)
                                // aquest valor pot variar depenent de la mecànica aplicada (mètrica de rosca, etc.) En metrica 8 el pas es de 1,232mm.
                                // aquests 1,232mm estan calculats pràctiment ja que en teoria cada volta de la varilla roscada M8 equival a 1mm
                                // si fem servir rosca de pas fi en la mateixa metrica aquest seria de 0,75mm (varilla especial)
boolean quinsentit;             // Defineix el sentit de la plataforma que mou el motor - amunt (1) o avall (0). Necessari per la interrupció

/*** variables MODES ***/
int DdPas = 100;                // Distancia en micres entre cada pas (parada, step, etc) 50 per defecte
long pInici = 0;                // Posició inicial (primera foto)
long pFinal = 0;                // Posició final (última foto)
long disTotal = 0;              // Distància total que s'ha de desplaçar el motor en micres
int numeroFotos = 0;            // Guarda el número de fotos (parades, steps, presses, etc) que s'han de fer

/*** variables menus ***/
byte menuActual = 1;            // Memoritza el menu que està en pantalla
byte menuAnterior = 1;          // Memoritza el menu del que es procedeix i al que es tornarà al polsar la tecla MENU
byte liniaCursor = 8;           // Linia on està situat el cursor, per defecte la linia 2 (8 bits per linia)
byte liniaMaxima = 24;          // Linia última on es pot desplaçar el cursor segons menú, pixel superior, de 0 a 40
byte liniaMinima = 8;           // Linia primera on es pot situar el cursor segons menú

//*** interrupcions ***//
const byte interruptor = 2;     // Pin on es conecta l'interruptor de final de carrera del motor
volatile boolean aturar = 0;    // Defineix si s'ha interromput un procés, 0 NO 1 SI, volatile perqué ho pot modificar una interrupció












void setup()
{
       
	/*** MOTOR ***/
	pinMode(stp, OUTPUT);
	pinMode(dir, OUTPUT);
  pinMode(ms1, OUTPUT); 
  pinMode(ms2, OUTPUT);
  digitalWrite(ms1, resMotor [velMotor]);    // defineix la velocitat 
  digitalWrite(ms2, resMotor [velMotor+1]);  // del motor al mínim (6 per defecte, el màxim nº de pasos 1600)
  digitalWrite(enable, HIGH);     // desactiva el driver, s'activarà quan funcioni el motor

	/*** POLSADORS ***/
	pinMode(polsadorAmunt, INPUT);
	pinMode(polsadorAvall, INPUT);
	pinMode(polsadorMode, INPUT);
	pinMode(polsadorSelec, INPUT);
	pinMode(polsadorAvanti, INPUT);  //
	pinMode(polsadorRecula, INPUT);
  pinMode(polsadorDisparador, INPUT);  //

	/*** DISPLAY ***/
	myGLCD.InitLCD(LCDcontrast);               // Inicialitzar pantalla
  myGLCD.setFont(SmallFont);            // Defineix tipus de font
  myGLCD.clrScr();                      // Borra la pantalla
  myGLCD.print("FOTOPILER", CENTER, 4);
  myGLCD.print("VERSIO", CENTER, 12);
  myGLCD.setFont(BigNumbers);
  myGLCD.printNumF(versio, 2, CENTER, 16);
  delay(2000);                          // Temps suficient per llegir la versió
  
  /*** altaveu ***/
  pinMode(buz, OUTPUT);
  digitalWrite(buz, HIGH);
  delay (60);
  digitalWrite(buz, LOW);

  /*** camera ***/
  //pinMode(cameraFocus, OUTPUT);
  pinMode(cameraDisparador, OUTPUT);

  /*** altres ***/
  pinMode(interruptor, INPUT_PULLUP);        // Interruptor final de carrera

  /*** habilita interrupció - interruptor final de carrera ***/
  attachInterrupt(digitalPinToInterrupt(2), interrupcio, nivell);

  menu1 ();
}














void loop()
{
	teclaPolsada = quinPolsador1 ();
	
  switch (teclaPolsada) 
  {
  	case 0:			// cap tecla ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    break;
  		



    case 1:			// tecla  (+)  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    	switch (menuActual)                                       // comprova en quin menu estem i actua en consequencia
      {
        case 1:   // quan està en el menu 1 PRINCIPAL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if (liniaCursor == 24)                                // comprova que estigui seleccionat la linia de Velocitat del motor
            {if(velMotor != 0)                                  // si el valor ja es el mínim (vel. max.) no fa res
              {
                velMotor = velMotor -2;                         // disminueix la velocitat un punt (0,2,4,6)
                myGLCD.printNumI(5-(velMotor/2+1), 74, 24, 1);  // (valor, x, y, digits) el valor es mostra de 1 a 4
                digitalWrite(ms1, resMotor [velMotor]);         // defineix la velocitat 
                digitalWrite(ms2, resMotor [velMotor+1]);       // del motor segons valor seleccionat
                delay(tempsAntiRebot);                          // espera un temps perqué no hi hagi efecte rebot
              }
            }
        break;


        case 2:  // quan està en el menu 2 CONFIGURAR +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if (liniaCursor == 16)                   // comprova que estigui seleccionat la linia de Auto Retorn  +++++++
            {if(autoRetorn == 0)                   // si el valor ja es 1 (No) no fa res
              {
                autoRetorn = autoRetorn +1;        // posa el retorn automàtic en No
                myGLCD.print(SiNo [autoRetorn], 71, 16);
                delay(tempsAntiRebot);             // espera un temps perqué no hi hagi efecte rebot
              }
            break;
            }

          if(liniaCursor == 24)                    // comprova que estigui en la linia Bloqueix del mirall  ++++++++
            {
            if(Bmirall == 0)                       // si el valor ja es 1 (Si) no fa res
              {
                Bmirall = Bmirall +1;              // posa el bloqueix del mirall en 1 (Si)
                myGLCD.print(SiNo [Bmirall], 71, 24);
                 delay(tempsAntiRebot);             // espera un temps perqué no hi hagi efecte rebot
              }
            break;
            }

          /*if(liniaCursor == 24)                    // comprova que estigui en la linia de #fotos presa  ++++++++
            {if(fotosPresa != 99)                  // si el valor ja es 99 (màxim) no fa res
              {
                fotosPresa = fotosPresa +1;        // incrementa el valor
                myGLCD.printNumI(fotosPresa, 71, 24, 2, '0');
                delay(tempsAntiRebot);            // espera un temps perqué no hi hagi efecte rebot
              }
            break;   
            }*/

          if(liniaCursor == 32)     // comprova que estigui en la linia de LCD Cont.  +++++++
            {if(LCDcontrast != 99)                // si el valor ja es 99 (màxim) no fa res
              {
                LCDcontrast = LCDcontrast +1;     // incrementa el valor
                myGLCD.printNumI(LCDcontrast, 71, 32, 2, '0'); // (valor, x, y, digits), el valor es mostra de 0 a 99
                myGLCD.InitLCD(LCDcontrast);      // inicia el LCD amb el nou contrast
                menu2 ();                         // torna a mostrar el menu de nou
                myGLCD.print(" ", 0, liniaCursor);// borrem el cursor de la primera linia que mostra el menu2 per defecte
                liniaCursor = 32;                 // restaurem el valor de la linia on estava el cursor abans del reset
                myGLCD.print(">", 0, liniaCursor);// situem el cursor a la linia on estava
                 delay(tempsAntiRebot);           // espera un temps perqué no hi hagi efecte rebot
              }
            break;   
            }
        break; 


        case 4:     // quan està en el menú 4 TEMPS FOTOS - CONFIGURAR


        break;


        case 5:     // quan està en el menu 5 DISTANCIA AUTOMATICA +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                           // comprova que estigui en la linia de DdPas  +++++++
            {
              if(DdPas < 10000)                           // si el valor ja es 10mm (màxim) no fa res
                {
                  DdPas = DdPas +1;                       // incrementa el valor
                  myGLCD.printNumI(DdPas, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
            }  
        break; 


        case 6:     // quan està en el menu 6 PAS AUTOMATIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                           // comprova que estigui en la linia de #Pasos  +++++++
            {
              if(numeroFotos < 1000)                      // si el valor ja es 1000 (màxim) no fa res
                {
                  numeroFotos = numeroFotos +1;           // incrementa el valor
                  myGLCD.printNumI(numeroFotos, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
            }  
        break; 


        case 7:     // quan està en el menu 7 DISTANCIA TOTAL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                           // comprova que estigui en la linia de #Pasos  +++++++
            {
              if(numeroFotos < 1000)                      // si el valor ja es 1000 (màxim) no fa res
                {
                  numeroFotos = numeroFotos +1;           // incrementa el valor
                  myGLCD.printNumI(numeroFotos, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
            }
          if(liniaCursor == 24)                           // comprova que estigui en la linia de disTotal  +++++++
            {
              if(disTotal < 99000)                        // si el valor ja es 9,9cm (màxim) no fa res
                {
                  disTotal = disTotal +5;                 // incrementa el valor (cada pas es de 0.78125u)
                  myGLCD.printNumI(disTotal, 53, 24, 3, '0');
                  delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
            } 
        break; 


        case 8:     // quan està en el menu 8 DISTANCIA DE PAS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                           // comprova que estigui en la linia de #Pasos  +++++++
            {
              if(numeroFotos < 1000)                      // si el valor ja es 1000 (màxim) no fa res
                {
                  numeroFotos = numeroFotos +1;           // incrementa el valor
                  myGLCD.printNumI(numeroFotos, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
            }
          if(liniaCursor == 24)                           // comprova que estigui en la linia de DdPas  +++++++
            {
              if(DdPas < 10000)                           // si el valor ja es 1cm (màxim) no fa res
                {
                  DdPas = DdPas +5;                       // incrementa el valor (cada pas es de 0.78125u)
                  myGLCD.printNumI(DdPas, 53, 24, 4, '0');
                  delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
            } 
        break; 


        case 9:      // quan està en el menu 9 PAS MANUAL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                           // comprova que estigui en la linia de Dist. pas  +++++++
            {
              if(DdPas < 10000)                           // si el valor ja es 10mm (màxim) no fa res
                {
                  DdPas = DdPas +1;                       // incrementa el valor
                  myGLCD.printNumI(DdPas, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
            }  
        break;  

      }  

    break;
  		


      
    case 2:			// tecla  (-) i (INTRO)- aquesta tecla te doble funció segons menú i opció /////////////////////////////////////////////////////
      switch (menuActual)         // comprova en quin menu estem i actua en consequencia
      {
            case 1:   // quan està en el menu 1 PRINCIPAL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              if (liniaCursor == 24)                        // comprova que estigui en la linia de Velocitat del motor
              {if(velMotor != 6)                            // si el valor ja es el mínim no fa res
                {
                  velMotor = velMotor +2;                   // disminueix la velocitat un punt (0,2,4,6)
                  myGLCD.printNumI(5-(velMotor/2+1), 74, 24, 1); // (valor, x, y, digits) el valor es mostra de 1 a 4
                  digitalWrite(ms1, resMotor [velMotor]);   // defineix la velocitat 
                  digitalWrite(ms2, resMotor [velMotor+1]); // del motor segons valor seleccionat
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot
                }
              break;
              }

              if(liniaCursor == 16)                         // comprova si està en la linia CONFIGURAR  +++++++
              {
                menu2 ();                                   // mostra el menú CONFIGURAR
                break;
              }

              if(liniaCursor == 8)                          // comprova si està en la linia MODES  +++++++
              {
                 menu3 ();                                  // mostra el menú MODES
                 break;
              }
            break;


            case 2:  // quan està en el menu 2 CONFIGURAR ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              if (liniaCursor == 16)                        // comprova que estigui en la linia de Auto Retorn  +++++++
              {
                if(autoRetorn == 1)                         // si el valor ja es 0 (No) no fa res
                {
                  autoRetorn = autoRetorn -1;               // posa el retorn automàtic en Si
                  myGLCD.print(SiNo [autoRetorn], 71, 16);
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot
                }
              break;
              }

              if(liniaCursor == 24)                         // comprova que estigui en la linia Bloqueix del mirall  ++++++++
              {
                if(Bmirall == 1)                            // si el valor ja es 0 (No) no fa res
                {
                  Bmirall = Bmirall -1;                     // posa el bloqueix del mirall en 0 (No)
                  myGLCD.print(SiNo [Bmirall], 71, 24);
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot
                }
              break;
              }

              /*if(liniaCursor == 24)     // comprova que estigui en la linia de #fotos presa  +++++++
                {
                  if(fotosPresa >> 1)      // si el valor ja es 1 (mínim) no fa res
                  {
                    fotosPresa = fotosPresa -1;     // decrementa el valor
                    myGLCD.printNumI(fotosPresa, 71, 24, 2, '0');
                    delay(tempsAntiRebot);                 // espera un temps perqué no hi hagi efecte rebot
                  }
                break;   
                }*/

              if(liniaCursor == 32)                         // comprova que estigui en la linia de LCD Cont.  ++++++++
                {
                if(LCDcontrast >> 0)                        // si el valor ja es 0 (mínim) no fa res
                  {
                    LCDcontrast = LCDcontrast -1;           // incrementa el valor
                    myGLCD.printNumI(LCDcontrast, 71, 32, 2, '0'); // (valor, x, y, digits), el valor es mostra de 0 a 99
                    myGLCD.InitLCD(LCDcontrast);            // inicia el LCD amb el nou contrast
                    menu2 ();                               // torna a mostrar el menu de nou
                    myGLCD.print(" ", 0, liniaCursor);      // borrem el cursor de la primera linia que mostra el menu2 per defecte
                    liniaCursor = 32;                       // restaurem el valor de la linia on estava el cursor abans del reset
                    myGLCD.print(">", 0, liniaCursor);      // situem el cursor a la linia on estava
                    delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                  }
                break;   
                }

              if(liniaCursor == 40)                         // comprova si està en la linia TEMPS FOTOS  +++++++
                {
                  menu4 ();                                 // mostra el menú TEMPS FOTOS
                  break;
                }
            break;


            case 3:  // quan està en el menu 3 MODES ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              if(liniaCursor == 8)                          // comprova si està en la linia DIST. AUTO.  +++++++
              {
                 menu5 ();                                  // mostra el menú DIST. AUTO.
                 break;
              }
              if(liniaCursor == 16)                         // comprova si està en la linia PAS AUTO.  +++++++
              {
                 menu6 ();                                  // mostra el menú PAS AUTO.
                 break;
              }
              if(liniaCursor == 24)                         // comprova si està en la linia DIST. TOTAL  +++++++
              {
                 menu7 ();                                  // mostra el menú DIST. TOTAL
                 break;
              }
              if(liniaCursor == 32)                         // comprova si està en la linia DIST. DEL PAS  +++++++
              {
                 menu8 ();                                  // mostra el menú DIST. DEL PAS
                 break;
              }
              if(liniaCursor == 40)                         // comprova si està en la linia PAS MANUAL +++++++
              {
                 menu9 ();                                  // mostra el menú PAS MANUAL
                 break;
              }
            break;


            case 4:  // quan està en el menú 4 TEMPS FOTOS - CONFIGURAR


            break;


            case 5:  // quan està en el menu 5 DISTANCIA AUTOMÀTICA ++++++++++++++++++++++++++++++++++++++++++++++++++
              if(liniaCursor == 16)                         // comprova que estigui en la linia de DdPas  +++++++
              {
                if(DdPas >> 1)                              // si el valor ja es 1 (mínim) no fa res
                {
                  DdPas = DdPas -1;                         // decrementa el valor
                  myGLCD.printNumI(DdPas, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot per exces de temps prement el polsador
                }
              break;   
              }
              if(liniaCursor == 32)                         // se selecciona l'inici del proces del menu5
                {
                  proces5 ();
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot
                  break;
                }
            break;


            case 6:   // quan està en el menu 6 PAS AUTOMÀTIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              if(liniaCursor == 16)                         // comprova que estigui en la linia de #Pasos  +++++++
              {
                if(numeroFotos > 0)                         // si el valor ja es 0 (mínim) no fa res
                {
                  numeroFotos = numeroFotos -1;             // incrementa el valor
                  myGLCD.printNumI(numeroFotos, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
              }
              if(liniaCursor == 32)                         // es selecciona l'inici del proces del menu6
                {
                  proces6 ();
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot
                  break;
                }  
            break; 


            case 7:   // quan està en el menu 7 DISTANCIA TOTAL +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              if(liniaCursor == 16)                         // comprova que estigui en la linia de #Pasos  +++++++
              {
                if(numeroFotos > 0)                         // si el valor ja es 0 (mínim) no fa res
                {
                  numeroFotos = numeroFotos -1;             // decrementa el valor 
                  myGLCD.printNumI(numeroFotos, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                   // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
              }
              if(liniaCursor == 24)                         // comprova que estigui en la linia de disTotal  +++++++
                {
                if(disTotal > 0)                            // si el valor ja es 0cm (mìnim) no fa res
                  {
                    disTotal = disTotal -5;                 // decrementa el valor
                    myGLCD.printNumI(disTotal, 41, 24, 5, '0');
                    delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                  }
                break;   
                }
            break; 


            case 8:   // quan està en el menu 8 DISTANCIA DEL PAS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              if(liniaCursor == 16)                         // comprova que estigui en la linia de #Pasos  +++++++
              {
                if(numeroFotos > 0)                         // si el valor ja es 0 (mínim) no fa res
                {
                  numeroFotos = numeroFotos -1;             // decrementa el valor 
                  myGLCD.printNumI(numeroFotos, 53, 16, 3, '0');
                  delay(tempsAntiRebot);                    // espera un temps perqué no hi hagi efecte rebot
                }
              break;   
              }
              if(liniaCursor == 24)                         // comprova que estigui en la linia de DdPas  +++++++
                {
                if(DdPas > 0)                               // si el valor ja es 0cm (mìnim) no fa res
                  {
                    DdPas = DdPas -5;                       // decrementa el valor
                    myGLCD.printNumI(DdPas, 53, 24, 4, '0');
                    delay(tempsAntiRebot);                  // espera un temps perqué no hi hagi efecte rebot
                  }
                break;   
                }
            break;


            case 9:    // quan està en el menu 9 PAS AMNUAL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              if(liniaCursor == 16)                        // comprova que estigui en la linia de Dist. pas  +++++++
                {
                if(DdPas > 0)                              // si el valor ja es 0mm (mínim) no fa res
                  {
                    DdPas = DdPas -1;                      // decrementa el valor
                    myGLCD.printNumI(DdPas, 53, 16, 3, '0');
                    delay(tempsAntiRebot);                 // espera un temps perqué no hi hagi efecte rebot
                  }
                break;   
                }  
            break; 
      }       
    break;
    	



    case 3:			// tecla MENU //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    	if(menuAnterior != menuActual)                       // comproba si pot canviar de menú i si es així mostra el corresponent
        {    
          if (menuActual == 11)                            // menu9 PAS MANUAL avançar
            {
              if (autoRetorn == 1)                         // torna a la posició d'inici si autoretorn està activat
                {
                  posicioRetorn = posicioRetorn * -1;      // valor negatiu per indicar desplaçament contrari del motor
                  situarMotor ();
                }
            }
          if (menuActual == 13)                            // menu9 PAS MANUAL recular
            {
              if (autoRetorn == 1)                         // torna a la posició d'inici si autoretorn està activat
                {
                  posicioRetorn = posicioRetorn * -1;      // valor negatiu per indicar desplaçament contrari del motor   !!!!!ull mirar si és correcte
                  situarMotor ();
                }
            }

          tornarMenu ();
          delay(tempsAntiRebot);
        }
    break;
    	



    case 4:			// tecla SELEC //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    	if(menuActual == 10 || menuActual == 12 || menuActual == 14 || menuActual == 15 || menuActual == 11 || menuActual == 13) 
        {                                                  // en tots aquests menús SELEC no està actiu
          break;
        }
      myGLCD.print(" ", 0, liniaCursor);                   // esborra el cursor de la linia actual
      if(liniaCursor == liniaMaxima)                       // mira si el cursor es a l'última linia 
        {
          liniaCursor = liniaMinima-8;                     // si es així torna a la primera
        }
      liniaCursor = liniaCursor + 8;                       // salta a la linia següent
      if((menuActual == 5 & liniaCursor == 32) || (menuActual == 6 & liniaCursor == 32))   // mostra la obligació de polsar INTRO per iniciar el procés
        {
          myGLCD.print("-INTRO iniciar-", 0, 40);
        }
      if((menuActual == 5 & liniaCursor != 32) || (menuActual == 6 & liniaCursor != 32))  // no mostra la obligació de polsar INTRO per iniciar el procés
        {
          myGLCD.print("               ", 0, 40);
        }
      myGLCD.print(">", 0, liniaCursor);

      // CONSIDERAR-HO --- nomes activa el motor quan està en seleccionar velocitat _______
      // if((menuActual == 1 & liniaCursor == 24) 
      // {digitalWrite(enable, LOW);                  // activa el driver
      // delay(retardMotor);     }                    // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment
      // if((menuActual == 1 & liniaCursor != 24) 
      // {digitalWrite(enable, HIGH);    }            // desactiva el driver

          //Serial.print(" pActual ");  Serial.println(pActual); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
      delay(tempsAntiRebot);                               // retard per evitar polsacions seguides 
    break;
    	



    case 5:			// tecla ENDAVANT/AMUNT (FWD > ó ⬆︎) ////////////////////////////////////////////////////////////////////////////////////////////////////
      switch (menuActual)
      {
        case 1:   // quan està en el menu 1 PRINCIPAL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                             // si està seleccionat CONFIG no activa el motor
            {
              break; 
            }
          digitalWrite(enable, LOW);                        // activa el driver
          delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
          do 
            {
              avanti (pasosMotor,1);
              //if (aturar == 1) {break;}
            } while (digitalRead(polsadorAvanti) == nivell); 
          digitalWrite(enable, HIGH);                       // desactiva el driver */
        break;


        case 5:  // quan està en el menu 5 DISTANCIA AUTOMÀTICA ++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                             // si està seleccionat DdPas no activa el motor
            {
              break; 
            }
          if(liniaCursor == 24)                             // si està seleccionat Inici
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment     ULL
              do 
                {
                  avanti (pasosMotor,1);                    // quan es mou el motor a traves del polsador pasosMotor sempre és 1
                  //posicio ();
                  
                } while (digitalRead(polsadorAvanti) == nivell);
              pInici = pActual;                             // defineix la posició inicial on començar a fer les fotos
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
          if(liniaCursor == 32)                             // si està seleccionat Final
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                    // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
              do 
                {
                  avanti (pasosMotor,1);                    // quan es mou el motor a traves del polsador pasosMotor sempre és 1
                  //posicio ();
                  
                } while (digitalRead(polsadorAvanti) == nivell);
              pFinal = pActual;                             // defineix la posició final on fer la última foto
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
        break;


        case 6:   // quan està en el menu 6 PAS AUTOMÀTIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                             // si està seleccionat #Pasos no activa el motor
            {
              break; 
            }
          if(liniaCursor == 24)                             // si està seleccionat Inici
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
              do 
                {
                  avanti (pasosMotor,1);
                  //posicio ();
                  
                } while (digitalRead(polsadorAvanti) == nivell);
              pInici = pActual;                             // defineix la posició inicial on començar a fer les fotos
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
          if(liniaCursor == 32)                             // si està seleccionat Final
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
              do 
                {
                  avanti (pasosMotor,1);
                  //posicio ();
                  
                } while (digitalRead(polsadorAvanti) == nivell);
              pFinal = pActual;                             // defineix la posició final on fer la última foto
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
        break;


        case 7:   // quan està en el menu 7 DISTANCIA TOTAL +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 32)                             // es selecciona l'inici del proces del menu7
            {
              proces7a ();                                  // menu7 DIST TOTAL- mou avançar >
              delay(tempsAntiRebot);                        // espera un temps perqué no hi hagi efecte rebot
              break;
            }
        break;


        case 8:   // quan està en el menu 8 DISTANCIA DEL PAS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 32)                             // es selecciona l'inici del proces del menu8
            {
              proces8a ();                                  // menu8 DISTANCIA DE PAS - endavant >
              delay(tempsAntiRebot);                        // espera un temps perqué no hi hagi efecte rebot
              break;
            }
        break;


        case 9:    // quan està en el menu 9 PAS AMNUAL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 24)                             // si està seleccionat avançar
          {
          proces9a ();
          ferFoto ();
          }
        break;


        // ??????????????????????????????????????????????????????      crec que es pot borrar
        case 11:
        so3;  // sols per depurar
          //pasosAmoure = DdPas / micraXpas;        pasos per cada desplaçament
                                                            // micraXpas es el desplaçament del pas mes petit a la velocitat mes lenta 1/8
          digitalWrite(enable, LOW);                        // activa el driver del motor
          delay(retardMotor);                               // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment
          // posa el motor al mínim de velocitat
          digitalWrite(ms1, HIGH);                  
          digitalWrite(ms2, HIGH);                
          avanti (pasosAmoure,1);
          ferFoto ();
          contador = contador +1;
          myGLCD.printNumI(contador, CENTER, 16, 3,'0');
          // torna a restablir la velocitat del motor seleccionada anterirment
          digitalWrite(ms1, resMotor [velMotor]);           // defineix la velocitat 
          digitalWrite(ms2, resMotor [velMotor+1]);         // del motor segons valor seleccionat
          digitalWrite(enable, HIGH);                       // desactiva el driver del motor
          posicioRetorn = posicioRetorn + pasosAmoure;      // va suman els moviments per retornara l'inici si autureturn està activat
        break;

        default:
        break;
      }

    break;
    	



    case 6:			// tecla AVALL/ENDARRERA (⬇︎ ó BKW ⬅︎) ////////////////////////////////////////////////////////////////////////////////////////////////
    	switch (menuActual)
      {
        case 1:
          if(liniaCursor == 16)                             // si està seleccionat CONFIG no activa el motor
            {
              break; 
            }
          digitalWrite(enable, LOW);                        // activa el driver
          delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
          do 
            {
              recula (pasosMotor,1);
              //pasosEndavant = pasosEndavant + pasosMotor;
                  // posicio ();
            } while (digitalRead(polsadorRecula) == nivell); 
          digitalWrite(enable, HIGH);                       // desactiva el driver */
        break;


        case 5:
          if(liniaCursor == 16)                             // si està seleccionat DdPas no activa el motor
            {
              break; 
            }
          if(liniaCursor == 24)                             // si està seleccionat Inici
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
              do 
                {
                  recula (pasosMotor,1);
                  //posicio ();
                  
                } while (digitalRead(polsadorRecula) == nivell);
              pInici = pActual;                             // defineix la posició inicial on començar a fer les fotos
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
          if(liniaCursor == 32)                             // si està seleccionat Final
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
              do 
                {
                  recula (pasosMotor,1);
                  //posicio ();
                  
                } while (digitalRead(polsadorRecula) == nivell);
              pFinal = pActual;                             // defineix la posició final on fer la última foto
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
        break;


        case 6:   // quan està en el menu 6 PAS AUTOMÀTIC ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if(liniaCursor == 16)                             // si està seleccionat #Pasos no activa el motor
            {
              break; 
            }
          if(liniaCursor == 24)                             // si està seleccionat Inici
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
              do 
                {
                  recula (pasosMotor,1);
                  //posicio ();
                  
                } while (digitalRead(polsadorRecula) == nivell);
              pInici = pActual;                             // defineix la posició inicial on començar a fer les fotos
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
          if(liniaCursor == 32)                             // si està seleccionat Final
            {
              digitalWrite(enable, LOW);                    // activa el driver
              delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
              do 
                {
                  recula (pasosMotor,1);
                  //posicio ();
                  
                } while (digitalRead(polsadorRecula) == nivell);
              pFinal = pActual;                             // defineix la posició final on fer la última foto
              digitalWrite(enable, HIGH);                   // desactiva el driver */ 
            }
        break;


        case 7:      // menu7 DIST TOTAL- mou avançar
          if(liniaCursor == 32)                             // es selecciona l'inici del proces del menu7
            {
              proces7b ();
              delay(tempsAntiRebot);                        // espera un temps perqué no hi hagi efecte rebot
              break;
            }
        break;


        case 8:      // DISTANCIA DEL PAS -- es mou segons paràmetres introduits
          if(liniaCursor == 32)                             // es selecciona l'inici del proces del menu8
            {
              proces8b ();                        
              delay(tempsAntiRebot);                        // espera un temps perqué no hi hagi efecte rebot
              break;
            }
        break;

        case 9:
          if(liniaCursor == 24)                             // si està seleccionat avançar
          {
            proces9b ();
            ferFoto ();
          }
        break;


        // ???????????????????????????      crec que es pot borrar
        case 13:
        so3;  // sols per depurar
          pasosAmoure = DdPas / micraXpas;                  // pasos per cada desplaçament
                                                            // micraXpas es el desplaçament del pas mes petit a la velocitat mes lenta 1/8
          digitalWrite(enable, LOW);                        // activa el driver del motor
          delay(retardMotor);                   // donem temps a que el motor s'activi i així evitem perdre pasos al primer moviment      ULL
          // posa el motor al mínim de velocitat
          digitalWrite(ms1, HIGH);                  
          digitalWrite(ms2, HIGH);                
          recula (pasosAmoure,1);
          ferFoto ();
          contador = contador +1;
          myGLCD.printNumI(contador, CENTER, 16, 3,'0');
          // torna a restablir la velocitat del motor seleccionada anterirment
          digitalWrite(ms1, resMotor [velMotor]);           // defineix la velocitat 
          digitalWrite(ms2, resMotor [velMotor+1]);         // del motor segons valor seleccionat
          digitalWrite(enable, HIGH);                       // desactiva el driver del motor

        break;

        default:
        break;
      }
    break;




    case 7:     // tecla DISPARADOR (SHUTTER)   ////////////////////////////////////////////////////////////////////////////////////////////////////////
      
      so3 ();
      ferFoto ();
      
    break;
	}

}


