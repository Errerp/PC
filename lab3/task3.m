clear;
clc
% Модель строит спектрограммы для лабораторной работы №3
[sig_orig,fs] = audioread ('var_DC.wav');

figure;
subplot(131);
specgram(sig_orig,512, fs , kaiser(512,7), 475);
set(gca,'Clim', [-65 15]);
xlabel('Время, с');
ylabel('Частота, Гц');
title('Исходный сигнал');
set(gca, 'FontName', 'Times New Roman');
set(gca, 'FontSize', 14);

[sig_proc_C,fs] = audioread ('var_DC_new_cpp.wav');

% xavepred = mean(sig_orig(1:min(200, length(sig_orig)))); % начальное среднее
% alpha = 0.999;
% y = zeros(size(sig_orig));
% for n = 1:length (sig_orig)
%     xave = sig_orig(n) * (1 - alpha) + xavepred * alpha;
%     xavepred = xave;
%     y(n) = sig_orig(n) - xave;
% end

subplot(132);
specgram(sig_proc_C,512, fs , kaiser(512,7), 475);
colormap('jet');
set(gca,'Clim', [-65 15]);
xlabel('Время, с');
ylabel('Частота, Гц');
title('Обработка на C++');
set(gca, 'FontName', 'Times New Roman');
set(gca, 'FontSize', 14);

[sig_proc_asm,fs] = audioread ('var_DC_new_fpu.wav');

subplot(133);
specgram(sig_proc_asm,512, fs , kaiser(512,7), 475);
colormap('jet');
set(gca,'Clim', [-65 15]);
xlabel('Время, с');
ylabel('Частота, Гц');
title('Обработка на FPU');
set(gca, 'FontName', 'Times New Roman');
set(gca, 'FontSize', 14);