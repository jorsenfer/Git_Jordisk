

/*
=========================================================================================================================================================
  __  __  ______  _   _  _    _   _____ 
 |  \/  ||  ____|| \ | || |  | | / ____|
 | \  / || |__   |  \| || |  | || (___  
 | |\/| ||  __|  | . ` || |  | | \___ \ 
 | |  | || |____ | |\  || |__| | ____) |
 |_|  |_||______||_| \_| \____/ |_____/ 
                                        

El text de cada menú s'ha d'adaptar a la mida del display 5 línies x 16 caràcters.

Es gestiona amb 7 polsadors (+ o -/INTRO), (⬇︎ o ⬆︎), SELEC, MENU, FOTO
La tecla MENU retorna sempre al menú anterior
La tecla SELEC ens permet seleccionar entre les linies del menú
La tecla FOTO activa la càmera
La tecla + augmenta el valor de la dada seleccionada
La tecla -/INTRO disminueix el valor de la dada seleccionada. Com a tecla INTRO avança un nivell dins el menú o activa l'acció corresponent
La tecla ⬆︎ mou la plataforma cap amunt (si està horitzontal allunya la plataforma del motor)
La tecla ⬇︎ mou la plataforma cap avall (si està horitzontal apropa la plataforma cap al motor)
  
    
  1 Menú PRINCIPAL

    MODES
    CONFIGURAR
    Vel. Motor:      Ens permet seleccionar la velocitat a la qual és moura el motor de 1 a 4 (1 mes lent)


  2 Menú CONFIGURAR
    
    Retorn A.:
    #Fotos/P.:
    LCD Cont.:
    TEMPS FOTOS     Entrem en en el menú on ajustem els temps relacionats amb la presa de fotos


  3 Menú MODES

    DIST. AUTO.
    PAS AUTO.
    DIST. TOTAL
    DIST. DEL PAS
    PAS MANUAL


  4 Menú TEMPS FOTOS - CONFIGURAR

    Tespera:   
    TentreF:   
    T pols :  


  5 Menú MODES - DISTANCIA AUTOMÀTICA - calcula el nº de fotos (parades) a fer

    DdPas:      u
    Inici <>SELEC
    Final <>SELEC


  6 Menú MODES - PAS AUTOMÀTIC -- calcula la distància entre pasos (parades ó fotos)

    #Pasos:
    Inici <>SELEC
    Final <>SELEC


  7 Menú MODES - DISTANCIA TOTAL -- calcula la distància entre pasos (parades ó fotos)

    #Pasos:
    D T:       u
    polsar < o > 


  8 Menú MODES - DISTANCIA DEL PAS -- es mou segons paràmetres introduits

    #Pasos:
    Ddpas:      u
    polsar < o >


  9 Menú MODES - PAS MANUAL -- es mou una distància determinada al polsar la tecla AMINT o AVALL

    Ddpas:      u
    polsar < o >



/***** GESTIO DELS MENUS **************************************************************************************************/
/*++++++++++++++++++++++++++++++++ mostra en el display el menú corresponent +++++++++++++++++++++++++++++++++++++++++++++*/

void menu1 ()         // Menú PRINCIPAL
{
  liniaCursor = 8;                      // Linia on es situa el cursor en pixels, 8 es linia 2
  menuAnterior = 1;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  myGLCD.setFont(SmallFont);            // Defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("MODES", 7, 8);
  myGLCD.print("CONFIGURAR", 7, 16);
  myGLCD.print("Vel. Motor: ", 7, 24);
  myGLCD.printNumI(5-(velMotor/2+1), 74, 24, 1); // (valor, x, y, digits) el valor es mostra de 1 a 4 (normalment es 0,2,4ó6)
  myGLCD.print(">", 0, liniaCursor);    // cursor, per defecte a la primera linia (columna,fila (6x8))
  menuActual = 1;                       // memoritza el menu actiu
  liniaMaxima = 24;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 24 = linia 4
  liniaMinima = 8;                      // Linia primera on es pot situar el cursor en aquest menú
}


