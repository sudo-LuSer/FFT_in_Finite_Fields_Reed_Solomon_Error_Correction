----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 01/26/2026 04:05:05 PM
-- Design Name: 
-- Module Name: Rotor - Behavioral
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

entity Rotor is
    Port ( 
        clk : in std_logic;
        rst,enable : in std_logic;
        num_rot : in std_logic_vector(2 downto 0);
        data_in : in std_logic_vector(7 downto 0);
        pos_ini : in std_logic_vector(7 downto 0);
        dv_in : in std_logic;
        data_out : out std_logic_vector(7 downto 0);
        dv_out : out std_logic
    );
end Rotor;

architecture Behavioral of Rotor is

component RAM_rot is 
    Port (data_in : in std_logic_vector(7 downto 0);
        num_rot : in std_logic_vector(2 downto 0);
        clk : in std_logic;
        data_out : out std_logic_vector(7 downto 0));
end component;

component ADD is
    Port ( 
        data_ram : in std_logic_vector(7 downto 0);
        data_out : out std_logic_vector(7 downto 0);
        cpt_rot : in std_logic_vector(7 downto 0)
    );
end component;

component SUB is
    Port (
        data_ram : in std_logic_vector(7 downto 0);
        cpt_rot  : in std_logic_vector(7 downto 0);
        data_out : out std_logic_vector(7 downto 0)
    );
end component;

component Cpt_rot is
    Port (
        dv_in : in std_logic;
        clk,rst,enable : in std_logic;
        pos_ini : in std_logic_vector(7 downto 0);
        cpt_out : out std_logic_vector(7 downto 0)
    );
end Component;

signal data_ram_out,data_ram,cpt_inter : std_logic_vector(7 downto 0) := (others => '0');
signal cpt_delay : unsigned(1 downto 0) := (others => '0');
signal dv_mid_1,dv_mid_2 : std_logic;
signal compt_dv : unsigned(7 downto 0) := (others => '0');
signal notch_pos : std_logic_vector(7 downto 0);

signal cpt_inter_delayed : std_logic_vector(7 downto 0) := (others => '0'); --signal rajouté

begin

notch_selec : process(clk)
begin
    if rising_edge(clk) then
        case num_rot is
            when "011" => notch_pos <= std_logic_vector(to_unsigned(21, 8)); -- Encoche sur V            
            when "010" => notch_pos <= std_logic_vector(to_unsigned(4, 8));
            when others => notch_pos <= std_logic_vector(to_unsigned(16, 8));
        end case;
    end if;
end process;

process(clk)
begin
    if rising_edge(clk) then
        cpt_inter_delayed <= cpt_inter;
    end if;
end process;

--bascule_1 : process(clk)
--begin
--    if rising_edge(clk) then 
--        if (dv_in = '1')then
--            if (compt_dv = to_unsigned(25,8))then  
--                dv_out <= '1';
--                compt_dv <= to_unsigned(0,8); 
--            else
--                dv_out <= '0';
--                compt_dv <= compt_dv + 1; 
--            end if;
--        else 
--            dv_out <= '0';  
--            compt_dv <= compt_dv;
--        end if;
--    end if;
--end process;

bascule_1 : process(clk) --process que j ai rajouté pur remplacer l ancien
begin
    if rising_edge(clk) then 
        if (dv_in = '1') then
          
            if (compt_dv = unsigned(notch_pos)) then  --on fait tourner le rotor si on est sur l encoche 
                dv_out <= '1';
            else
                dv_out <= '0';
            end if;

            if (compt_dv = to_unsigned(25,8)) then --rotation classique
                compt_dv <= to_unsigned(0,8); 
            else
                compt_dv <= compt_dv + 1; 
            end if;

        else 
            dv_out <= '0';
            compt_dv <= compt_dv;
        end if;
    end if;
end process;

--bascule_2 : process(clk)
--begin
--    if rising_edge(clk) then 
--        if (dv_mid_1 = '1' and dv_in = '0')then
--            dv_mid_2 <= '1';
--        else
--            dv_mid_2 <= '0';
--        end if;
--    end if;
--end process;

--bascule_3 : process(clk)
--begin
--    if rising_edge(clk) then 
--        if (dv_mid_2 = '1' and dv_mid_1 = '0')then
--            dv_out <= '1';
--        else
--            dv_out <= '0';
--        end if;
--    end if;
--end process;

RAM : RAM_rot
    Port map(
        data_in => data_ram,
        num_rot => num_rot,
        clk => clk,
        data_out => data_ram_out);

adder : ADD
    Port map(
        data_ram => data_in,
        data_out => data_ram,
        cpt_rot => cpt_inter);
        
sous : SUB 
    Port map(
        data_ram => data_ram_out,
        --cpt_rot  => cpt_inter,
        cpt_rot  => cpt_inter_delayed, --rajouté
        data_out => data_out);

compteur : Cpt_rot 
    port map(
        dv_in => dv_in,
        clk => clk,
        enable => enable,
        pos_ini => pos_ini,
        rst => rst,
        cpt_out => cpt_inter);    

end Behavioral;
