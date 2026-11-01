% filepath: /Users/bearxiong/Documents/ShanghaiTech/SI114_2025_spring/HW2/problem1_part3.m
function problem1_part3()
    % FEM solution to plot solutions for n = 4, 8, 1000
    
    n_values = [4, 8, 1000];
    colors = {'r-o', 'b-*', 'g-'};
    legends = {};
    
    figure('Position', [100, 100, 800, 500]);
    hold on;
    
    for idx = 1:length(n_values)
        n = n_values(idx);
        legends{idx} = sprintf('n = %d', n);
        
        % Generate grid points
        x = linspace(0, 1, n+1);
        
        % Initialize stiffness matrix and force vector
        A = zeros(n+1, n+1);
        f = zeros(n+1, 1);
        
        % Element width
        h = 1/n;
        
        % Assemble the stiffness matrix and force vector
        for i = 1:n
            x_left = x(i);
            x_right = x(i+1);
            
            % Determine C(x) for this element
            if x_right <= 0.5
                C = 1;
            elseif x_left >= 0.5
                C = 0.5;
            else
                % Element contains the interface at x=0.5
                % Use more accurate handling for interface elements
                mid_point = 0.5;
                left_ratio = (mid_point - x_left) / h;
                
                % Contribution from left part (C=1)
                A_left = 1 * [1, -1; -1, 1] / (h * left_ratio);
                f_left = h * left_ratio * [0.5; 0.5];
                
                % Contribution from right part (C=0.5)
                A_right = 0.5 * [1, -1; -1, 1] / (h * (1 - left_ratio));
                f_right = h * (1 - left_ratio) * [0.5; 0.5];
                
                % Combine contributions
                A_local = A_left + A_right;
                f_local = f_left + f_right;
                
                A(i:i+1, i:i+1) = A(i:i+1, i:i+1) + A_local;
                f(i:i+1) = f(i:i+1) + f_local;
                
                continue;
            end
            
            % Local stiffness matrix
            A_local = C * [1, -1; -1, 1] / h;
            
            % Local force vector (from right-hand side = 1)
            f_local = h * [0.5; 0.5];
            
            % Assemble into global matrices
            A(i:i+1, i:i+1) = A(i:i+1, i:i+1) + A_local;
            f(i:i+1) = f(i:i+1) + f_local;
        end
        
        % Apply boundary conditions (u(0) = u(1) = 0)
        A_reduced = A(2:n, 2:n);
        f_reduced = f(2:n);
        
        % Solve the linear system
        u_reduced = A_reduced \ f_reduced;
        
        % Reconstruct full solution with boundary conditions
        u = zeros(n+1, 1);
        u(2:n) = u_reduced;
        
        % For high n, use fewer markers
        if n == 1000
            plot(x, u, colors{idx}, 'MarkerIndices', 1:100:n+1);
        else
            plot(x, u, colors{idx});
        end
    end
    
    % Add a vertical line at x = 0.5 to denote the interface
    plot([0.5 0.5], ylim, 'k--');
    
    % Add labels and legend
    xlabel('x');
    ylabel('u(x)');
    title('FEM Solutions for Different Mesh Sizes');
    legend(legends, 'Location', 'best');
    grid on;
    
    % Add text annotation for interface
    text(0.51, 0.9*max(ylim), 'C(x) interface', 'FontSize', 10);
    
    hold off;
    
    % Save the figure
    saveas(gcf, 'fem_solutions.png');
    fprintf('Figure saved as fem_solutions.png\n');
end