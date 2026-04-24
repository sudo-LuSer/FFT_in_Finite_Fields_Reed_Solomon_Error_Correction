----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/24/2026 09:50:59 AM
-- Design Name: 
-- Module Name: TX_CHIMERE - Behavioral
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

entity TX_CHIMERE is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           data_in : in STD_LOGIC_VECTOR (7 downto 0);
           data_out : out STD_LOGIC_VECTOR (20 downto 0);
           ready : out STD_LOGIC;
           enable : in STD_LOGIC);
end TX_CHIMERE;

architecture Behavioral of TX_CHIMERE is

component rsenc_top_level is
    Port (
        clk        : in  std_logic;
        rst        : in  std_logic;
        enable     : in  std_logic;
        data_in    : in  std_logic_vector (8 downto 0);
        data_out   : out std_logic_vector(20 downto 0);
        ready_flag : out std_logic
    );
end component;

component transmitter is
    Port ( rst : in STD_LOGIC;
           clk : in STD_LOGIC;  
           enable : in STD_LOGIC;
           stream_in : in STD_LOGIC_VECTOR(7 downto 0);
           stream_out : out STD_LOGIC_VECTOR(8 downto 0);
           data_valid : out std_logic);
end component;

component control_sys is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           compute_valid : out STD_LOGIC;
           eni_valid : in STD_LOGIC;
           ecc_valid : in STD_LOGIC;
           en_ecc    : out STD_LOGIC;
           en_eni    : out STD_LOGIC;
           en_in : in STD_LOGIC);
end component;

signal v_eni , v_ecc : STD_LOGIC; 
signal en_eni, en_ecc : STD_LOGIC;

signal eni_out : STD_LOGIC_VECTOR(8 downto 0);

begin


STATE_MACHINE : control_sys port map(
    clk => clk, 
    rst => rst, 
    eni_valid => v_eni, 
    ecc_valid => v_ecc, 
    en_in => enable, 
    en_ecc => en_ecc, 
    en_eni => en_eni,
    compute_valid => ready);
    
ENIGMA : transmitter port map(
    clk => clk, 
    rst => rst, 
    enable => en_eni,
    stream_in => data_in, 
    data_valid => v_eni,
    stream_out => eni_out);
    
ECC : rsenc_top_level port map(
    clk => clk, 
    rst => rst, 
    enable => en_ecc, 
    data_in => eni_out,
    data_out => data_out,
    ready_flag => v_ecc);

end Behavioral;
