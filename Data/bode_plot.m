% ボード線図をプロットするプログラム

clear
close all
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
set(0, 'defaultAxesFontSize', 16);
set(0, 'defaultAxesFontName', 'times');
set(0, 'defaultTextFontSize', 16);
set(0, 'defaultTextFontName', 'times');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

addpath(pwd, 'FrequencyResponse\10.15.2');
file_list = ["pos_kp20_ki40_kd50_4.csv", "pos_kp40_ki40_kd50_3.csv", "pos_kp80_ki40_kd50_2.csv", "pos_kp120_ki40_kd50_1.csv", "pos_kp160_ki40_kd50_5.csv"];
% file_list = ["pos_kp20_ki40_kd50_PRO6.csv", "pos_kp40_ki40_kd50_PRO7.csv", "pos_kp80_ki40_kd50_PRO8.csv", "pos_kp120_ki40_kd50_PRO9.csv", "pos_kp160_ki40_kd50_PRO10.csv"];
freqset = [0.1 0.2, 0.4, 0.8, 1.6, 3.2, 6.4, 12.8, 25.6];
c_set = ['b', 'g', 'r', 'c', 'k'];


for i_n = 1:length(file_list)
    F = importdata(file_list(i_n), ',', 1);
    time = F.data(:, 2);
    num = F.data(:, 3);
    input = F.data(:, 5) ./ 600;
%     cur = F.data(:, 6);
%     vel = F.data(:, 7);
    output = F.data(:, 8);
    
%     figure
    
    
    for i = 1:9
        X0 = find(num == i - 1, 1);
        X = find(num == i, 1);
        bode_data(i).inputAmp = max(input(X0+10 : X - 1));
        bode_data(i).outputAmp = max(output(X0+10 : X - 1));
        
        if i == 9
            bode_data(i).inputAmp = max(input(X0+10 : end));
            bode_data(i).outputAmp = max(output(X0+10 : end));
        end
        
        list(i) = bode_data(i).outputAmp/bode_data(i).inputAmp;

        semilogx(freqset(i), list(i), 'ko')
        hold on
    end
    
%     plot(freqset, list, c_set(i_n))
    p = polyfit(freqset, list, 2);
    x = linspace(0,30);
    y = polyval(p, x);
    plot(x, y)
    xlabel("frequency [Hz]")
    ylabel("magnitude [dB]")

end


% title(["RMD-X8 PRO"; "A=17.5, kp=100, ki=100, kd=50"])


