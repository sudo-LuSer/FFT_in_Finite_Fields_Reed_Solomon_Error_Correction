----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/22/2026 09:09:32 AM
-- Design Name: 
-- Module Name: pc_rs - Behavioral
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

entity pc_rs is
    Port ( 
        clk      : in  std_logic;
        rst      : in  std_logic;    -- asynchrone
        sync_rst : in  std_logic;    -- synchrone (RAZ en début de trame)
        enable   : in  std_logic;
        counter  : out std_logic_vector(2 downto 0)
    );
end pc_rs;

architecture Behavioral of pc_rs is
    signal cmp : unsigned(2 downto 0) := (others => '0');
begin
    process(clk)
    begin
        if rising_edge(clk) then
            if sync_rst = '1' then
                cmp <= (others => '0');
            elsif enable = '1' then
                if cmp = to_unsigned(6, 3) then
                    cmp <= (others => '0');
                else
                    cmp <= cmp + 1;
                end if;
            end if;
        end if;
    end process;
    counter <= std_logic_vector(cmp);
end Behavioral;
