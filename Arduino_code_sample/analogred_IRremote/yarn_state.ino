void yarnStates :: yarn_state() {

  yarn_chance = count_yarn*100/count_total;
  torn = 0; dirt1 = 0; dirt2 = 0; flaw = 0;

  if (read_sens <= 100 || read_sens == 4095)
    flaw = 1;
  else if (read_sens <= 500 && read_sens > 100)
    dirt2 = 1;
  else if (yarn_chance < 10)
    torn = 1;
  else if (read_sens <= 1300 && read_sens > 500)
    dirt1 = 1;

  maxim = 0;
  minim = pow(2, res) - 1;
  count_yarn = 0;
  count_total =0;


}
