
void Penalite() {
  if (!coursesCommencees)
  {
    EcranEnJeu();
    myDFPlayer.playMp3Folder(19);
    lcd.setCursor(11,3);
    lcd.print("TR: ");
    coursesCommencees = true;
//    Serial.println("coursesCommencees");
    // Réinitialisation des couleurs et états
    memset(colorIndexes, 0, sizeof(colorIndexes)); // Réinitialise tous les index de couleur
    memset(lastColorChanges, 0, sizeof(lastColorChanges));
    startTime = millis();
    tempsGenere = false;  // ✅ Réinitialise l'état pour la prochaine manche
  }
  if(!coursesFinies && coursesCommencees) {
    if (!tempsGenere) {
      tempstotalF = 5;  // ⏳ Temps 5 secondes
      Period1 = tempstotalF * 1000;
      Period2 = tempstotalF * 1000;
      tempsGenere = true;  // ✅ Marque que le temps a été généré
      // ✅ Affichage immédiat du temps restant pour éviter qu'il ne soit manquant
      lcd.setCursor(15,3);
      lcd.print(tempstotalF);
      // 🎨 Affichage du code couleur correspondant
      if (tempstotalF == 1) fill_solid(leds, NUM_LEDS, CRGB::Red); // 🔴 Rouge
      else if (tempstotalF >= 2 && tempstotalF <= 5) { 
        int index = tempstotalF -2;
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]); // 🟠 Orange <- 🟡 Jaune <- 🟢 Vert <- 🔵 Bleu ( Sens de décomptage )
        lastColorChanges[index] = millis();
      }
      FastLED.show();
      Moteur();  // ✅ Activation immédiate
      // ✅ Démarre le délai non bloquant pour éteindre les LEDs après 1 seconde
      ledsAllumeesMillis = millis();
      ledsAllumees = true;
    }
     // ✅ Éteindre les LEDs après 1 seconde sans bloquer le code
     if (ledsAllumees && millis() - ledsAllumeesMillis >= 900) { //500) {
      FastLED.clear(); FastLED.show(); // ⭕ Éteint les LEDs après 1 seconde
      ledsAllumees = false;
     }
    // ✅ Changement de couleur toutes les secondes si tempstotalF entre 2 et 5
    if (tempstotalF >= 2 && tempstotalF <= 5) {
      int index = tempstotalF - 2;
      if (millis() - lastColorChanges[index] >= 1000) {
        colorIndexes[index] = (colorIndexes[index] + 1) % colorSizes[index];
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]); //  🔵 Bleu -> 🟢 Vert -> 🟡 Jaune -> 🟠 Orange ( Sens réel )
        FastLED.show();
        lastColorChanges[index] = millis();
      }
    }
     // ⏳ Temps écoulé, on passe au joueur/tour suivant
     lcd.setCursor(15,3);
     tempsrestant = tempstotalF - ( millis() - startTime) / 1000;
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
      GererInterruption();  // 🔄 Passe au joueur suivant immédiatement 
     }
     else lcd.print(tempsrestant);
    }         
}

