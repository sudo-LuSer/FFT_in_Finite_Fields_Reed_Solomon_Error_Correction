--ici, la séquence est la suivante : bq cr di ej kw mt os px uz gh (cryptii)

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity plugboard is
    Port (
        clk : in std_logic;
        data_rotor_out : in std_logic_vector(7 downto 0);
        data_rotor_out_valid : in std_logic;
        data_plugboard_out : out std_logic_vector(7 downto 0);
        data_plugboard_out_valid : out std_logic
    );
end plugboard;

architecture Behavioral of plugboard is
    signal tmp : unsigned(7 downto 0) := to_unsigned(0,8);
begin

    process(clk)
    begin
        if rising_edge(clk) then
            data_plugboard_out_valid <= data_rotor_out_valid; -- mise a jour du flag à l'avance

            if data_rotor_out_valid = '1' then
                case to_integer(unsigned(data_rotor_out)) is
                
                    -- B (66) <-> Q (81)
                    when 66 => tmp <= to_unsigned(81, 8);
                    when 81 => tmp <= to_unsigned(66, 8);
                    
                    -- C (67) <-> R (82)
                    when 67 => tmp <= to_unsigned(82, 8);
                    when 82 => tmp <= to_unsigned(67, 8);
                    
                    -- D (68) <-> I (73)
                    when 68 => tmp <= to_unsigned(73, 8);
                    when 73 => tmp <= to_unsigned(68, 8);
                    
                    -- E (69) <-> J (74)
                    when 69 => tmp <= to_unsigned(74, 8);
                    when 74 => tmp <= to_unsigned(69, 8);
                    
                    -- K (75) <-> W (87)
                    when 75 => tmp <= to_unsigned(87, 8);
                    when 87 => tmp <= to_unsigned(75, 8);
                    
                    -- M (77) <-> T (84)
                    when 77 => tmp <= to_unsigned(84, 8);
                    when 84 => tmp <= to_unsigned(77, 8);
                    
                    -- O (79) <-> S (83)
                    when 79 => tmp <= to_unsigned(83, 8);
                    when 83 => tmp <= to_unsigned(79, 8);
                    
                    -- P (80) <-> X (88)
                    when 80 => tmp <= to_unsigned(88, 8);
                    when 88 => tmp <= to_unsigned(80, 8);
                    
                    -- U (85) <-> Z (90)
                    when 85 => tmp <= to_unsigned(90, 8);
                    when 90 => tmp <= to_unsigned(85, 8);
                    
                    -- G (71) <-> H (72)
                    when 71 => tmp <= to_unsigned(72, 8);
                    when 72 => tmp <= to_unsigned(71, 8);
                    
      
                    when others => tmp <= unsigned(data_rotor_out);
                end case;
            end if;
        end if;
    end process;
  
    data_plugboard_out <= std_logic_vector(tmp);              

end Behavioral;