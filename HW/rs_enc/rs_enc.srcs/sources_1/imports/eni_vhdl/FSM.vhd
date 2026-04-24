----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/10/2026 05:12:51 PM
-- Design Name: 
-- Module Name: FSM - Behavioral
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

entity FSM is
    Port (
        rst : in STD_LOGIC;
        clk : in STD_LOGIC;
        stream_in : in STD_LOGIC_VECTOR(7 downto 0);
        data_valid_trait : out std_logic;
        data_valid_mem_init : out std_logic
        );
end FSM;

architecture Behavioral of FSM is

type fsm_state is (RESET,INIT,TRAITEMENT);
signal next_state , current_state : fsm_state;

begin

process(clk)

begin

    IF (Clk'EVENT AND Clk = '1') THEN
        IF Rst = '1' THEN
            current_state <= RESET;
        ELSE
            current_state <= next_state;
        END IF;
     END IF;
     
END PROCESS;

process(rst,stream_in)
begin
case current_state is 
    when RESET => if rst = '1' then next_state <= RESET;
                    else next_state <= INIT;
                    end if;
                
    when INIT => if (unsigned(stream_in) < to_unsigned(65,8))then next_state <= INIT;
                    else next_state <= TRAITEMENT;
                    end if;
    
    when TRAITEMENT => next_state <= TRAITEMENT;
end case;
end process;

process ( current_state)
begin                        
                                        
case current_state is 
    when RESET => data_valid_trait <= '0';
                    data_valid_mem_init <= '0';
    when INIT => data_valid_trait <= '0';
                data_valid_mem_init <= '1';
    when TRAITEMENT => data_valid_trait <= '1';
                data_valid_mem_init <= '0';

end case;
end process;

end Behavioral;
