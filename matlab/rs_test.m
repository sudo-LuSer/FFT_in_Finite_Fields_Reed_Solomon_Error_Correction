m = 3;           % Number of bits per symbol
n = 2^m - 1;     % Codeword length 
k = 3;           % Message length

msg = gf([1 2 3],m)
code = rsenc(msg,n,k)