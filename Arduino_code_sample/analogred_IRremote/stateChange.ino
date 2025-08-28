void ledStates ::  stateChange() {


  axtim = tim_now;
  axtim_command_delay = tim_now;
  ind = num_led - 1;
  direc = "backw";
  for (int j = num_led - 1; j >= 0; j--) {
    strip1.setPixelColor(j, strip1.Color(0, 0, 0));
    strip2.setPixelColor(j, strip2.Color(0, 0, 0));
    strip1.show();
    strip2.show();
  }

}
