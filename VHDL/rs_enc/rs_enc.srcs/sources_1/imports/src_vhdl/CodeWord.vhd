----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/22/2026 08:57:48 AM
-- Design Name: 
-- Module Name: CodeWord - Behavioral
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

entity CodeWord is
    Port ( 
        clk         : in  std_logic;
        rst         : in  std_logic;
        enable      : in  std_logic;
        addr        : in  std_logic_vector(2 downto 0); 
        data_in     : in  std_logic_vector (2 downto 0);
        data_out0    : out std_logic_vector (2 downto 0);
        data_out1    : out std_logic_vector (2 downto 0);
        data_out2    : out std_logic_vector (2 downto 0);
        data_out3    : out std_logic_vector (2 downto 0);
        data_out4    : out std_logic_vector (2 downto 0);
        data_out5    : out std_logic_vector (2 downto 0);
        data_out6    : out std_logic_vector (2 downto 0));
end CodeWord;

architecture Behavioral of CodeWord is

begin


process(clk)
begin
    if (rst = '1')then
        data_out0 <= (others => '0');
        data_out1 <= (others => '0');
        data_out2 <= (others => '0');
        data_out3 <= (others => '0');
        data_out4 <= (others => '0');
        data_out5 <= (others => '0');
        data_out6 <= (others => '0');
    elsif rising_edge(clk) then 
        if (enable = '1')then
            case addr is 
                when "000" =>
                    data_out0 <= data_in;
                when "001" =>
                    data_out1 <= data_in;
                when "010" =>
                    data_out2 <= data_in;
                when "011" =>
                    data_out3 <= data_in;
                when "100" =>
                    data_out4 <= data_in;
                when "101" =>
                    data_out5 <= data_in;
                when "110" =>
                    data_out6 <= data_in;
                when others =>
                    data_out0 <= data_in;                 
            end case;
        end if;
    end if;
end process;

end Behavioral;
