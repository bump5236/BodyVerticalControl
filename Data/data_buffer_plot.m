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
file = importdata('output.csv', ',', 1);

time = file.data(:, 1);
arduino_time = file.data(:, 2);
tgt_pos = file.data(:, 3);
cmd_byte = file.data(:, 4);
temperature = file.data(:, 5);
cur_L = file.data(:, 6);
cur_H = file.data(:, 7);
vel_L = file.data(:, 8);
vel_H = file.data(:, 9);
enc_L = file.data(:, 10);
enc_H = file.data(:, 11);
pos_cmd = file.data(:, 12);
pos_low_1 = file.data(:, 13);
pos_2 = file.data(:, 14);
pos_3 = file.data(:, 15);
pos_4 = file.data(:, 16);
pos_5 = file.data(:, 17);
pos_6 = file.data(:, 18);
pos_7 = file.data(:, 19);

cur = bitshift(cur_H, 8, 'int16') + cur_L;
vel = bitshift(vel_H, 8, 'int16') + vel_L;
enc_pos = bitshift(enc_H, 8, 'int16') + enc_L;

pos = bitshift(pos_7, 48, 'int64') + bitshift(pos_6, 40, 'int64') + bitshift(pos_5, 32, 'int64') + bitshift(pos_4, 24, 'int64') + bitshift(pos_3, 16, 'int64') + bitshift(pos_2, 8, 'int64') + pos_low_1;

% Arduino -> Matlabのmulti angleの変換はこれを用いる
for l = 1:length(pos)
    if pos(l)  > 36028797018963967
        pos(l) = pos(l) - 72057594037927935;
    end
end

tgt_pos = tgt_pos/100/6;
pos = enc_pos/65536*360/6;
vel = vel/6;

for i = 1:length(pos)
    if i == 1
        cvel(i) = 0;
    else 
        cvel(i) = (pos(i) - pos(i-1))/(time(i)-time(i-1));
    end
end

figure
plot(time, cur)

figure
plot(time, vel)
hold on
plot(time, cvel)

figure
plot(time, tgt_pos)
hold on
plot(time, pos)
xlabel("time [ms]")
ylabel("input/output [deg]")
legend({'input', 'output'})

filename = ['outputFig'];        
saveas(gcf, filename, 'png')