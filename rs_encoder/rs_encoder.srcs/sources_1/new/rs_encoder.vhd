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
      message : in std_logic_vector(k*m downto 0);
      codeword : in std_logic_vector(n*m downto 0));
end rs_encoder;

architecture Behavioral of rs_encoder is

begin


end Behavioral;
