

/*
  ______  _    _  _   _   _____  _____   ____   _   _   _____ 
 |  ____|| |  | || \ | | / ____||_   _| / __ \ | \ | | / ____|
 | |__   | |  | ||  \| || |       | | || |  | ||  \| || (___  
 |  __|  | |  | || . ` || |       | | || |  | || . ` | \___ \ 
 | |     | |__| || |\  || |____  _| |_|| |__| || |\  | ____) |
 |_|      \____/ |_| \_| \_____||_____| \____/ |_| \_||_____/                                                                                   

*/

//  Rutina de PROCESSAMENT del menu5 DISTANCIA AUTOMATICA///////////////////////////////////////////////////////////////////////////////////////////
void proces5 ()  
{
    // PER DEPURACIO
    /*
    Serial.println(" Proces 5 "); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" micraXpas ");  Serial.println(micraXpas); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pasosAmoure ");  Serial.println(pasosAmoure); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD  
    Serial.print(" pInici ");  Serial.println(pInici); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD 
    Serial.print(" pFinal ");  Serial.println(pFinal); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pActual ");  Serial.println(pActual); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.println(); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    */
  menuAnterior = 5;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 10;
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  pasosAmoure = pInici - pFinal;                // calcula el total de pasos que s'ha mogut el motor
  posicioRetorn = pasosAmoure;                  // memoritza els pasos per tornar a la posició inicial en rutina "situaMotor"
    //Serial.print(" pasosAmoure ");  Serial.println(pasosAmoure); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD  
    //Serial.println(); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
  //posicioRetorn = pasosAmoure;                  // memoritza la posició inicial on ha de retornar xq autoRetorn està activat
  situarMotor ();                             // situar motor a la posició inici
  calcul1 ();                                 // Calcula els pasos a fer a partir de la posició inicial i final i la velocitat (resolució) del motor
    /*
    Serial.print(" micraXpas ");  Serial.println(micraXpas); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" DdPas ");  Serial.println(DdPas); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pasosXfoto ");  Serial.println(pasosXfoto); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD  
    Serial.print(" numeroFotos ");  Serial.println(numeroFotos); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pActual ");  Serial.println(pActual); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" posicioRetorn ");  Serial.println(posicioRetorn); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.println(); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD  
    */
  myGLCD.print("Dist.      u",7,8);
  myGLCD.printNumI(disTotal, 40, 8, 5,'0');      // DdPas * numeroFotos
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("000",0,16);                      // mostra zeros en el display
  myGLCD.print("-", CENTER, 16);
  myGLCD.printNumI(numeroFotos, 48, 16, 3,'0');

  if (numeroFotos != 0) { ferFoto ();}           // si no hi ha hagut moviment no fa la primera foto

  mouMotor ();                                   // Rutina que fa les fotos i mou el motor

  myGLCD.setFont(SmallFont);
  myGLCD.print("            ", CENTER, 0);       // Borra la paraula PROCESSANT
  myGLCD.print("FINALITZAT",CENTER,40);
  so1 ();

    /*Serial.println(" Fi Proces 5 ");  // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pasosAmoure ");  Serial.println(pasosAmoure); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD  
    Serial.print(" pInici ");  Serial.println(pInici); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD 
    Serial.print(" pFinal ");  Serial.println(pFinal); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pActual ");  Serial.println(pActual); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" posicioRetorn ");  Serial.println(posicioRetorn); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.println(); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    */
  if (autoRetorn == 1)                          // torna a la posició d'inici si autoretorn està activat
    {
      if (posicioRetorn <0) { posicioRetorn = (pasosAmoure * numeroFotos) * -1;} // si la posicio retorn guardada és negativa
                                                                                 // manté la dada de la posició en negatiu
      else  {posicioRetorn = pasosAmoure * numeroFotos;}     // calcula la posició inicial on ha de retornar xq autoRetorn està activat
      situarMotorFinal ();                           // Mou el motor a la posició d'inici de les fotos
    }
    /*
    Serial.println(" Fi Proces 5 despres d autoretorn");  // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pasosAmoure ");  Serial.println(pasosAmoure); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD  
    Serial.print(" pInici ");  Serial.println(pInici); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD 
    Serial.print(" pFinal ");  Serial.println(pFinal); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" pActual ");  Serial.println(pActual); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.print(" posicioRetorn ");  Serial.println(posicioRetorn); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    Serial.println(); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    */
}

//============================================================================ FI =======================================================================




