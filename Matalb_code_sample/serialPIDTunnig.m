clear all; clc;
%% Defining serial Object

obj = instrfind('Type','serial','Port','COM5','Tag','');

if isempty(obj)
    obj = serial('COM5');
else
    fclose(obj);
    obj = obj(1);
end
obj.BaudRate = 115200;

 % Connect to instrument object, obj.
%%
fopen(obj);
Acx=[];
Acy=[];
Acz=[];
wx = [];
figure;
for i=1:70000
    Data = fscanf(obj);
    Data = str2num(Data)/1000;
    if length(Data)==10 && mod(i,10)==0

        Acx = [Acx Data(1)];
        Acy = [Acy Data(2)];
        Acz = [Acz Data(3)];
        Omeg = [OmegPre Data(2)];
        plot(Time,Omeg)
        hold on;
        drawnow;
        TimePre = Time;
        OmegPre = Omeg;
    end
end
%% Close Object
fclose(obj)