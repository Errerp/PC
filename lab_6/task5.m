threads = [1, 2, 4, 8, 16];  % ���������� ������� (��� X)
time_serial = [1.304, 1.304, 1.304, 1.304, 1.304];  % ����� �������� C-���� (��� Y)
time_parallel = [1.302, 1.302, 1.302, 1.3, 1.29];  % ����� OpenMP (��� Y)

figure;
plot(threads, time_serial, '-o', 'LineWidth', 2, 'DisplayName', 'C-���');
hold on;
plot(threads, time_parallel, '-s', 'LineWidth', 2, 'DisplayName', 'OpenMP');
hold off;

xlabel('���������� �������', 'FontSize', 12);
ylabel('����� ���������� (���)', 'FontSize', 12);
title('��������� ������������������', 'FontSize', 14);
legend('Location', 'best');
grid on;

ylim([1, 1.5]);  