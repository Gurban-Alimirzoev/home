from pathlib import Path
import shutil
from os import listdir
#name = input("Введите название изделия: ")

def properties_name(name):
    if name == "bus-70":
        list_imports= {
            'arinc429_input_mezzanine_test.js',
            'arinc429_mezzanine_setup.js',
            'arinc429_output_mezzanine_test.js',
            'arinc429_setup.js',
            'bus-70_tu_requirements.js',
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js',
            'configuration_pins_test.js',
            'dec_to_bin_array.js',
            'device_military_ports_identification.js',
            'ethernet_tests.js',
            'generate_data_for_arinc_test.js',
            'generate_data_for_can_test.js',
            'generate_data_for_military_ports.js',
            'generate_data_for_mkio.js',
            'generate_data_for_uart_test.js',
            'logging.js',
            'mcp2515_can_id_mezzanine_converter.js',
            'military_ports_types.js',
            'mkio_mezzanine_setup.js',
            'mkio_send_data_bc_to_rt_mezzanine_test.js',
            'mkio_send_data_device_rt_to_stand_rt_mezzanine_test.js',
            'mkio_send_data_rt_to_bc_mezzanine_test.js',
            'mkio_send_data_stand_rt_to_device_rt_mezzanine_test.js',
            'mkio_setup.js',
            'mkio_send_data_stand_rt_to_device_rt_mezzanine_test.js',
            'pins_and_pouts_mezzanine_setup.js',
            'pins_and_pouts_setup.js',
            'pins_test.js',
            'pouts_test.js',
            'random.js',
            'repetitions.js',
            'time.js',
            'uart_setup.js',
            'uart_test.js'
            }
        return list_imports

    elif name == "bus-70n":
        list_imports= {
            'bus-70n_tu_requirements.js',
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js',
            'configuration_pins_test.js',
            'dec_to_bin_array.js',
            'device_military_ports_identification.js',
            'ethernet_tests.js',
            'generate_data_for_can_test.js',
            'generate_data_for_military_ports.js',
            'generate_data_for_uart_test.js',
            'logging.js',
            'mcp2515_can_id_mezzanine_converter.js',
            'military_ports_types.js',
            'pins_and_pouts_mezzanine_setup.js',
            'pins_and_pouts_setup.js',
            'pins_test.js',
            'pouts_test.js',
            'random.js',
            'repetitions.js',
            'time.js',
            'uart_setup.js',
            'uart_test.js'
            }
        return list_imports

    elif name == "ivs-70b":
        list_imports= {
            'time.js',
            'random.js',
            'can_receive_mezzanine_test.js',
            'ivs-70b_tu_requirements.js',
            'generate_data_for_can_test.js',
            'repetitions.js',
            'ethernet_tests.js',
            'generate_data_for_uart_test.js',
            'mcp2515_can_id_mezzanine_converter.js',       
            'uart_receive_mezzanine_test.js',
            'uart_send_mezzanine_test.js',
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js'}
        return list_imports

    elif name == "dsps":
        list_imports= {
            'arinc429_input_mezzanine_test.js',
            'arinc429_mezzanine_setup.js',
            'arinc429_output_mezzanine_test.js',
            'arinc429_setup.js',
            'dsps_tu_requirements.js',
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js',
            'configuration_pins_test.js',
            'dec_to_bin_array.js',
            'device_military_ports_identification.js',
            'ethernet_tests.js',
            'generate_data_for_arinc_test.js',
            'generate_data_for_can_test.js',
            'generate_data_for_military_ports.js',
            'generate_data_for_mkio.js',
            'generate_data_for_uart_test.js',
            'logging.js',
            'mcp2515_can_id_mezzanine_converter.js',
            'military_ports_types.js',
            'mkio_mezzanine_setup.js',
            'mkio_send_data_bc_to_rt_mezzanine_test.js',
            'mkio_send_data_device_rt_to_stand_rt_mezzanine_test.js',
            'mkio_send_data_rt_to_bc_mezzanine_test.js',
            'mkio_send_data_stand_rt_to_device_rt_mezzanine_test.js',
            'mkio_setup.js',
            'mkio_send_data_stand_rt_to_device_rt_mezzanine_test.js',
            'pins_and_pouts_mezzanine_setup.js',
            'pins_and_pouts_setup.js',
            'pins_test.js',
            'pouts_test.js',
            'random.js',
            'repetitions.js',
            'time.js',
            'uart_setup.js',
            'uart_test.js',
            'uart_receive_mezzanine_test.js',
            'uart_send_mezzanine_test.js'
            } 
        return list_imports

    elif name == "dpu":
        list_imports= {
            'arinc429_input_mezzanine_test.js',
            'arinc429_mezzanine_setup.js',
            'arinc429_output_mezzanine_test.js',
            'arinc429_setup.js',
            'dpu_tu_requirements.js',
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js',
            'configuration_pins_test.js',
            'dec_to_bin_array.js',
            'device_military_ports_identification.js',
            'ethernet_tests.js',
            'generate_data_for_arinc_test.js',
            'generate_data_for_can_test.js',
            'generate_data_for_military_ports.js',
            'generate_data_for_mkio.js',
            'generate_data_for_uart_test.js',
            'logging.js',
            'mcp2515_can_id_mezzanine_converter.js',
            'military_ports_types.js',
            'mkio_mezzanine_setup.js',
            'mkio_send_data_bc_to_rt_mezzanine_test.js',
            'mkio_send_data_device_rt_to_stand_rt_mezzanine_test.js',
            'mkio_send_data_rt_to_bc_mezzanine_test.js',
            'mkio_send_data_stand_rt_to_device_rt_mezzanine_test.js',
            'mkio_setup.js',
            'mkio_send_data_stand_rt_to_device_rt_mezzanine_test.js',
            'pins_and_pouts_mezzanine_setup.js',
            'pins_and_pouts_setup.js',
            'pins_test.js',
            'pouts_test.js',
            'random.js',
            'repetitions.js',
            'time.js',
            'uart_setup.js',
            'uart_test.js',
            'dpu_internal_can_test.js'
            }     
        return list_imports           

    elif name == "mpp-70nr":
        list_imports= {
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js',
            'configuration_pins_test.js',
            'ethernet_tests.js',
            'generate_data_for_can_test.js',
            'logging.js',
            'mcp2515_can_id_mezzanine_converter.js',
            'mpp-70nr_tu_requirements.js',
            'random.js',
            'repetitions.js',
            'time.js'}
        return list_imports

    elif name == "mpp-70r":
        list_imports= {
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js',
            'configuration_pins_test.js',
            'ethernet_tests.js',
            'generate_data_for_can_test.js',
            'logging.js',
            'mcp2515_can_id_mezzanine_converter.js',
            'mpp-70r_tu_requirements.js',
            'random.js',
            'repetitions.js',
            'time.js'}
        return list_imports

    elif name == "ppb-1b":
        list_imports= {
            'can_receive_mezzanine_test.js',
            'can_send_mezzanine_test.js',
            'configuration_pins_test.js',
            'ethernet_tests.js',
            'generate_data_for_can_test.js',
            'logging.js',
            'mcp2515_can_id_mezzanine_converter.js',
            'ppb-1b_tu_requirements.js',
            'random.js',
            'repetitions.js',
            'time.js'}            
        return list_imports

def folder_mk(path_folder):
    if not path_folder.exists():
        path_folder.mkdir()
    else:
        shutil.rmtree(path_folder)
        path_folder.mkdir()


def copy_folder_imports (list_imports, path_imports):
    for i in list_imports:
        shutil.copy(
            home/ 'imports'/ i,
            path_imports
        )

def copy_folder_name(name, path_blocks):
    shutil.copytree(
            home / name,
            path_blocks,
            ignore=None,
            ignore_dangling_symlinks=False,
            dirs_exist_ok=False
        )

blocks = listdir(Path.cwd())

if 'results' in blocks:
    blocks.remove('results')
if '.git' in blocks:
    blocks.remove('.git')


blocks.remove('stepa_test_case.py')
blocks.remove('imports')
blocks.remove('generate.exe')
blocks.remove('generate.py')


home = Path.cwd()
path_results = home / 'results'
folder_mk(path_results)

for name in blocks:    

    name_test_cases = name + "_test_cases"

    path_name_test_cases = path_results  / name_test_cases
    path_copy_imports = path_name_test_cases /'imports'
    path_blocks = path_name_test_cases /  name
    
    folder_mk(path_name_test_cases)
    folder_mk(path_copy_imports)

    list_imports_name = properties_name(name)

    copy_folder_imports (list_imports_name, path_copy_imports)
    copy_folder_name(name, path_blocks)




#shutil.make_archive(path_folder/name_new_folder, "zip", path_folder)