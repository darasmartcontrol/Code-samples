%
clc
clear;
% close all;

xy=[0 2;-1 2;-1 1; 3 1; 3 1.5;2 1.5]*1000;
% xy=[2.2 3.1;1.5 2.5; 3.5 2.5]*1000;
% xy=[1 2;-1 3;-2 2; -2 -2; 1 3;2 3]*1000;
% xy=[1.5 2.5; .9 -4.5;1 -4;1.2 -4.5]*1000;    %**
% xy=[1.1 2;1.5 2.5; 1 3; -2 -2;-2 2;-1 3;1 2;3 .5;3 -1; 4 -1; 4 -1.5; 2 -1.5;1 -4;1.5 -4.5]*1000;
% xy=[.85 3;.92 3.1;1 3.05;1 3]*1000; %%Short Distance **
% xy=[1 5;1 3]*1000; %%Two points


% Compute the reference velocity and position
[x, y, Time  ] = PathDef(xy);
[Time, index] = unique(Time);
index = sort(index);
y = y(index);
x = x(index);

T1 = .01;
t = 0:T1:Time(end);
for j = 1:length(t)
    xIntPol(j) = interp1(Time, x, t(j));
    yIntPol(j) = interp1(Time, y, t(j));
    XRef(:,j) = [xIntPol(j);yIntPol(j)];
end


p1 = 200; %(cm) Distance error range
p2 = pi/2; %angle error between -pi/2 and pi/2
pu = 20; % Motor speed is between 0 and 6 rad/sec

Fis = newfis('InputFuzzy');
Fis = addvar(Fis, 'input', 'Dist', [0, p1]);
Fis = addvar(Fis, 'input', 'Ang', [-pi,pi]);
Fis = addvar(Fis, 'output', 'OmegaRight', [-10,pu]);
Fis = addvar(Fis, 'output', 'OmegaLeft', [-10,pu]);

%Input1 Dist (Distance Errror)
Fis = addmf(Fis,'input', 1, 'Z', 'trimf', [0,0,10]);
Fis = addmf(Fis,'input', 1, 'N', 'trimf', [0,10,40]);
Fis = addmf(Fis,'input', 1, 'M', 'trimf', [10,40,200]);

%Input2 Ang (Angle Errror)
Fis = addmf(Fis,'input', 2, 'BN', 'trimf', [-pi,-pi/2,-pi/10]);
Fis = addmf(Fis,'input', 2, 'N', 'trimf', [-pi/2,-pi/10,0]);
Fis = addmf(Fis,'input', 2, 'Z', 'trimf', [-pi/10,0,pi/10]);
Fis = addmf(Fis,'input', 2, 'P', 'trimf', [0,pi/10,pi/2]);
Fis = addmf(Fis,'input', 2, 'BP', 'trimf', [pi/10,pi/2,pi]);

%Output1 OmegaRight
Fis = addmf(Fis,'output', 1, 'N', 'trimf', [-8,-4,0]);
Fis = addmf(Fis,'output', 1, 'z', 'trimf', [-4,0,4]);
Fis = addmf(Fis,'output', 1, 'p', 'trimf', [0,4,8]);
Fis = addmf(Fis,'output', 1, 'BP', 'trimf', [4,8,12]);
Fis = addmf(Fis,'output', 1, 'vBP', 'trimf', [8,12,16]);
Fis = addmf(Fis,'output', 1, 'vvBP', 'trimf', [12,16,20]);


%Output2 OmegaLeft
Fis = addmf(Fis,'output', 2, 'N', 'trimf', [-8,-4,0]);
Fis = addmf(Fis,'output', 2, 'z', 'trimf', [-4,0,4]);
Fis = addmf(Fis,'output', 2, 'p', 'trimf', [0,4,8]);
Fis = addmf(Fis,'output', 2, 'BP', 'trimf', [4,8,12]);
Fis = addmf(Fis,'output', 2, 'vBP', 'trimf', [8,12,16]);
Fis = addmf(Fis,'output', 2, 'vvBP', 'trimf', [12,16,20]);

%%
ruleslist = [
    1 1 1 4 ;
    1 2 1 4 ;
    1 3 2 2 ;
    1 4 4 1 ;
    1 5 4 1 ;
    2 1 1 5 ;
    2 2 1 5 ;
    2 3 3 3 ;
    2 4 5 1 ;
    2 5 5 1 ;
    3 1 2 6 ;
    3 2 2 6 ;
    3 3 4 4 ;
    3 4 6 2 ;
    3 5 6 2 ];
ruleslist(: , 5:6) = 1;
Fis=addrule(Fis,ruleslist);
% writefis(Fis,'FuzzyLogicTest');
%%
%Robot Model simulation
L = 280;%Distance between wheels (mm)
r=82.55; %Radius of wheel (mm)

