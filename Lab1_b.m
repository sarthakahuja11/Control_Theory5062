%% PARAMETERS

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

k = 1;
 
num = [10 100];  % assign from right to left?
dem = [1 100];



%% TRANSFER FUNCTION
d = tf(num,dem);


c = k*d ;

l = c*p ;