void GererInterruption() {
  Serial.println("GererInterruption - Debut");

  Serial.print("joueurEnCours: "); Serial.println(joueurEnCours);
  Serial.print("tourEnCours: "); Serial.println(tourEnCours);
  Serial.print("resteEnCours: "); Serial.println(resteEnCours);
  Serial.print("nbJoueurs: "); Serial.println(nbJoueurs);
  Serial.print("nbTours: "); Serial.println(nbTours);

  if (joueurEnCours == nbJoueurs && tourEnCours == nbTours && resteEnCours == 1) {
    Serial.println("GererInterruption - Fin de partie");
    testKiller();
    coursesCommencees = false;
    coursesFinies = false;
    partieEnCours = false;
    partieFinie = true;
    SMRAZ();
  }
  else if (joueurEnCours == nbJoueurs && resteEnCours == 1) {
    Serial.println("GererInterruption - Nouveau tour");
    myDFPlayer.stop();
    testKiller();
    joueurEnCours = 1;           
    resteEnCours = 3;
    tourEnCours++;
    Serial.print("Nouveau tourEnCours: "); Serial.println(tourEnCours);
    EcranTourSuivant();    
    myDFPlayer.playMp3Folder(30);   
    EcranJoueurSuivant();
    // === ENVOI "NEXT_TURN" ===
    Serial1.println("NEXT_TURN");
    Serial.println("📤 Envoi à ESP32: NEXT_TURN");
    lcd.clear();
    // Attente du bouton physique OU virtuel
    while (digitalRead(boutonE) == HIGH && !boutonVirtuelE) {
      AcquisitionCapteurs();        
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0,0);
      lcd.print("Tour  : " + String(tourEnCours));
      lcd.setCursor(0,2);
      lcd.print("Quand pret");         
      lcd.setCursor(8,3);
      lcd.print("Start --> OK");
      FastLED.clear(); FastLED.show();
      communications();
      delay(1);
    }
    boutonVirtuelE = false; // Reset du bouton virtuel après validation
    EcranGo();
    Serial.println("GO_TS");                  
  }
  else if (resteEnCours != 1) {
    Serial.println("GererInterruption - Reste en cours");
    resteEnCours--; 
  }
  else {
    Serial.println("GererInterruption - Nouveau joueur");
    testKiller();
    joueurEnCours++;
    resteEnCours = 3;
    EcranJoueurSuivant();   
    myDFPlayer.playMp3Folder(30);
    myDFPlayer.playMp3Folder(23);   
    lcd.clear();
    // === ENVOI "NEXT_PLAYER" ===
    Serial1.println("NEXT_PLAYER");
    Serial.println("📤 Envoi à ESP32: NEXT_PLAYER");

    // Attente du bouton physique OU virtuel
    while (digitalRead(boutonE) == HIGH && !boutonVirtuelE) {
      AcquisitionCapteurs();             
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0,0);
      lcd.print("Tour  : " + String(tourEnCours));
      lcd.setCursor(0,2);
      lcd.print("Quand pret");        
      lcd.setCursor(8,3);
      lcd.print("Start --> OK");
      FastLED.clear(); FastLED.show();
      communications();
      delay(1);
    }
    boutonVirtuelE = false; // Reset du bouton virtuel après validation
    EcranGo();
    Serial.println("GO_JS");   
  }

  Serial.println("GererInterruption - Fin");
  // Ajout de l'appel à tous les groupes
  lcd.clear();
  // === Ici, appel dynamique du mode courant ===
  if (groupeSelectionne.startsWith("CF")) {
    if (groupeSelectionne == "CF1") { CF1(); }
    else if (groupeSelectionne == "CF2") { CF2(); }
    else if (groupeSelectionne == "CF3") { CF3(); }
    else if (groupeSelectionne == "CF4") { CF4(); }
  } 
  else if (groupeSelectionne.startsWith("CSP")) {
    if (groupeSelectionne == "CSP1") { CSP1(); }
    else if (groupeSelectionne == "CSP2") { CSP2(); }
    else if (groupeSelectionne == "CSP3") { CSP3(); }
    else if (groupeSelectionne == "CSP4") { CSP4(); }
  } 
  else if (groupeSelectionne.startsWith("CP")) {
    if (groupeSelectionne == "CP1") { CP1(); }
    else if (groupeSelectionne == "CP2") { CP2(); }
    else if (groupeSelectionne == "CP3") { CP3(); }
    else if (groupeSelectionne == "CP4") { CP4(); }
  } 
  else if (groupeSelectionne.startsWith("CC")) {
    if (groupeSelectionne == "CC1") { CC1(); }
    else if (groupeSelectionne == "CC2") { CC2(); }
    else if (groupeSelectionne == "CC3") { CC3(); }
    else if (groupeSelectionne == "CC4") { CC4(); }
  } 
  else if (groupeSelectionne.startsWith("CL")) {
    if (groupeSelectionne == "CL1") { CL1(); }
    else if (groupeSelectionne == "CL2") { CL2(); }
    else if (groupeSelectionne == "CL3") { CL3(); }
    else if (groupeSelectionne == "CL4") { CL4(); }
  } 
  else if (groupeSelectionne.startsWith("LJ")) {
    if (groupeSelectionne == "LJ1") { LJ1(); }
    else if (groupeSelectionne == "LJ2") { LJ2(); }
    else if (groupeSelectionne == "LJ3") { LJ3(); }
    else if (groupeSelectionne == "LJ4") { LJ4(); }
  } 
  else if (groupeSelectionne.startsWith("MS")) {
    if (groupeSelectionne == "MS1") { MS1(); }
    else if (groupeSelectionne == "MS2") { MS2(); }
    else if (groupeSelectionne == "MS3") { MS3(); }
    else if (groupeSelectionne == "MS4") { MS4(); }
  } 
  else if (groupeSelectionne == "CPT1") {
    CPT1(); 
  }
  else {
    Serial.println("Groupe non reconnu !");
  }
}

// 📨 📥
void envoyerMessage(String message) {
  Serial1.println(message);
  Serial.print("📨 Message envoyé à ESP32 : ");
  Serial.println(message);
}

