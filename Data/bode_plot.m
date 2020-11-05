% ボード線図をプロットするプログラム

clear
close all
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
set(0, 'defaultAxesFontSize', 18);
set(0, 'defaultAxesFontName', 'times');
set(0, 'defaultTextFontSize', 20);
set(0, 'defaultTextFontName', 'times');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

addpath(pwd, 'FrequencyResponse\10.26');
% file_list = ["pos_kp20_ki40_kd50_4.csv", "pos_kp40_ki40_kd50_3.csv", "pos_kp80_ki40_kd50_2.csv", "pos_kp120_ki40_kd50_1.csv", "pos_kp160_ki40_kd50_5.csv"];
% file_list = ["pos_kp20_ki40_kd50_PRO6.csv", "pos_kp40_ki40_kd50_PRO7.csv", "pos_kp80_ki40_kd50_PRO8.csv", "pos_kp120_ki40_kd50_PRO9.csv", "pos_kp160_ki40_kd50_PRO10.csv"];
% file_list = ["pos_kp20_ki40_kd50_PRO1.csv", "pos_kp40_ki40_kd50_PRO2.csv", "pos_kp60_ki40_kd50_PRO3.csv", "pos_kp80_ki40_kd50_PRO4.csv", "pos_kp100_ki40_kd50_PRO5.csv"...
%                 "pos_kp120_ki40_kd50_PRO6.csv", "pos_kp140_ki40_kd50_PRO7.csv", "pos_kp160_ki40_kd50_PRO8.csv"];
% file_list = ["pos_kp20_ki40_kd50_PRO1.csv", "pos_kp60_ki40_kd50_PRO3.csv", "pos_kp100_ki40_kd50_PRO5.csv"...
%                 "pos_kp140_ki40_kd50_PRO7.csv"];
file_list = ["cur_kp20_kd20_PRO1.csv", "pos_kp40_kd20_PRO2.csv", "pos_kp60_kd20_PRO3.csv"];
freqset = [0.2,  0.2*sqrt(2), 0.4, 0.4*sqrt(2), 0.8, 0.8*sqrt(2), 1.6, 1.6*sqrt(2), 3.2, 3.2*sqrt(2), 6.4, 6.4*sqrt(2), 12.8];
c_set = ['b', 'g', 'r', 'm'];
kp_list = ["Kp=20", "Kp=60", "Kp=100", "Kp=140"];


for i_n = 1:length(file_list)
    F = importdata(file_list(i_n), ',', 1);
    time = F.data(:, 2);
    num = F.data(:, 3);
    input = F.data(:, 5);
    output = F.data(:, 6);
%     vel = F.data(:, 7);
%     output = F.data(:, 8);
    
%     figure
    
    
    for i = 1:13
        X0 = find(num == i - 1, 1);
        X = find(num == i, 1);
        bode_data(i).inputAmp = max(input(X0+10 : X - 1));
        bode_data(i).outputAmp = max(output(X0+10 : X - 1));
        
        if i == 13
            bode_data(i).inputAmp = max(input(X0+10 : end));
            bode_data(i).outputAmp = max(output(X0+10 : end));
        end
        
        list(i) = bode_data(i).outputAmp/bode_data(i).inputAmp;

        semilogx(freqset(i), list(i), 'ko')
        hold on
    end
    
    plt(i_n) = plot(freqset, list, c_set(i_n), 'DisplayName', kp_list(i_n));
    grid on
    % xlim([0.5 18])
    xlabel("Frequency [Hz]")
    ylabel("Magnitude [dB]")

end

legend([plt(1), plt(2), plt(3), plt(4)])