// Rutina de PROCESSAMENT del menu6 PAS AUTOMATIC  ////////////////////////////////////////////////////////////////////////////////////////7/////////////
void proces6 ()  
{
  menuAnterior = 6;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 12;                              // Defineix el menú/rutina en que estem
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  pasosAmoure = pInici - pFinal;
  posicioRetorn = pasosAmoure;                  // memoritza la posició inicial on ha de retorner xq autoRetorn està activat
  situarMotor ();                               // situar motor a la posició inici
  calcul2 ();
  myGLCD.print("Dist.      u",7,8);
  myGLCD.printNumI(disTotal, 40, 8, 5,'0');
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("000",0,16);                     // mostra zeros en el display
  myGLCD.print("-", CENTER, 16);
  myGLCD.printNumI(numeroFotos, 48, 16, 3,'0');

  if (numeroFotos != 0) { ferFoto ();}          // si no hi ha hagut moviment no fa la primera foto

  if (pasosAmoure != 0)                         // perqué el driver del motor no entri en bucle al ser 0 no es mou el motor
    {
      mouMotor ();
    }

  myGLCD.setFont(SmallFont);
  myGLCD.print("            ", CENTER, 0);      // Borra la paraula PROCESSANT
  myGLCD.print("FINALITZAT",CENTER,40);

  if (pasosAmoure != 0)
    {
      so1 ();                                   // senyal de finalització 
    }
  else
    {
      so2 ();                                   // senyal d'alarma al ser 0 els pasos a moure
    }

  if (autoRetorn == 1)                          // torna a la posició d'inici si autoretorn està activat
    {
      situarMotorFinal ();
    }
}

//============================================================================ FI =======================================================================




// Rutina de PROCESSAMENT del menu7 DIST TOTAL- mou avançar >////////////////////////////////////////////////////////////////////////////////////////////
void proces7b ()   
{
  menuAnterior = 7;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 14;                              // Defineix el menú/rutina en que estem
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  myGLCD.print("Dist.      u",7,8);
  myGLCD.printNumI(disTotal, 40, 8, 5,'0');
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("000",0,16);                     // mostra zeros en el display
  myGLCD.print("-", CENTER, 16);
  myGLCD.printNumI(numeroFotos, 48, 16, 3,'0');

  if (numeroFotos != 0) { ferFoto ();}          // fa la primera foto abans de moure el motor

  contador = disTotal / micraXpas;              // calcula el numero total de pasos del motor         !!!!!
  pasosAmoure = contador / numeroFotos;         // pasos que s'ha de moure el motor en cada moviment
  posicioRetorn = contador;                     // memoritza la posició inicial on ha de retorner (retrocedir) si autoRetorn està activat
  mouMotor ();

  myGLCD.setFont(SmallFont);
  myGLCD.print("            ", CENTER, 0);      // Borra la paraula PROCESSANT
  myGLCD.print("FINALITZAT",CENTER,40);
  so1 ();

  if (autoRetorn == 1 & numeroFotos != 0)       // torna a la posició d'inici si autoretorn està activat i s'ha fet alguna foto
    {
      situarMotorFinal ();
    }
}

//============================================================================ FI =======================================================================




// Rutina de PROCESSAMENT del menu7 DIST TOTAL- recular >   /////////////////////////////////////////////////////////////////////////////////////////////
void proces7a ()   
{
  menuAnterior = 7;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 14;                              // Defineix el menú/rutina en que estem
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  myGLCD.print("Dist.      u",7,8);
  myGLCD.printNumI(disTotal, 40, 8, 5,'0');
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("000",0,16);                     // mostra zeros en el display
  myGLCD.print("-", CENTER, 16);
  myGLCD.printNumI(numeroFotos, 48, 16, 3,'0');

  if (numeroFotos != 0) { ferFoto ();}          // fa la primera foto abans de moure el motor

  contador = disTotal / micraXpas;              // calcula el numero total de pasos del motor             !!!!!!!!!!
  pasosAmoure = contador / numeroFotos;         // pasos que s'ha de moure el motor en cada moviment
  //pasosAmoure = pasosAmoure * -1;               // valor negatiu per dir a la rutina que ha de retrocedir el motor
  posicioRetorn = contador * -1;  // memoritza la posició inicial on ha de retorner (retrocedir) si autoRetorn està activat
  mouMotor ();

  myGLCD.setFont(SmallFont);
  myGLCD.print("            ", CENTER, 0);      // Borra la paraula PROCESSANT
  myGLCD.print("FINALITZAT",CENTER,40);
  so1 ();

  //if (autoRetorn == 1)                          // torna a la posició d'inici si autoretorn està activat
  if (autoRetorn == 1 & numeroFotos != 0)       // torna a la posició d'inici si autoretorn està activat i s'ha fet alguna foto
    {
      situarMotorFinal ();
    }
}