void communications() {
  if (Serial1.available() > 0) {
    String messageLu = Serial1.readStringUntil('\n');
    messageLu.trim();

    Serial.print("📥 Message reçu de l'ESP32 (brut) : ");
    Serial.println(messageLu); // Affiche le message brut

    char messageRecu[32]; // Tableau pour stocker le message reçu
    messageLu.toCharArray(messageRecu, 32);
    messageRecu[messageLu.length()] = '\0';

    Serial.print("📥 Message reçu de l'ESP32 : ");
    Serial.println(messageRecu);

    // 1. Bouton virtuel : SORTIE IMMÉDIATE
    if (strcmp(messageRecu, "VALID_E") == 0) {
      boutonVirtuelE = true;
      Serial.println("Validation bouton E reçue à distance !");
      return; // ←←← Ajout essentiel : on sort tout de suite de la fonction
    }

    // 2. Traitement d’un nom de groupe (valide uniquement)
    if (messageRecu[0] != '\0'
        && (strcmp(messageRecu, "CF1") == 0 || strcmp(messageRecu, "CF2") == 0 || strcmp(messageRecu, "CF3") == 0 || strcmp(messageRecu, "CF4") == 0
            || strcmp(messageRecu, "CSP1") == 0 || strcmp(messageRecu, "CSP2") == 0 || strcmp(messageRecu, "CSP3") == 0 || strcmp(messageRecu, "CSP4") == 0
            || strcmp(messageRecu, "CP1") == 0 || strcmp(messageRecu, "CP2") == 0 || strcmp(messageRecu, "CP3") == 0 || strcmp(messageRecu, "CP4") == 0
            || strcmp(messageRecu, "CC1") == 0 || strcmp(messageRecu, "CC2") == 0 || strcmp(messageRecu, "CC3") == 0 || strcmp(messageRecu, "CC4") == 0
            || strcmp(messageRecu, "CL1") == 0 || strcmp(messageRecu, "CL2") == 0 || strcmp(messageRecu, "CL3") == 0 || strcmp(messageRecu, "CL4") == 0
            || strcmp(messageRecu, "LJ1") == 0 || strcmp(messageRecu, "LJ2") == 0 || strcmp(messageRecu, "LJ3") == 0 || strcmp(messageRecu, "LJ4") == 0
            || strcmp(messageRecu, "MS1") == 0 || strcmp(messageRecu, "MS2") == 0 || strcmp(messageRecu, "MS3") == 0 || strcmp(messageRecu, "MS4") == 0
            || strcmp(messageRecu, "CPT1") == 0)) {
      groupeSelectionne = String(messageRecu);
      Serial.print("Groupe sélectionné : ");
      Serial.println(groupeSelectionne);
      Serial.print("Groupe enregistré : ");
      Serial.println(groupeSelectionne);
      Serial1.println("START"); // Active le bouton "Attente groupe"
      Serial.print("📨 Message envoyé à ESP32 : ");
      Serial.println("START");
    } 
    else if (strcmp(messageRecu, "START_GAME") == 0 && !partieDemarree) {
      Serial.println("Message reçu : START_GAME");
      Serial.print("Partie démarrée avec le groupe : ");
      Serial.println(groupeSelectionne);

      if (groupeSelectionne == "CF1") { CF1(); }
      else if (groupeSelectionne == "CF2") { CF2(); }
      else if (groupeSelectionne == "CF3") { CF3(); }
      else if (groupeSelectionne == "CF4") { CF4(); }
      else if (groupeSelectionne == "CSP1") { CSP1(); }
      else if (groupeSelectionne == "CSP2") { CSP2(); }
      else if (groupeSelectionne == "CSP3") { CSP3(); }
      else if (groupeSelectionne == "CSP4") { CSP4(); }
      else if (groupeSelectionne == "CP1") { CP1(); }
      else if (groupeSelectionne == "CP2") { CP2(); }
      else if (groupeSelectionne == "CP3") { CP3(); }
      else if (groupeSelectionne == "CP4") { CP4(); }
      else if (groupeSelectionne == "CC1") { CC1(); }
      else if (groupeSelectionne == "CC2") { CC2(); }
      else if (groupeSelectionne == "CC3") { CC3(); }
      else if (groupeSelectionne == "CC4") { CC4(); }
      else if (groupeSelectionne == "CL1") { CL1(); }
      else if (groupeSelectionne == "CL2") { CL2(); }
      else if (groupeSelectionne == "CL3") { CL3(); }
      else if (groupeSelectionne == "CL4") { CL4(); }
      else if (groupeSelectionne == "LJ1") { LJ1(); }
      else if (groupeSelectionne == "LJ2") { LJ2(); }
      else if (groupeSelectionne == "LJ3") { LJ3(); }
      else if (groupeSelectionne == "LJ4") { LJ4(); }
      else if (groupeSelectionne == "MS1") { MS1(); }
      else if (groupeSelectionne == "MS2") { MS2(); }
      else if (groupeSelectionne == "MS3") { MS3(); }
      else if (groupeSelectionne == "MS4") { MS4(); }
      else if (groupeSelectionne == "CPT1") { CPT1(); }
      else {
        Serial.println("Groupe non reconnu !");
      }
      partieDemarree = true;
    } 
    // 3. Gestion Joueur suivant et Tour suivant (identique)
    else if ((strcmp(messageRecu, "NEXT_PLAYER") == 0 || strcmp(messageRecu, "NEXT_TURN") == 0) && partieDemarree) {
      Serial.print("📥 Message reçu de l'ESP32 : ");
      Serial.println(messageRecu);
      Serial1.println("GO");
      Serial.print("📨 Message envoyé à ESP32 : ");
      Serial.println("GO");
    } 
    else if (strcmp(messageRecu, "FIN_GAME") == 0 && partieDemarree) {
      Serial.println("📥 Message reçu de l'ESP32 : FIN_GAME");
      partieDemarree = false;
    }
  }
}
