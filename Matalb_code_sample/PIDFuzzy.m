% function [kp, kd, ki] = PIDFuzzy(Er, ErDot)
Er=.2;ErDot=.3;
Er = abs(Er);
ErDot = abs(ErDot);
ErRange = [0 1/3 2/3 1 10];
aE = [ErRange(1) ErRange(1) ErRange(2) ErRange(3)];
bE = [ErRange(1) ErRange(2) ErRange(3) ErRange(4)];
cE = [ErRange(2) ErRange(3) ErRange(4) ErRange(5)];
 for i=1:4
     muE(i) = max(0,min( (Er-aE(i))/(bE(i)-aE(i)),(cE(i)-Er)/(cE(i)-bE(i)) ) );
 end
 
ErDotRange = [0 1/3 2/3 1 10];
aED = [ErDotRange(1) ErDotRange(1) ErDotRange(2) ErDotRange(3)];
bED = [ErDotRange(1) ErDotRange(2) ErDotRange(3) ErDotRange(4)];
cED = [ErDotRange(2) ErDotRange(3) ErDotRange(4) ErDotRange(5)];
 for i=1:4
     muED(i) = max(0,min( (ErDot-aED(i))/(bED(i)-aED(i)),(cED(i)-ErDot)/(cED(i)-bED(i)) ) );
 end
 

 %kpp
 muSkp =max([ min(muE(1),muED(4)) min(muE(1),muED(3)) min(muE(1),muED(2)) min(muE(2),muED(4)) ...
     min(muE(2),muED(3)) min(muE(3),muED(4))]);
 muBkp = max([min(muE(4),muED(4)) min(muE(4),muED(3)) min(muE(4),muED(2)) min(muE(4),muED(1))...
     min(muE(3),muED(3)) min(muE(3),muED(2)) min(muE(3),muED(1)) min(muE(2),muED(2))...
     min(muE(2),muED(1)) min(muE(1),muED(1))]);
 
 %kdp
 muBkd =max([ min(muE(1),muED(4)) min(muE(1),muED(3)) min(muE(1),muED(2)) min(muE(2),muED(4)) ...
     min(muE(2),muED(3)) min(muE(3),muED(4))]);
 muSkd = max([min(muE(4),muED(4)) min(muE(4),muED(3)) min(muE(4),muED(2)) min(muE(4),muED(1))...
     min(muE(3),muED(3))  min(muE(3),muED(2)) min(muE(3),muED(1)) min(muE(2),muED(2)) min(muE(2),muED(1)) min(muE(1),muED(1))]);
 
 kpp = PIDCoeff(muSkp, muBkp);
 kdp = PIDCoeff(muSkd, muBkd);
 
 %Alpha
 
 muAlph2 = max([min(muE(4),muED(4)) min(muE(4),muED(3)) min(muE(4),muED(2)) min(muE(4),muED(1))  ...
    min(muE(3),muED(3)) min(muE(3),muED(2)) min(muE(3),muED(1)) min(muE(2),muED(2)) min(muE(2),muED(1)) min(muE(1),muED(1))]);
 muAlph3 = max([min(muE(3),muED(4))  ...
      min(muE(2),muED(3))  min(muE(1),muED(2))]);
 muAlph4 = max([min(muE(2),muED(4)) min(muE(1),muED(3))]);
 muAlph5 = min(muE(1),muED(4));
 
 A = muAlph2+muAlph3+muAlph4+muAlph5;
 B = 2*muAlph2+ 3*muAlph3+ 4*muAlph4+ 5*muAlph5;
 
 Alpha = B/A;
 
 ku = 2;
 Tu = 3.7;
 
 kpMin = 0.32*ku;
 kpMax = 0.6*ku;
 kdMin = 0.08*ku*Tu;
 kdMax = 0.15*ku*Tu;
 
 kp = kpMin +(kpMax-kpMin)*kpp;
 kd = kdMin +(kdMax-kdMin)*kdp;
 ki = kp^2/(Alpha*kd);
 
 
 kpp
 kdp
 Alpha