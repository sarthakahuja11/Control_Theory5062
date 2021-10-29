%% PARAMETERS
%g = 9.81; %m/s^2
%m = 5;    %kg
%l = 0.4;  %m
%c = 0.5;  %Nm/rad
%I=m*(l/2)^2;
%wn=sqrt(m*g*l/(2*I));
%xi=c/(2*I*wn);

Rm = 8.4;
kt = 0.042;
km = 0.042;
Jm = 0.000004;
Lm = 1.16;
Mh = 0.0106;
Rh = 0.111;
Md = 0.053;
Rd = 0.0248;

Jeq = Jm + 0.5*Mh*Rh^2 + 0.5*Md*Rd^2;
 
num = kt;
dem = [Jeq*Rm kt*km 0];

dem = dem./(Jeq*Rm);

%% TRANSFER FUNCTION
p = tf(num,dem);




