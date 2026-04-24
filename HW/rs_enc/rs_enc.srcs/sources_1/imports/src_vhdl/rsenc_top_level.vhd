----------------------------------------------------------------------------------
-- Company:
-- Engineer:
--
-- Create Date: 04/22/2026 09:19:40 AM
-- Design Name:
-- Module Name: rsenc_top_level - Behavioral
-- Project Name:
-- Target Devices:
-- Tool Versions:
-- Description: Top-level for Reed-Solomon RS(7,3) encoder
--
-- Dependencies: pc_rs.vhd, rs_encoder.vhd, CodeWord.vhd, rs_pkg.vhd
--
-- Revision:
-- Revision 0.02 - Corrected counter reset issue, added reset for outputs,
--                 removed unused signals, fixed sensitivity list.
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity rsenc_top_level is
    Port (
        clk        : in  std_logic;
        rst        : in  std_logic;
        enable     : in  std_logic;
        data_in    : in  std_logic_vector (8 downto 0);
        data_out   : out std_logic_vector (20 downto 0);
        ready_flag : out std_logic
    );
end rsenc_top_level;

architecture Behavioral of rsenc_top_level is

    type compute is (IDLE, ENCODE_1, ENCODE_2, ENCODE_3, WAIT_COMPUTE, READY);
    signal compute_state : compute := IDLE;
    signal next_state    : compute;

    signal data_in_reg    : std_logic_vector(2 downto 0);
    signal data_reg       : std_logic_vector(2 downto 0);
    signal data_out_reg   : std_logic_vector(20 downto 0);

    signal data_out0 : std_logic_vector(2 downto 0);
    signal data_out1 : std_logic_vector(2 downto 0);
    signal data_out2 : std_logic_vector(2 downto 0);
    signal data_out3 : std_logic_vector(2 downto 0);
    signal data_out4 : std_logic_vector(2 downto 0);
    signal data_out5 : std_logic_vector(2 downto 0);
    signal data_out6 : std_logic_vector(2 downto 0);

    signal valid_out     : std_logic;
    signal counter       : std_logic_vector(2 downto 0);
    signal sync_rst_cnt  : std_logic; 
    
    signal enable_reg    : std_logic;

    component pc_rs is
        Port (
            clk      : in  std_logic;
            rst      : in  std_logic;
            sync_rst : in  std_logic;  
            enable   : in  std_logic;
            counter  : out std_logic_vector(2 downto 0)
        );
    end component;

    component rs_encoder is
        Port (
            clk       : in  std_logic;
            rst       : in  std_logic;
            enable    : in  std_logic;
            data_in   : in  std_logic_vector (2 downto 0);
            data_out  : out std_logic_vector (2 downto 0);
            valid_out : out std_logic
        );
    end component;

    component CodeWord is
        Port (
            clk       : in  std_logic;
            rst       : in  std_logic;
            enable    : in  std_logic;
            addr      : in  std_logic_vector(2 downto 0);
            data_in   : in  std_logic_vector (2 downto 0);
            data_out0 : out std_logic_vector (2 downto 0);
            data_out1 : out std_logic_vector (2 downto 0);
            data_out2 : out std_logic_vector (2 downto 0);
            data_out3 : out std_logic_vector (2 downto 0);
            data_out4 : out std_logic_vector (2 downto 0);
            data_out5 : out std_logic_vector (2 downto 0);
            data_out6 : out std_logic_vector (2 downto 0)
        );
    end component;

begin


    process (compute_state, data_in)
    begin
        data_in_reg <= (others => '0');
        enable_reg  <= '0';
    
        case compute_state is
            when ENCODE_1 =>
                enable_reg  <= '1';
                data_in_reg <= data_in(2 downto 0);
            when ENCODE_2 =>
                enable_reg  <= '1';   -- <- ajout
                data_in_reg <= data_in(5 downto 3);
            when ENCODE_3 =>
                enable_reg  <= '1';   -- <- ajout
                data_in_reg <= data_in(8 downto 6);
            when others => 
                -- conserve mm valeurs par default
        end case;
    end process;


    process (rst, clk)
    begin
        if (rst = '1') then
            compute_state <= IDLE;
            data_out_reg  <= (others => '0');
            ready_flag    <= '0';
        elsif (rising_edge(clk)) then
            compute_state <= next_state;

            if (compute_state = READY) then
                data_out_reg <= data_out6 & data_out5 & data_out4 &
                                data_out3 & data_out2 & data_out1 & data_out0;
                ready_flag   <= '1';
            else
                ready_flag <= '0';
            end if;
        end if;
    end process;


    process (compute_state, enable, valid_out)
    begin
        case compute_state is
            when IDLE =>
                if (enable = '1') then
                    next_state <= ENCODE_1;
                else
                    next_state <= IDLE;
                end if;
                sync_rst_cnt <= '1';
              
            when ENCODE_1 =>
                sync_rst_cnt <= '0';
                next_state <= ENCODE_2; 

            when ENCODE_2 =>
                next_state <= ENCODE_3;

            when ENCODE_3 =>
                next_state <= WAIT_COMPUTE;

            when WAIT_COMPUTE =>
                if (valid_out = '1') then
                    next_state <= WAIT_COMPUTE;
                else
                    next_state <= READY;
                end if;

            when READY =>
                next_state <= IDLE;

            when others =>
                next_state <= IDLE;
        end case;
    end process;

    ptr_data : pc_rs
        port map (
            clk      => clk,
            rst      => rst,
            sync_rst => sync_rst_cnt, 
            enable   => valid_out,
            counter  => counter
        );

    rs_enc : rs_encoder
        port map (
            clk       => clk,
            rst       => rst,
            enable    => enable_reg,
            data_in   => data_in_reg,
            data_out  => data_reg,
            valid_out => valid_out
        );

    stock : CodeWord
        port map (
            clk       => clk,
            rst       => rst,
            enable    => valid_out,
            addr      => counter,
            data_in   => data_reg,
            data_out0 => data_out0,
            data_out1 => data_out1,
            data_out2 => data_out2,
            data_out3 => data_out3,
            data_out4 => data_out4,
            data_out5 => data_out5,
            data_out6 => data_out6
        );

    data_out <= data_out_reg;

end Behavioral;