void menu2 ()         // Menú CONFIGURAR
{
  liniaCursor = 16;                     // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 1;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("-CONFIGURACIO-", CENTER, 0);
  myGLCD.print("Retorn A.: ", 6, 16);
  myGLCD.print(SiNo [autoRetorn], 71, 16);
  myGLCD.print("B. Mirall: ", 6, 24);
  myGLCD.print(SiNo [Bmirall], 71, 24);
  myGLCD.print("LCD Cont.: ", 6, 32);
  myGLCD.printNumI(LCDcontrast, 71, 32, 2, '0'); // (valor, x, y, digits), el valor es mostra de 0 a 99
  myGLCD.print("TEMPS FOTOS", 6, 40);
  myGLCD.print(">", 0, liniaCursor);    // cursor, per defecte a la primera linia (columna,fila (6x8))
  menuActual = 2;                       // memoritza el menu actiu
  liniaMaxima = 40;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 24 = linia 4
  liniaMinima = 16;                     // Linia primera on es pot situar el cursor en aquest menú, 16 = linia 3
}


void menu3 ()         // Menú MODES
{
  liniaCursor = 8;                      // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 1;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("-MODES-", CENTER, 0);
  myGLCD.print("DIST. AUTO.", 6, 8);
  myGLCD.print("PAS AUTO.", 6, 16);    
  myGLCD.print("DIST. TOTAL", 6, 24);
  myGLCD.print("DIST. DEL PAS", 6, 32);
  myGLCD.print("PAS MANUAL", 6, 40);
  myGLCD.print(">", 0, liniaCursor);    // cursor, per defecte a la primera linia (columna,fila (6x8))
  menuActual = 3;                       // memoritza el menu actiu
  liniaMaxima = 40;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 40 = linia 6
  liniaMinima = 8;                      // Linia primera on es pot situar el cursor en aquest menú, 8 = linia 2
  delay(tempsAntiRebotMenu);            // al venir-hi polsant la tecla INTRO pot haver rebot, així ho evitem
}


void menu4 ()         // Menú TEMPS FOTOS - CONFIGURAR
{
  liniaCursor = 16;                     // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 2;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("-TEMPS FOTOS-", CENTER, 0);
  myGLCD.print("Espera:    ms", 6, 16);
  myGLCD.printNumI(Tespera, 47, 16, 3,'0');
  myGLCD.print("Repos:     ms", 6, 24);
  myGLCD.printNumI(Trepos, 47, 24, 4,'0');
  myGLCD.print("Pols :     ms", 6, 32);
  myGLCD.printNumI(Tpols, 53, 32, 3,'0');
  myGLCD.print(">", 0, liniaCursor);
  menuActual = 4;                       // memoritza el menu actiu
  liniaMaxima = 32;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 32 = linia 5
  liniaMinima = 16;                     // Linia primera on es pot situar el cursor en aquest menú, 16 = linia 3
}


void menu5 ()         // Menú MODES - DISTANCIA AUTOMÀTICA - calcula el nº de fotos (parades) a fer
{
  liniaCursor = 16;                     // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 3;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("-DIST. AUTO.-", CENTER, 0);
  myGLCD.print("DdPas:      u", 6, 16);
  myGLCD.printNumI(DdPas, 53, 16, 3,'0');
  myGLCD.print("Inici <>SELEC", 6, 24);
  myGLCD.print("Final <>SELEC", 6, 32);
  myGLCD.print(">", 0, liniaCursor);
  menuActual = 5;                       // memoritza el menu actiu
  liniaMaxima = 32;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 32 = linia 5
  liniaMinima = 16;
  //pasosEndavant = 0;                    // Posa a 0 els contadors de pasos i de situació
  //pasosEndarrera = 0;
  //pActual = 0;
  pInici = pActual;                     // si no es mou el motor memoritza la posició actual, si fos 0 el moviment del motor entra en bucle
  pFinal = pActual;                     // si no es mou el motor memoritza la posició actual, si fos 0 el moviment del motor entra en bucle
  delay(tempsAntiRebotMenu);            // al venir-hi polsant la tecla INTRO pot haver rebot, així ho evitem
}


