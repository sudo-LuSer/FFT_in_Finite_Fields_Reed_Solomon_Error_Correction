----------------------------------------------------------------------------------
-- Testbench for rsenc_top_level
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_rsenc_top is
end tb_rsenc_top;

architecture sim of tb_rsenc_top is

    -- Component declaration for the DUT
    component rsenc_top_level
        Port (
            clk        : in  std_logic;
            rst        : in  std_logic;
            enable     : in  std_logic;
            data_in    : in  std_logic_vector(8 downto 0);
            data_out   : out std_logic_vector(20 downto 0);
            ready_flag : out std_logic
        );
    end component;

    -- Signals
    signal clk         : std_logic := '0';
    signal rst         : std_logic := '1';
    signal enable      : std_logic := '0';
    signal data_in     : std_logic_vector(8 downto 0) := (others => '0');
    signal data_out    : std_logic_vector(20 downto 0);
    signal ready_flag  : std_logic;

    -- Clock period
    constant clk_period : time := 10 ns;

    -- Procedure to send a frame and check the output
    procedure send_frame (
        signal clk      : in  std_logic;
        signal enable   : out std_logic;
        signal data_in  : out std_logic_vector(8 downto 0);
        constant frame   : in  std_logic_vector(8 downto 0)
    ) is
    begin
        -- Apply data and enable
        data_in <= frame;
        enable <= '1';
        -- Wait for ENCODE_1, ENCODE_2, ENCODE_3 states (3 cycles)
        wait for 3 * clk_period;
        -- Then we can deassert enable (the FSM continues independently)
        enable <= '0';
    end procedure;

begin

    -- Instantiate the Unit Under Test (UUT)
    uut: rsenc_top_level
        port map (
            clk        => clk,
            rst        => rst,
            enable     => enable,
            data_in    => data_in,
            data_out   => data_out,
            ready_flag => ready_flag
        );

    -- Clock process
    clk_process: process
    begin
        while true loop
            clk <= '0';
            wait for clk_period / 2;
            clk <= '1';
            wait for clk_period / 2;
        end loop;
    end process;

    -- Stimulus process
    stim_proc: process
        -- Test frames (9 bits each)
        constant frame1 : std_logic_vector(8 downto 0) := "001010011"; -- 3 symbols : 001, 010, 011
        constant frame2 : std_logic_vector(8 downto 0) := "101110111"; -- 101, 110, 111
        constant frame3 : std_logic_vector(8 downto 0) := "000001010"; -- 000, 001, 010
    begin
        -- Initial reset
        rst <= '1';
        wait for 2 * clk_period;
        rst <= '0';
        wait for clk_period;

        -- Send first frame
        send_frame(clk, enable, data_in, frame1);

        -- Wait for ready_flag to be asserted
        wait until rising_edge(clk) and ready_flag = '1';
        -- Capture output value on the same edge (data_out is already registered)
        report "Frame 1 output: " & integer'image(to_integer(unsigned(data_out)));
        -- Check if output is non-zero (basic check)
        assert data_out /= "000000000000000000000" report "ERROR: Frame 1 output is zero" severity error;

        wait for clk_period;  -- let ready_flag go low again

        -- Send second frame
        send_frame(clk, enable, data_in, frame2);
        wait until rising_edge(clk) and ready_flag = '1';
        report "Frame 2 output: " & integer'image(to_integer(unsigned(data_out)));
        assert data_out /= "000000000000000000000" report "ERROR: Frame 2 output is zero" severity error;

        wait for clk_period;

        -- Send third frame
        send_frame(clk, enable, data_in, frame3);
        wait until rising_edge(clk) and ready_flag = '1';
        report "Frame 3 output: " & integer'image(to_integer(unsigned(data_out)));
        assert data_out /= "000000000000000000000" report "ERROR: Frame 3 output is zero" severity error;

        wait for clk_period;

        -- Test sequential frames without pause
        -- Apply a new frame immediately after the previous ready
        data_in <= "111000001"; -- symbols 111, 000, 001
        enable <= '1';
        wait for 3 * clk_period;
        enable <= '0';
        wait until rising_edge(clk) and ready_flag = '1';
        report "Frame 4 output: " & integer'image(to_integer(unsigned(data_out)));

        -- End simulation
        wait for 5 * clk_period;
        report "Simulation finished successfully." severity note;
        assert false report "End of simulation" severity failure; -- stops simulation (optional)
        wait;
    end process;

    -- Optional: check that after a frame the encoder returns to IDLE and counter is reset
    -- This can be observed in waveforms.

end sim;