//============================================================================ FI =======================================================================




// Rutina de PROCESSAMENT del menu8 DISTANCIA DE PAS - amunt -endavant >   ///////////////////////////////////////////////////////////////////////////////
void proces8b ()      //  el motor es mou cap amunt (avanti) pero la plataforma baixa
{
  menuAnterior = 8;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 15;                              // Defineix el menú/rutina en que estem
  disTotal = numeroFotos * DdPas;               // Distancia total que s'ha de meure el motor
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  myGLCD.print("Dist.      u",7,8);
  myGLCD.printNumI(disTotal, 40, 8, 5,'0');
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("000",0,16);                     // mostra zeros en el display
  myGLCD.print("-", CENTER, 16);
  myGLCD.printNumI(numeroFotos, 48, 16, 3,'0');

  if (numeroFotos != 0) { ferFoto ();}          // fa la primera foto abans de moure el motor

  contador = disTotal / micraXpas;              // calcula el numero total de pasos del motor         !!!!!!!!!!!!!
  pasosAmoure = contador / numeroFotos;         // pasos que s'ha de moure el motor en cada moviment
  posicioRetorn = contador;  // memoritza la posició inicial on ha de retorner (retrocedir) si autoRetorn està activat
  mouMotor ();

  myGLCD.setFont(SmallFont);
  myGLCD.print("            ", CENTER, 0);      // Borra la paraula PROCESSANT
  myGLCD.print("FINALITZAT",CENTER,40);
  so1 ();

  if (autoRetorn == 1)                          // torna a la posició d'inici si autoretorn està activat
    {
      situarMotorFinal ();
    }
}

//============================================================================ FI =======================================================================




// Rutina de PROCESSAMENT del menu7 DISTANCIA DE PAS - avall -recular >   ////////////////////////////////////////////////////////////////////////////////
void proces8a ()   //  el motor es mou cap avall (recula) pero la plataforma puja
{
  menuAnterior = 8;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 15;                              // Defineix el menú/rutina en que estem
  disTotal = numeroFotos * DdPas;               // Distancia total que s'ha de meure el motor
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  myGLCD.print("Dist.      u",7,8);
  myGLCD.printNumI(disTotal, 40, 8, 5,'0');
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("000",0,16);                     // mostra zeros en el display
  myGLCD.print("-", CENTER, 16);
  myGLCD.printNumI(numeroFotos, 48, 16, 3,'0');

  if (numeroFotos != 0) { ferFoto ();}          // fa la primera foto abans de moure el motor

  contador = disTotal / micraXpas;              // calcula el numero total de pasos del motor           !!!!!!!!!!!!!!!!
  pasosAmoure = contador / numeroFotos;         // pasos que s'ha de moure el motor en cada moviment
  //pasosAmoure = pasosAmoure * -1;               // valor negatiu per dir a la rutina que ha de retrocedir el motor
  posicioRetorn = contador * -1;  // memoritza la posició inicial on ha de retorner (retrocedir) si autoRetorn està activat
    //Serial.print(" pasosAmoure ");  Serial.println(pasosAmoure); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD 
    //Serial.print(" posicioRetorn ");  Serial.println(posicioRetorn); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
    //Serial.println(); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
  mouMotor ();

  myGLCD.setFont(SmallFont);
  myGLCD.print("            ", CENTER, 0);      // Borra la paraula PROCESSANT
  myGLCD.print("FINALITZAT",CENTER,40);
  so1 ();

  if (autoRetorn == 1)                          // torna a la posició d'inici si autoretorn està activat
    {
      situarMotorFinal ();
    }
}

//============================================================================ FI =======================================================================




// Rutina de PROCESSAMENT del menu9 PAS MANUAL avançar   ////////////////////////////////////////////////////////////////////////////////////////////////
void proces9a ()  
{
  menuAnterior = 9;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 11;                              // Defineix el menú/rutina en que estem
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  myGLCD.print("DdPas.      u",5,8);
  myGLCD.printNumI(DdPas, 46, 8, 5,'0');
  myGLCD.print("polsar >", CENTER, 40);
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("001", CENTER, 16);
  contador = 1;                                 // va contant les fotos que es fan
  pasosAmoure = DdPas / micraXpas;              // pasos per cada desplaçament            !!!!!!!!
                                                // micraXpas es el desplaçament del pas mes petit a la velocitat mes lenta 1/8
  posicioRetorn = pasosAmoure;                  // memoritza la posició inicial on ha de retorner (retrocedir) si autoRetorn està activat
}

