    %% Lab Defined Constants
kt = 0.042; % torque constant in [N.m/A]
km = 0.042; % motor back-emf constant in [v/(rad/s)]
Jm = 4e-6; % rotor inertia in [kgm^2]
Lm = 1.16; % rotor inductance in [mH]
mh = 0.0106; % load hub mass in [kg]
rh = 0.0111; % load hub radius in [m]
md = 0.053; % mass of disk in [kg]
rd = 0.0248; % radius of disk in [m]
Jh = 0.5*mh*rh^2;
Jd = 0.5*md*rd^2;
Jeq = Jm + Jh +Jd; %equivalent inertia in [kgm^2]
Rm = 8.4; % terminal resistance in [ohms]

%% User Defined Constants
A = kt/(Jeq*Rm);
B = (kt*km)/(Jeq*Rm);

%% Setup of transfer Functions
P = tf(A,[1,B,0]);      
K = 1;
C = K*D;
L = C*P;

%% Open Loop Plots
figure(1); 
bode(P,C,L)
legend P C L

[gm,pm,wcg,wcp]=margin(P)
[gm1,pm1,wcg1,wcp1]=margin(L)
figure(2);
rlocus(L, [0:0.001:5])

%% Closed Loop Function Setup
K_A = 0.2;
K_B = 2.2;
D = tf([10,100],[1,100]);

%% Closed Loop Plots
figure(3);
bode(K_A*L, K_B*L)
legend K_A K_B

figure(4);
nyquist(K_A*L,K_B*L) 
legend K_A K_B

%% Digital Control Setup
T = 0.04;
P_zoh = c2d(P, T, 'zoh');

NUM = [1,-0.9048];
DEN = [1,-0.3679];
Dd = tf(NUM,DEN,T);
K_d = 14.6;


%% Digital Plots
figure(5);
bode(K_B*D, 'b', K_d*Dd, 'r--')
