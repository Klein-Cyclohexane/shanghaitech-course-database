% --- Parameters ---
L = pi; % Periodicity of the square wave is 2*L
N_terms_array = [5, 15, 50, 100]; % Number of terms in the Fourier series to plot
num_points = 1000; % Number of points for plotting

% --- Define the time vector ---
% We'll plot over a little more than one period to see the behavior clearly
t = linspace(-1.5*L, 1.5*L, num_points);

% --- Define the ideal square wave function (for reference) ---
% Square wave: f(t) = 1 for 0 < t < L, -1 for -L < t < 0
% and periodic with period 2L
ideal_square_wave = zeros(size(t));
for i = 1:length(t)
    t_mod = mod(t(i) + L, 2*L) - L; % Bring t into the interval [-L, L]
    if t_mod > 0 && t_mod < L
        ideal_square_wave(i) = 1;
    elseif t_mod < 0 && t_mod > -L
        ideal_square_wave(i) = -1;
    elseif t_mod == 0 || t_mod == L || t_mod == -L % At discontinuities, average value
        ideal_square_wave(i) = 0;
    end
end

% --- Plotting ---
figure;
hold on;
grid on;
title('Gibbs Phenomenon for a Square Wave');
xlabel('t');
ylabel('f(t)');
plot(t, ideal_square_wave, 'k--', 'LineWidth', 1.5, 'DisplayName', 'Ideal Square Wave');

colors = lines(length(N_terms_array)); % Get distinct colors for plotting

% --- Calculate and plot Fourier series for different numbers of terms ---
for k_idx = 1:length(N_terms_array)
    N = N_terms_array(k_idx);
    f_approx = zeros(size(t)); % Initialize the Fourier series sum

    % The Fourier series for a square wave f(t) = 1 for (0, L) and -1 for (-L, 0)
    % is f(t) = (4/pi) * sum_{n=1,3,5,...}^{N} (1/n) * sin(n*pi*t/L)
    % For our specific square wave (amplitude 1), the coefficients are:
    % a0 = 0
    % an = 0 (since it's an odd function)
    % bn = (2/L) * integral_{-L}^{L} f(t)*sin(n*pi*t/L) dt
    %    = (2/L) * [ integral_{-L}^{0} (-1)*sin(n*pi*t/L) dt + integral_{0}^{L} (1)*sin(n*pi*t/L) dt ]
    %    = (2/L) * [ (L/(n*pi))*cos(n*pi*t/L) |_{-L}^{0} - (L/(n*pi))*cos(n*pi*t/L) |_{0}^{L} ]
    %    = (2/(n*pi)) * [ (cos(0) - cos(-n*pi)) - (cos(n*pi) - cos(0)) ]
    %    = (2/(n*pi)) * [ (1 - (-1)^n) - ((-1)^n - 1) ]
    %    = (2/(n*pi)) * [ 2 - 2*(-1)^n ]
    %    = (4/(n*pi)) * (1 - (-1)^n)
    % So, bn = 0 if n is even, and bn = 8/(n*pi) if n is odd.
    % However, a simpler form for this specific square wave is often given as:
    % f(t) = (4/pi) * sum_{k=1}^{N_odd_terms} (1/(2k-1)) * sin((2k-1)*pi*t/L)
    % where N_odd_terms corresponds to the number of *odd* terms.
    % If N is the total number of terms (1, 2, 3, ...), we sum up to n=N.

    for n = 1:N % Sum up to N terms
        if mod(n, 2) ~= 0 % Only odd terms contribute (n = 1, 3, 5, ...)
            bn = 4 / (n * pi);
            f_approx = f_approx + bn * sin(n * t); % L=pi, so n*pi*t/L becomes n*t
        end
    end

    plot(t, f_approx, 'Color', colors(k_idx,:), 'LineWidth', 1, 'DisplayName', ['N = ' num2str(N) ' terms']);
end

% --- Add details for Gibbs phenomenon ---
% The overshoot is approximately 9% of the jump.
% Jump = 1 - (-1) = 2.
% Overshoot height = 0.09 * 2 = 0.18 (approximately)
% So the peak will be around 1 + 0.09*(1 - (-1))/2 = 1.09 (for the positive jump)
% or more precisely, the overshoot is (2/pi) * integral_0^pi (sin(x)/x) dx - 1 which is approx 0.08949 * (Jump Height)
jump_height = 2; % From -1 to 1
overshoot_theoretical_max = 1 + (2/pi * 1.8519 - 1) * (jump_height/2); % Si(pi) approx 1.8519
undershoot_theoretical_min = -1 - (2/pi * 1.8519 - 1) * (jump_height/2);

plot([min(t), max(t)], [overshoot_theoretical_max, overshoot_theoretical_max], 'r:', 'DisplayName', 'Theoretical Overshoot Max');
plot([min(t), max(t)], [undershoot_theoretical_min, undershoot_theoretical_min], 'b:', 'DisplayName', 'Theoretical Undershoot Min');


legend('show', 'Location', 'EastOutside');
ylim([-1.5, 1.5]); % Adjust y-axis limits to better see the overshoot
hold off;

disp('Gibbs Phenomenon Demonstration:');
disp('The plot shows the Fourier series approximation of a square wave.');
disp('Notice the overshoot and undershoot near the discontinuities (t=0, t=pi, t=-pi).');
disp('As the number of terms (N) increases, the approximation gets better overall,');
disp('but the height of the overshoot remains significant (approx. 9% of the jump),');
disp('though it gets squeezed into a narrower region around the discontinuity.');