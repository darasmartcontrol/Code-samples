function [OmegR, OmegL] = MyWrittenFuzzyImproved(Dist, Ang)
% Dist=1;Ang=1;
DistRange = [0 10 40 80 200];
aD = [DistRange(1) DistRange(1) DistRange(2) DistRange(3)];
bD = [DistRange(1) DistRange(2) DistRange(3) DistRange(4)];
cD = [DistRange(2) DistRange(3) DistRange(4) DistRange(5)];
 for i=1:4
     muDist(i) = max(0,min( (Dist-aD(i))/(bD(i)-aD(i)),(cD(i)-Dist)/(cD(i)-bD(i)) ) );
 end
 
AngRange = [-pi -pi/2 -pi/10 0 pi/10 pi/2 pi];
aA = [AngRange(1) AngRange(2) AngRange(3) AngRange(4) AngRange(5)];
bA = [AngRange(2) AngRange(3) AngRange(4) AngRange(5) AngRange(6)];
cA = [AngRange(3) AngRange(4) AngRange(5) AngRange(6) AngRange(7)];
 for i=1:5
     muAng(i) = max(0,min( (Ang-aA(i))/(bA(i)-aA(i)),(cA(i)-Ang)/(cA(i)-bA(i)) ) );
 end
 
 Range = [-10 -8 -4 0 4 8 12 16 20];
 a = [-10 -8 -4 0 4 8 12];
 b= [-8 -4 0 4 8 12 16];
 c= [ -4 0 4 8 12 16 20];
 
 %Right angular velocity
 muBN = min(muDist(1), muAng(1));
 muN = max(min(muDist(1), muAng(2)), min(muDist(2), muAng(1)));
 muZ = max([min(muDist(1), muAng(3))  min(muDist(2), muAng(2))  min(muDist(3), muAng(1))]);
 muP = max([min(muDist(1), muAng(4))  min(muDist(2), muAng(3))  min(muDist(3), muAng(2))]);
 muBP = max([min(muDist(1), muAng(5))  min(muDist(2), muAng(4))  min(muDist(3), muAng(3))  min(muDist(4), muAng(1))  min(muDist(4), muAng(2))]);
 muVP = max([min(muDist(2), muAng(5))  min(muDist(3), muAng(4))  min(muDist(4), muAng(3))]);
 muEP = max([min(muDist(3), muAng(5))  min(muDist(4), muAng(4))  min(muDist(4), muAng(5))]);

 muR = [muBN muN muZ muP muBP muVP muEP];
 
 index = find(muR~=0);
 k=1;
 for i=1:length(index)
     ind = index(i);
     if i==1
         x(k) = a(ind);
         y(k)=0;
         k=k+1;
         x(k) = muR(ind)*(b(ind)-a(ind))+a(ind);
         y(k) = muR(ind);
         k=k+1;
     end
     if  i==length(index)
         x(k) = muR(ind)*(b(ind)-c(ind))+c(ind);
         y(k) = muR(ind);
         k=k+1;
         x(k) = c(ind);
         y(k) =0;
         k=k+1;
     end
     if i~=length(index)
         if muR(ind)<0.5 && muR(ind)<muR(ind+1)
             x(k) = muR(ind)*(b(ind+1)-a(ind+1))+a(ind+1);
             y(k) = muR(ind);
             k=k+1;
             x(k) = muR(ind+1)*(b(ind+1)-a(ind+1))+a(ind+1);
             y(k)=muR(ind+1);
             k=k+1;
         else
             x(k) = muR(ind)*(b(ind)-c(ind))+c(ind);
             y(k) = muR(ind);
             k=k+1;
             if muR(ind)>=0.5 && muR(ind+1)>=0.5
                 x(k) = (c(ind)+a(ind+1))/2;
                 y(k)=0.5;
                 k=k+1;
                 x(k) = muR(ind+1)*(b(ind+1)-a(ind+1))+a(ind+1);
                 y(k)=muR(ind+1);
             else
                 x(k) = muR(ind+1)*(b(ind+1)-c(ind+1))+c(ind);
                 y(k) = muR(ind+1);
                 k=k+1;
             end
         end
     end
 end
 x=x+10;
 A=0;B=0;
 for i = 1: length(x)-1
     if y(i+1)==y(i)
         area = y(i)*(x(i+1)-x(i));
         A= A+area;
         h = (x(i+1)+x(i))/2;
         B = B+area*h;
     end
     if y(i)==0 
         area = y(i+1)*(x(i+1)-x(i))/2;
         A = A+area;
         h = x(i)+ 2/3*(x(i+1)-x(i));
         B=B+h*area;
     end
     if y(i+1)==0
         area = y(i)*(x(i+1)-x(i))/2;
         A = A+area;
         h = x(i)+ 1/3*(x(i+1)-x(i));
         B=B+h*area;
     end
     if y(i)*y(i+1)~=0 && y(i)~=y(i+1)
         area = (x(i+1)-x(i))*(y(i)+y(i+1))/2;
         if y(i)<y(i+1)
             h = x(i)+(x(i+1)-x(i))*(2*y(i)+y(i+1))/3/(y(i)+y(i+1));
         else
             
             h = x(i)+2*(x(i+1)-x(i))*(y(i)+2*y(i+1))/3/(y(i)+y(i+1));
         end
         A=A+area;
         B=B+area*h;
     end
 end
 OmegR = B/A-10;
     
  %Left angular velocity
 muBN = min(muDist(1), muAng(5));
 muN = max(min(muDist(1), muAng(4)), min(muDist(2), muAng(5)));
 muZ = max([min(muDist(1), muAng(3))  min(muDist(2), muAng(4))  min(muDist(3), muAng(5))]);
 muP = max([min(muDist(1), muAng(2))  min(muDist(2), muAng(3))  min(muDist(3), muAng(4))]);
 muBP = max([min(muDist(1), muAng(1))  min(muDist(2), muAng(2))  min(muDist(3), muAng(3))  min(muDist(4), muAng(4))  min(muDist(4), muAng(5))]);
 muVP = max([min(muDist(2), muAng(1))  min(muDist(3), muAng(2))  min(muDist(4), muAng(3))]);
 muEP = max([min(muDist(3), muAng(1))  min(muDist(4), muAng(1))  min(muDist(4), muAng(2))]);

 muL = [muBN muN muZ muP muBP muVP muEP];
 clear x y index
 index = find(muL~=0);
 k=1;
 for i=1:length(index)
     ind = index(i);
     if i==1
         x(k) = a(ind);
         y(k)=0;
         k=k+1;
         x(k) = muL(ind)*(b(ind)-a(ind))+a(ind);
         y(k) = muL(ind);
         k=k+1;
     end
     if  i==length(index)
         x(k) = muL(ind)*(b(ind)-c(ind))+c(ind);
         y(k) = muL(ind);
         k=k+1;
         x(k) = c(ind);
         y(k) =0;
         k=k+1;
     end
     if i~=length(index)
         if muL(ind)<0.5 && muL(ind)<muL(ind+1)
             x(k) = muL(ind)*(b(ind+1)-a(ind+1))+a(ind+1);
             y(k) = muL(ind);
             k=k+1;
             x(k) = muL(ind+1)*(b(ind+1)-a(ind+1))+a(ind+1);
             y(k)=muL(ind+1);
             k=k+1;
         else
             x(k) = muL(ind)*(b(ind)-c(ind))+c(ind);
             y(k) = muL(ind);
             k=k+1;
             if muL(ind)>=0.5 && muL(ind+1)>=0.5
                 x(k) = (c(ind)+a(ind+1))/2;
                 y(k)=0.5;
                 k=k+1;
                 x(k) = muL(ind+1)*(b(ind+1)-a(ind+1))+a(ind+1);
                 y(k)=muL(ind+1);
             else
                 x(k) = muL(ind+1)*(b(ind+1)-c(ind+1))+c(ind);
                 y(k) = muL(ind+1);
                 k=k+1;
             end
         end
     end
 end
 x=x+10;
 A=0;B=0;
 for i = 1: length(x)-1
     if y(i+1)==y(i)
         area = y(i)*(x(i+1)-x(i));
         A= A+area;
         h = (x(i+1)+x(i))/2;
         B = B+area*h;
     end
     if y(i)==0 
         area = y(i+1)*(x(i+1)-x(i))/2;
         A = A+area;
         h = x(i)+ 2/3*(x(i+1)-x(i));
         B=B+h*area;
     end
     if y(i+1)==0
         area = y(i)*(x(i+1)-x(i))/2;
         A = A+area;
         h = x(i)+ 1/3*(x(i+1)-x(i));
         B=B+h*area;
     end
     if y(i)*y(i+1)~=0 && y(i)~=y(i+1)
         area = (x(i+1)-x(i))*(y(i)+y(i+1))/2;
         if y(i)<y(i+1)
             h = x(i)+(x(i+1)-x(i))*(2*y(i)+y(i+1))/3/(y(i)+y(i+1));
         else
             
             h = x(i)+2*(x(i+1)-x(i))*(y(i)+2*y(i+1))/3/(y(i)+y(i+1));
         end
         A=A+area;
         B=B+area*h;
     end
 end
 OmegL =B/A;
         
 
 
         
         