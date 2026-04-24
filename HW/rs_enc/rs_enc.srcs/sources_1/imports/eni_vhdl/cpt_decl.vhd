----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 29.03.2026 14:37:51
-- Design Name: 
-- Module Name: cpt_decl - Behavioral
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
use IEEE.NUMERIC_STD.ALL;

entity cpt_decl is
    Port ( 
        clk     : in  std_logic;
        rst     : in  std_logic;
        decl    : in  std_logic;
        cpt_out : out std_logic_vector(3 downto 0)
    );
end cpt_decl;

architecture Behavioral of cpt_decl is

signal tmp : unsigned(3 downto 0) := (others => '0');

begin

process(clk, rst)
begin
    if rst = '1' then
        tmp <= (others => '0');
        
    elsif rising_edge(clk) then
        if decl = '1' then
            tmp <= tmp + 1;
        end if;
    end if;
end process;

cpt_out <= std_logic_vector(tmp);

end Behavioral;