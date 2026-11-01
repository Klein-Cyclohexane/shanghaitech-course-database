% Metroplis-Hastings algorithm for Bayesian inverse problems
% Assume the problem is: y = f(x) + epsilon where epsilon follows
% N(0,sigma^2). Assume also the prior is standard Gaussian. The proposal
% distribution is taken to be isotropic Gaussian
% x0: initial point, Nsample: the number of samples y: data
close all; clear all;

x0=[-0.8;-0.8];
Nsamp=10^6;
d=1/2;


dim_x = length(x0); % dimensionality of the unknown
beta = 0.1; % stepsize
chain = zeros(dim_x,Nsamp); % save all the samples in chain
chain(:,1) = x0;

x = x0;

ar = 0;

for n = 1:Nsamp-1
    
    z = x + beta*randn(dim_x,1);
    
    px = postdis(x,d);
    pz = postdis(z,d);
    
    a = min(1,pz/px);
    
   if rand<a
       
       x = z;
       ar = ar+1;
   end
   
   chain(:,n+1)=x;
end
 
% compute acceptance rate
ar = ar/Nsamp
% plot for 2D    
plot(chain(1,:),chain(2,:),'*')    

    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
    function p = postdis(x,d)
   % compute the posterior distribution    
    
    sigma = 0.1;

    G = forwardfun(x); 
    
    x_min = -1*ones(size(x));
    x_max = 1*ones(size(x));
    
    if (x>x_min) & (x<x_max)
    
    p = exp(-0.5*sum((G - d).^2)/sigma^2);
    
    else
        p = 0;
    end
    
    end


    function G = forwardfun(x)
            
           % G = x.*3;
            G=x'*x;
        end 