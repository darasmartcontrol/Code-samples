void yarnStates :: resetCalibration() {

  maxim = 0;
  minim = pow(2, res) - 1;
  per = 0;

  thresh = max(thresh, maxper);
  maxper = 0;
}
