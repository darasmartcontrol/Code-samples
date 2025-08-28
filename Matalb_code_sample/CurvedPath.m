function [Disp, x, y]=CurvedPath(xy,DO, Rad, Tet, ind, PhiDirect,DistPrec)
N=length(xy);
for i=2:N-1
    n(i) = ceil(norm(Rad(i,:))*Tet(i)/DistPrec);
end
% Phi = zeros((N-2)*2+2+sum(n),1);
XY = zeros(2+sum(n),2);
Disp = zeros(2+sum(n),1);
XY(1, :)=xy(1,:);

% Phi(1:2,1) = [PhiDirect(1);PhiDirect(1)];
for i=2:N-1
    a1=0;a2=0;
    for j=1:i-2
        a1 = a1+n(j+1);
    end
    for j=1:i-1
        a2 = a2+n(j+1);
    end
    [xycorner, PhiCorner] = CrossCorner( DO(i,:), Rad(i,:), Tet(i), ind(i), PhiDirect(i-1), n(i));
    PhiDirect(i) = PhiCorner(end);
%     Phi(2+1+a1:a2,1) =PhiCorner';
    if i==N-1
%         xyStraight =xy(end,:);
    XY(1+1+a1:1+a2+1,:)=[xycorner;xy(end,:)];
    else
%         xyStraight = xyGuide(2*(i-1)+1,:);
    XY(1+1+a1:1+a2,:)=xycorner;
    end
%     if i~=N-1
%         Phi(1+a2:2+a2,1) =[PhiDirect(i);PhiDirect(i)];
%     else
%         Phi(1+a2:2+a2,1) =[PhiDirect(end);PhiDirect(end)];
%     end
end
x=XY(:,1);
y=XY(:,2);
Disp(1)=0;
for i=1:length(x)-1
    Disp(i+1,1) = Disp(i)+norm(XY(i+1,:)-XY(i,:));
end