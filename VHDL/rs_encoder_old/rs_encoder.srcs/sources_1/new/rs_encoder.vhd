----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 20.03.2026 02:08:18
-- Design Name: 
-- Module Name: rs_encoder - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity rs_encoder is
generic(m : integer := 3;
        n : integer := 7;
        k : integer := 3;
        t : integer := 2);
Port (clk : in std_logic;
      rst : in std_logic; 
      message : in std_logic_vector(k*m-1 downto 0);
      codeword : out std_logic_vector(n*m-1 downto 0));
end rs_encoder;

architecture Behavioral of rs_encoder is

type prim_poly is array (0 to 20) of std_logic_vector(31 downto 0);
type gen_poly is array (0 to n-k) of std_logic_vector(m-1 downto 0);
type alpha_array is array (0 to 2**m -1) of std_logic_vector(m-1 downto 0);
type index_array is array (0 to 2**m -1) of integer range -1 to 2**m-1;


signal val      : unsigned(m downto 0) := to_unsigned(1, m+1);
signal cpt_val  : unsigned(m-1 downto 0) := (others => '0');

signal alpha_to : alpha_array;
signal index_of : index_array;

signal poly_gen : gen_poly;

signal parity : std_logic_vector((n-k)*m -1 downto 0); 

constant poly : prim_poly := (
    0  => x"00000000",
    1  => x"00000000",
    2  => x"00000007",
    3  => x"0000000B",
    4  => x"00000013",
    5  => x"00000025",
    6  => x"00000043",
    7  => x"00000083",
    8  => x"0000011D",
    9  => x"00000211",
    10 => x"00000409",
    11 => x"00000805",
    12 => x"00001053",
    13 => x"0000201B",
    14 => x"00004443",
    15 => x"00008003",
    16 => x"0001100B",
    17 => x"00080009",
    18 => x"00100065",
    19 => x"00200009",
    20 => x"00080021"
);

begin

index_of(0) <= -1;

define_gf_m : process(clk)
begin 
    if rising_edge(clk) then

        if rst = '1' then
            val     <= to_unsigned(1, m+1);
            cpt_val <= (others => '0');

        elsif cpt_val < to_unsigned(2**m - 1, m) then

            alpha_to(to_integer(cpt_val)) <= std_logic_vector(val(m-1 downto 0));
            index_of(to_integer(val(m-1 downto 0))) <= to_integer(cpt_val);

            if val(m) = '1' then
                val <= (val(m-1 downto 0) & '0') xor unsigned(poly(m)(m downto 0));
            else
                val <= (val(m-1 downto 0) & '0');
            end if;

            cpt_val <= cpt_val + 1;

        end if;

    end if;    
end process;

poly_gen(0) <= std_logic_vector(TO_UNSIGNED(1, m));

define_generator : process(clk)
variable a_idx, sum_idx : integer range 0 to 2**m-2;
constant modulus : integer := 2**m - 1;
begin 
    if(rising_edge(clk))then
        if (poly_gen(n-k) = std_logic_vector(TO_UNSIGNED(0, m))) then 
            poly_gen(0) <= alpha_to(index_of(to_integer(unsigned(poly_gen(0))) +1));
            a_idx := index_of(to_integer(unsigned(poly_gen(to_integer(unsigned(cpt_val+1))))));
            sum_idx := a_idx + 1;
            if sum_idx >= modulus then
                sum_idx := sum_idx - modulus; 
            end if;
            poly_gen(to_integer(unsigned(cpt_val+1))) <= poly_gen(to_integer(unsigned(cpt_val+1))-1) xor alpha_to(sum_idx);        
        end if;
    end if;
end process define_generator; 

--encode : process(clk)
--variable sous_msg : std_logic_vector(k-1 downto 0); 
--begin 
--    for i in 0 to m-1 loop 
--        sous_msg := message(i*m +k-1 downto 0);
--        for j in 0 to k-1 loop
--        end loop; 
--    end loop; 
--end process encode; 

codeword(n*m-1 downto (n-k)*m) <= message;
codeword(m downto 0) <= std_logic_vector(val);

end Behavioral;
