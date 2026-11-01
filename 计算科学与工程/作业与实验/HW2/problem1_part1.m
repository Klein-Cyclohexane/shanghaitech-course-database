function problem1_part1()
    % FEM solution for the given BVP with n=4 elements
    % -d/dx[C(x)*du/dx] = 1, with u(0) = u(1) = 0
    % where C(x) = 1 for x < 1/2, and C(x) = 1/2 for x >= 1/2
    
    n = 4; % Number of elements
    h = 1/n; % Element size
    
    % Generate grid points
    x = linspace(0, 1, n+1);
    
    % Initialize stiffness matrix and force vector
    A = zeros(n+1, n+1); % Stiffness matrix
    f = zeros(n+1, 1); % Force vector
    
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
            % For simplicity, use average value of C
            C = 0.75;
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
    
    % Display results
    disp('Stiffness Matrix A:');
    disp(A);
    
    disp('Force Vector f:');
    disp(f);
    
    disp('Solution u:');
    disp(u);
    
    % Display the solution at internal nodes
    disp('Solution at internal nodes:');
    for i = 2:n
        fprintf('u(%.4f) = %.6f\n', x(i), u(i));
    end
end