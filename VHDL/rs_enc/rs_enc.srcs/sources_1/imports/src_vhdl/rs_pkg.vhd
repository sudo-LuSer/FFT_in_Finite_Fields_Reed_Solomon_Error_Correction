library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

package rs_pkg is
    constant N       : integer := 7;
    constant K       : integer := 3;
    constant N_RDNCY : integer := 4;
    
    type gf_array_t is array (0 to N_RDNCY-1) of std_logic_vector(2 downto 0);
    
    -- RS(7,5) dans GF(2^3)
    -- g(x) = x^2 + (alpha^2+alpha)*x + (alpha+1). En binaire : 6="110", 3="011"
    constant G_COEF : gf_array_t := ("011", "010", "001", "011");    
    -- Multiplication dans GF(2^3)
    function gf_mult_3(a : std_logic_vector(2 downto 0); b : std_logic_vector(2 downto 0)) return std_logic_vector;
end package rs_pkg;

package body rs_pkg is
    -- P(x) = x^3 + x + 1 (binaire 1011)
    function gf_mult_3(a : std_logic_vector(2 downto 0); b : std_logic_vector(2 downto 0)) return std_logic_vector is
        variable p : std_logic_vector(2 downto 0) := (others => '0'); -- résultat
        variable a_var : std_logic_vector(2 downto 0) := a; -- Copies locales de 
        variable b_var : std_logic_vector(2 downto 0) := b; -- a et b pour les manipuler
        variable high_bit : std_logic; -- MSB
    begin
        for i in 0 to 2 loop
            if b_var(i) = '1' then
                p := p xor a_var;
            end if;
            high_bit := a_var(2); -- stocker la valeur du MSB de a
            a_var := a_var(1 downto 0) & '0'; 
            if high_bit = '1' then
                a_var := a_var xor "011"; 
            end if;
        end loop;
        return p;
    end function;
end package body rs_pkg;