void menu6 ()         // Menú MODES - PAS AUTOMÀTIC -- calcula la distància entre pasos (parades ó fotos)
{
  liniaCursor = 16;                     // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 3;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  numeroFotos = 0;                      // Posa a 0 el número de fotos (passos, parades, etc.)
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("-PASOS AUTO.-", CENTER, 0);
  myGLCD.print("#Pasos: ", 6, 16);
  myGLCD.printNumI(numeroFotos, 53, 16, 3,'0');
  myGLCD.print("Inici <>SELEC", 6, 24);
  myGLCD.print("Final <>SELEC", 6, 32);
  myGLCD.print(">", 0, liniaCursor);
  menuActual = 6;                       // memoritza el menu actiu
  liniaMaxima = 32;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 32 = linia 5
  liniaMinima = 16;
  //pasosEndavant = 0;                    
  //pasosEndarrera = 0;
                                        // Posa a 0 els contadors
  pasosAmoure = 0;
  pInici = pActual;                     // si no es mou el motor memoritza la posició actual, si fos 0 el moviment del motor entra en bucle
  pFinal = pActual;                     // si no es mou el motor memoritza la posició actual, si fos 0 el moviment del motor entra en bucle
  delay(tempsAntiRebotMenu);            // al venir-hi polsant la tecla INTRO pot haver rebot, així ho evitem
}


void menu7 ()         // Menú MODES - DISTANCIA TOTAL -- calcula la distància entre pasos (parades ó fotos)
{
  liniaCursor = 16;                     // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 3;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  numeroFotos = 0;                      // Posa a 0 el número de fotos (passos, parades, etc.)
  disTotal = 0;                         // Posa a 0 la distància que s'ha de moure el motor
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("-DIST. TOTAL-", CENTER, 0);
  myGLCD.print("#Pasos: ", 6, 16);
  myGLCD.printNumI(numeroFotos, 53, 16, 3,'0');
  myGLCD.print("D T:       u", 6, 24);
  myGLCD.printNumI(disTotal, 41, 24, 5,'0');
  myGLCD.print("polsar < o >", 6, 32);
  myGLCD.print(">", 0, liniaCursor);
  menuActual = 7;                       // memoritza el menu actiu
  liniaMaxima = 32;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 32 = linia 5
  liniaMinima = 16;
}


void menu8 ()         // Menú MODES - DISTANCIA DEL PAS -- es mou segons paràmetres introduits
{
  liniaCursor = 16;                     // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 3;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  numeroFotos = 0;                      // Posa a 0 el número de fotos (passos, parades, etc.)
  disTotal = 0;                         // Posa a 0 la distància que s'ha de moure el motor
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("DIST. DEL PAS", CENTER, 0);
  myGLCD.print("#Pasos: ", 6, 16);
  myGLCD.printNumI(numeroFotos, 53, 16, 3,'0');
  myGLCD.print("Ddpas:      u", 6, 24);
  myGLCD.printNumI(DdPas, 53, 24, 4, '0');
  myGLCD.print("polsar < o >", 6, 32);
  myGLCD.print(">", 0, liniaCursor);
  menuActual = 8;                       // memoritza el menu actiu
  liniaMaxima = 32;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 32 = linia 5
  liniaMinima = 16;
}


void menu9 ()         // Menú MODES - PAS MANUAL -- es mou una distància determinada al polsar una tecla
{
  liniaCursor = 16;                     // Linia on es situa el cursor en pixels, 8 es linia 3
  menuAnterior = 3;                     // Menú al qual s'ha de tornar al polsar la tecla MENU
  myGLCD.setFont(SmallFont);            // defineix tipus de font
  myGLCD.clrScr();                      // Borrar pantalla
  myGLCD.print("-PAS MANUAL-", CENTER, 0);
  myGLCD.print("Ddpas:      u", 6, 16);
  myGLCD.printNumI(DdPas, 53, 16, 3, '0');
  myGLCD.print("polsar < o >", 6, 24);
  myGLCD.print(">", 0, liniaCursor);
  menuActual = 9;                       // memoritza el menu actiu
  liniaMaxima = 24;                     // Última linia on es pot desplaçar el cursor en aquest menú en pixels, 32 = linia 5
  liniaMinima = 16;
}


/*  no implementat encara
void menu20 ()         // Menú principal - TIME LAPSE
{
  menuAnterior = 1;                 // Menú al qual s'ha de tornar al polsar la tecla MENU
} */


void tornarMenu ()      // Retorna al menú anterior
{
  switch (menuAnterior)
  {
      case 1:
        menu1 ();
        break;
      case 2:
        menu2 ();
        break;
      case 3:
        menu3 ();
        break;
      case 5:
        menu5 ();
        break;
      case 6:
        menu6 ();
        break;
      case 7:
        menu7 ();
        break;
      case 8:
        menu8 ();
        break;
      case 9:
        menu9 ();
      break;
  }

}


