
void Penalite(int numeroDuMode) {
  Serial.print("numeroDuMode = "); Serial.println(numeroDuMode);

  int tempstotal = 5; // Valeur par défaut (CF)
  int ledTimeout = 900;

  switch (numeroDuMode) {
    case 1: // CF
      tempstotal = 5;
      ledTimeout = 900;
      break;
    case 2: // CSP
      tempstotal = 4;
      ledTimeout = 500;
      break;
    case 3: // CP
      tempstotal = 3;
      ledTimeout = 500;
      break;
    case 4: // CC
      tempstotal = 2;
      ledTimeout = 500;
      break;
    case 5: // CL
      tempstotal = 1;
      ledTimeout = 900;
      break;
    default:
      // Gérer le cas où le mode est inconnu
      tempstotal = 5;
      ledTimeout = 900;
      break;
  }

  if (!coursesCommencees) {
    EcranEnJeu();
    myDFPlayer.playMp3Folder(19);
    lcd.setCursor(11,3); lcd.print("TR: ");
    coursesCommencees = true;
    memset(colorIndexes, 0, sizeof(colorIndexes));
    memset(lastColorChanges, 0, sizeof(lastColorChanges));
    startTime = millis();
    tempsGenere = false;
  }
  if (!coursesFinies && coursesCommencees) {
    if (!tempsGenere) {
      Period1 = tempstotal * 1000;
      Period2 = tempstotal * 1000;
      tempsGenere = true;
      lcd.setCursor(15,3); lcd.print(tempstotal);
      if (tempstotal == 1) fill_solid(leds, NUM_LEDS, CRGB::Red);
      else if (tempstotal >= 2 && tempstotal <= 5) {
        int index = tempstotal - 2;
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]);
        lastColorChanges[index] = millis();
      }
      FastLED.show();
      Moteur();
      ledsAllumeesMillis = millis();
      ledsAllumees = true;
    }
    if (ledsAllumees && millis() - ledsAllumeesMillis >= ledTimeout) {
      FastLED.clear(); FastLED.show();
      ledsAllumees = false;
    }
    if (tempstotal >= 2 && tempstotal <= 5) {
      int index = tempstotal - 2;
      if (millis() - lastColorChanges[index] >= 1000) {
        colorIndexes[index] = (colorIndexes[index] + 1) % colorSizes[index];
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]);
        FastLED.show();
        lastColorChanges[index] = millis();
      }
    }
    lcd.setCursor(15,3);
    tempsrestant = tempstotal - ( millis() - startTime ) / 1000;
    if (tempsrestant < 0) {
      coursesFinies = true;
      SMRAZTP();
      lcd.clear();
      clignotementLED(1);
      lcd.setCursor(3, 1); lcd.print("Temps ecoule !");
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
      resetAllBonus0();
      SMRAZ();
      EcranEnJeu();
      GererInterruption();
    } 
    else {
      lcd.print(tempsrestant);
    }
  }
}