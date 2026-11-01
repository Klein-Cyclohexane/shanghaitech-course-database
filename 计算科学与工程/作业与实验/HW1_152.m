K = toeplitz([2,-1,0,0,0])
e = eig(K);
e = sort(e);

disp('Eigenvalues of K5:');
disp(e);
expected_e = 2*ones(5,1) - 2*cos((1:5)'*pi/6);
expected_e = sort(expected_e);

disp('Expected eigenvalues from formula:');
disp(expected_e);

if all(abs(e - expected_e) < 1e-14)
    disp('Verification successful');
else
    disp('Verification failed');
    disp('Difference:');
    disp(e - expected_e);
end