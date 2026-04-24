----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/24/2026 09:36:24 AM
-- Design Name: 
-- Module Name: control_sys - Behavioral
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

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity control_sys is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           compute_valid : out STD_LOGIC;
           eni_valid : in STD_LOGIC;
           ecc_valid : in STD_LOGIC;
           en_ecc    : out STD_LOGIC;
           en_eni    : out STD_LOGIC;
           en_in     : in STD_LOGIC);
end control_sys;

architecture Behavioral of control_sys is
    type state_type is (IDLE, ENIGMA_ENC, COMPUTE_ENIGMA, ECC, READY);
    signal state      : state_type := IDLE;
    signal en_in_prev : STD_LOGIC := '0';  
begin

    process(clk, rst)
    begin
        if rst = '1' then
            state      <= IDLE;
            en_eni     <= '0';
            en_ecc     <= '0';
            compute_valid <= '0';
            en_in_prev <= '0';
        elsif rising_edge(clk) then
            if en_in = '1' and en_in_prev = '0' then
                if state = IDLE then
                    state <= ENIGMA_ENC;
                end if;
            end if;
            en_in_prev <= en_in;

            case state is
                when IDLE =>
                    en_eni      <= '0';
                    en_ecc      <= '0';
                    compute_valid <= '0';

                when ENIGMA_ENC =>
                    en_eni      <= '1';
                    en_ecc      <= '0';
                    compute_valid <= '0';
                    state <= COMPUTE_ENIGMA; 

                when COMPUTE_ENIGMA =>
                    en_eni      <= '0';
                    en_ecc      <= '0';
                    compute_valid <= '0';
                    if eni_valid = '1' then
                        state <= ECC;
                    end if;

                when ECC =>
                    en_eni      <= '0';
                    en_ecc      <= '1';
                    compute_valid <= '0';
                    if ecc_valid = '1' then
                        state <= READY;
                    end if;

                when READY =>
                    en_eni      <= '0';
                    en_ecc      <= '0';
                    compute_valid <= '1';  
                    state <= IDLE;      
            end case;
        end if;
    end process;

end Behavioral;
