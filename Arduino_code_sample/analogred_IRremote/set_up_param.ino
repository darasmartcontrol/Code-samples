void ledStates :: setup_param(int interval, byte rd, byte gr, byte bl, int com_del) {
  led_intrv = interval;
  red = rd;
  green = gr;
  blue = bl;
  command_delay = com_del;
}
