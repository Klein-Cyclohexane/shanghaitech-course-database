% filepath: /Users/bearxiong/Documents/ShanghaiTech/SI114_2025_spring/HW3/gibbs_phenomenon.m
% MATLAB code to demonstrate Gibbs phenomenon

clear all;
close all;

% Define the square wave function (period 2π, amplitude 1)
f = @(x) sign(sin(x));

% Create a fine grid for plotting
x = linspace(-pi, pi, 1000);
y_exact = f(x);

% Plot the exact function
figure('Position', [100, 100, 1000, 600]);
subplot(2, 1, 1);
plot(x, y_exact, 'k-', 'LineWidth', 1.5);
hold on;

% Compute Fourier series approximations with different numbers of terms
N_values = [1, 5, 15, 50];
colors = {'r-', 'g-', 'b-', 'm-'};

for i = 1:length(N_values)
    N = N_values(i);
    y_approx = zeros(size(x));
    
    % Compute the Fourier series approximation with N terms
    for n = 1:2:N
        y_approx = y_approx + (4/(n*pi))*sin(n*x);
    end
    
    % Plot the approximation
    plot(x, y_approx, colors{i}, 'LineWidth', 1.2);
end

title('Square Wave and Fourier Approximations');
xlabel('x');
ylabel('f(x)');
legend('Exact', 'N=1', 'N=5', 'N=15', 'N=50');
grid on;
xlim([-pi, pi]);
ylim([-1.5, 1.5]);

% Show a zoomed view of the Gibbs phenomenon
subplot(2, 1, 2);
x_zoom = linspace(0, pi/2, 500);
y_exact_zoom = f(x_zoom);
plot(x_zoom, y_exact_zoom, 'k-', 'LineWidth', 1.5);
hold on;

for i = 1:length(N_values)
    N = N_values(i);
    y_approx_zoom = zeros(size(x_zoom));
    
    % Compute the Fourier series approximation with N terms
    for n = 1:2:N
        y_approx_zoom = y_approx_zoom + (4/(n*pi))*sin(n*x_zoom);
    end
    
    % Plot the approximation
    plot(x_zoom, y_approx_zoom, colors{i}, 'LineWidth', 1.2);
end

title('Gibbs Phenomenon - Zoom at Discontinuity');
xlabel('x');
ylabel('f(x)');
legend('Exact', 'N=1', 'N=5', 'N=15', 'N=50');
grid on;
xlim([0, pi/2]);
ylim([0.5, 1.5]);

% Calculate and display the overshoot percentages
fprintf('Gibbs Phenomenon Overshoot Values:\n');
for i = 1:length(N_values)
    N = N_values(i);
    y_approx = zeros(size(x));
    
    for n = 1:2:N
        y_approx = y_approx + (4/(n*pi))*sin(n*x);
    end
    
    % Find maximum value (should be near discontinuity)
    max_val = max(y_approx);
    overshoot_percent = (max_val - 1) * 100;
    fprintf('N = %d: Maximum value = %.4f, Overshoot = %.2f%%\n', N, max_val, overshoot_percent);
end

% Theoretical asymptotic overshoot (approximately 9%)
fprintf('Theoretical asymptotic overshoot ≈ 9%% of jump height\n');