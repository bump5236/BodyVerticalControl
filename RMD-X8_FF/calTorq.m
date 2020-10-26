%% initial settings
clear
close all

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
set(0,'defaultAxesFontSize',16);
set(0,'defaultAxesFontName', 'times');
set(0,'defaultTextFontSize',16);
set(0,'defaultTextFontName', 'times');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

time =(0 : 0.01 : 10);
A = 10;  % [Nm]
B = 0.04;   % [m]
C = 20 / 180 * 3.14;    % [rad]
f = 0.7;   % [Hz]

%% ロボットのパラメータ
L1 = 0.33;   % 前脚の作用点[m]
L2 = 0.15;   % 後ろ脚の作用点[m]
l = 0.13;   % ゴムの力点[m]
r = 0.06;   % プーリーの半径[m]
Lm = 0.145;   % モータと脚の軸間距離[m]
Lx = 0.25;   % 初期ワイヤ長さ[m]

%% 計算
omega = 2* 3.14 * f;   % 脚の角度
lm = sqrt(r^2 + Lm^2 -2*r*Lm*cos(160));   % 脚の軸とモータの力点との距離


for i=1:length(time)
    %% ゴムによる脚トルク算出
    x(i) = - B * cos(omega * time(i)) + 0.29 + B;   % ゴムの変位
    T(i) = 192.9*x(i) + 6.96;   % ゴムの張力の近似式（ゴムは二重並列）
    tau_g(i) = T(i) * L1 * sqrt(1 - ((L1^2 + x(i)^2 - l^2) / (2* L1 * x(i)))^2);  % ゴムのトルク
    
    %% モータによる脚トルク算出
    lx(i) = Lx - L2 * (- C * cos(omega * time(i)) + C);
    sin_alpha(i) = sqrt(1 - ((L2^2 + lx(i)^2 - lm^2) / (2* L2 * lx(i)))^2);
    
    tgt(i) = A * cos(omega* time(i));   % 脚にかけたいトルク
    tau_m(i) = (tgt(i) + tau_g(i)) * r / L2 /sin_alpha(i);
    
 end

figure
plot(time, tau_g)
hold on
plot(time, tgt)
plot(time, tau_m)

legend("\tau_G", "target", "\tau_M")