//============================================================================ FI =======================================================================




// Rutina de PROCESSAMENT del menu9 PAS MANUAL recular   ////////////////////////////////////////////////////////////////////////////////////////////////
void proces9b ()  
{
  menuAnterior = 9;                             // Menú al qual s'ha de tornar al polsar la tecla MENU o d'aturada del proces
  menuActual = 13;                              // Defineix el menú/rutina en que estem
  myGLCD.setFont(SmallFont);                    // Defineix tipus de font
  myGLCD.clrScr();                              // Borrar pantalla
  myGLCD.print("-PROCESSANT-", CENTER, 0);
  myGLCD.print("DdPas.      u",5,8);
  myGLCD.printNumI(DdPas, 46, 8, 5,'0');
  myGLCD.print("polsar <", CENTER, 40);
  myGLCD.setFont(MediumNumbers);
  myGLCD.print("001", CENTER, 16);
  contador = 1;                                 // va contant les fotos que es fan
}

//============================================================================ FI =======================================================================




// Calcula els pasos a partir de la posició inicial i final   ///////////////////////////////////////////////////////////////////////////////////////////
void calcul1 ()   
{
  // s'hauria de canviar contador per pasosXfoto per ser mes entenedor   FET
  pasosXfoto = DdPas / micraXpas;   // pasos per cada moviment (foto), micraXpas es el desplaçament del pas mes petit a la velocitat mes lenta 1/8
  switch (velMotor)
    {
      case 0:
        pasosAmoure = (pInici - pFinal) * 8;
        break;
      case 2:
        pasosAmoure = (pInici - pFinal) * 4;
        break;
      case 4:
        pasosAmoure = (pInici - pFinal) * 2;
        break;
      case 6:
        pasosAmoure = pInici - pFinal;
        break;
    }
  numeroFotos = pasosAmoure / pasosXfoto;   // Número de moviments (fotos) a fer segons el totals de pasos i els pasos de cada moviment
  disTotal = DdPas * numeroFotos;                         // Distància total del desplaçament - si es cap avll surt negatiu
  if (numeroFotos <0) {numeroFotos = numeroFotos * -1;}   // com que no pot ser negatiu ho convertim a positiu
  
  if (numeroFotos * pasosXfoto < pasosAmoure) {++numeroFotos;}    // Afegeix una foto mes (també un desplaçament) perquè com a mínim arribi o superi 
                                                                  // el límit selecionat com a final de tote les fotos. Com que al fer la divisió de
                                                                  // pasosAmoure / pasosXfoto molts cops no serà exacta i es despreciaran els decimals
                                                                  // llavors no arribaria al límit. Així la última foto serà al límit escollit o una 
                                                                  // mica mes enllà. Mai pot acabar la sequencia abans del limit seleccionat

  pasosAmoure = pasosXfoto;                                       // guardem els pasos que s'han de fer en cada moviment entre fotos
}

//============================================================================ FI =======================================================================




// Calcula els pasos a partir de la posició inicial i final    //////////////////////////////////////////////////////////////////////////////////////////
void calcul2 ()   
{
  switch (velMotor)
    {
      case 0:
        pasosAmoure = (pInici - pFinal) * 8;
        break;
      case 2:
        pasosAmoure = (pInici - pFinal) * 4;
        break;
      case 4:
        pasosAmoure = (pInici - pFinal) * 2;
        break;
      case 6:
        pasosAmoure = pInici - pFinal;
        break;
    }
  pasosXfoto = pasosAmoure / numeroFotos;         // Com que sabem les fotos (steps) a fer, calculem els pasos de motor x foto
  disTotal = pasosAmoure * micraXpas;             // Distància total del desplaçament (pasos a fer * les micres que fa cada pas)
  if (pasosXfoto <0) {pasosXfoto = pasosXfoto * -1;}   // com que no pot ser negatiu ho convertim a positiu
  pasosAmoure = pasosXfoto;                       // guardem els pasos de motor que s'han de fer en cada moviment entre fotos
}

//============================================================================ FI =======================================================================




// Calcula pasos a tornar per interrupció final de carrera    //////////////////////////////////////////////////////////////////////////////////////////
// 
void calcul3 ()   
{
  switch (velMotor)
    {
      case 0:
        pasosAmoure = 600;
        break;
      case 2:
        pasosAmoure = 1000;
        break;
      case 4:
        pasosAmoure = 1600;
        break;
      case 6:
        pasosAmoure = 2000;
        break;
    }
  //pasosXfoto = pasosAmoure / numeroFotos;         // Com que sabem les fotos (steps) a fer, calculem els pasos de motor x foto
  //disTotal = pasosAmoure * micraXpas;           // Distància total del desplaçament (pasos a fer * les micres que fa cada pas)
  
  //pasosAmoure = pasosXfoto;                       // guardem els pasos de motor que s'han de fer en cada moviment entre fotos
}

