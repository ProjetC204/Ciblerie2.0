
void CL3(){ //------------------------------------------------------------------------------------
  Serial.println(F("CL3"));
  Serial.println(F("CL3 , Ok pour le lancement du jeu"));

  Serial1.println("START_GAME");
  Serial.println("📨 Envoi à ESP32: START_GAME");

  AcquisitionCapteurs();
  if (initialisation==true){InitGameL3();}     
  if (partieEnCours==true){trtPartieEnCoursL3();} 
  if (partieFinie==true){trtPartieFinieL3();}
}
//---------------------------------------------FIN LOOP-------------------------------

void trtPartieFinieL3(){
    FinGame();   
    while(killer[1]==1 || killer[2]==1 || killer[3]==1 || killer[4]==1 ){
      Serial.println("KILLER STATUS J1 " + String(killer[1]) + String(killer[2]) + String(killer[3]) + String(killer[4]));   
        for (int i = 1; i <= nbJoueurs ; i++){
          if (killer[i] == 1){
            joueurEnCours = i;
          }
        } 
    }
  partieFinie = false;
  partieEnCours = false;
  initialisation = true;
  oldNbJoueurs = 0;
  triclassement();
  AfficheFin();
}
// ---------------------------------FIN trtPartieFinie ----------------------------------
    
