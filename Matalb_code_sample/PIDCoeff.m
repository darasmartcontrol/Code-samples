function k = PIDCoeff(muS, muB)
 %%Creat functions to compute F1 = int(y1.dx), F2 = int(y2.dx), Fh1 = int(x.y1dx),
 %%Fh2 = int(x.y2dx)
F1 = @(x) -1/4*(x*log(x)-x);
F2 = @(x) -1/4*((x-1)*log(1-x)-x);
Fh1 = @(x) -1/4*(x^2/2*log(x)-x^2/4);
Fh2 = @(x) -1/8*(log(1-x)*(x^2-1)-(x+x^2/2));
A =0;B =0;
if muS>= -1/4*log(0.5)
    x1 = exp(-4*muS);
    y1 = muS;
    x2 = 0.5;
    y2 = -1/4*log(0.5);
    x3 = 1-exp(-4*muB);
    y3 = muB;
    x4 = 1;
    
    A1 = x1*y1;
    B1 = A1*x1/2;
    A2 = F1(x2)-F1(x1);
    B2 = Fh1(x2)-Fh1(x1);
    A3 = F2(x3)-F2(x2);
    B3 = Fh2(x3)-Fh2(x2);
    A4 = (x4-x3)*y3;
    B4 = A4*(x4+x3)/2;
    
    A = A1+A2+A3+A4;
    B = B1+B2+B3+B4;
end
if (muS>=-1/4*log(0.5) && muB<-1/4*log(0.5)) || (muS<-1/4*log(0.5) && muB<muS)
    x1 = exp(-4*muS);
    y1 = muS;
    x2 = exp(-4*muB);
    y2 = muB;
    x3 = 1;
    y3 = muB;
    
    A1 = x1*y1;
    B1 = A1*x1/2;
    A2 = F1(x2)-F1(x1);
    B2 = Fh1(x2)-Fh1(x1);
    A3 = (x3-x2)*y2;
    B3 = A3*(x2+x3)/2;
    
    A = A1+A2+A3;
    B = B1+B2+B3;
end
if muS<-1/4*log(0.5) && muB>=muS
    x1 = 1-exp(-4*muS);
    y1 = muS;
    x2 = 1-exp(-4*muB);
    y2 = muB;
    x3 = 1;
    y3 = muB;
    
    A1 = x1*y1;
    B1 = A1*x1/2;
    A2 = F2(x2)-F2(x1);
    B2 = Fh2(x2)-Fh2(x1);
    A3 = (x3-x2)*y2;
    B3 = A3*(x3+x2)/2;
    
    
    A = A1+A2+A3;
    B = B1+B2+B3;
end
if A==0
    k=0;
else
    k = B/A;
end
end