//============================================================================ FI =======================================================================




// Fa la foto segons paràmetres predefinits    //////////////////////////////////////////////////////////////////////////////////////////////////////////
void ferFoto ()   
{
  
  //for(int i=0; i <fotosPresa; i++)
  //{

  delay(Tespera);                               // Espera (milisegons) despres de moures i abans de fer la foto per evitar vibracions

  //digitalWrite(cameraFocus, HIGH);            // activa el focus
  //delay(Tpols);
  if (Bmirall == 1)                             // comprova si el bloqueix de mirall està actiu
  {
    digitalWrite(cameraDisparador, HIGH);       // activa el disparador per bloquejar el mirall
    delay(Tpols);
    digitalWrite(cameraDisparador, LOW);        // desactiva el disparador
    delay (Trepos);                             // espera un temps perqué desaparegui qualsevol vibració
  }
  digitalWrite(cameraDisparador, HIGH);         // activa el disparador
  delay(Tpols);
  //digitalWrite(cameraFocus, LOW);               // desactiva el focus
  digitalWrite(cameraDisparador, LOW);          // desactiva el disparador

  so0 ();                                       // avis sonor

  teclastop ();                                 // comprova si s'ha polsat la tecla STOP
  //}
  
}

//============================================================================ FI =======================================================================





// comprova si s'ha pulsat la tecla STOP mentres fa algun proces   //////////////////////////////////////////////////////////////////////////////////////
void teclastop ()                                    
{
  teclaPolsada = quinPolsador1 ();
  if(teclaPolsada == 3)
  {
      so2 (); //////// 
      delay (500);
      numeroFotos = 0;
  }
  
}

//============================================================================ FI =======================================================================





// rutina d'aturada quan hi ha hagut una inetrrupcióm i s'ha d'acabar un procés, normalment per interruptor final de carrera
//void stop ()
//{
  //aturar = 0;                     // s'efectua la aturada i la variable retorna al seu valor normal
  //so2 ();                         // emet so d'aturada
//}

//============================================================================ FI =======================================================================





void interrupcio ()
{
  
  tone (buz,1000,500);
  if (quinsentit == 0)
    {
      calcul3 ();
      recula (pasosAmoure,1);
    } 
    else
    {
      //pasosAmoure = 5000;
      calcul3 ();
      avanti (pasosAmoure,1);
    }
  aturar = 1; 
    tone (buz,1000,500);
                          // definir variable com a 1 pq s'aturi el procés quan es comprovi el seu valor
  //Serial.print(" aturar dins interrupcio");  Serial.println(aturar); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
  //Serial.println(); // sols per depurar           DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD 
}

//============================================================================ FI =======================================================================





// AVIS SONOR DE FOTO FETA    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void so0 ()     // ----------
{
  
      digitalWrite(buz, HIGH);
      delay(40);
      digitalWrite(buz, LOW);
}

//============================================================================ FI =======================================================================





// AVIS SONOR DE FI DE PROCES    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void so1 ()     // ----------
{
  for(int i=0; i<30; i++)                       
    {
      digitalWrite(buz, HIGH);
      delay(10);
      digitalWrite(buz, LOW);
      delay(15);
    }
}


//============================================================================ FI =======================================================================





// AVIS SONOR QUAN S'HA POLSAT STOP   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void so2 ()   
{
      digitalWrite(buz, HIGH);
      delay (500);
      digitalWrite(buz, LOW);
    
}

//============================================================================ FI =======================================================================






// AVIS SONOR QUANT S'HA POLSAT DISPARADOR///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void so3 ()     
{
      digitalWrite(buz, HIGH);
      delay (50);
      digitalWrite(buz, LOW);
      delay (50);
      digitalWrite(buz, HIGH);
      delay (50);
      digitalWrite(buz, LOW);
}

//============================================================================ FI =======================================================================




/*
// AVIS SONOR prova   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void so4 ()   
{
  /*int melody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(A5, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(A5);
  }*/

/*

      tone (buz,NOTE_C4,500);
      tone (buz,NOTE_G3,150);
      tone (buz,NOTE_A3,450);
      noTone(buz);

// NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
    
}
*/

//============================================================================ FI =======================================================================
