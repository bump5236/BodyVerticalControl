% ボード線図をプロットするプログラム

clear
close all
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
set(0, 'defaultAxesFontSize', 16);
set(0, 'defaultAxesFontName', 'times');
set(0, 'defaultTextFontSize', 16);
set(0, 'defaultTextFontName', 'times');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

addpath(pwd, 'FrequencyResponse');
file_list = ["pos_kp40_ki40_kd50_PRO.csv", "pos_kp80_ki40_kd50_PRO.csv", "pos_kp120_ki40_kd50_PRO.csv"];
freqset = [0.1 0.2, 0.4, 0.8, 1.6, 3.2, 6.4, 12.8];
c_set = ['b', 'g', 'r'];


for i_n = 1:length(file_list)
    F = importdata(file_list(i_n), ',', 1);
    time = F.data(:, 2);
    num = F.data(:, 3);
    input = F.data(:, 5) ./ 600;
%     cur = F.data(:, 6);
%     vel = F.data(:, 7);
    output = F.data(:, 8);
    
%     figure
    
    
    for i = 1:8
        X0 = find(num == i - 1, 1);
        X = find(num == i, 1);
        
        bode_data(i).inputAmp = max(input(X0+10 : X - 1));
        bode_data(i).outputAmp = max(output(X0+10 : X - 1));
        list(i) = bode_data(i).outputAmp/bode_data(i).inputAmp;

        semilogx(freqset(i), list(i), 'ko')
        hold on
    end
    
    plot(freqset, list, c_set(i_n))
    xlabel("frequency [Hz]")
    ylabel("magnitude [dB]")

end


% title(["RMD-X8 PRO"; "A=17.5, kp=100, ki=100, kd=50"])


