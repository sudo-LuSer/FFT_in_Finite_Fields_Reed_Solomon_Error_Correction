library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_chimere_in is
end tb_chimere_in;

architecture sim of tb_chimere_in is
    component TX_CHIMERE
        port ( clk : in std_logic; rst : in std_logic; enable : in std_logic;
               data_in : in std_logic_vector(7 downto 0);
               data_out : out std_logic_vector(20 downto 0);
               ready : out std_logic );
    end component;
    signal clk : std_logic := '0';
    signal rst, enable : std_logic;
    signal data_in : std_logic_vector(7 downto 0);
    signal data_out : std_logic_vector(20 downto 0);
    signal ready : std_logic;
    constant CLK_PERIOD : time := 10 ns;
begin
    uut: TX_CHIMERE port map (clk, rst, enable, data_in, data_out, ready);
    clk <= not clk after CLK_PERIOD/2;
    process
    begin
        rst <= '1'; enable <= '0'; wait for 3*CLK_PERIOD;
        rst <= '0'; wait for CLK_PERIOD;
        -- 'A'
        data_in <= X"41"; enable <= '1';
        wait until rising_edge(clk) and ready = '1';
        enable <= '0'; wait for 2*CLK_PERIOD;
        -- 'B'
        data_in <= X"42"; enable <= '1';
        wait until rising_edge(clk) and ready = '1';
        enable <= '0'; wait for 2*CLK_PERIOD;
        -- 'C'
        data_in <= X"43"; enable <= '1';
        wait until rising_edge(clk) and ready = '1';
        enable <= '0'; wait for 2*CLK_PERIOD;
        wait;
    end process;
end sim;