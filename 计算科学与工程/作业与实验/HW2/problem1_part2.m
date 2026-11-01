function problem1_part2()
    % FEM solution to calculate u(1/4) and u(3/4) for n = 4, 8, 1000
    
    n_values = [4, 8, 1000];
    
    % Points of interest
    poi = [0.25, 0.75];
    
    % Store results
    results = zeros(length(n_values), length(poi));
    
    for idx = 1:length(n_values)
        n = n_values(idx);
        
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
                % by splitting the element at the interface
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
        
        % Interpolate to find values at points of interest
        for j = 1:length(poi)
            point = poi(j);
            
            % Find the element containing the point
            for i = 1:n
                if point >= x(i) && point <= x(i+1)
                    % Linear interpolation
                    alpha = (point - x(i)) / h;
                    results(idx, j) = (1-alpha) * u(i) + alpha * u(i+1);
                    break;
                end
            end
        end
    end
    
    % Display results
    disp('Results for u(1/4) and u(3/4):');
    for i = 1:length(n_values)
        fprintf('n = %d: u(1/4) = %.6f, u(3/4) = %.6f\n', n_values(i), results(i, 1), results(i, 2));
    end
end