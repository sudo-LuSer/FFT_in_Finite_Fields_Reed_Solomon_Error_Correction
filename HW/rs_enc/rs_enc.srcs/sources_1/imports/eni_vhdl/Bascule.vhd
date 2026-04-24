----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/09/2026 02:53:56 PM
-- Design Name: 
-- Module Name: Bascule - Behavioral
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
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Bascule is
    Port ( 
        v_in,clk,rst : in std_logic;
        v_out : out std_logic
    );
end Bascule;

architecture Behavioral of Bascule is

begin

bascule_1 : process(clk)
begin
    if (rst = '1')then
        v_out <= '0';
    else
        if rising_edge(clk) then 
            if (v_in = '1')then
                v_out <= '1';
            else 
                v_out <= '0';  
            end if;
        end if;
    end if;
end process;

end Behavioral;
