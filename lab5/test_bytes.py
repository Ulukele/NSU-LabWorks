import os

# Consts
BIG_FILE_LEN = 3000

# Filenames
IN_FILENAME = "in.txt"
OUT_FILENAME = "out.txt"

# Colors consts
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

# Printing
# (with some decorations)
def print_colored(color, msg, end='\n'):
    print(color + msg + bcolors.ENDC, end=end)

def compare_io(title, first, second):
    print_colored(bcolors.OKCYAN, "--- " + title +" ---")
    print_colored(bcolors.OKBLUE, "first:", end=' ')
    print(first)
    print_colored(bcolors.OKBLUE, "second:", end=' ')
    print(second)
    print()

def print_action(action_str):
    print_colored(bcolors.OKGREEN, '> ' + action_str)

def print_message(message_str):
    print_colored(bcolors.WARNING, '> ' + message_str)

def print_error(error_str):
    print_colored(bcolors.FAIL, '> ' + error_str)

# Conversions
# (to different types of data representation)
def raw_bytes_to_str(raw_bytes):
    raw_bytes = raw_bytes.split(b'\n')
    result = ['']
    for row in raw_bytes:
        result.append(str(row))
    return  '\n'.join(result)

def print_bytes_rows(first, second):
    first_str = raw_bytes_to_str(first)
    second_str = raw_bytes_to_str(second)
    compare_io("Bytes Rows", first_str, second_str)

def print_bytes(first, second):
    compare_io("Bytes", first, second)

def print_bytes_arr(first, second):
    compare_io("Bytes (array)", list(first), list(second))

def print_hex(first, second):
    compare_io("Hexes", first.hex(), second.hex())

def print_size(first, second):
    compare_io("Size", len(first), len(second))
    return (len(first), len(second))

# Works
# Analysis -- compare 2 files (print some info about both)
# Prepare data to encode -- simple text --> request to encode it
# Prepare data to decode -- encoded text --> request to decode it
# Execute -- executes some executable, in_stream == in_filename, out_stream == out_filename
def analysis(first_filename, second_filename):
    print_action("ANALYSIS OF: {} AND {}".format(first_filename, second_filename))
    first_file = open(first_filename, "rb")
    second_file = open(second_filename, "rb")

    first = first_file.read()
    first_file.close()
    second = second_file.read()
    second_file.close()

    first_file_size, second_file_size = print_size(first, second)
    if first_file_size > BIG_FILE_LEN:
        print_message("FIRST FILE TOO BIG TO PRINT")
        first = b'TOO BIG'
    if second_file_size > BIG_FILE_LEN:
        print_message("SECOND FILE TOO BIG TO PRINT")
        second = b'TOO BIG'
    if second_file_size > first_file_size:
        print_error("SECOND FILE BIGGER THAN FIRST")

    print_bytes_rows(first, second)

def prepare_data_to_encode(from_filename, to_filename):
    print_action("PREPARING DATA TO ENCODE. FROM {} TO {} WITH ADDING 'c\\r\\n'".format(from_filename, to_filename))
    sample_file = open(from_filename, "rb")
    sample_data = sample_file.read()
    sample_file.close()
    if sample_data[:3] == b'c\r\n':
        print_message("DATA ALREADY PREPARED")
        return

    prepared_data = 'c\r\n'.encode() + sample_data

    prepared_file = open(to_filename, "wb")
    prepared_file.write(prepared_data)
    prepared_file.close()

def prepare_data_to_decode(from_filename, to_filename):
    print_action("PREPARING DATA TO DECODE. FROM {} TO {} WITH ADDING 'd\\r\\n'".format(from_filename, to_filename))
    unprepared_file = open(from_filename, "rb")
    unprepared_data = unprepared_file.read()
    unprepared_file.close()
    if unprepared_data[:3] == b'd\r\n':
        print_message("DATA ALREADY PREPARED")
        return

    prepared_data = 'd\r\n'.encode() + unprepared_data

    prepared_file = open(to_filename, "wb")
    prepared_file.write(prepared_data)
    prepared_file.close()

def execute_launch(in_filename, out_filename):
    command = "cat {} | ./lab5 > {}".format(in_filename, out_filename)
    print_action("EXECUTING: {}".format(command))
    os.system(command)

# Scenarios
def check_full(in_filename, out_filename):
    print_action("CHECK ONE SAMPLE (ENCODE + DECODE). STATE=START, INPUT={}, OUTPUT={}".format(in_filename, out_filename))

    prepare_data_to_encode(in_filename, in_filename)

    in_out_filename = "output_of_" + in_filename
    out_in_filename = "input_for_" + out_filename

    execute_launch(in_filename, in_out_filename)
    analysis(in_filename, in_out_filename)

    prepare_data_to_decode(in_out_filename, out_in_filename)

    execute_launch(out_in_filename, out_filename)
    analysis(out_in_filename, out_filename)

    print_action("CHECK ONE SAMPLE (ENCODE + DECODE). STATE=END")

def check_one(in_filename, out_filename):
    print_action("CHECK ONE SAMPLE (ENCODE OR DECODE). STATE=START, INPUT={}, OUTPUT={}".format(in_filename, out_filename))

    execute_launch(in_filename, out_filename)
    analysis(in_filename, out_filename)

    print_action("CHECK ONE SAMPLE (ENCODE OR DECODE). STATE=END")

def just_check(first, second):
    print_action("JUST CHECK TWO FILES. STATE=START, FIRST={}, SECOND={}".format(first, second))
    analysis(first, second)
    print_action("JUST CHECK TWO FILES. STATE=END")

    print_action("CHECK ONE SAMPLE (ENCODE OR DECODE). STATE=END")

def check_test(test_in_filename, test_out_filename):
    print_action("CHECK TEST. STATE=START, INPUT={}, OUTPUT={}".format(test_in_filename, test_out_filename))
    analysis(test_in_filename, test_out_filename)

    print_action("CHECK TEST. STATE=END")

if __name__ == '__main__':
    just_check("in.txt", "out.txt")
    # check_one("in.txt", "out.txt")
    # check_full("in.txt", "out.txt")
    # check_test("in.txt", "out.txt")
