----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/13/2026 02:42:15 PM
-- Design Name: 
-- Module Name: num_selec - Behavioral
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

entity num_selec is
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
end num_selec;

architecture Behavioral of num_selec is

component cpt_decl is
    Port ( 
        clk,rst : in std_logic;
        decl : in std_logic;
        cpt_out : out std_logic_vector(3 downto 0)
    );
end component;

component Bascule is
    Port ( 
        v_in,clk : in std_logic;
        rst : in std_logic;
        v_out : out std_logic
    );
end component;

signal cpt,cpt_out : std_logic_vector(3 downto 0):= (others => '0');
signal num_pos_11,num_pos_22,num_pos_33 : std_logic_vector(7 downto 0);
signal pos_11,pos_22,pos_33 : unsigned(7 downto 0) := (others => '0');
signal decl,tmp_1,tmp_2 : std_logic;

begin

cpt_init : cpt_decl
    Port map(
        clk => clk,
        rst => rst,
        decl => decl,
        cpt_out => cpt_out);

cpt <= cpt_out;

process(clk,rst)
begin
    if (rst = '1') then
        num_pos_11 <= "00000000";
    else
        if rising_edge(clk)then
            if (tmp_1 = '1' and enable = '1')then
                if cpt = "0000" then
                    num_pos_11 <= std_logic_vector(unsigned(data_in_tb)-to_unsigned(48,8)); 
                end if;      
            end if;
        end if;
    end if;
end process;

process(clk,rst)
begin
    if (rst = '1') then
        num_pos_22 <= "00000000";
    else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "0011" then
                num_pos_22 <= std_logic_vector(unsigned(data_in_tb)-to_unsigned(48,8));  
            end if;     
        end if;
        end if; 
    end if;
end process;

process(clk,rst)
begin
    if (rst = '1') then
        num_pos_33 <= "00000000";
    else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "0110" then
                num_pos_33 <= std_logic_vector(unsigned(data_in_tb)-to_unsigned(48,8));       
            end if;
        end if;
        end if; 
    end if;
end process;

process(clk,rst)
begin
     if (rst = '1') then
        pos_11 <= to_unsigned(0,8);
     else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "0001" then
                if (unsigned (data_in_tb) = to_unsigned(48,8))then
                    pos_11 <= to_unsigned(0,8);
                elsif (unsigned (data_in_tb) = to_unsigned(49,8))then
                    pos_11 <= to_unsigned(10,8);
                else
                    pos_11 <= to_unsigned(20,8);
                end if;    
            end if;
        end if;
        end if; 
    end if;
end process;

process(clk,rst)
begin
     if (rst = '1') then
        pos_22 <= to_unsigned(0,8);
     else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "0100" then
                if (unsigned (data_in_tb) = to_unsigned(48,8))then
                    pos_22 <= to_unsigned(0,8);
                elsif (unsigned (data_in_tb) = to_unsigned(49,8))then
                    pos_22 <= to_unsigned(10,8);
                else
                    pos_22 <= to_unsigned(20,8);
                end if;    
            end if;
        end if;
        end if; 
    end if;
end process;

process(clk,rst)
begin
     if (rst = '1') then
        pos_33 <= to_unsigned(0,8);
     else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "0111" then
                if (unsigned (data_in_tb) = to_unsigned(48,8))then
                    pos_33 <= to_unsigned(0,8);
                elsif (unsigned (data_in_tb) = to_unsigned(49,8))then
                    pos_33 <= to_unsigned(10,8);
                else
                    pos_33 <= to_unsigned(20,8);
                end if;    
            end if;
        end if;
        end if; 
    end if;
end process;

process(clk,rst)
begin
    if (rst = '1') then
        pos_1 <= "00000000";
    else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "0010" then
                pos_1 <= std_logic_vector(unsigned(data_in_tb)-to_unsigned(48,8)+pos_11);  
            end if;     
        end if;
        end if; 
    end if;
end process;

process(clk,rst)
begin
    if (rst = '1') then
        pos_2 <= "00000000";
    else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "0101" then
                pos_2 <= std_logic_vector(unsigned(data_in_tb)-to_unsigned(48,8)+pos_22);  
            end if;     
        end if;
        end if; 
    end if;
end process;

process(clk,rst)
begin
    if (rst = '1') then
        pos_3 <= "00000000";
    else
        if rising_edge(clk)then
        if (tmp_1 = '1' and enable = '1')then
            if cpt = "1000" then
                pos_3 <= std_logic_vector(unsigned(data_in_tb)-to_unsigned(48,8)+pos_33);  
            end if;     
        end if;
        end if; 
    end if;
end process;

num_pos_1 <= num_pos_11(2 downto 0);
num_pos_2 <= num_pos_22(2 downto 0);
num_pos_3 <= num_pos_33(2 downto 0);

tempo : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => dv_in_tb,
        v_out => tmp_1);
        
tempo_2 : Bascule
    Port map(
        clk => clk,
        rst => rst,
        v_in => tmp_1,
        v_out => tmp_2);

process(clk)
begin
    if rising_edge(clk)then
        if (tmp_2 = '1' and enable = '1') then
            decl <= '1';
        else 
            decl <= '0';
        end if;
    end if;
end process;

end Behavioral;