N=length(t);
for i=1:length(t)-1
    Phi(i) = atan2( (XRef(2,i+1)-XRef(2,i)),(XRef(1,i+1)-XRef(1,i)) );
end
Phi(N)= Phi(N-1);

XRef = [XRef;Phi];
clear X Y Teta Dist Ang AA OmegaRight OmegaLeft 
N = length(t);
X(1) = xy(1,1);
Y(1) =xy(1,2);
Teta(1) = Phi(1);

Vx(1)=0;
Vy(1)=0;
for i=1:N
    if i~=1
        Teta(i) = Teta(i-1) +T1*r*(OmegaRight(i-1)-OmegaLeft(i-1))/L;
        X(i) = X(i-1) +T1*( r*(OmegaRight(i-1)+OmegaLeft(i-1))/2*cos(Teta(i-1)) );
        Y(i) = Y(i-1) +T1*( r*(OmegaRight(i-1)+OmegaLeft(i-1))/2*sin(Teta(i-1)) );
        Vx(i) = r*(OmegaRight(i-1)+OmegaLeft(i-1))/2*cos(Teta(i-1));
        Vy(i) = r*(OmegaRight(i-1)+OmegaLeft(i-1))/2*sin(Teta(i-1));
    end
    % This part checks if the present state has passed the (i+1)th Ref point
    % We define a new coordinate with the origin at XRef(i+1) pointing to
    % XRef(i+2). Then, the state is transformed to the new coordinate.
    % its x value is named xp in the new coordinate.
    if i~=N
        xx = X(i) - XRef(1,i+1);
        yy = Y(i) - XRef(2,i+1);
        ind=0;
        if i==N-1
            i=i-1;
            ind = norm(XRef(:,N-1)-XRef(:,N));
        end
        thet = atan2(XRef(2,i+2)-XRef(2,i+1),XRef(1,i+2)-XRef(1,i+1));
        TransMat = [cos(thet) sin(thet);-sin(thet) cos(thet)];
        xyp = TransMat *[xx;yy];
        xp = xyp(1);
        xp=xp+ind;
        if ind~=0
            i=i+1;
        end
    end
    %If xp<0 means that the present state has not passed the i+1 th reference
    %point. Otherwise, if it has passed we wont use the fuzzy rules and
    %just keep the inputs at a very slow value to let the system catch the
    %state.
    AbsV(i) = norm([Vx(i);Vy(i)]);
    if xp<0 || i==N
        if i~=N
            AngDist(i) = atan2((XRef(2,i+1)-Y(i)),(XRef(1,i+1)-X(i)));       
            Dist(i) = norm( XRef(1:2,i+1) - [X(i);Y(i)]);

        else
            AngDist(i) = atan2((XRef(2,i)-Y(i)),(XRef(1,i)-X(i)));
        end
       
        Ang(i)= AngDist(i)-Teta(i);
        while Ang(i)>pi
            Ang(i) = Ang(i)-2*pi;
        end
        while Ang(i)<-pi
            Ang(i)= Ang(i)+2*pi;
        end
         if i~=N
            Dist(i) = norm( XRef(1:2,i+1) - [X(i);Y(i)]);
         else
            Dist(i) = norm( XRef(1:2,i) - [X(i);Y(i)]);
         end
        if Dist(i)<0
            Dist(i)=0;
            Ang(i)=0;
        end
    else
        Dist(i) =0;
        Ang(i)=0;
    end
    Control = evalfis([Dist(i) Ang(i)], Fis);
    OmegaRight(i) = Control(1);
    OmegaLeft(i) = Control(2);
    if i==N
        OmegaRight(N)=0;
        OmegaLeft(N)=0;
    end
        
    if OmegaRight(i)>14
        OmegaRight(i)=14;
    end
    if OmegaRight(i)<0
        OmegaRight(i)=0;
    end
    if OmegaLeft(i)>14
        OmegaLeft(i)=14;
    end
    if OmegaLeft(i)<0
        OmegaLeft(i)=0;
    end
    
%     
%     if t(i)>=5 && t(i)<=5.5
%         X(i) = X(i)+.4;
%         Y(i)= Y(i)-.4;
%     end
end

%%  plot results

figure;plot( XRef(1,1:i), XRef(2,1:i),X(1:i) , Y(1:i),'r')
xlabel('x');ylabel('y');
legend('Reference Path', 'Controlled path')
grid minor
axis equal

figure;plot(  t,AbsV, 'r')
xlabel('Time(Sec)')
ylabel('Velocity')
legend('Reference Velocity', 'Controlled Velocity')

figure;plot(t(1:i), OmegaRight(1:i),t(1:i), OmegaLeft(1:i))
xlabel('Time')
ylabel('Control Inputs')
legend('Omega Right wheel','Omega Left wheel')
% % 