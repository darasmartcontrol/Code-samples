void yarnStates :: read_thresh_perturb(int i) {
  //Set the thereshold parameter according to light intensity
  if ( read_sens <= 900)
    threshold = thresh + 5;
  else if ( read_sens <= 1500 )
    threshold = thresh + 8;
  else
    threshold = thresh + 10;
//
  read_sens = analogRead(rec_sens[i]);
  maxim = max(read_sens, maxim);
  minim = min(read_sens, minim);
  per = maxim - minim;
  maxper = max(per, maxper);
  count_total++;
  if (per > threshold)
    count_yarn++;  //compute abondance of number of perturbations higher than threshold (computed in calibration) 
}
