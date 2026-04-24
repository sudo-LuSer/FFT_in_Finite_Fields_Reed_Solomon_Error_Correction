----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/09/2026 03:14:17 PM
-- Design Name: 
-- Module Name: Traitement - Behavioral
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

entity Traitement is
    Port ( 
       rst : in STD_LOGIC;
       clk : in STD_LOGIC;
       dv_in_tb : in std_logic;
       enable : in STD_LOGIC;
       stream_in : in STD_LOGIC_VECTOR(7 downto 0);
       stream_out : out STD_LOGIC_VECTOR(7 downto 0);
       dv_out_tb : out std_logic;
       chgt_val : out std_logic;
       num_pos_1 : in std_logic_vector(2 downto 0);
       num_pos_2 : in std_logic_vector(2 downto 0);
       num_pos_3 : in std_logic_vector(2 downto 0);
       pos_1 : in std_logic_vector(7 downto 0);
       pos_2 : in std_logic_vector(7 downto 0);
       pos_3 : in std_logic_vector(7 downto 0)
        );
end Traitement;

architecture Behavioral of Traitement is

component Rotor is
    Port ( 
        rst : in std_logic;
        clk,enable : in std_logic;
        num_rot : in std_logic_vector(2 downto 0);
        data_in : in std_logic_vector(7 downto 0);
        pos_ini : in std_logic_vector(7 downto 0);
        dv_in : in std_logic;
        data_out : out std_logic_vector(7 downto 0);
        dv_out : out std_logic
    );
end component;

component Rotor_inv is
    Port ( 
        rst : in std_logic;
        clk,enable : in std_logic;
        num_rot : in std_logic_vector(2 downto 0);
        pos_ini : in std_logic_vector(7 downto 0);
        data_in : in std_logic_vector(7 downto 0);
        dv_in : in std_logic;
        data_out : out std_logic_vector(7 downto 0);
        dv_out : out std_logic
    );
end component;

component plugboard is
    Port (
        clk : in std_logic;
        data_rotor_out : in std_logic_vector(7 downto 0);
        data_rotor_out_valid : in std_logic;
        data_plugboard_out : out std_logic_vector(7 downto 0);
        data_plugboard_out_valid : out std_logic
    );
end component;

component reflecteur is
    Port (
    data_in : in std_logic_vector(7 downto 0);
    clk : in std_logic;
    data_out : out std_logic_vector(7 downto 0));
end component;

component Bascule is
    Port ( 
        v_in,clk : in std_logic;
        rst : in std_logic;
        v_out : out std_logic
    );
end component;

signal dv_out,dv_plug_out,dv_in,tmp_1,tmp_2,tmp_3,tmp_4,tmp_5,tmp_6,tmp_7,tmp_8,tmp_9,tmp_10,tmp_11,tmp_12,tmp_13,rand_1,rand_2,rand_3,rand_4,dv_1,dv_2 : std_logic := '0';
signal data_1,data_2,data_3,data_4,data_5,data_6,data_plugboard_out,data_to_plug : std_logic_vector(7 downto 0);

begin

process(clk)
begin
    if rising_edge(clk) then
        if (tmp_2 = '1')then
            if (enable = '1')then
                dv_in <= '1';
            else
                dv_in <= '0';
            end if;
        else 
            dv_in <= '0';
        end if;
    end if;
end process;

DUT : Rotor
        port map (
            clk      => clk,
            rst => rst,
            enable => enable,
            pos_ini => pos_3,
            num_rot => num_pos_3,
            data_in  => data_plugboard_out,
            dv_in => dv_plug_out,
            dv_out => dv_1,
            data_out => data_1
        );
        
DUT2 : Rotor
        port map (
            clk      => clk,
            rst => rst,
            num_rot => num_pos_2,
            enable => enable,
            pos_ini => pos_2,
            data_in  => data_1,
            dv_in => dv_1,
            dv_out => dv_2,
            data_out => data_2
        );   

DUT3 : Rotor
        port map (
            clk      => clk,
            rst => rst,
            enable => enable,
            pos_ini => pos_1,
            num_rot => num_pos_1,
            data_in  => data_2,
            dv_in => dv_2,
            dv_out => rand_4,
            data_out => data_3
        );
        
REF : reflecteur
    port map(
        data_in => data_3,
        clk => clk,
        data_out => data_4
    );
    
DUT3R : Rotor_inv
        port map (
            clk      => clk,
            rst => rst,
            enable => enable,
            pos_ini => pos_1,
            num_rot => num_pos_1,
            data_in  => data_4,
            dv_in => dv_2,
            dv_out => rand_1,
            data_out => data_5
        );
        
DUT2R : Rotor_inv
        port map (
            clk      => clk,
            rst => rst,
            enable => enable,
            pos_ini => pos_2,
            num_rot => num_pos_2,
            data_in  => data_5,
            dv_in => dv_1,
            dv_out => rand_2,
            data_out => data_6
        );
        
        
DUTR : Rotor_inv
        port map (
            clk      => clk,
            rst => rst,
            enable => enable,
            pos_ini => pos_3,
            num_rot => num_pos_3,
            data_in  => data_6,
            dv_in => dv_plug_out,
            dv_out => rand_3,
            data_out => data_to_plug
        );
        
Plug_aller : plugboard
        Port map(
            clk => clk,
            data_rotor_out => stream_in,
            data_rotor_out_valid => dv_in,
            data_plugboard_out => data_plugboard_out,
            data_plugboard_out_valid => dv_plug_out);
            
Plug_retour : plugboard
        Port map(
            clk => clk,
            data_rotor_out => data_to_plug,
            data_rotor_out_valid => tmp_12,
            data_plugboard_out => stream_out,
            data_plugboard_out_valid => rand_4);        
 
        
tempo_1 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => dv_in_tb,
        v_out => tmp_1);

tempo_2 : Bascule
    Port map(
        clk => clk,
        v_in => tmp_1,
        rst => rst,
        v_out => tmp_2);

tempo_3 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_2,
        v_out => tmp_3);    

tempo_4 : Bascule
    Port map(
        clk => clk,
        v_in => tmp_3,
        rst => rst,
        v_out => tmp_4);

tempo_5 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_4,
        v_out => tmp_5);

tempo_6 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_5,
        v_out => tmp_6);

tempo_7 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_6,
        v_out => tmp_7);

tempo_8 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_7,
        v_out => tmp_8);
        
tempo_9 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_8,
        v_out => tmp_9);
        
tempo_10 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_9,
        v_out => tmp_10);

tempo_11 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_10,
        v_out => tmp_11);

tempo_12 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_11,
        v_out => tmp_12);

tempo_13 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_12,
        v_out => tmp_13);
        
tempo_14 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_13,
        v_out => dv_out);

process(clk)
begin 
    if rising_edge(clk)then
        chgt_val <= tmp_13;
        dv_out_tb <= dv_out;
    end if;
end process;

end Behavioral;
