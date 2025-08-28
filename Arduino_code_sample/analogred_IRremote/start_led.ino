void ledStates ::  start_led(int stat) {


  if ( tim_now - axtim_command_delay > command_delay) {
    if ( tim_now - axtim > led_intrv) {
      boolInd = !boolInd; //boolInd is defined to provide blink
      // dirt1 alarm has different behavior
      if (stat == 1) {
        pink++;
        if (pink >= 20) {
          axtim_command_delay = tim_now;
          pink = 0;
          boolInd = 0;
        }
      }
      axtim = tim_now;

      digitalWrite (led_regl, HIGH);
      delay(20);
      led(stat);
    }
  }
}
