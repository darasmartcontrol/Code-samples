void yarnStates :: calibration(int i) {
  read_sens = analogRead(rec_sens[i]);
  maxim = max(read_sens, maxim);
  minim = min(read_sens, minim);
  per = maxim - minim;
  maxper = max(per, maxper);
}
