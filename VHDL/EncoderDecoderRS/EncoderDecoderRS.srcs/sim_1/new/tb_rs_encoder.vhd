library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_rs_encoder is
end tb_rs_encoder;

architecture Behavioral of tb_rs_encoder is

    component rs_encoder
        Port ( 
            clk         : in  std_logic;
            rst         : in  std_logic;
            enable      : in  std_logic;
            data_in     : in  std_logic_vector (2 downto 0);
            data_out    : out std_logic_vector (2 downto 0);
            valid_out   : out std_logic
        );
    end component;

    -- 2. Declare signals to connect to our component
    signal clk       : std_logic := '0';
    signal rst       : std_logic := '0';
    signal enable    : std_logic := '0';
    signal data_in   : std_logic_vector(2 downto 0) := (others => '0');
    signal data_out  : std_logic_vector(2 downto 0);
    signal valid_out : std_logic;

    -- Define the clock speed (100 MHz)
    constant clk_period : time := 10 ns;

begin

    -- 3. Instantiate the Encoder and wire it up
    DUT: rs_encoder port map (
        clk       => clk,
        rst       => rst,
        enable    => enable,
        data_in   => data_in,
        data_out  => data_out,
        valid_out => valid_out
    );

    -- 4. Clock Generation Process
    clk_process :process
    begin
        clk <= '0';
        wait for clk_period/2;
        clk <= '1';
        wait for clk_period/2;
    end process;

    -- 5. Stimulus Process (The actual test sequence)
    stim_proc: process
    begin
        -- Step A: Hold reset high for two clock cycles to clear the system
        rst <= '1';
        wait for clk_period * 2;
        rst <= '0';
        wait for clk_period;

        -- Step B: Start the ENCODE phase (Feed K=4 message symbols)
        -- Let's send the message: 1, 2, 3, 4
        enable <= '1';
        
        data_in <= "001"; -- Symbol 1
        wait for clk_period;
        
        data_in <= "010"; -- Symbol 2
        wait for clk_period;
        
        data_in <= "011"; -- Symbol 3
        wait for clk_period;
        
        --data_in <= "100"; -- Symbol 4
        --wait for clk_period;    

        -- Step C: Enter the OUT_PARITY phase (Wait for N-K=3 parity symbols)
        -- We drop enable, and the encoder will automatically push out the parity
        enable <= '0';
        data_in <= "000"; 
        
        -- Wait enough time for the 3 parity bytes to output, plus a little extra buffer
        wait for clk_period * 5;

        -- Step D: Stop the simulation gracefully
        assert false report "Simulation Finished Successfully" severity note;
        wait;
    end process;

end Behavioral;