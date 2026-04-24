----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02/09/2026 04:20:17 PM
-- Design Name: 
-- Module Name: reflecteur - Behavioral
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

entity reflecteur is
    Port (
    data_in : in std_logic_vector(7 downto 0);
    clk : in std_logic;
    data_out : out std_logic_vector(7 downto 0));
end reflecteur;

architecture Behavioral of reflecteur is

type grid_ram_type is array (0 to 25) of std_logic_vector(6 downto 0); 
signal RAM_ref : grid_ram_type := (others => std_logic_vector(to_unsigned(0, 7)));

begin

    RAM_ref(0)  <= "1011001"; -- A -> Y (89)
    RAM_ref(1)  <= "1010010"; -- B -> R (82)
    RAM_ref(2)  <= "1010101"; -- C -> U (85)
    RAM_ref(3)  <= "1001000"; -- D -> H (72)
    RAM_ref(4)  <= "1010001"; -- E -> Q (81)
    RAM_ref(5)  <= "1010011"; -- F -> S (83)
    RAM_ref(6)  <= "1001100"; -- G -> L (76)
    RAM_ref(7)  <= "1000100"; -- H -> D (68)
    RAM_ref(8)  <= "1010000"; -- I -> P (80)
    RAM_ref(9)  <= "1011000"; -- J -> X (88)
    RAM_ref(10) <= "1001110"; -- K -> N (78)
    RAM_ref(11) <= "1000111"; -- L -> G (71)
    RAM_ref(12) <= "1001111"; -- M -> O (79)
    RAM_ref(13) <= "1001011"; -- N -> K (75)
    RAM_ref(14) <= "1001101"; -- O -> M (77)
    RAM_ref(15) <= "1001001"; -- P -> I (73)
    RAM_ref(16) <= "1000101"; -- Q -> E (69)
    RAM_ref(17) <= "1000010"; -- R -> B (66)
    RAM_ref(18) <= "1000110"; -- S -> F (70)
    RAM_ref(19) <= "1011010"; -- T -> Z (90)
    RAM_ref(20) <= "1000011"; -- U -> C (67)
    RAM_ref(21) <= "1010111"; -- V -> W (87)
    RAM_ref(22) <= "1010110"; -- W -> V (86)
    RAM_ref(23) <= "1001010"; -- X -> J (74)
    RAM_ref(24) <= "1000001"; -- Y -> A (65)
    RAM_ref(25) <= "1010100"; -- Z -> T (84)

process(clk)
begin
    if rising_edge (clk) then
        if (unsigned(data_in) < to_unsigned(91,8) and unsigned(data_in) > to_unsigned(64,8))then
            data_out <= '0'&RAM_ref(to_integer(unsigned(data_in)-to_unsigned(65,8)));
        else
            data_out <= data_in;
        end if;
    end if;
end process;


end Behavioral;
