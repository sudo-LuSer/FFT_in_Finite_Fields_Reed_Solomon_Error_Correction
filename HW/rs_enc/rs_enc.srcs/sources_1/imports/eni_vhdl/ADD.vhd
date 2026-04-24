library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity ADD is
    Port (
        data_ram : in std_logic_vector(7 downto 0);
        data_out : out std_logic_vector(7 downto 0);
        cpt_rot  : in std_logic_vector(7 downto 0)
    );
end ADD;

architecture Behavioral of ADD is

signal temp, data_out_tmp : unsigned (7 downto 0);
 
begin

temp <= unsigned(data_ram) + unsigned(cpt_rot);

process(temp)
begin
    if (temp > to_unsigned(90,8)) then
        data_out_tmp <= temp - to_unsigned(26,8);
    else
        data_out_tmp <= temp;
    end if;
end process;

data_out <= std_logic_vector(data_out_tmp);

end Behavioral;