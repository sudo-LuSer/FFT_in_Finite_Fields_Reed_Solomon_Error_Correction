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

entity RAM_rot_inv is
    Port (data_in : in std_logic_vector(7 downto 0);
        num_rot : in std_logic_vector(2 downto 0);
        clk : in std_logic;
        data_out : out std_logic_vector(7 downto 0));
end RAM_rot_inv;

architecture Behavioral of RAM_rot_inv is

type grid_ram_type is array (0 to 25) of std_logic_vector(6 downto 0); 
signal RAM_rot_1_inv, RAM_rot_2_inv, RAM_rot_3_inv : grid_ram_type := (others => std_logic_vector(to_unsigned(0, 7)));

begin
    
    RAM_rot_1_inv(0)  <= "1010101"; -- A -> U
    RAM_rot_1_inv(1)  <= "1010111"; -- B -> W
    RAM_rot_1_inv(2)  <= "1011001"; -- C -> Y
    RAM_rot_1_inv(3)  <= "1000111"; -- D -> G
    RAM_rot_1_inv(4)  <= "1000001"; -- E -> A
    RAM_rot_1_inv(5)  <= "1000100"; -- F -> D
    RAM_rot_1_inv(6)  <= "1000110"; -- G -> F
    RAM_rot_1_inv(7)  <= "1010000"; -- H -> P
    RAM_rot_1_inv(8)  <= "1010110"; -- I -> V
    RAM_rot_1_inv(9)  <= "1011010"; -- J -> Z
    RAM_rot_1_inv(10) <= "1000010"; -- K -> B
    RAM_rot_1_inv(11) <= "1000101"; -- L -> E
    RAM_rot_1_inv(12) <= "1000011"; -- M -> C
    RAM_rot_1_inv(13) <= "1001011"; -- N -> K
    RAM_rot_1_inv(14) <= "1001101"; -- O -> M
    RAM_rot_1_inv(15) <= "1010100"; -- P -> T
    RAM_rot_1_inv(16) <= "1001000"; -- Q -> H
    RAM_rot_1_inv(17) <= "1011000"; -- R -> X
    RAM_rot_1_inv(18) <= "1010011"; -- S -> S
    RAM_rot_1_inv(19) <= "1001100"; -- T -> L
    RAM_rot_1_inv(20) <= "1010010"; -- U -> R
    RAM_rot_1_inv(21) <= "1001001"; -- V -> I
    RAM_rot_1_inv(22) <= "1001110"; -- W -> N
    RAM_rot_1_inv(23) <= "1010001"; -- X -> Q
    RAM_rot_1_inv(24) <= "1001111"; -- Y -> O
    RAM_rot_1_inv(25) <= "1001010"; -- Z -> J


    RAM_rot_2_inv(0)  <= "1000001"; -- A -> A
    RAM_rot_2_inv(1)  <= "1001010"; -- B -> J
    RAM_rot_2_inv(2)  <= "1010000"; -- C -> P
    RAM_rot_2_inv(3)  <= "1000011"; -- D -> C
    RAM_rot_2_inv(4)  <= "1011010"; -- E -> Z
    RAM_rot_2_inv(5)  <= "1010111"; -- F -> W
    RAM_rot_2_inv(6)  <= "1010010"; -- G -> R
    RAM_rot_2_inv(7)  <= "1001100"; -- H -> L
    RAM_rot_2_inv(8)  <= "1000110"; -- I -> F
    RAM_rot_2_inv(9)  <= "1000010"; -- J -> B
    RAM_rot_2_inv(10) <= "1000100"; -- K -> D
    RAM_rot_2_inv(11) <= "1001011"; -- L -> K
    RAM_rot_2_inv(12) <= "1001111"; -- M -> O
    RAM_rot_2_inv(13) <= "1010100"; -- N -> T
    RAM_rot_2_inv(14) <= "1011001"; -- O -> Y
    RAM_rot_2_inv(15) <= "1010101"; -- P -> U
    RAM_rot_2_inv(16) <= "1010001"; -- Q -> Q
    RAM_rot_2_inv(17) <= "1000111"; -- R -> G
    RAM_rot_2_inv(18) <= "1000101"; -- S -> E
    RAM_rot_2_inv(19) <= "1001110"; -- T -> N
    RAM_rot_2_inv(20) <= "1001000"; -- U -> H
    RAM_rot_2_inv(21) <= "1011000"; -- V -> X
    RAM_rot_2_inv(22) <= "1001101"; -- W -> M
    RAM_rot_2_inv(23) <= "1001001"; -- X -> I
    RAM_rot_2_inv(24) <= "1010110"; -- Y -> V
    RAM_rot_2_inv(25) <= "1010011"; -- Z -> S
    

    RAM_rot_3_inv(0)  <= "1010100"; -- A -> T
    RAM_rot_3_inv(1)  <= "1000001"; -- B -> A
    RAM_rot_3_inv(2)  <= "1000111"; -- C -> G
    RAM_rot_3_inv(3)  <= "1000010"; -- D -> B
    RAM_rot_3_inv(4)  <= "1010000"; -- E -> P
    RAM_rot_3_inv(5)  <= "1000011"; -- F -> C
    RAM_rot_3_inv(6)  <= "1010011"; -- G -> S
    RAM_rot_3_inv(7)  <= "1000100"; -- H -> D
    RAM_rot_3_inv(8)  <= "1010001"; -- I -> Q
    RAM_rot_3_inv(9)  <= "1000101"; -- J -> E
    RAM_rot_3_inv(10) <= "1010101"; -- K -> U
    RAM_rot_3_inv(11) <= "1000110"; -- L -> F
    RAM_rot_3_inv(12) <= "1010110"; -- M -> V
    RAM_rot_3_inv(13) <= "1001110"; -- N -> N
    RAM_rot_3_inv(14) <= "1011010"; -- O -> Z
    RAM_rot_3_inv(15) <= "1001000"; -- P -> H
    RAM_rot_3_inv(16) <= "1011001"; -- Q -> Y
    RAM_rot_3_inv(17) <= "1001001"; -- R -> I
    RAM_rot_3_inv(18) <= "1011000"; -- S -> X
    RAM_rot_3_inv(19) <= "1001010"; -- T -> J
    RAM_rot_3_inv(20) <= "1010111"; -- U -> W
    RAM_rot_3_inv(21) <= "1001100"; -- V -> L (Au lieu de K)
    RAM_rot_3_inv(22) <= "1010010"; -- W -> R
    RAM_rot_3_inv(23) <= "1001011"; -- X -> K (Au lieu de L)
    RAM_rot_3_inv(24) <= "1001111"; -- Y -> O
    RAM_rot_3_inv(25) <= "1001101"; -- Z -> M


process(clk)
begin
    if rising_edge (clk) then
        if (unsigned(data_in) < to_unsigned(91,8) and unsigned(data_in) > to_unsigned(64,8))then
            if (unsigned(num_rot) = to_unsigned(1,3))then
                data_out <= '0'&RAM_rot_1_inv(to_integer(unsigned(data_in)-to_unsigned(65,8)));
            elsif(unsigned(num_rot) = to_unsigned(2,3))then
                data_out <= '0'&RAM_rot_2_inv(to_integer(unsigned(data_in)-to_unsigned(65,8)));
            else
                data_out <= '0'&RAM_rot_3_inv(to_integer(unsigned(data_in)-to_unsigned(65,8)));
            end if;
        else
            data_out <= data_in;
        end if;
    end if;
end process;

end Behavioral;
