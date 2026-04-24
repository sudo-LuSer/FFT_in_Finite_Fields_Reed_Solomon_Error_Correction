----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/18/2017 04:03:43 PM
-- Design Name: 
-- Module Name: transmitter - Behavioral
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

entity transmitter is
    Port ( rst : in STD_LOGIC;
           clk : in STD_LOGIC;  
           enable : in STD_LOGIC;
           stream_in : in STD_LOGIC_VECTOR(7 downto 0);
           stream_out : out STD_LOGIC_VECTOR(8 downto 0);
           data_valid : out std_logic);
end transmitter;

architecture Behavioral of transmitter is   

component Bascule is
    Port ( 
        v_in,clk : in std_logic;
        rst : in std_logic;
        v_out : out std_logic
    );
end component;

component num_selec is
    Port ( 
    clk,rst : in std_logic;
    dv_in_tb : in std_logic;
    enable : in std_logic;
    data_in_tb : in std_logic_vector(7 downto 0);
    num_pos_1 : out std_logic_vector(2 downto 0);
    num_pos_2 : out std_logic_vector(2 downto 0);
    num_pos_3 : out std_logic_vector(2 downto 0);
    pos_1 : out std_logic_vector(7 downto 0);
    pos_2 : out std_logic_vector(7 downto 0);
    pos_3 : out std_logic_vector(7 downto 0)
    );
end component;

component FSM is
    Port (
        rst : in STD_LOGIC;
        clk : in STD_LOGIC;
        stream_in : in STD_LOGIC_VECTOR(7 downto 0);
        data_valid_trait : out std_logic;
        data_valid_mem_init : out std_logic
        );
end component;

component Traitement is
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
end component;


-- Signaux internes du testbench
signal dv_in_tb,dv_out_tb,trait_enable,init_enable,chgt_val    : std_logic := '0';
signal data_in_tb,tmp_out,tmp_in,data_out_trait  : std_logic_vector(7 downto 0) := (others => '0');
signal data_1,data_2,data_3,data_4,data_5,data_6,pos_1,pos_2,pos_3 : std_logic_vector(7 downto 0);
signal num_pos_1, num_pos_2,num_pos_3 : std_logic_vector(2 downto 0);
signal stream_out_8b : std_logic_vector(7 downto 0);


begin

machine_detats: FSM 
    port map(
        clk => clk,
        rst => rst,
        stream_in => data_in_tb,
        data_valid_mem_init => init_enable,
        data_valid_trait => trait_enable
    );

selec : num_selec
    port map(
        clk => clk,
        rst => rst,
        dv_in_tb => dv_in_tb,
        enable => init_enable,
        data_in_tb => data_in_tb,
        num_pos_1 => num_pos_1,
        num_pos_2 => num_pos_2,
        num_pos_3 => num_pos_3,
        pos_1 => pos_1,
        pos_2 => pos_2,
        pos_3 => pos_3
        );

codage : traitement
    port map( 
        clk => clk,
        rst => rst,
        dv_in_tb => dv_in_tb,
        enable => trait_enable,
        stream_in => data_in_tb,
        num_pos_1 => num_pos_1,
        num_pos_2 => num_pos_2,
        num_pos_3 => num_pos_3,
        pos_1 => pos_1,
        pos_2 => pos_2,
        pos_3 => pos_3,
        chgt_val => chgt_val,
        dv_out_tb => dv_out_tb,
        stream_out => data_out_trait
    );         

process( clk)
begin
    if(rising_edge(clk)) then
        if(enable = '1') then
            tmp_in <= stream_in;
        else 
            tmp_in <= tmp_in;
        end if;
            data_in_tb <= tmp_in;
    end if;
    
end process;

process(clk)
begin
    if(rising_edge(clk)) then
        dv_in_tb <= enable;
    end if;
end process;

process(clk)
begin
    if(rising_edge(clk)) then
        if (trait_enable = '1')then
            if(chgt_val = '1') then
                tmp_out <= data_out_trait;
            else 
                tmp_out <= tmp_out;
            end if;
        elsif (init_enable = '1')then
            if(chgt_val = '1') then
                tmp_out <= stream_in;
            else 
                tmp_out <= tmp_out;
            end if;
        end if;
        stream_out_8b <= tmp_out;
    end if;
end process;

process(clk)
begin
    if(rising_edge(clk)) then
        if (trait_enable = '1')then
            data_valid <= dv_out_tb;
        else 
            data_valid <= '0';
        end if;
    end if;
end process;

process(clk)
begin
    if rising_edge(clk)then
        stream_out <= '0'& stream_out_8b;
    end if;
end process;

end Behavioral;