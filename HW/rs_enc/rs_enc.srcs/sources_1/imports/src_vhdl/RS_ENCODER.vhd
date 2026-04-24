library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.rs_pkg.all;

entity rs_encoder is
    Port ( 
        clk         : in  std_logic;
        rst         : in  std_logic;
        enable      : in  std_logic;
        data_in     : in  std_logic_vector (2 downto 0); -- 3
        data_out    : out std_logic_vector (2 downto 0); -- 3
        valid_out   : out std_logic
    );
end rs_encoder;

architecture Behavioral of rs_encoder is
    
    signal lfsr : gf_array_t := (others => (others => '0'));
    signal feedback_sym : std_logic_vector(2 downto 0);
    
    type state_type is (IDLE, ENCODE_MSG, OUT_PARITY);
    signal state : state_type := IDLE;
    
    signal byte_cnt : integer range 0 to N := 0;

begin
    feedback_sym <= data_in xor lfsr(N_RDNCY - 1);

    process(clk, rst)
    begin
        if rst = '1' then
            state <= IDLE;
            byte_cnt <= 0;
            lfsr <= (others => (others => '0'));
            data_out <= (others => '0');
            valid_out <= '0';
            
        elsif rising_edge(clk) then
            valid_out <= '0';
            
            case state is
                when IDLE =>
                    if enable = '1' then
                        state <= ENCODE_MSG;
                        byte_cnt <= 1;
                        
                        -- Output message directly
                        data_out <= data_in;
                        valid_out <= '1';
                        
                        -- Shift and multiply LFSR
                        for i in 1 to N_RDNCY-1 loop
                            lfsr(i) <= lfsr(i-1) xor gf_mult_3(feedback_sym, G_COEF(i));
                        end loop;
                        lfsr(0) <= gf_mult_3(feedback_sym, G_COEF(0));
                    end if;

                when ENCODE_MSG =>
                    if enable = '1' then
                        data_out <= data_in;
                        valid_out <= '1';
                        
                        for i in 1 to N_RDNCY-1 loop
                            lfsr(i) <= lfsr(i-1) xor gf_mult_3(feedback_sym, G_COEF(i));
                        end loop;
                        lfsr(0) <= gf_mult_3(feedback_sym, G_COEF(0));
                        
                        if byte_cnt = K - 1 then
                            state <= OUT_PARITY;
                            byte_cnt <= 0;
                        else
                            byte_cnt <= byte_cnt + 1;
                        end if;
                    end if;

                when OUT_PARITY =>
                    valid_out <= '1';
                    
                    data_out <= lfsr(N_RDNCY - 1);
                    
                    for i in 1 to N_RDNCY-1 loop
                        lfsr(i) <= lfsr(i-1);
                    end loop;
                    lfsr(0) <= (others => '0');
                    
                    if byte_cnt = N_RDNCY - 1 then
                        state <= IDLE;
                        byte_cnt <= 0;
                    else
                        byte_cnt <= byte_cnt + 1;
                    end if;
            end case;
        end if;
    end process;

end Behavioral;