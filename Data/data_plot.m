%% initial settings
clear
close all

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
set(0,'defaultAxesFontSize',16);
set(0,'defaultAxesFontName', 'times');
set(0,'defaultTextFontSize',16);
set(0,'defaultTextFontName', 'times');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% csv読み込み
file = importdata('10.14\output2.csv', ',', 1);

time = file.data(:, 1);
arduino_time = file.data(:, 2);
sync = file.data(:, 3);
R_temp = file.data(:, 4);
R_add_cur = file.data(:, 5);
R_tgt_cur = file.data(:, 6);
R_cur = file.data(:, 7);
R_vel = file.data(:, 8);
R_pos = file.data(:, 9);

L_temp = file.data(:, 10);
L_add_cur = file.data(:, 11);
L_tgt_cur = file.data(:, 12);
L_cur = file.data(:, 13);
L_vel = file.data(:, 14);
L_pos = file.data(:, 15);

% 16bit Encoder
LSB = 65536;
RGR = 6;

R_torq = R_cur/2000*12.5*3.3;
R_vel = R_vel*RGR;
R_tgt_torq = R_tgt_cur/2000*12.5*3.3;

L_torq = L_cur/2000*12.5*3.3;
L_vel = L_vel*RGR;
L_tgt_torq = L_tgt_cur/2000*12.5*3.3;

figure
plot(time, R_tgt_torq, 'DisplayName', "R_tgt_torq")
hold on
plot(time, R_torq, 'DisplayName', "R_torq")
plot(time, R_pos, 'DisplayName', "R_pos")
legend

figure
plot(time, L_tgt_torq, 'DisplayName', "L_tgt_torq")
hold on
plot(time, L_torq, 'DisplayName', "L_torq")
plot(time, L_pos, 'DisplayName', "L_pos")
legend

% filename = ['outputFig'];        
% saveas(gcf, filename, 'png')