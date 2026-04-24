----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 01/26/2026 03:18:47 PM
-- Design Name: 
-- Module Name: Cpt_rot - Behavioral
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

entity Cpt_rot is
    Port (
        dv_in : in std_logic;
        clk,rst,enable : in std_logic;
        pos_ini : in std_logic_vector(7 downto 0);
        cpt_out : out std_logic_vector(7 downto 0)
    );
end Cpt_rot;

architecture Behavioral of Cpt_rot is

signal tmp : unsigned(7 downto 0);

begin

process(clk)
begin
    if(rst = '1')then
        tmp <= to_unsigned(0,8);
    else   
        if rising_edge(clk)then
            if enable = '1' then
                if (dv_in = '1') then
                    if (tmp = to_unsigned(25,8))then
                        tmp <= to_unsigned(0,8);
                    else
                        tmp <= tmp + 1;
                    end if;
                end if;
            else 
                tmp <= unsigned(pos_ini);
            end if;
        end if;
    end if;
end process;

cpt_out <= std_logic_vector(tmp);

end Behavioral;
