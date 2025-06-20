threads = [1, 2, 4, 8, 16];  % Количество потоков (ось X)
time_serial = [1.304, 1.304, 1.304, 1.304, 1.304];  % Время обычного C-кода (ось Y)
time_parallel = [1.302, 1.302, 1.302, 1.3, 1.29];  % Время OpenMP (ось Y)

figure;
plot(threads, time_serial, '-o', 'LineWidth', 2, 'DisplayName', 'C-код');
hold on;
plot(threads, time_parallel, '-s', 'LineWidth', 2, 'DisplayName', 'OpenMP');
hold off;

xlabel('Количество потоков', 'FontSize', 12);
ylabel('Время выполнения (сек)', 'FontSize', 12);
title('Сравнение производительности', 'FontSize', 14);
legend('Location', 'best');
grid on;

ylim([1, 1.5]);  