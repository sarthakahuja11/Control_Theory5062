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
km = 0.042;     %rad/s
Jm = 0.000004;
Lm = 1.16*10^(-3);      
Mh = 0.0106;
Rh = 0.0111;
Md = 0.053;
Rd = 0.0248;

Jeq = Jm + 0.5*Mh*Rh^(2) + 0.5*Md*Rd^(2);
 
num = kt/(Jeq*Rm);
dem = [Jeq*Rm kt*km 0];

dem = dem./(Jeq*Rm);

%% TRANSFER FUNCTION
p = tf(num,dem);


k = 1;
 
num1 = [10 100];  % assign from right to left?
dem1 = [1 100];

%% TRANSFER FUNCTION
d = tf(num1,dem1);

c = k*d ;

l = c*p ;

ka = 0.2;
kb = 2.2;
l1 = ka*l;
l2 = kb*l;
l3 = ka*l;
l4 = kb*l;

%T = 0.01;
T = 0.04;


kd = 2.2;      %check alter ?

p_zoh = c2d(p,T,'zoh');

d_z = c2d(d, T, 'zoh');

[num, den] = tfdata(d_z);   %Check alter?

dd = tf(num, den , T);


