----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 01/26/2026 03:21:51 PM
-- Design Name: 
-- Module Name: RAM_rot - Behavioral
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

entity RAM_rot is
    Port (data_in : in std_logic_vector(7 downto 0);
        num_rot : in std_logic_vector(2 downto 0);
        clk : in std_logic;
        data_out : out std_logic_vector(7 downto 0));
end RAM_rot;

architecture Behavioral of RAM_rot is

type grid_ram_type is array (0 to 25) of std_logic_vector(6 downto 0); 
signal RAM_rot_1, RAM_rot_2, RAM_rot_3 : grid_ram_type := (others => std_logic_vector(to_unsigned(0, 7)));

begin
    
    RAM_rot_1(0)  <= "1000101"; -- A -> E (69)
    RAM_rot_1(1)  <= "1001011"; -- B -> K (75)
    RAM_rot_1(2)  <= "1001101"; -- C -> M (77)
    RAM_rot_1(3)  <= "1000110"; -- D -> F (70)
    RAM_rot_1(4)  <= "1001100"; -- E -> L (76)
    RAM_rot_1(5)  <= "1000111"; -- F -> G (71)
    RAM_rot_1(6)  <= "1000100"; -- G -> D (68)
    RAM_rot_1(7)  <= "1010001"; -- H -> Q (81)
    RAM_rot_1(8)  <= "1010110"; -- I -> V (86)
    RAM_rot_1(9)  <= "1011010"; -- J -> Z (90)
    RAM_rot_1(10) <= "1001110"; -- K -> N (78)
    RAM_rot_1(11) <= "1010100"; -- L -> T (84)
    RAM_rot_1(12) <= "1001111"; -- M -> O (79)
    RAM_rot_1(13) <= "1010111"; -- N -> W (87)
    RAM_rot_1(14) <= "1011001"; -- O -> Y (89)
    RAM_rot_1(15) <= "1001000"; -- P -> H (72)
    RAM_rot_1(16) <= "1011000"; -- Q -> X (88)
    RAM_rot_1(17) <= "1010101"; -- R -> U (85)
    RAM_rot_1(18) <= "1010011"; -- S -> S (83)
    RAM_rot_1(19) <= "1010000"; -- T -> P (80)
    RAM_rot_1(20) <= "1000001"; -- U -> A (65)
    RAM_rot_1(21) <= "1001001"; -- V -> I (73)
    RAM_rot_1(22) <= "1000010"; -- W -> B (66)
    RAM_rot_1(23) <= "1010010"; -- X -> R (82)
    RAM_rot_1(24) <= "1000011"; -- Y -> C (67)
    RAM_rot_1(25) <= "1001010"; -- Z -> J (74)


    RAM_rot_2(0)  <= "1000001"; -- A -> A
    RAM_rot_2(1)  <= "1001010"; -- B -> J
    RAM_rot_2(2)  <= "1000100"; -- C -> D
    RAM_rot_2(3)  <= "1001011"; -- D -> K
    RAM_rot_2(4)  <= "1010011"; -- E -> S
    RAM_rot_2(5)  <= "1001001"; -- F -> I
    RAM_rot_2(6)  <= "1010010"; -- G -> R
    RAM_rot_2(7)  <= "1010101"; -- H -> U
    RAM_rot_2(8)  <= "1011000"; -- I -> X
    RAM_rot_2(9)  <= "1000010"; -- J -> B
    RAM_rot_2(10) <= "1001100"; -- K -> L
    RAM_rot_2(11) <= "1001000"; -- L -> H
    RAM_rot_2(12) <= "1010111"; -- M -> W
    RAM_rot_2(13) <= "1010100"; -- N -> T
    RAM_rot_2(14) <= "1001101"; -- O -> M
    RAM_rot_2(15) <= "1000011"; -- P -> C
    RAM_rot_2(16) <= "1010001"; -- Q -> Q
    RAM_rot_2(17) <= "1000111"; -- R -> G
    RAM_rot_2(18) <= "1011010"; -- S -> Z
    RAM_rot_2(19) <= "1001110"; -- T -> N
    RAM_rot_2(20) <= "1010000"; -- U -> P
    RAM_rot_2(21) <= "1011001"; -- V -> Y
    RAM_rot_2(22) <= "1000110"; -- W -> F
    RAM_rot_2(23) <= "1010110"; -- X -> V
    RAM_rot_2(24) <= "1001111"; -- Y -> O
    RAM_rot_2(25) <= "1000101"; -- Z -> E


    RAM_rot_3(0)  <= "1000010"; -- A -> B
    RAM_rot_3(1)  <= "1000100"; -- B -> D
    RAM_rot_3(2)  <= "1000110"; -- C -> F
    RAM_rot_3(3)  <= "1001000"; -- D -> H
    RAM_rot_3(4)  <= "1001010"; -- E -> J
    RAM_rot_3(5)  <= "1001100"; -- F -> L
    RAM_rot_3(6)  <= "1000011"; -- G -> C
    RAM_rot_3(7)  <= "1010000"; -- H -> P
    RAM_rot_3(8)  <= "1010010"; -- I -> R
    RAM_rot_3(9)  <= "1010100"; -- J -> T
    RAM_rot_3(10) <= "1011000"; -- K -> X (Au lieu de V)
    RAM_rot_3(11) <= "1010110"; -- L -> V (Au lieu de X)
    RAM_rot_3(12) <= "1011010"; -- M -> Z
    RAM_rot_3(13) <= "1001110"; -- N -> N
    RAM_rot_3(14) <= "1011001"; -- O -> Y
    RAM_rot_3(15) <= "1000101"; -- P -> E
    RAM_rot_3(16) <= "1001001"; -- Q -> I
    RAM_rot_3(17) <= "1010111"; -- R -> W
    RAM_rot_3(18) <= "1000111"; -- S -> G
    RAM_rot_3(19) <= "1000001"; -- T -> A
    RAM_rot_3(20) <= "1001011"; -- U -> K
    RAM_rot_3(21) <= "1001101"; -- V -> M
    RAM_rot_3(22) <= "1010101"; -- W -> U
    RAM_rot_3(23) <= "1010011"; -- X -> S
    RAM_rot_3(24) <= "1010001"; -- Y -> Q
    RAM_rot_3(25) <= "1001111"; -- Z -> O


process(clk)
begin
    if rising_edge (clk) then
        if (unsigned(data_in) < to_unsigned(91,8) and unsigned(data_in) > to_unsigned(64,8))then
            if (unsigned(num_rot) = to_unsigned(1,3))then
                data_out <= '0'&RAM_rot_1(to_integer(unsigned(data_in)-to_unsigned(65,8)));
            elsif(unsigned(num_rot) = to_unsigned(2,3))then
                data_out <= '0'&RAM_rot_2(to_integer(unsigned(data_in)-to_unsigned(65,8)));
            else
                data_out <= '0'&RAM_rot_3(to_integer(unsigned(data_in)-to_unsigned(65,8)));
            end if;
        else
            data_out <= data_in;
        end if;
    end if;
end process;

end Behavioral;
