
void ledStates ::  led(int stat) {
    
  //  Serial1.println(direc);
  // stat 0 refers to LED off mode, stat 1 & 2 refer to dirt modes, stat 3 is  flaw mode, 4 is torn mode, 5 is outcircuit,

  if (stat == 4 || stat == 7) {
    if (direc == "backw") {
      strip1.setPixelColor(ind, red, green, blue);
      strip2.setPixelColor(ind, red, green, blue);
      strip1.show();
      strip2.show();
    }
    else {
      strip1.setPixelColor(ind, 0, 0, 0);
      strip2.setPixelColor(ind, 0, 0, 0);
      strip1.show();
      strip2.show();
    }
    if (direc == "forw" && ind == num_led - 1 )
      direc = "backw";
    else if (direc == "backw" && ind == 0)
      direc = "forw";

    else if (direc == "backw")
      ind--;
    else
      ind++;

  }

  else  {
    for (int j = num_led - 1; j >= 0; j--) {
      if (boolInd) {
        strip1.setPixelColor(j, red, green, blue);
        strip2.setPixelColor(j, red, green, blue);
        strip1.show();
        strip2.show();
      }
      else {
        strip1.setPixelColor(j, 0, 0, 0);
        strip2.setPixelColor(j, 0, 0, 0);
        strip1.show();
        strip2.show();
      }
    }
  }

}