void trtPartieEnCoursL3(){

    FastLED.clear(); FastLED.show();
    Temporisation();
    EcranEnJeu();            
    while (statusCible1==HIGH && statusCible2==HIGH && statusCible3==HIGH && statusCible4==HIGH && statusCible5==HIGH && statusCible6==HIGH && statusCible7==HIGH && statusCible8==HIGH && statusCible9==HIGH && statusCible10==HIGH ){
      AcquisitionCapteurs();    
      PenaliteL3();     
      if (partieFinie==true){
        break;
        }
      else {      
          killer[joueurEnCours] = 0;
      }
      delay(1); // Ajout d'un délai minimal
    }

    statusCible1 = digitalRead(cible1);
    if (statusCible1 == LOW) {
      coursesCommencees = false;
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Red); FastLED.show();
      myDFPlayer.playMp3Folder(1); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours] + 0 + pointBonus0[joueurEnCours];
      pointBonus0[joueurEnCours] = pointBonus0[joueurEnCours] + 0;
      resetAllBonus0();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(0) + " : " + String((pointBonus0[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }

    statusCible2 = digitalRead(cible2);
    if (statusCible2 == LOW) {
      coursesCommencees = false;
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Orange); FastLED.show();
      myDFPlayer.playMp3Folder(2); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours] + 5 + pointBonus5[joueurEnCours];
      pointBonus5[joueurEnCours] = pointBonus5[joueurEnCours] + 1;
      resetAllBonus5();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(5) + " : " + String((pointBonus5[joueurEnCours]) -1)+ " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32 : " + message);
    }

    statusCible3 = digitalRead(cible3);
    if (statusCible3 == LOW) {
      coursesCommencees = false;
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Yellow); FastLED.show();
      myDFPlayer.playMp3Folder(3); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours] + 10 + pointBonus10[joueurEnCours];
      pointBonus10[joueurEnCours] = pointBonus10[joueurEnCours] + 2;
      resetAllBonus10();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(10) + " : " + String((pointBonus10[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }

    statusCible4 = digitalRead(cible4);
    if (statusCible4 == LOW){
      coursesCommencees = false; 
     coursesFinies = false;                              
      fill_solid( leds, NUM_LEDS, CRGB::Green);  FastLED.show();
      myDFPlayer.playMp3Folder(5); delay(3000);
      FastLED.clear (); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours]+15+pointBonus15[joueurEnCours];
      pointBonus15[joueurEnCours] = pointBonus15[joueurEnCours]+3;
      resetAllBonus15();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(15) + " : " + String((pointBonus15[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }  

    statusCible5 = digitalRead(cible5);
    if (statusCible5 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Blue);  FastLED.show();
      myDFPlayer.playMp3Folder(8); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours]+25+pointBonus25[joueurEnCours];
      pointBonus25[joueurEnCours] = pointBonus25[joueurEnCours]+4;
      resetAllBonus25();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(25) + " : " + String((pointBonus25[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);     
    }   

    statusCible6 = digitalRead(cible6);
    if (statusCible6 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      Cinquante();               
      scores[joueurEnCours] = scores[joueurEnCours]+50+pointBonus50[joueurEnCours];
      pointBonus50[joueurEnCours] = pointBonus50[joueurEnCours]+5;
      resetAllBonus50();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(50) + " : " + String((pointBonus50[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }

    statusCible7 = digitalRead(cible7);
    if (statusCible7 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      Cent();
      scores[joueurEnCours] = scores[joueurEnCours]+100+pointBonus100[joueurEnCours];
      pointBonus100[joueurEnCours] = pointBonus100[joueurEnCours]+10;
      resetAllBonus100();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(100) + " : " + String((pointBonus100[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }

    statusCible8 = digitalRead(cible8);
    if (statusCible8 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      CentCinquante();
      scores[joueurEnCours] = scores[joueurEnCours]+150+pointBonus150[joueurEnCours];
      pointBonus150[joueurEnCours] = pointBonus150[joueurEnCours]+15;
      resetAllBonus150();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(150) + " : " + String((pointBonus150[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }

    statusCible9 = digitalRead(cible9);
    if (statusCible9 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      DeuxCent();
      scores[joueurEnCours] = scores[joueurEnCours]+200+pointBonus200[joueurEnCours];
      pointBonus200[joueurEnCours] = pointBonus200[joueurEnCours]+20;
      resetAllBonus200();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(200) + " : " + String((pointBonus200[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }

    statusCible10 = digitalRead(cible10);
    if (statusCible10 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      DeuxCentCinquante();
      scores[joueurEnCours] = scores[joueurEnCours]+250+pointBonus250[joueurEnCours];
      pointBonus250[joueurEnCours] = pointBonus250[joueurEnCours]+25;
      resetAllBonus250();
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(250) + " : " + String((pointBonus250[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
           
    else {
      killer[joueurEnCours] = 0;
      }
  SMRAZ();                   
  GererInterruptionL3();  
}

void PenaliteL3() { 
  if (!coursesCommencees)
  {
    EcranEnJeu();
    myDFPlayer.playMp3Folder(19);
    lcd.setCursor(11,3);
    lcd.print("TR: ");
    coursesCommencees = true;
    // Réinitialisation des couleurs et états
    memset(colorIndexes, 0, sizeof(colorIndexes)); // Réinitialise tous les index de couleur
    memset(lastColorChanges, 0, sizeof(lastColorChanges));
    startTime = millis();
    tempsGenere = false;  // ✅ Réinitialise l'état pour la prochaine manche
  }
  if(!coursesFinies && coursesCommencees) {
    if (!tempsGenere) {
      tempstotalL = 1;  // ⏳ Temps 1 secondes
      Period1 = tempstotalL * 1000;
      Period2 = tempstotalL * 1000;
      tempsGenere = true;  // ✅ Marque que le temps a été généré
      // ✅ Affichage immédiat du temps restant pour éviter qu'il ne soit manquant
      lcd.setCursor(15,3);
      lcd.print(tempstotalL);
      // 🎨 Affichage du code couleur correspondant
      if (tempstotalL == 1) fill_solid(leds, NUM_LEDS, CRGB::Red); // 🔴 Rouge
      else if (tempstotalL >= 1 && tempstotalL <= 1) { 
        int index = tempstotalL - 1;
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]); // 🔴 Rouge ( Sens de décomptage )
        lastColorChanges[index] = millis();
      }
      FastLED.show();
      Moteur();  // ✅ Activation immédiate
      // ✅ Démarre le délai non bloquant pour éteindre les LEDs après 1 seconde
      ledsAllumeesMillis = millis();
      ledsAllumees = true;
    }
     // ✅ Éteindre les LEDs après 1 seconde sans bloquer le code
     if (ledsAllumees && millis() - ledsAllumeesMillis >= 900) {
      FastLED.clear(); FastLED.show(); // ⭕ Éteint les LEDs après 1 seconde
      ledsAllumees = false;
     }
    // ✅ Changement de couleur toutes les secondes si tempstotalL 1
    if (tempstotalL >= 1 && tempstotalL <= 1) {
      int index = tempstotalL - 1;
      if (millis() - lastColorChanges[index] >= 1000) {
        colorIndexes[index] = (colorIndexes[index] + 1) % colorSizes[index];
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]); // 🔴 Rouge ( Sens réel )
        FastLED.show();
        lastColorChanges[index] = millis();
      }
    }
     // ⏳ Temps écoulé, on passe au joueur/tour suivant
     lcd.setCursor(15,3);
     tempsrestant = tempstotalL - ( millis() - startTime) / 1000;
     if(tempsrestant < 0)
     {
      coursesFinies = true;
      SMRAZTP();
      lcd.clear();
      clignotementLED(1);
      lcd.setCursor(3, 1);
      lcd.print("Temps ecoule !");
      clignotementLED(1);
      myDFPlayer.playMp3Folder(1);
      clignotementLED(1);
      Messages();
      clignotementLED(1);
      fill_solid(leds, NUM_LEDS, CRGB::Red);  FastLED.show();
      delay(2000);
      lcd.clear();
      FastLED.clear(); FastLED.show();
      coursesCommencees = false;
      coursesFinies = false;
      // Remise à zéro des autres bonus
      resetAllBonus0();  // Appel à la fonction de réinitialisation
      SMRAZ();
      EcranEnJeu();
      GererInterruptionL3();
     }
     else lcd.print(tempsrestant); 
    }         
}

void GererInterruptionL3()
{ 
  if ( joueurEnCours == nbJoueurs3 && tourEnCours == nbTours && resteEnCours == 1 ){
    testKiller();
    coursesCommencees = false;
    coursesFinies = false;
    partieEnCours = false;
    partieFinie = true;
    SMRAZ();
  }
  else if (  joueurEnCours == nbJoueurs3 && resteEnCours == 1 )
  {
    myDFPlayer.stop();
    testKiller();
    joueurEnCours = 1;           
    resteEnCours = 3;
    tourEnCours++;
    EcranTourSuivant();    
    myDFPlayer.playMp3Folder(30);   
    EcranJoueurSuivant();  
    lcd.clear();
   while (statusBoutonE==HIGH){
        AcquisitionCapteurs();
        printBigNum(21, 13, 0);
        printBigNum(joueurEnCours,17,0);
        lcd.setCursor(0,0);
        lcd.print("Tour  : "+ String(tourEnCours));
        lcd.setCursor(0,2);
        lcd.print("Quand pret");         
        lcd.setCursor(8,3);
        lcd.print("Start --> OK");
        FastLED.clear(); FastLED.show();             
   }
   EcranGo();                      
  }
  else if ( resteEnCours != 1 ){
    resteEnCours--; 
  }  
  else
  {      
    testKiller();
    joueurEnCours++;
    resteEnCours = 3;
    EcranJoueurSuivant();   
    myDFPlayer.playMp3Folder(30);
    myDFPlayer.playMp3Folder(23);   
    lcd.clear();
   while (statusBoutonE==HIGH){
        AcquisitionCapteurs();      
        printBigNum(21, 13, 0);
        printBigNum(joueurEnCours,17,0);
        lcd.setCursor(0,0);
        lcd.print("Tour  : "+ String(tourEnCours));
        lcd.setCursor(0,2);
        lcd.print("Quand pret");        
        lcd.setCursor(8,3);
        lcd.print("Start --> OK");
        FastLED.clear(); FastLED.show();
   }
   EcranGo();    
  }
}

void EcranInitialisationL3()
{
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Nb joueurs : " + String(nbJoueurs3) + "      ");        
}

void InitGameL3(){

  FastLED.clear(); FastLED.show();
  myDFPlayer.stop();
  myDFPlayer.playMp3Folder(19);
  tourEnCours = 1;
  joueurEnCours = 1;
  resteEnCours = 3; 
  for(int i=1; i<=4; i++){
    scores[i] = 0;
    killer[1] = 0;
    levels[1] = 0;
    oldClassement[1] = 0;
    classement[1] = 0;
  }
  statusBoutonE = digitalRead(boutonE);
    
    if (nbJoueurs3 == 1){
       nbJoueurs3 = 1;
    }
    if (nbJoueurs3 != oldNbJoueurs){
      oldNbJoueurs = nbJoueurs3;
    }
    if (statusBoutonE==LOW){ 
        for (int i = 1; i <= nbJoueurs3 ; i++) {
          killer[i] = 1;         
       }
    }  
    initialisation = false;
    partieEnCours = true;
    EcranEnJeu();
    EcranGo();
}

// ----------------------------------------------------------------------------------------------
// ----------------AFFICHAGE FIN DE PARTIE
// ----------------------------------------------------------------------------------------------
