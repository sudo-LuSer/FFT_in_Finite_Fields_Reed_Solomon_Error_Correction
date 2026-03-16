m = 8;           % Number of bits per symbol
n = 2^m - 1;     % Codeword length 
k = 3;           % Message length

msg = gf([3 2 1],m)
code = rsenc(msg,n,k)